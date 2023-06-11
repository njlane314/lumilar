#include "Optics.hh"
//_________________________________________________________________________________________
Optics::Optics() {}
//_________________________________________________________________________________________
Optics::~Optics() {}
//_________________________________________________________________________________________
ThreadPool Optics::optics_thread_pool_(std::thread::hardware_concurrency());
double Optics::detector_width = 0.0;
double Optics::detector_height = 0.0;
double Optics::detector_depth = 0.0;
//_________________________________________________________________________________________
void Optics::CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector& optical_sensors) {
    auto start_time = std::chrono::high_resolution_clock::now();
    bool debug_true = true;

    int photons_geometrically_expected = 0;
    int photons_collected = 0;
    double expected_geometric_acceptance = 0;
    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();

    DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    detector_construction->GetDetectorDimensions(detector_width, detector_height, detector_depth);

    std::vector<std::future<void>> results;
    std::vector<std::pair<PhotonRadiant, int>> radiant_count_pairs = CreateRadiantCountPairs(signal);
    for (const auto& radiant_count_pair : radiant_count_pairs) {
        results.emplace_back(
            optics_thread_pool_.enqueue([&](){
                ProcessRadiant(radiant_count_pair, optical_sensors, total_photons, photons_geometrically_expected, photons_collected, expected_geometric_acceptance);
            })
        );
    }

    for (auto& result : results) {
        result.get();
    }

    if (debug_true) {
        PrintDebug(expected_geometric_acceptance, photons_geometrically_expected, photons_collected, signal, start_time);
    }
}
//_________________________________________________________________________________________
std::vector<std::pair<PhotonRadiant, int>> Optics::CreateRadiantCountPairs(const Signal* signal) {
    std::vector<int> radiant_count_vector = signal->GetScintillation()->GetRadiantSizes();
    std::vector<PhotonRadiant> photon_radiants = signal->GetScintillation()->GetPhotonRadiants();
    assert(radiant_count_vector.size() == photon_radiants.size());

    std::vector<std::pair<PhotonRadiant, int>> radiant_count_pairs;
    for (int radiant_idx = 0; radiant_idx < radiant_count_vector.size(); radiant_idx++) {
        radiant_count_pairs.emplace_back(std::make_pair(photon_radiants[radiant_idx], radiant_count_vector[radiant_idx]));
    }

    return radiant_count_pairs;
}
//_________________________________________________________________________________________
void Optics::ProcessRadiant(const std::pair<PhotonRadiant, int>& radiant_count_pair, const OpticalSensorVector& optical_sensors, int& total_photons, int& photons_geometrically_expected, int& photons_collected, double& expected_geometric_acceptance) {
    PhotonRadiant radiant_copy = radiant_count_pair.first;
    int a_radiant_count = radiant_count_pair.second;

    double total_geometric_quenching_factor = 0;
    std::vector<OpticalPhoton> photons_copy;
    for (const auto& a_optical_sensor : optical_sensors) {
        Eigen::Vector3d separation = (a_optical_sensor->GetPosition() - (radiant_copy.position));
        double distance = separation.norm();
        
        double geometric_quenching_factor = Optics::GeometricQuenching(&radiant_copy, a_optical_sensor.get(), &separation);                
        int num_photons_geometrically_expected = CLHEP::RandBinomial::shoot(a_radiant_count, geometric_quenching_factor);
        photons_geometrically_expected += num_photons_geometrically_expected;
        if (num_photons_geometrically_expected >= 1) {
            photons_copy.clear();
            photons_copy = radiant_copy.photons;
            for (int photon_idx = 0; photon_idx < num_photons_geometrically_expected; photon_idx++) {
                if (!photons_copy.empty()) {
                    OpticalPhoton selected_photon = photons_copy[0];
                    ProcessAttenuation(radiant_copy, selected_photon, *a_optical_sensor, distance, photons_collected);
                    photons_copy.erase(photons_copy.begin());
                } else {
                    std::cout << "-- Optics::ProcessRadiant: photons_copy is empty" << std::endl;
                }
            }
            radiant_copy.photons = photons_copy;
        } 
        expected_geometric_acceptance += geometric_quenching_factor * ((double)a_radiant_count / (double)total_photons);
    }
}
//_________________________________________________________________________________________
void Optics::ProcessAttenuation(const PhotonRadiant& photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor& optical_sensor, const double distance, int& photons_collected) {
    double attenuation_factor = AttenuationFactor(distance, optical_photon.GetWavelength());
    
    if (std::isnan(attenuation_factor)) {
        std::cout << "\n-- Attenuation factor is null." << std::endl;
    }
    
    if (attenuation_factor <= 0) {
        std::cout << "\n-- Attenuation factor is zero or negative." << std::endl;
    }

    int is_accepted = CLHEP::RandBinomial::shoot(1, attenuation_factor);
    if (is_accepted == 1) {
        OpticalPhoton arrival_photon = CreateArrivalPhoton(&photon_radiant, optical_photon, &optical_sensor);
        const_cast<OpticalSensor&>(optical_sensor).AddPhoton(arrival_photon);
        photons_collected++;
    }
}
//_________________________________________________________________________________________
double Optics::AttenuationFactor(const double distance, const double wavelength) {
    double absorption_length = MediumProperties::GetInstance()->GetAbsorptionLength();
    double attenuation_length = 1. / (1. / absorption_length);

    return std::exp(- distance / attenuation_length);
}
//_________________________________________________________________________________________
OpticalPhoton Optics::CreateArrivalPhoton(const PhotonRadiant* photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor* optical_sensor) {
    Eigen::Vector3d separation = (optical_sensor->GetPosition() - (photon_radiant->position));
    double distance = separation.norm();
    double convert_m_per_s_to_cm_per_ns = 1e2 * 1e-9;

    double group_velocity = MediumProperties::GetInstance()->GetGroupVelocity(optical_photon.GetWavelength()) * convert_m_per_s_to_cm_per_ns;
    double arrival_time = optical_photon.GetEmissionTime() + Propagation::SampleArrivalTime(photon_radiant, optical_photon, optical_sensor, detector_width, detector_height, detector_depth);

    OpticalPhoton arrival_photon = optical_photon;
    arrival_photon.SetArrivalTime(arrival_time);

    return arrival_photon;
}
//_________________________________________________________________________________________
double Optics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    // https://arxiv.org/pdf/2010.00324.pdf
    double solid_angle = CalculateSolidAngle(sensor, separation);
    double visibility = solid_angle / (4 * M_PI);

    return visibility;
}
//_________________________________________________________________________________________
double Optics::CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    double solid_angle = 0.;

    Eigen::Vector3d projection = CreateProjectionGeometry(sensor, separation);
    if (sensor->IsRectangular()) {
        auto [height, width] = sensor->GetDimensions();
        solid_angle = RectangularSolidAngle(separation, height, width);
    }
    else {
        throw std::invalid_argument("-- Invalid optical detector geometry");
    }

    return solid_angle;
}
//_________________________________________________________________________________________
Eigen::Vector3d Optics::CreateProjectionGeometry(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    Eigen::Vector3d projection;
    if (sensor->GetOrientation() == PlaneOrientation::X_POS || sensor->GetOrientation() == PlaneOrientation::X_NEG) {
        projection = *separation;
    } 
    else if (sensor->GetOrientation() == PlaneOrientation::Y_POS || sensor->GetOrientation() == PlaneOrientation::Y_NEG) {
        projection = Eigen::Vector3d(separation->y(), separation->x(), separation->z());
    } 
    else if (sensor->GetOrientation() == PlaneOrientation::Z_POS || sensor->GetOrientation() == PlaneOrientation::Z_NEG) {
        projection = Eigen::Vector3d(separation->z(), separation->y(), separation->x());
    } 
    else {
        throw std::invalid_argument("-- Invalid optical detector plane orientation.");
    }

    return projection;
}
//_________________________________________________________________________________________
double Optics::RectangularSolidAngle(const Eigen::Vector3d* projection, const double& height, const double& width) {
    // https://vixra.org/pdf/2001.0603v2.pdf
    auto omega = [&](double a, double b, double d) -> double {
        double aa = a / (2. * d);
        double bb = b / (2. * d);
        double aux = (1. + aa * aa + bb * bb) / ((1. + aa * aa) * (1. + bb * bb));
        return 4. * std::acos(std::sqrt(aux));
    };

    if (std::abs(projection->y()) == 0 && projection->z() == 0) /* -- the on-axis case -- */ {
        return omega(width, height, std::abs(projection->x()));
    }

    // otherwise, decomposition of off-axis geometry into four possible symmetric cases
    if ((std::abs(projection->y()) > width/2.) && (std::abs(projection->z()) > height/2.)) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - width/2.;
        B = std::abs(projection->z()) - height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        double to_return = (omega(2*(A+a),2*(B+b),d)-omega(2*A,2*(B+b),d)-omega(2*(A+a),2*B,d)+omega(2*A,2*B,d))/4.0;
        return to_return;
    } 

    if ((std::abs(projection->y()) <= width/2.) && (std::abs(projection->z()) <= height/2.)) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + width/2.;
        B = -std::abs(projection->z()) + height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        double to_return = (omega(2*(a-A),2*(b-B),d)+omega(2*A,2*(b-B),d)+omega(2*(a-A),2*B,d)+omega(2*A,2*B,d))/4.0;
        return to_return;
    }

    if ((std::abs(projection->y()) > width/2.) && (std::abs(projection->z()) <= height/2.)) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - width/2.;
        B = -std::abs(projection->z()) + height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        double to_return = (omega(2*(A+a),2*(b-B),d)-omega(2*A,2*(b-B),d)+omega(2*(A+a),2*B,d)-omega(2*A,2*B,d))/4.0;
        return to_return;
    }

    if ((std::abs(projection->y()) <= width/2.) && (std::abs(projection->z()) > height/2.)) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + width/2.;
        B = std::abs(projection->z()) - height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        double to_return = (omega(2*(a-A),2*(B+b),d)-omega(2*(a-A),2*B,d)+omega(2*A,2*(B+b),d)-omega(2*A,2*B,d))/4.0;
        return to_return;
    }
    
    throw std::invalid_argument("-- Invalid optical detector geometry.");   
}
//_________________________________________________________________________________________
void Optics::PrintDebug(double expected_geometric_acceptance, int photons_geometrically_expected, int photons_collected, const Signal* signal, std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::string line = "=======================================================================";
    std::string subheader = " Optical Transport Simulation Results ";
    
    std::cout << std::endl;
    std::cout << line << std::endl;
    std::cout << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::right << "" << subheader << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::left << std::endl;
    std::cout << line << std::endl;
    
    std::cout << std::setw(60) << std::left << "Expected Geometric Acceptance: " << std::fixed << std::setprecision(5) << expected_geometric_acceptance * 100 << "%" << std::endl;
    std::cout << std::setw(60) << std::left << "Photons Geometrically Expected: " << photons_geometrically_expected << std::endl;
    std::cout << std::setw(60) << std::left << "Photons Collected: " << photons_collected << std::endl;
    std::cout << std::setw(60) << std::left << "Photons Total: " << signal->GetScintillation()->GetTotalPhotonCount() << std::endl;
    std::cout << std::setw(60) << std::left << "Detection Efficiency: " << std::fixed << std::setprecision(5) << (double)photons_collected / (double)signal->GetScintillation()->GetTotalPhotonCount() * 100 << "%" << std::endl;
    std::cout << std::setw(60) << std::left << "Elapsed Time: " << elapsed_time.count() << " ms" << std::endl;
    
    std::cout << line << std::endl << std::endl;
}


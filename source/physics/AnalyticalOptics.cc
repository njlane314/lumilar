#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

void AnalyticalOptics::CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector& optical_sensors) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> radiant_count_vector = signal->GetScintillation()->GetRadiantSizes();
    std::vector<PhotonRadiant> photon_radiants = signal->GetScintillation()->GetPhotonRadiants();

    assert(radiant_count_vector.size() == photon_radiants.size());

    std::vector<std::pair<PhotonRadiant, int>> radiant_count_pairs;
    for (int radiant_idx = 0; radiant_idx < radiant_count_vector.size(); radiant_idx++) {
        radiant_count_pairs.emplace_back(std::make_pair(photon_radiants[radiant_idx], radiant_count_vector[radiant_idx]));
    }

    int photons_detected = 0;
    int photons_arrived = 0;

    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();

    ThreadPool pool(std::thread::hardware_concurrency());

    std::vector<std::future<void>> results;

    double expected_geometric_acceptance = 0;
    
    for (const auto& radiant_count_pair : radiant_count_pairs) {
        results.emplace_back(
            pool.enqueue([&](){
                PhotonRadiant radiant_copy = radiant_count_pair.first;
                int a_radiant_count = radiant_count_pair.second;
                double total_geometric_quenching_factor = 0;
                 std::vector<OpticalPhoton> photons_copy;
                for (const auto& a_optical_sensor : optical_sensors) {
                    double geometric_quenching_factor = AnalyticalOptics::GeometricQuenching(&radiant_copy, a_optical_sensor.get());                
                    int num_photons_detected = CLHEP::RandBinomial::shoot(a_radiant_count, geometric_quenching_factor);
                    photons_detected += num_photons_detected;
                    if (num_photons_detected >= 1) {
                        photons_copy.clear();
                        photons_copy = radiant_copy.photons;
                        for (int photon_idx = 0; photon_idx < num_photons_detected; photon_idx++) {
                            if (!photons_copy.empty()) {
                                auto selected_photon = photons_copy[0];
                                a_optical_sensor->AddPhoton(CreateArrivalPhoton(&radiant_copy, selected_photon, a_optical_sensor.get()));
                                photons_arrived++;
                                
                                photons_copy.erase(photons_copy.begin());
                            } else {
                                std::cout << "-- No Photons Left" << std::endl;
                            }
                        }
                        radiant_copy.photons = photons_copy;
                    } 
                    expected_geometric_acceptance += geometric_quenching_factor * ((double)a_radiant_count / (double)total_photons);
                }
            })
        );
    }

    for (auto& result : results) {
        result.get();
    }

    std::cout << "Expected Geometric Acceptance: " << expected_geometric_acceptance << std::endl;
    std::cout << "Photons Detected: " << photons_detected << std::endl;
    std::cout << "Photons Arrived: " << photons_arrived << std::endl;
    std::cout << "Photons Total: " << signal->GetScintillation()->GetTotalPhotonCount() << std::endl;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Elapsed time: " << elapsed_time.count() << " ms" << std::endl;
}

OpticalPhoton AnalyticalOptics::CreateArrivalPhoton(const PhotonRadiant* photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor* optical_sensor) {
    Eigen::Vector3d separation = (optical_sensor->GetPosition() - (photon_radiant->position));
    double distance = separation.norm();
    double convert_m_per_s_to_cm_per_ns = 1e2 * 1e-9;

    double group_velocity = MediumProperties::GetInstance()->GetGroupVelocity(optical_photon.GetWavelength()) * convert_m_per_s_to_cm_per_ns;
    double arrival_time = optical_photon.GetEmissionTime() + distance / group_velocity;
    
    OpticalPhoton arrival_photon = optical_photon;
    arrival_photon.SetArrivalTime(arrival_time);

    return arrival_photon;
}

double AnalyticalOptics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor) {
    // https://arxiv.org/pdf/2010.00324.pdf
    Eigen::Vector3d separation = (sensor->GetPosition() - (a_photon_radiant->position));
    double distance = separation.norm();

    double solid_angle = CalculateSolidAngle(sensor, &separation);
    double visibility = solid_angle / (4 * M_PI);
    double absoprtion_quenching = AbsorptionQuenching(distance);

    return visibility * absoprtion_quenching;
}

double AnalyticalOptics::CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
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

Eigen::Vector3d AnalyticalOptics::CreateProjectionGeometry(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
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

double AnalyticalOptics::RectangularSolidAngle(const Eigen::Vector3d* projection, const double& height, const double& width) {
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
    
    throw std::invalid_argument("-- Solid angle calculation failed.");   
}

double AnalyticalOptics::AbsorptionQuenching(double distance) {
    return std::exp(- distance / MediumProperties::GetInstance()->GetMediumProperties()->absorption_length);
}
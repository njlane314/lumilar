#include "Optics.hh"
//_________________________________________________________________________________________
Optics::Optics() {}
//_________________________________________________________________________________________
Optics::~Optics() {}
//_________________________________________________________________________________________
int num_threads = 1; //std::thread::hardware_concurrency();
ThreadPool Optics::optics_thread_pool_(num_threads);
//_________________________________________________________________________________________
bool Optics::debug_status_ = true;
int Optics::debug_emission_photons_ = 0;
double Optics::debug_geometric_acceptance_ = 0;
int Optics::debug_photons_collected_ = 0;
//_________________________________________________________________________________________
void Optics::CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector* optical_sensors) {
    auto start_time = std::chrono::high_resolution_clock::now();

    debug_emission_photons_ = signal->GetScintillation()->GetTotalPhotonCount();
    debug_geometric_acceptance_ = 0.0;
    debug_photons_collected_ = 0;

    std::vector<std::future<void>> results;
    std::vector<PhotonRadiant> photon_radiants = signal->GetScintillation()->GetPhotonRadiants();
    for (const auto& photon_radiant : photon_radiants) {
        results.emplace_back(
            optics_thread_pool_.enqueue([&, photon_radiant](){
                // calculate the optical intensity at each optical sensor for each photon radiant
                ProcessRadiant(photon_radiant, optical_sensors);
            })
        );
    }

    for (auto& result : results) {
        result.get();
    }

    if (debug_status_) {
        PrintDebug(start_time);
    }
}
//_________________________________________________________________________________________
void Optics::ProcessRadiant(const PhotonRadiant& photon_radiant, const OpticalSensorVector* optical_sensor_vector) {
    larnest::LArInteraction radiant_interaction = photon_radiant.GetInteraction();
    int radiant_photon_count = photon_radiant.GetPhotonCount();

    Eigen::Vector3d radiant_position = photon_radiant.GetPosition();
    double initial_time = photon_radiant.GetTime();

    for (const auto& optical_sensor : *optical_sensor_vector) {
        // process each sensor
        Eigen::Vector3d optical_sensor_position = optical_sensor->GetPosition();

        Eigen::Vector3d separation = optical_sensor_position - radiant_position;
        double distance = separation.norm();
        
        double sensor_visibility_factor = Optics::GeometricQuenching(optical_sensor.get(), &separation);  
        double absorption_quenching_factor = Optics::AbsorptionQuenching(distance);
        double geometric_acceptance = sensor_visibility_factor * absorption_quenching_factor;

        if (std::isnan(sensor_visibility_factor) || std::isnan(absorption_quenching_factor)) {
            std::cout << "\n -- Optics::ProcessRadiant: sensor visibility or absorption quenching factor is null." << std::endl;
            geometric_acceptance = 0.;
        }

        if (sensor_visibility_factor <= 0 || absorption_quenching_factor <= 0) {
            std::cout << "\n -- Optics::ProcessRadiant: sensor visibility or absorption quenching factor is zero or negative." << std::endl;
            geometric_acceptance = 0.;
        }

        int photons_collected = CLHEP::RandBinomial::shoot(radiant_photon_count, geometric_acceptance);
        debug_geometric_acceptance_ += geometric_acceptance * radiant_photon_count / debug_emission_photons_;

        if (photons_collected > 0) {
            for (int i = 0; i < photons_collected; i++) {
                double QUANTUM_EFFIC = 0.1;
                double ACCEPTANCE_AREA = 0.1;

                if (CLHEP::RandBinomial::shoot(1, QUANTUM_EFFIC) == 1 && CLHEP::RandBinomial::shoot(1, ACCEPTANCE_AREA) == 1) {
                    double angle = 90.;
                    double arrival_time = SampleArrivalTime(initial_time, distance, angle, radiant_interaction);
                    if (arrival_time > 0) {
                        const_cast<OpticalSensor&>(*optical_sensor).AddArrivalTime(arrival_time);
                        debug_photons_collected_ += 1;
                    }
                } 
            }
        }
    }
}
//_________________________________________________________________________________________
double Optics::SampleArrivalTime(const double initial_time, const double distance, const double angle, const larnest::LArInteraction interaction) {
    double emission_time = Relaxation::SampleEmissionTime(interaction);
    double propagation_time = PropagationTime::GetInstance()->SamplePropagationTime(distance, angle);

    double arrival_time = initial_time + emission_time + propagation_time;

    return arrival_time;
}
//_________________________________________________________________________________________
double Optics::AbsorptionQuenching(const double distance) {
    double absorption_length = 33107.6;
    double attenuation_length = 1. / (1. / absorption_length);

    return std::exp(- distance / attenuation_length);
}
//_________________________________________________________________________________________
double Optics::GeometricQuenching(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    // https://arxiv.org/pdf/2010.00324.pdf
    double solid_angle = CalculateSolidAngle(sensor, separation);
    double visibility = solid_angle / (4 * M_PI);

    return visibility;
}
//_________________________________________________________________________________________
double Optics::CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    double solid_angle = 0.;

    Eigen::Vector3d projection = CreateProjectionGeometry(sensor, separation);
    if (sensor->isRectangular()) {
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
void Optics::PrintDebug(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::string line = "=======================================================================";
    std::string subheader = " Optical Transport Simulation Results ";
    
    std::cout << std::endl;
    std::cout << line << std::endl;
    std::cout << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::right << "" << subheader << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::left << std::endl;
    std::cout << line << std::endl;
    
    std::cout << std::setw(60) << std::left << "Total Emission Photons: " << debug_emission_photons_ << std::endl;
    std::cout << std::setw(60) << std::left << "Geometric Acceptance: " << std::fixed << std::setprecision(5) << debug_geometric_acceptance_ * 100 << "%" << std::endl;
    std::cout << std::setw(60) << std::left << "Photons Collected: " << debug_photons_collected_ << std::endl;
    std::cout << std::setw(60) << std::left << "Collection Efficiency: " << static_cast<double>(debug_photons_collected_) / static_cast<double>(debug_emission_photons_) * 100 << "%" << std::endl;
    std::cout << std::setw(60) << std::left << "System Elapsed Time: " << elapsed_time.count() << " ms" << std::endl;
    
    std::cout << line << std::endl << std::endl;
}

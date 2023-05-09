#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

void AnalyticalOptics::CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector& optical_sensors) {
    std::vector<int> radiant_count_vector = signal->GetScintillation()->GetRadiantSizes();
    std::vector<PhotonRadiant> photon_radiants = signal->GetScintillation()->GetPhotonRadiants();

    //multithread this
    int radiant_idx = 0;
    for (const int& a_radiant_count : radiant_count_vector) {
        PhotonRadiant radiant_copy = photon_radiants[radiant_idx];
        double total_geometric_quenching_factor = 0;
        for (const auto& a_optical_sensor : optical_sensors) {
            
            double geometric_quenching_factor = AnalyticalOptics::GeometricQuenching(&radiant_copy, a_optical_sensor.get());
            if (geometric_quenching_factor > 1) {
                throw std::invalid_argument("-- Geometric Quenching Factor Greater Than 1");
            } 
            
            total_geometric_quenching_factor += geometric_quenching_factor;

            int num_photons_detected = floor(geometric_quenching_factor * a_radiant_count);
            if (num_photons_detected > 1) {
                std::vector<OpticalPhoton> photons_copy = radiant_copy.photons;
                for (int photon_idx = 0; photon_idx < num_photons_detected; photon_idx++) {
                    if (!photons_copy.empty()) {
                        auto selected_photon = photons_copy[0];
                        a_optical_sensor->AddPhoton(CreateArrivalPhoton(&radiant_copy, selected_photon, a_optical_sensor.get()));
                        
                        photons_copy.erase(photons_copy.begin());
                    } else {
                        throw std::invalid_argument("-- No Photons Left");
                    }
                }
                radiant_copy.photons = photons_copy;
            }  
        }
        radiant_idx++;
    }
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
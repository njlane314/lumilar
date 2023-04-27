#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

void AnalyticalOptics::CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector& optical_sensors) {
    std::vector<PhotonRadiant> photon_radiants = signal->getScintillation()->get_photon_radiants();

    int total_photons_detected = 0;
    for (const auto& a_photon_radiant : photon_radiants) {
        for (const auto& a_optical_sensor : optical_sensors) {
            double geometric_quenching_factor = AnalyticalOptics::GeometricQuenching(&a_photon_radiant, a_optical_sensor.get());
            int num_photons_detected = floor(geometric_quenching_factor * (double)a_photon_radiant.photons.size());
            total_photons_detected += num_photons_detected;
            for (int i = 0; i < num_photons_detected; i++) {
                a_optical_sensor->addPhoton(a_photon_radiant.photons[i]);
            }
        }
    }
}

double AnalyticalOptics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor) {
    // https://arxiv.org/pdf/2010.00324.pdf
    Eigen::Vector3d separation = (sensor->getPosition() - (a_photon_radiant->position * mm));
    double distance = separation.norm();

    double solid_angle = CalculateSolidAngle(sensor, &separation);
    double visibility = solid_angle / (4 * M_PI);
    double absoprtion_quenching = AbsorptionQuenching(distance);

    return visibility * absoprtion_quenching;
}

double AnalyticalOptics::CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    double solid_angle = 0.;

    Eigen::Vector3d projection = CreateProjectionGeometry(sensor, separation);
    if (sensor->isRectangular()) {
        auto [width, height] = sensor->getDimensions();
        solid_angle = RectangularSolidAngle(separation, width, height);
    }
    else {
        throw std::invalid_argument("-- Invalid optical detector geometry");
    }

    return solid_angle;
}

Eigen::Vector3d AnalyticalOptics::CreateProjectionGeometry(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    Eigen::Vector3d projection;
    if (sensor->getOrientation() == PlaneOrientation::X_POS || sensor->getOrientation() == PlaneOrientation::X_NEG) {
        projection = *separation;
    } 
    else if (sensor->getOrientation() == PlaneOrientation::Y_POS || sensor->getOrientation() == PlaneOrientation::Y_NEG) {
        projection = Eigen::Vector3d(separation->y(), separation->x(), separation->z());
    } 
    else if (sensor->getOrientation() == PlaneOrientation::Z_POS || sensor->getOrientation() == PlaneOrientation::Z_NEG) {
        projection = Eigen::Vector3d(separation->z(), separation->y(), separation->x());
    } 
    else {
        throw std::invalid_argument("-- Invalid optical detector plane orientation.");
    }

    return projection;
}

double AnalyticalOptics::RectangularSolidAngle(const Eigen::Vector3d* projection, const double& width, const double& height) {
    // https://vixra.org/pdf/2001.0603v2.pdf
    
    auto omega = [&](double a, double b, double d) -> double {
        double aa = a / (2. * d);
        double bb = b / (2. * d);
        double aux = (1. + aa * aa + bb * bb) / ((1. + aa * aa) * (1. + bb * bb));
        return 4. * std::acos(std::sqrt(aux));
    };

    if (std::abs(projection->y()) == 0 && projection->z() == 0) /* -- the on-axis case -- */ {
        return omega(height, width, std::abs(projection->x()));
    }

    // otherwise, decomposition of off-axis geometry into four possible symmetric cases
    if ((std::abs(projection->y()) > height/2.) && (std::abs(projection->z()) > width/2.)) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - height/2.;
        B = std::abs(projection->z()) - width/2.;
        a = height;
        b = width;
        d = std::abs(projection->x());
        double to_return = (omega(2*(A+a),2*(B+b),d)-omega(2*A,2*(B+b),d)-omega(2*(A+a),2*B,d)+omega(2*A,2*B,d))/4.0;
        return to_return;
    } 

    if ((std::abs(projection->y()) <= height/2.) && (std::abs(projection->z()) <= width/2.)) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + height/2.;
        B = -std::abs(projection->z()) + width/2.;
        a = height;
        b = width;
        d = std::abs(projection->x());
        double to_return = (omega(2*(a-A),2*(b-B),d)+omega(2*A,2*(b-B),d)+omega(2*(a-A),2*B,d)+omega(2*A,2*B,d))/4.0;
        return to_return;
    }

    if ((std::abs(projection->y()) <= height/2.) && (std::abs(projection->z()) > width/2.)) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - height/2.;
        B = -std::abs(projection->z()) + width/2.;
        a = height;
        b = width;
        d = std::abs(projection->x());
        double to_return = (omega(2*(A+a),2*(b-B),d)-omega(2*A,2*(b-B),d)+omega(2*(A+a),2*B,d)-omega(2*A,2*B,d))/4.0;
        return to_return;
    }

    if ((std::abs(projection->y()) > height/2.) && (std::abs(projection->z()) <= width/2.)) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + height/2.;
        B = std::abs(projection->z()) - width/2.;
        a = height;
        b = width;
        d = std::abs(projection->x());
        double to_return = (omega(2*(a-A),2*(B+b),d)-omega(2*(a-A),2*B,d)+omega(2*A,2*(B+b),d)-omega(2*A,2*B,d))/4.0;
        return to_return;
    }
    
    throw std::invalid_argument("-- Solid angle calculation failed.");   
}

double AnalyticalOptics::AbsorptionQuenching(double distance) {
    return std::exp(- distance / MaterialProperties::getInstance()->getMaterialProperties()->absorption_length);
}
#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

double AnalyticalOptics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor) {
    // https://arxiv.org/pdf/2010.00324.pdf
    Eigen::Vector3d separation = sensor->getPosition() - a_photon_radiant->position;
    double distance = separation.norm();

    double solid_angle = CalculateSolidAngle(sensor, &separation);
    double visibility = solid_angle / (4 * M_PI);

    return AbsorptionQuenching(distance) * visibility;
}

double AnalyticalOptics::CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation) {
    double solid_angle = 0.;

    Eigen::Vector3d projection = CreateProjectionGeometry(sensor, separation);
    if (sensor->isRectangular()) {
        auto [width, height] = sensor->getDimensions();
        solid_angle += RectangularSolidAngle(separation, width, height);
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
        double aa = std::pow(a / (2 * d), 2);
        double bb = std::pow(b / (2 * d), 2);

        double acos_arg = (1 + aa + bb) / ((1 + aa) * (1 + bb));

        return 4 * std::acos(std::sqrt(acos_arg));
    };

    if (std::abs(projection->y()) == 0 && projection->z() == 0) /* -- the on-axis case -- */ {
        return omega(width, height, projection->x());
    }

    // otherwise, decomposition of off-axis geometry into four possible symmetric cases
    if ((std::abs(projection->y()) > width/2.) && std::abs(projection->z()) > height/2.) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - width/2.;
        B = std::abs(projection->z()) - height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        return (omega(2*(A+a),2*(B+b),d)-omega(2*A,2*(B+b),d)-omega(2*(A+a),2*B,d)+omega(2*A,2*B,d))/4.0;
    } 

    if ((std::abs(projection->y()) <= width/2.) && std::abs(projection->z()) <= height/2.) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + width/2.;
        B = -std::abs(projection->z()) + height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        return (omega(2*(a-A),2*(b-B),d)+omega(2*A,2*(b-B),d)+omega(2*(a-A),2*B,d)+omega(2*A,2*B,d))/4.0;
    }

    if ((std::abs(projection->y()) > width/2.) && (std::abs(projection->z()) <= height/2.)) {
        double A, B, a, b, d;
        A = std::abs(projection->y()) - width/2.;
        B = -std::abs(projection->z()) + height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        return (omega(2*(A+a),2*(b-B),d)-omega(2*A,2*(b-B),d)+omega(2*(A+a),2*B,d)-omega(2*A,2*B,d))/4.0;
    }

    if ((std::abs(projection->y()) <= width/2.) && (std::abs(projection->z()) > height/2.)) {
        double A, B, a, b, d;
        A = -std::abs(projection->y()) + width/2.;
        B = std::abs(projection->z()) - height/2.;
        a = width;
        b = height;
        d = std::abs(projection->x());
        return (omega(2*(a-A),2*(B+b),d)-omega(2*(a-A),2*B,d)+omega(2*A,2*(B+b),d)-omega(2*A,2*B,d))/4.0;
    }
    
    //throw std::invalid_argument("-- Solid angle calculation failed.");   

    std::cout << "-- Solid angle calculation failed." << std::endl;
    return 0.; 
}

double AnalyticalOptics::AbsorptionQuenching(double distance) {
    return std::exp(- distance / MaterialProperties::get_instance()->get_material_properties()->absorption_length);
}
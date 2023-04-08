#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

double AnalyticalOptics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor) {
    // https://arxiv.org/pdf/2010.00324.pdf
    Eigen::Vector3d separation = sensor->position - a_photon_radiant->position;
    double distance = separation.norm();

    double solid_angle = CalculateSolidAngle(sensor, separation);
    double visibility = solid_angle / (4 * M_PI);

    return AbsorptionQuenching(distance) * visibility;
}

double CalculateSolidAngle(const OpticalSensor& sensor, const Eigen::Vector3d& separation) {
    double solid_angle = 0.;
    if (auto square = std::get_if<std::unique_ptr<SquareSensor>>(&sensor.shape)) {
        solid_angle += SquareSolidAngle(separation, (*square)->size);
    } 
    
    else if (auto rectangular = std::get_if<std::unique_ptr<RectangularSensor>>(&sensor.shape)) {
        solid_angle += RectangularSolidAngle(separation, (*rectangular)->width, (*rectangular)->height);
    } 
    
    else if (auto circular = std::get_if<std::unique_ptr<CircularSensor>>(&sensor.shape)) {
        solid_angle += CircularSolidAngle(separation, (*circular)->radius);
    } 
    
    else {
        throw std::invalid_argument("-- Invalid optical detector shape.");
    }

    return solid_angle;
}

Eigen::Vector3d AnalyticalOptics::CreateProjectionGeometry(const OpticalSensor& sensor, const Eigen::Vector3d& separation) {
    Eigen::Vector3d projection();
    if (auto sensor.orienation == PlaneOrientation::X_POS || auto sensor.orienation == PlaneOrientation::X_NEG) {
        projection = separation;
    } 
    
    else if (auto sensor.orienation == PlaneOrientation::Y_POS || auto sensor.orienation == PlaneOrientation::Y_NEG) {
        projection = Eigen::Vector3d(separation.y(), separation.x(), separation.z());
    } 
    
    else if (auto sensor.orienation == PlaneOrientation::Z_POS || auto sensor.orienation == PlaneOrientation::Z_NEG) {
        projection = Eigen::Vector3d(separation.z(), separation.y(), separation.x());
    } 
    
    else {
        throw std::invalid_argument("-- Invalid optical detector [lane orientation.");
    }

    return projection();
}

double AnalyticalOptics::RectangularSolidAngle(const Eigen::Vector3d& separation, const OpticalSensor& sensor) {
    // https://vixra.org/pdf/2001.0603v2.pdf
    Eigen::Vector3d projection = CreateProjectionGeometry(separation, sensor);
    
    auto omega = [&](double a, double b, double d) -> double {
        double aa = std::pow(a / (2 * d), 2);
        double bb = std::pow(b / (2 * d), 2);

        double acos_arg = (1 + aa + bb) / ((1 + aa) * (1 + bb));

        return 4 * std::acos(std::sqrt(acos_arg));
    };

    if (std::abs(projection.y()) == 0 && projection.z() == 0) /* -- the on-axis case -- */ {
        return omega(sensor.shape.width, sensor.shape.height, projection.x());
    }

    // otherwise, decomposition of off-axis geometry into four possible symmetric cases
    if ((std::abs(projection.y()) > sensor.shape.width * 0.5) && std::abs(projection.z()) > sensor.shape.height * 0.5) {
        double A, B, a, b, d;
        A = std::abs(projection.y()) - sensor.shape.width * 0.5;
        B = std::abs(projection.z()) - sensor.shape.height * 0.5;
        a = sensor.shape.width;
        b = sensor.shape.height;
        d = std::abs(projection.x());
        return (omega(2*(A+a),2*(B+b),d)-omega(2*A,2*(B+b),d)-omega(2*(A+a),2*B,d)+omega(2*A,2*B,d))/4.0;
    } 

    if ((std::abs(projection.y()) <= sensor.shape.width * 0.5) && std::abs(projection.z()) <= sensor.shape.height * 0.5) {
        double A, B, a, b, d;
        A = -std::abs(projection.y()) + sensor.shape.width * 0.5;
        B = -std::abs(projection.z()) + sensor.shape.height * 0.5;
        a = sensor.shape.width;
        b = sensor.shape.height;
        d = std::abs(projection.x());
        return (omega(2*(a-A),2*(b-B),d)+omega(2*A,2*(b-B),d)+omega(2*(a-A),2*B,d)+omega(2*A,2*B,d))/4.0;
    }

    if ((std::abs(projection.y()) > sensor.shape.width * 0.5) && (std::abs(projection.z()) <= sensor.shape.height * 0.5)) {
        double A, B, a, b, d;
        A = std::abs(projection.y()) - sensor.shape.width * 0.5;
        B = -std::abs(projection.z()) + sensor.shape.height * 0.5;
        a = sensor.shape.width;
        b = sensor.shape.height;
        d = std::abs(projection.x());
        return (omega(2*(A+a),2*(b-B),d)-omega(2*A,2*(b-B),d)+omega(2*(A+a),2*B,d)-omega(2*A,2*B,d))/4.0;
    }

    if ((std::abs(projection.y() <= sensor.width * 0.5)) && (std::abs(projection.z() > sensor.shape.height * 0.5))) {
        double A, B, a, b, d;
        A = -std::abs(projection.y()) + sensor.shape.width * 0.5;
        B = std::abs(projection.z()) - sensor.shape.height * 0.5;
        a = sensor.shape.width;
        b = sensor.shape.height;
        d = std::abs(projection.x());
        return (omega(2*(a-A),2*(B+b),d)-omega(2*(a-A),2*B,d)+omega(2*A,2*(B+b),d)-omega(2*A,2*B,d))/4.0;
    }
    
    throw std::invalid_argument("-- Invalid optical detector shape.");    
}

double AnalyticalOptics::AbsorptionQuenching(double distance, double mean_absorption_length) {
    return std::exp(- distance / mean_absorption_length);
}
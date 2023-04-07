#include "AnalyticalOptics.hh"

AnalyticalOptics::AnalyticalOptics() {}

AnalyticalOptics::~AnalyticalOptics() {}

double AnalyticalOptics::GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalDetector* a_optical_detector) {
    // https://arxiv.org/pdf/2010.00324.pdf
    Eigen::Vector3d separation = a_optical_detector->position - a_photon_radiant->position;
    double distance = separation.norm();
    
    double cos_theta = 0.0;
    if (a_optical_detector.orientation == "X-" || a_optical_detector.orientation == "X+") {
        cos_theta = std::abs(sqrt(pow(a_photon_radiant.position.y() - a_photon_radiant.position.y(),2))) / distance;
    }
    else {
        cos_theta = std::abs(sqrt(pow(a_photon_radiant.position.x() - a_photon_radiant.position.x(),2))) / distance;
    }

    const double theta = std::acos(cos_theta) * 180. / M_PI;

    double solid_angle = 0.0;
    switch (a_optical_detector.shape) {
    case 0: 
        solid_angle = RectangularSolidAngle(separation);
        break;
    
    default:
        throw std::invalid_argument("Invalid optical detector shape.");
    }

    double SolidAngleVisibility = solid_angle / (4 * M_PI);

    return AbsorptionQuenching(distance) * SolidAngleVisibility;
}

double AnalyticalOptics::RectangularSolidAngle(const Eigen::Vector3d* separation, const OpticalDetector* a_optical_detector) {
    // https://vixra.org/pdf/2001.0603v2.pdf
    
    auto on_axis_geometry = [&](double a, double b, double d) -> double {
        double aa = std::pow(a / (2 * d), 2);
        double bb = std::pow(b / (2 * d), 2);

        double acos_arg = (1 + aa + bb) / ((1 + aa) * (1 + bb));
        return 4 * std::acos(std::sqrt(acos_arg));
    };

    if (std::abs(separation.y()) == 0 && separation.z() == 0) /* -- the on-axis case -- */ {
        return on_axis_geometry();
    }

    // otherwise, decomposition of off-axis geometry into four possible symmetric cases
    else {
        if ((std::abs(separation.y()) > a_optical_detector.width * 0.5) && std::abs(separation.z()) > a_optical_detector.height * 0.5) {
            double A, B, a, b, d;
        } 
    }
}

double AnalyticalOptics::AbsorptionQuenching(double distance, double mean_absorption_length) {
    return std::exp(- distance / mean_absorption_length);
}
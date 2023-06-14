#include "Propagation.hh"
//_________________________________________________________________________________________
Propagation::Propagation() {}
//_________________________________________________________________________________________
Propagation::~Propagation() {}
//_________________________________________________________________________________________
double Propagation::SampleArrivalTime(const PhotonRadiant* photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor* optical_sensor, const double detector_width, const double detector_height, const double detector_depth) {
    std::vector<double> t_values;
    double t_start = 1.0;
    double t_end = 1000.0;
    double t_step = 1.0;
    int num_steps = std::floor((t_end - t_start) / t_step);
    
    for (int i = 0; i <= num_steps; i++) {
        double t = t_start + i * t_step;
        t_values.push_back(t);
    }

    std::vector<double> p_values;

    double x_b = detector_width / 2; double y_b = detector_height / 2; double z_b = detector_depth / 2;

    double x_0 = photon_radiant->position.x(); double y_0 = photon_radiant->position.y(); double z_0 = photon_radiant->position.z();
    double x_1 = optical_sensor->GetPosition().x(); double y_1 = optical_sensor->GetPosition().y(); double z_1 = optical_sensor->GetPosition().z();
    double wl = optical_photon.GetWavelength();
    PlaneOrientation plane_orient = optical_sensor->GetOrientation();

    for (double t : t_values) {
        double p = FluxProbabilityDensity(x_1, x_0, x_b, y_1, y_0, y_b, z_1, z_0, z_b, t, plane_orient, wl) * t_step;
        p_values.push_back(p);
    }

    double p_sum = 0.0;
    for (double p : p_values) {
        p_sum += p;
    }

    std::vector<double> normalized_p_values;
    if (p_sum != 0.0) {
        for (double p : p_values) {
            double normalized_p = p / static_cast<double>(p_sum);
            normalized_p_values.push_back(normalized_p);
        }
    } else {
        for (double p : p_values) {
            normalized_p_values.push_back(0.0);
        }
    }

    std::vector<double> cumulative_p_values;
    double cumulative_p_sum = 0.0;
    for (double normalized_p : normalized_p_values) {
        cumulative_p_sum += normalized_p;
        cumulative_p_values.push_back(cumulative_p_sum);
    }

    double r = CLHEP::RandFlat::shoot();
    double t_sample = 0.0;
    for (int i = 0; i < cumulative_p_values.size(); i++) {
        if (r <= cumulative_p_values[i]) {
            t_sample = t_values[i];
            break;
        }
    }   

    return t_sample;
}
//_________________________________________________________________________________________
double Propagation::FluxProbabilityDensity(double x, double x_0, double x_b, double y, double y_0, double y_b, double z, double z_0, double z_b, double t, PlaneOrientation plane_orient, double wl) {
    int accuracy_scale = 10;

    double D = CalculateDiffusion(wl);
    double norm = 1.0 / pow(4 * M_PI * D * t, 3.0 / 2.0);

    if (plane_orient == PlaneOrientation::X_POS) {
        return norm * SingleDimensionDiffusionDerivative(+x_b, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(y, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(z, z_0, z_b, t, D, accuracy_scale);
    } else if (plane_orient == PlaneOrientation::X_NEG) {
        return norm * SingleDimensionDiffusionDerivative(-x_b, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(y, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(z, z_0, z_b, t, D, accuracy_scale);
    } else if (plane_orient == PlaneOrientation::Y_POS) {
        return norm * SingleDimensionDiffusion(x, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusionDerivative(+y_b, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(z, z_0, z_b, t, D, accuracy_scale);
    } else if (plane_orient == PlaneOrientation::Y_NEG) {
        return norm * SingleDimensionDiffusion(x, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusionDerivative(-y_b, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(z, z_0, z_b, t, D, accuracy_scale);
    } else if (plane_orient == PlaneOrientation::Z_POS) {
        return norm * SingleDimensionDiffusion(x, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(y, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusionDerivative(+z_b, z_0, z_b, t, D, accuracy_scale);
    } else if (plane_orient == PlaneOrientation::Z_NEG) {
        return norm * SingleDimensionDiffusion(x, x_0, x_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusion(y, y_0, y_b, t, D, accuracy_scale) 
                    * SingleDimensionDiffusionDerivative(-z_b, z_0, z_b, t, D, accuracy_scale);
    } else {
        throw std::invalid_argument("-- Invalid plane orientation passed to propagation class.");
    }
}
//_________________________________________________________________________________________
double Propagation::CalculateDiffusion(const double wl) {
    double convert_m_per_s_to_mm_per_ns = 1e3 * 1e-9;

    double anisotropy = 0.0;
    double rayleigh_length = MediumProperties::GetInstance()->GetRayleighScatteringLength(wl);
    double velocity = MediumProperties::GetInstance()->GetGroupVelocity(wl) * convert_m_per_s_to_mm_per_ns;

    double D = (velocity / (6 * ((1.0 - anisotropy) * (1.0 / rayleigh_length))));
    return D;
}
//_________________________________________________________________________________________
double Propagation::SingleDimensionDiffusion(double x, double x_0, double x_b, double t, double D, int accuracy_scale) {
    double prob_density = 0.0;
    for (int n = -accuracy_scale; n < accuracy_scale; n++) {
        double series_term = exp(-pow(x - x_0 + (2 * n * x_b), 2) / (4 * D * t));
        if (n % 2 == 0) {
            prob_density += series_term;
        } else {
            prob_density -= series_term;
        }
    }
    prob_density = (prob_density < 0) ? 0 : prob_density;
    return prob_density;
}
//_________________________________________________________________________________________
double Propagation::SingleDimensionDiffusionDerivative(double x, double x_0, double x_b, double t, double D, int accuracy_scale) {
    double prob_density = 0.0;
    for (int n = -accuracy_scale; n < accuracy_scale; n++) {
        double series_term = ((2 * (2 * n * x_b)) / (4 * D * t)) * exp(-pow(x - x_0 + (2 * n * x_b), 2) / (4 * D * t));
        if (n % 2 == 0) {
            prob_density += series_term;
        } else {
            prob_density -= series_term;
        }
    }
    prob_density = (prob_density < 0) ? 0 : prob_density;
    return prob_density;
}
#include "PropagationTime.hh"
//_________________________________________________________________________________________
PropagationTime* PropagationTime::instance_ = nullptr;
//_________________________________________________________________________________________
PropagationTime::PropagationTime() {
    // *************************************************************************************************
	// Discretisation of time parameterisations
	// *************************************************************************************************

	// *************************************************************************************************
	// Definition of the parameters for timings parameterisation: DUNE-SP, RS = 99.9cm, 45 degree bins
	// *************************************************************************************************
	// Parameters of the Landau + Exponential (<= 350 cm) and Landau (> 350 cm) models
    parameters_distances_landau_ = {0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650, 675, 700, 725};
    parameters_norm_over_entries_ = {
		{0, 27.41, 2.57134, 1.47775, 0.96034, 0.668723, 0.487808, 0.362789, 0.277659, 0.224883, 0.185199, 0.159048, 0.143393, 0.134121, 0.125978, 0.115751, 0.0921523, 0.0907023, 0.0856498, 0.0814134, 0.0750844, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 3.79032, 1.25862, 0.928939, 0.566822, 0.319336, 0.331336, 0.219171, 0.178579, 0.135635, 0.119011, 0.109138, 0.0991792, 0.091753, 0.0787281, 0.0743768, 0.0663083, 0.0648284, 0.0608437, 0.0572599, 0.0561134, 0.0543579, 0.0529609, 0.0519021, 0.0510434, 0.0504866, 0.0503902, 0.0507405, 0.0503883, 0.0497511}
	};	
    parameters_most_probable_value_ = {
		{0, 1.77134, 3.85131, 5.593, 7.46645, 9.44108, 11.5899, 13.8189, 16.2753, 18.8619, 21.6428, 24.4828, 27.189, 29.6607, 32.3706, 35.2212, 41.4392, 43.2475, 45.939, 48.5748, 51.4841, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1.91282, 4.31774, 5.90297, 8.90329, 11.8104, 12.9629, 17.0555, 19.5031, 24.7774, 27.175, 29.8128, 33.5619, 36.6553, 43.0021, 46.059, 52.6953, 55.1056, 59.9728, 65.2979, 67.9593, 71.0675, 74.7855, 79.689, 85.3232, 90.818, 97.0409, 103.2, 111.306, 117.6}
	};
    parameters_width_ = {
		{0, 0.10303, 0.313506, 0.525492, 0.716541, 0.969894, 1.29961, 1.73657, 2.30482, 2.94669, 3.71951, 4.53258, 5.24435, 5.79945, 6.53951, 7.34736, 12.0305, 12.4263, 13.4917, 14.7245, 16.6202, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0.281535, 0.633816, 0.80694, 1.46701, 2.32209, 2.20895, 3.56751, 4.23966, 6.11748, 6.97923, 7.82035, 9.05225, 10.1422, 12.6053, 13.6305, 17.7093, 18.3244, 19.9961, 22.1072, 23.4653, 25.3052, 27.5297, 30.5683, 34.8304, 39.7456, 45.1447, 51.3363, 58.4681, 63.0122}
	};
    parameters_distances_exp_ = {0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375};
    parameters_slope_ = {
		{0, -0.205783, -0.157344, -0.124546, -0.0949427, -0.0640574, -0.0509928, -0.0421876, -0.0360771, -0.0327956, -0.02997, -0.0280924, -0.0269181, -0.0259104, -0.0248679, -0.0233511},
		{0, -0.157151, -0.10447, -0.0875509, -0.0605052, -0.0435416, -0.040128, -0.0325951, -0.0292046, -0.0255942, -0.0241776, -0.0226607, -0.0213594, -0.02037, -0.0181767, -0.0172752}
	};
    parameters_expo_over_landau_norm_ = {
		{0, 0.00276789, 0.0362807, 0.081067, 0.111523, 0.106485, 0.123852, 0.144239, 0.168534, 0.201111, 0.234443, 0.269284, 0.300041, 0.320541, 0.342403, 0.356125},
		{0, 0.0242526, 0.086063, 0.101552, 0.134787, 0.171863, 0.152519, 0.203461, 0.224872, 0.279135, 0.304123, 0.317034, 0.341433, 0.360053, 0.382877, 0.391287}
	};

    min_group_velocity_ = 9.; // cm / ns
    mean_group_velocity_ = 13.5; // cm / ns
    max_group_velocity_ = 18.0; // cm / ns

    maximum_distance_ = 2500.; // cm
    minimum_distance_ = 25.; // cm

    distance_step_size_ = 25.; // cm
    angular_step_size_ = 45.; // degrees

    inflexion_point_distance_ = 350.; // cm

    if (!instance_) {
        instance_ = this;
        this->GenerateTimeDistributions();
    }
}
//_________________________________________________________________________________________
PropagationTime::~PropagationTime() {}
//_________________________________________________________________________________________
PropagationTime* PropagationTime::GetInstance() {
    if (!instance_) {
        instance_ = new PropagationTime();
    }
    return instance_;
}
//_________________________________________________________________________________________
double PropagationTime::SamplePropagationTime(double distance, const size_t angle) {
    distance = distance / 10.; // convert to cm

    const size_t num_params = (maximum_distance_ - minimum_distance_) / distance_step_size_;
    const size_t num_angles = std::round(90. / angular_step_size_);

    double propagation_time;
    if (distance <= minimum_distance_) {
        // calculate direct propagation time
        propagation_time = distance / mean_group_velocity_;
    } 
    else {
        int index = std::round((distance - minimum_distance_) / distance_step_size_);
        int angle_bin = std::round(angle / angular_step_size_) - 1;

        if (angle_bin < num_angles && index < num_params) {
            propagation_time = timing_generator_[angle_bin][index].shoot() * (timing_max_[angle_bin][index] - timing_min_[angle_bin][index]) + timing_min_[angle_bin][index];
        } 
        else {
            propagation_time = CLHEP::RandGauss::shoot(distance / mean_group_velocity_, sqrt((distance / min_group_velocity_) - (distance / mean_group_velocity_)));
        }
    }

    return propagation_time;
}
//_________________________________________________________________________________________
void PropagationTime::GenerateTimeDistributions() {
    std::cout << "-- Generating time distributions..." << std::endl;

    const size_t num_params = (maximum_distance_ - minimum_distance_) / distance_step_size_;
    const size_t num_angles = std::round(90. / angular_step_size_);

    TF1 timing_parameterisation;

    double dummy[1] = {1.};
    timing_generator_ = std::vector(num_angles, std::vector(num_params, CLHEP::RandGeneral(dummy, 1)));
    timing_max_ = std::vector(num_angles, std::vector(num_params, 0.));
    timing_min_ = std::vector(num_angles, std::vector(num_params, 0.));

    const double signal_time_range = 5000.;

    for (size_t index = 0; index < num_params; ++index) {
        double distance = (index * distance_step_size_) + minimum_distance_;
        
        double time_direct_mean = distance / mean_group_velocity_;
        double time_direct_min = distance / max_group_velocity_;

        int sampling;
        if (distance < 2. * minimum_distance_) {
            sampling = 10000;
        } 
        else if (distance < 4. * minimum_distance_) {
            sampling = 5000;
        } 
        else {
            sampling = 1000;
        }
        
        for (size_t angle_bin = 0; angle_bin < num_angles; ++angle_bin) {
            std::array<double, 3> pars_landau;
            interpolate3(pars_landau,
                        parameters_distances_landau_,
                        parameters_most_probable_value_[angle_bin],
                        parameters_width_[angle_bin],
                        parameters_norm_over_entries_[angle_bin], 
                        distance,
                        true);
            
            TF1 arrival_timing;
            if (distance >= inflexion_point_distance_) {
                double pars_far[4] = {time_direct_min, pars_landau[0], pars_landau[1], pars_landau[2]};
                arrival_timing = TF1("arrival_timing", ModelFar, 0., signal_time_range, 4);
                arrival_timing.SetParameters(pars_far);
            }
            else {
                double pars_expo[2];
                
                pars_expo[1] = interpolate(parameters_distances_exp_, parameters_slope_[angle_bin], distance, true);
                
                pars_expo[0] = interpolate(parameters_distances_exp_, parameters_expo_over_landau_norm_[angle_bin], distance, true);
                pars_expo[0] *= pars_landau[2];
                pars_expo[0] = std::log(pars_expo[0]);

                TF1 intersection_distance = TF1("intersection_distance", IntersectionDistance, pars_landau[0], 4. * time_direct_mean, 5);

                double pars_intersection[5] = {pars_landau[0], pars_landau[1], pars_landau[2], pars_expo[0], pars_expo[1]};

                intersection_distance.SetParameters(pars_intersection);

                double time_intersection = intersection_distance.GetMinimumX();
                double min_value = intersection_distance.Eval(time_intersection);

                if (min_value > 0.015) {
                    std::cout << "-- The parameterisation of the timing distribution is not valid for this distance: " << distance << std::endl;
                }
                double parameters_final[7] = {time_intersection,
                                            pars_landau[0],
                                            pars_landau[1],
                                            pars_landau[2],
                                            pars_expo[0],
                                            pars_expo[1],
                                            time_direct_min};
                arrival_timing = TF1("arrival_timing", ModelNear, 0., signal_time_range, 7);
                arrival_timing.SetParameters(parameters_final);
            }

            double xq_max[1] = {0.975};
            double yq_max[1];

            arrival_timing.SetNpx(sampling);
            arrival_timing.GetQuantiles(1, yq_max, xq_max);

            double max = yq_max[0];
            double min = time_direct_min;

            arrival_timing.SetRange(min, max); 
            timing_max_[angle_bin][index] = max;
            timing_min_[angle_bin][index] = min;

            auto sampling_histogram = (TH1D*) arrival_timing.GetHistogram();
            std::vector<double> arrival_timings(sampling, 0.);
            for (int i = 0; i < sampling; ++i) {
                arrival_timings[i] = sampling_histogram->GetBinContent(i + 1);
                
                timing_generator_[angle_bin][index] = CLHEP::RandGeneral(arrival_timings.data(), arrival_timings.size());
            } 
        }
    }

    std::cout << "-- Time distributions generated." << std::endl;
}
//_________________________________________________________________________________________
double PropagationTime::IntersectionDistance(const double* x, const double* par) {
    double y1 = par[2] * TMath::Landau(x[0], par[0], par[1]);
    double y2 = TMath::Exp(par[3] + x[0] * par[4]);

    return TMath::Abs(y1 - y2);
}
//_________________________________________________________________________________________
double PropagationTime::ModelNear(const double* x, const double* par) {
    double y1 = par[3] * TMath::Landau(x[0], par[1], par[2]);
    double y2 = TMath::Exp(par[4] + x[0] * par[5]);
    if (x[0] <= par[6] || x[0] > par[0]) y1 = 0.;
    if (x[0] <= par[0]) y2 = 0.;

    return (y1 + y2);
}
//_________________________________________________________________________________________
double PropagationTime::ModelFar(const double* x, const double* par) {
    if (x[0] <= par[0]) return 0.;

    return par[3] * TMath::Landau(x[0], par[1], par[2]);
}
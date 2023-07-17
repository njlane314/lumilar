#ifndef PROPAGATIONTIME_HH
#define PROPAGATIONTIME_HH

#include <iostream>
#include <vector>
#include <cmath>
#include "CLHEP/Random/RandGeneral.h"
#include "CLHEP/Random/RandGauss.h"
#include "TF1.h"
#include "TH1D.h"
#include "TMath.h"

class PropagationTime {
public:
    PropagationTime();
    ~PropagationTime();

    static PropagationTime* GetInstance();

    double SamplePropagationTime(double distance, const size_t angle_bin);
    void GenerateTimeDistributions();

private:

    static double IntersectionDistance(const double* x, const double* par);
    static double ModelNear(const double* x, const double* par);
    static double ModelFar(const double* x, const double* par);

    std::vector<std::vector<CLHEP::RandGeneral>> timing_generator_;
    std::vector<std::vector<double>> timing_max_;
    std::vector<std::vector<double>> timing_min_;

    std::vector<double> parameters_distances_landau_;
    std::vector<std::vector<double>> parameters_norm_over_entries_;
    std::vector<std::vector<double>> parameters_most_probable_value_;
    std::vector<std::vector<double>> parameters_width_;
    std::vector<double> parameters_distances_exp_;
    std::vector<std::vector<double>> parameters_slope_;
    std::vector<std::vector<double>> parameters_expo_over_landau_norm_;

    static PropagationTime* instance_;

    double min_group_velocity_;
    double mean_group_velocity_;
    double max_group_velocity_;

    double maximum_distance_;
    double minimum_distance_;
    double distance_step_size_;
    double angular_step_size_;
    double inflexion_point_distance_;

    double interpolate(const std::vector<double> &x_data, 
                    const std::vector<double> &y_data, 
                    const double &x, 
                    const bool &extrapolate) {
        int size = x_data.size();
        int i = 0;                                         
        if (x >= x_data[size - 2]) {
            i = size - 2;
        }
        else {
            while (x > x_data[i+1]) i++;
        }
        double xL = x_data[i], yL = y_data[i], xR = x_data[i+1], yR = y_data[i+1]; 
        if (!extrapolate) {
            if (x < xL) yR = yL;
            if (x > xR) yL = yR;
        }
        double dydx = (yR - yL) / (xR - xL);   

        return yL + dydx * (x - xL);   
    }

    void interpolate3(std::array<double, 3>& inter,
                                         const std::vector<double>& xData,
                                         const std::vector<double>& yData1,
                                         const std::vector<double>& yData2,
                                         const std::vector<double>& yData3,
                                         double x,
                                         bool extrapolate) const {
        size_t size = xData.size();
        size_t i = 0;               
        if (x >= xData[size - 2]) { 
        i = size - 2;
        }
        else {
        while (x > xData[i + 1])
            i++;
        }
        double xL = xData[i];
        double xR = xData[i + 1]; 
        double yL1 = yData1[i];
        double yR1 = yData1[i + 1];
        double yL2 = yData2[i];
        double yR2 = yData2[i + 1];
        double yL3 = yData3[i];
        double yR3 = yData3[i + 1];

        if (!extrapolate) { 
        if (x < xL) {
            inter[0] = yL1;
            inter[1] = yL2;
            inter[2] = yL3;
            return;
        }
        if (x > xR) {
            inter[0] = yL1;
            inter[1] = yL2;
            inter[2] = yL3;
            return;
        }
        }
        const double m = (x - xL) / (xR - xL);
        inter[0] = m * (yR1 - yL1) + yL1;
        inter[1] = m * (yR2 - yL2) + yL2;
        inter[2] = m * (yR3 - yL3) + yL3;
    }
};

#endif // PROPAGATIONTIME_HH
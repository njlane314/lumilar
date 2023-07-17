#ifndef ELECTRONCLOUD_HH
#define ELECTRONCLOUD_HH

#include <vector>

#include <Eigen/Core>

class ElectronCloud {
public:
    ElectronCloud(int cloud_electron_count, double cloud_time, Eigen::Vector3d cloud_position) : cloud_electron_count_(cloud_electron_count), cloud_time_(cloud_time), cloud_position_(cloud_position) {};
    ~ElectronCloud() {};

    int GetElectronCount() const {
        return cloud_electron_count_;
    }

    Eigen::Vector3d GetPosition() const {
        return cloud_position_;
    }

    double GetTime() const {
        return cloud_time_;
    }

private:
    int cloud_electron_count_;
    double cloud_time_;
    Eigen::Vector3d cloud_position_;
};

#endif // ELECTRONCLOUD_HH
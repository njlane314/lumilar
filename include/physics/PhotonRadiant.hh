#ifndef PHOTONRADIANT_HH
#define PHOTONRADIANT_HH

#include <vector>

#include <Eigen/Core>

class PhotonRadiant {
public: 
    PhotonRadiant(int radiant_photon_count, double radiant_time, Eigen::Vector3d radiant_position) : radiant_photon_count_(radiant_photon_count), radiant_time_(radiant_time), radiant_position_(radiant_position) {};
    ~PhotonRadiant() {};

    int GetPhotonCount() const {
        return radiant_photon_count_;
    }

    Eigen::Vector3d GetPosition() const {
        return radiant_position_;
    }

    double GetTime() const {
        return radiant_time_;
    }

private:
    int radiant_photon_count_;
    double radiant_time_;
    Eigen::Vector3d radiant_position_;
};

#endif // PHOTONRADIANT_HH
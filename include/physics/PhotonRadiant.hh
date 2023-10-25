#ifndef PHOTONRADIANT_HH
#define PHOTONRADIANT_HH

#include <vector>

#include <Eigen/Core>

#include "LArNEST.h"

class PhotonRadiant {
public: 
    PhotonRadiant(int radiant_photon_count, double radiant_time, Eigen::Vector3d radiant_position, std::string particle_name) : radiant_photon_count_(radiant_photon_count), radiant_time_(radiant_time), radiant_position_(radiant_position), particle_name_(particle_name) {};
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

    std::string GetParticleName() const {
        return particle_name_;
    }

private:
    int radiant_photon_count_;
    double radiant_time_;
    Eigen::Vector3d radiant_position_;
    std::string particle_name_;
};

#endif // PHOTONRADIANT_HH
#ifndef PHOTONRADIANT_HH
#define PHOTONRADIANT_HH

#include <iostream>
#include <vector>

#include <Eigen/Core>

#include "OpticalPhoton.hh"

class PhotonRadiant {
 public: 
    PhotonRadiant() {};
    PhotonRadiant(Eigen::Vector3d position, std::vector<OpticalPhoton> optical_photons) : position_(position), optical_photons_(optical_photons) {};
    ~PhotonRadiant() {};

    Eigen::Vector3d GetPosition() const {
        return position_;
    }
    
    std::vector<OpticalPhoton> GetOpticalPhotons() const {
        return optical_photons_;
    }

    void SetPosition(Eigen::Vector3d position) {
        position_ = position;
    }

    void SetOpticalPhotons(std::vector<OpticalPhoton> optical_photons) {
        optical_photons_ = optical_photons;
    }

 private:
    Eigen::Vector3d position_;
    std::vector<OpticalPhoton> optical_photons_;
};

#endif // PHOTONRADIANT_HH
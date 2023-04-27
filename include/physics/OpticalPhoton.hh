#ifndef OPTICAL_PHOTON_HH
#define OPTICAL_PHOTON_HH

class OpticalPhoton {

public:
    OpticalPhoton(double emission_time = 0.0) : emission_time_(emission_time) {}

    void setEmissionTime(double emission_time) { emission_time_ = emission_time; }
    double getEmissionTime() const { return emission_time_; }

private:
    double emission_time_;
};

#endif // OPTICAL_PHOTON_HH
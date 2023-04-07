#ifndef OPTICAL_PHOTON_HH
#define OPTICAL_PHOTON_HH

class OpticalPhoton {

public:
    OpticalPhoton(double emission_time = 0.0) : emission_time_(emission_time) {}

    double get_emission_time() const { return emission_time_; }
    void set_emission_time(double emission_time) { emission_time_ = emission_time; }

private:
    double emission_time_;
};

#endif // OPTICAL_PHOTON_HH
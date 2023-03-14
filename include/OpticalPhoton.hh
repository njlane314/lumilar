#ifndef OPTICAL_PHOTON_HH
#define OPTICAL_PHOTON_HH

class OpticalPhoton {

public:
    OpticalPhoton(double emission_time = 0.0, double wavelength = 0.0) : emission_time_(emission_time), wavelength_(wavelength) {}

    double get_emission_time() const { return emission_time_; }
    void set_emission_time(double emission_time) { emission_time_ = emission_time; }

    double get_wavelength() const { return wavelength_; }
    void set_wavelength(double wavelength) { wavelength_ = wavelength; }

private:
    double emission_time_;
    double wavelength_;
};

#endif // OPTICAL_PHOTON_HH
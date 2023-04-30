#ifndef OPTICAL_PHOTON_HH
#define OPTICAL_PHOTON_HH

class OpticalPhoton {

public:
    OpticalPhoton(double emission_time = -1., double wavelength = -1., double arrival_time = -1.) : emission_time_(emission_time), wavelength_(wavelength), arrival_time_(arrival_time) {}

    void SetEmissionTime(double emission_time) { emission_time_ = emission_time; }
    double GetEmissionTime() const { return emission_time_; }

    void SetWavelength(double wavelength) { wavelength_ = wavelength; }
    double GetWavelength() const { return wavelength_; }

    void SetArrivalTime(double arrival_time) { arrival_time_ = arrival_time; }
    double GetArrivalTime() const { return arrival_time_; }

    bool operator==(const OpticalPhoton& other) const {
        return emission_time_ == other.emission_time_ &&
               wavelength_ == other.wavelength_ &&
               arrival_time_ == other.arrival_time_;
    }

private:
    double emission_time_;
    double wavelength_;
    double arrival_time_;
};

#endif // OPTICAL_PHOTON_HH
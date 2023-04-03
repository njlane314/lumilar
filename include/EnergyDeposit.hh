#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

class EnergyDeposit {
public:
    EnergyDeposit(double visible, double linear_transfer, std::string particle_type, std::vector<double> position, double length, double time) : visible(visible), linear_transfer(linear_transfer), particle_type(particle_type), position(position), length(length), time(time) {};
    ~EnergyDeposit() {};

    double get_visible_energy() const { return visible; }
    void set_visible_energy(double value) { visible = value; }
    
    double get_linear_transfer() const { return linear_transfer; }
    void set_linear_transfer(double value) { linear_transfer = value; }
    
    std::string get_particle_type() const { return particle_type; }
    void set_particle_type(std::string value) { particle_type = value; }
    
    std::vector<double> get_position() const { return position; }
    void set_position(std::vector<double> value) { position = value; }

    double get_length() const { return length; }
    void set_length(double value) { length = value; }
    
    double get_time() const { return time; }
    void set_time(double value) { time = value; }
    
private:
    double visible;
    double linear_transfer;
    std::string particle_type;
    std::vector<double> position;
    double length;
    double time;
};

#endif // ENERGY_DEPOSIT_HH
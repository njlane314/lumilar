#ifndef EJECTION_HH
#define EJECTION_HH

#include "ThermalElectron.hh"

class Ejection {
 public:
    Ejection();
    ~Ejection();
    
    ThermalElectron create_electron();
};

#endif // EJECTION_HH
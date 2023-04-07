#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include "MaterialProperties.hh"
#include "Excitation.hh"
#include "Recombination.hh"
#include "EnergyDeposit.hh"

#include <random>
#include <string>
#include <tuple>

class MediumResponse {
 public:
   MediumResponse();
   ~MediumResponse();
   std::pair<double, double> create_response(const EnergyDeposit* energy_deposit);
    
 private:
   Properties* material_properties_;
};

#endif // MEDIUM_RESPONSE_HH

#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include "MaterialProperties.hh"
#include "Excitation.hh"
#include "Recombination.hh"
#include "EnergyDeposit.hh"
#include "Signal.hh"

#include "G4Step.hh"

#include <random>
#include <string>
#include <tuple>

class MediumResponse {
public:
   MediumResponse();
   ~MediumResponse();
   static void ProcessResponse(const G4Step* step);
    
private:
   static EnergyDeposit* CreateEnergyDeposit(const G4Step* step);
};

#endif // MEDIUM_RESPONSE_HH

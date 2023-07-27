#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include <random>
#include <string>
#include <tuple>

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

#include "Signal.hh"
#include "EnergyDeposit.hh"

#include "LArNEST.h"

#include <chrono>
#include <thread>

class MediumResponse {
public:
   MediumResponse();
   ~MediumResponse();

   static  void ProcessResponse(const G4Step* step);
    
private:
   static EnergyDeposit* CreateEnergyDeposit(const G4Step* step);

   static bool isStepInLiquidArgon(const G4Step* step);
   static bool isParticleCharged(const G4Step* step);
   
   static void PrintResponse(const larnest::LArNESTResult& result);
};

#endif // MEDIUM_RESPONSE_HH
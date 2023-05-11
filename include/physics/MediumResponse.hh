//____________________________________________________________________________
/*!

\class   physics::MediumResponse

\brief   This class interfaces with the discrete step creation and medium response
        methods.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include <random>
#include <string>
#include <tuple>

#include "G4Step.hh"

#include "MediumProperties.hh"
#include "Signal.hh"
#include "Recombination.hh"
#include "Excitation.hh"
#include "EnergyDeposit.hh"

class MediumResponse {
public:
   MediumResponse();
   ~MediumResponse();

   static void ProcessResponse(const G4Step* step);
    
private:
   static EnergyDeposit* CreateEnergyDeposit(const G4Step* step);
};

#endif // MEDIUM_RESPONSE_HH

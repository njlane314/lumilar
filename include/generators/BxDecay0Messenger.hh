//____________________________________________________________________________
/*!

\class   generators::BxDecay0Messenger

\brief   This class sets the parameters of the radiological generator for the simulation.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 13, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef BXDECAY0G4_PRIMARY_GENERATOR_ACTION_MESSENGER_HH
#define BXDECAY0G4_PRIMARY_GENERATOR_ACTION_MESSENGER_HH

#include <memory>

#include <G4UImessenger.hh>
#include <globals.hh>

class G4UIcmdWithABool;
class G4UIcommand;

#include "BxDecay0Generator.hh"

namespace BxDecay0Generator {
  class PrimaryGeneratorActionMessenger : public G4UImessenger {
  public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction * );
    ~PrimaryGeneratorActionMessenger() override;

    G4String GetCurrentValue (G4UIcommand * command_) override;
    void SetNewValue(G4UIcommand *, G4String) override;
    
  private:
    PrimaryGeneratorAction * _pga_ = nullptr;    
    G4UIdirectory *          _pga_directory_ = nullptr;
    G4UIdirectory *          _pga_gtor_directory_ = nullptr;
    G4UIcommand *            _pga_gtor_log_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_bkgd_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_dbd_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_dbdr_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_mdl_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_mdlr_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_apply_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_destroy_cmd_ = nullptr;
    G4UIcommand *            _pga_gtor_dump_cmd_ = nullptr;
  };
}

#endif // BXDECAY0G4_PRIMARY_GENERATOR_ACTION_MESSENGER_HH
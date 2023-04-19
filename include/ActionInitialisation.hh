#ifndef ACTION_INITIALISATION_HH
#define ACTION_INITIALISATION_HH

#include <string>

#include "G4VUserActionInitialization.hh"

class ActionInitialisation : public G4VUserActionInitialization
{
public:
	ActionInitialisation(std::string output_filename);
	virtual ~ActionInitialisation();

  	virtual void Build() const;
private:
	std::string output_filename_;
};

#endif // ACTION_INITIALISATION_HH
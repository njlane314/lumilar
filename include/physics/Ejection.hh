//____________________________________________________________________________
/*!

\class   physics::Ejection

\brief   The class generates thermal electrons.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef EJECTION_HH
#define EJECTION_HH

#include "ThermalElectron.hh"

class Ejection {
public:
    Ejection();
    ~Ejection();
    
    ThermalElectron CreateThermalElectron();
};

#endif // EJECTION_HH
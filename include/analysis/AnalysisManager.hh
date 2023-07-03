//____________________________________________________________________________
/*!

\class   analysis::AnalysisManager

\brief   This class manages the analysis output, through a singleton pattern, 
        with methods for setting and getting the output filename.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include <string>
#include "AnalysisMessenger.hh"

class AnalysisManager {
public:
    AnalysisManager(AnalysisMessenger* analysis_messenger);
    ~AnalysisManager();

    static AnalysisManager* GetInstance();
    void SetOutputFilename(std::string filename);
    std::string GetOutputFilename() const;
    void SetAnalysisFilename(std::string filename);
    std::string GetAnalysisFilename() const;

    bool IsHitsEnabled() const;
    bool IsCalorimetryEnabled() const;
    bool IsPulseShapeEnabled() const;

private:
    static AnalysisManager* instance_;
    std::string output_filename_;
    std::string analysis_filename_;

    AnalysisMessenger* analysis_messenger_;
};

#endif // ANALYSIS_MANAGER_HH

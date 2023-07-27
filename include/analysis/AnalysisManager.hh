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

class AnalysisManager {
public:
    AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* GetInstance();
    void SetOutputFilename(std::string filename);
    std::string GetOutputFilename() const;

private:
    static AnalysisManager* instance_;
    std::string output_filename_;
};

#endif // ANALYSIS_MANAGER_HH

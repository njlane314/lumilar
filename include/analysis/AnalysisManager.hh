#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include <string>

class AnalysisManager {
public:
    AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* GetInstance();
    void setOutputFilename(std::string output_filename);
    std::string getOutputFilename() const;

private:
    static AnalysisManager* instance_;
    std::string output_filename;
};

#endif // ANALYSIS_MANAGER_HH

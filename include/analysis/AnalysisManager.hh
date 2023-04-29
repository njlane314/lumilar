#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include <string>

class AnalysisManager {
public:
    AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* GetInstance();
    void SetOutputFilename(std::string output_filename);
    std::string GetOutputFilename() const;

private:
    static AnalysisManager* instance_;
    std::string output_filename;
};

#endif // ANALYSIS_MANAGER_HH

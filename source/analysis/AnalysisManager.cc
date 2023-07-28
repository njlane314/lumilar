#include "AnalysisManager.hh"
//_________________________________________________________________________________________
AnalysisManager* AnalysisManager::instance_ = nullptr;
//_________________________________________________________________________________________
AnalysisManager::AnalysisManager() {
    if (!instance_) {
        instance_ = this;
    } 
}
//_________________________________________________________________________________________
AnalysisManager::~AnalysisManager() {}
//_________________________________________________________________________________________
AnalysisManager* AnalysisManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new AnalysisManager();
    }
    return instance_;
}
//_________________________________________________________________________________________
void AnalysisManager::SetOutputFilename(std::string filename) {
    this->output_filename_ = filename;
}
//_________________________________________________________________________________________
std::string AnalysisManager::GetOutputFilename() const {
    return output_filename_;
}
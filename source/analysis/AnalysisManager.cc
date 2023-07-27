#include "AnalysisManager.hh"
//_________________________________________________________________________________________
AnalysisManager* AnalysisManager::instance_ = nullptr;
//_________________________________________________________________________________________
AnalysisManager::AnalysisManager(AnalysisMessenger* analysis_messenger) : 
analysis_messenger_(analysis_messenger) {
    if (!instance_) {
        instance_ = this;
    } 
}
//_________________________________________________________________________________________
AnalysisManager::~AnalysisManager() {}
//_________________________________________________________________________________________
AnalysisManager* AnalysisManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new AnalysisManager(new AnalysisMessenger());
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
//_________________________________________________________________________________________
void AnalysisManager::SetAnalysisFilename(std::string filename) {
    this->analysis_filename_ = filename;
}
//_________________________________________________________________________________________
std::string AnalysisManager::GetAnalysisFilename() const {
   return analysis_filename_;
}
//_________________________________________________________________________________________
bool AnalysisManager::AreHitsOutput() const {
    return analysis_messenger_->GetHitsOutputEnabled();
}
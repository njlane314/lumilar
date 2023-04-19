#include "AnalysisManager.hh"

AnalysisManager* AnalysisManager::instance_ = nullptr;

AnalysisManager::AnalysisManager() {}

AnalysisManager::~AnalysisManager() {}

AnalysisManager* AnalysisManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new AnalysisManager();
    }
    return instance_;
}

void AnalysisManager::setOutputFilename(std::string output_filename) {
    this->output_filename = output_filename;
}

std::string AnalysisManager::getOutputFilename() const {
    return output_filename;
}
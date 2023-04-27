#include "MediumResponse.hh"

MediumResponse::MediumResponse() 
: material_properties_(MaterialProperties::getInstance()->getMaterialProperties()) {}

MediumResponse::~MediumResponse() {}

std::pair<double, double> MediumResponse::CreateResponse(const EnergyDeposit* energy_deposit) {
    std::pair<double, double> intrinsic_response = Excitation::CreateExctiation(energy_deposit, material_properties_);
    return Recombination::ProcessRecombination(energy_deposit, material_properties_, intrinsic_response);
}

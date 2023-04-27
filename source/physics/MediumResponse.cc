#include "MediumResponse.hh"

MediumResponse::MediumResponse() 
: material_properties_(MaterialProperties::getInstance()->getMaterialProperties()) {}

MediumResponse::~MediumResponse() {}

std::pair<double, double> MediumResponse::create_response(const EnergyDeposit* energy_deposit) {
    std::pair<double, double> intrinsic_response = Excitation::create_exctiation(energy_deposit, material_properties_);
    return Recombination::ProcessRecombination(energy_deposit, material_properties_, intrinsic_response);
}

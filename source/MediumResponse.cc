#include "MediumResponse.hh"

MediumResponse::MediumResponse() 
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {}

MediumResponse::~MediumResponse() {}

std::pair<double, double> MediumResponse::create_response(EnergyDeposit* energy_deposit) {
    std::pair<double, double> intrinsic_response = Excitation::create_exctiation(energy_deposit, material_properties_);
    return Recombination::create_recombination(energy_deposit, material_properties_, intrinsic_response);
}
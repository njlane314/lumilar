#include "IonisationCollection.hh"
//_________________________________________________________________________________________
AnalysisResults<TH2F> IonisationCollection::TH2F_run_plots_;
//_________________________________________________________________________________________
void IonisationCollection::EventAnalysis(const Signal* signal) {
    PlotThermalElectrons(signal);
    PlotCollectionElectrons(signal);
}
//_________________________________________________________________________________________
void IonisationCollection::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}
//_________________________________________________________________________________________
void IonisationCollection::PlotThermalElectrons(const Signal* signal) {
    int thermalised_electrons = signal->GetIonisation()->GetTotalElectronCount();
    double primary_energy = signal->GetPrimaryEnergy();

    std::string thermalised_electrons_name = "thermalised_energy_hist";

    TH2F* thermal_energy_hist = TH2F_run_plots_.GetHistogram(thermalised_electrons_name);

    if (thermal_energy_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(thermalised_electrons_name, "Neutrino energy [MeV]", "Thermalised electrons", 16, 0, 16, 15, 0, 150000);
        thermal_energy_hist = TH2F_run_plots_.GetHistogram(thermalised_electrons_name);
    }

    thermal_energy_hist->Fill(primary_energy, thermalised_electrons);
}
//_________________________________________________________________________________________
void IonisationCollection::PlotCollectionElectrons(const Signal* signal) {
    int thermalised_electrons = signal->GetIonisation()->GetTotalElectronCount();

    std::string collected_electrons_name = "collected_electrons_hist";

    TH2F* collected_electrons_hist = TH2F_run_plots_.GetHistogram(collected_electrons_name);

    if (collected_electrons_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(collected_electrons_name, "Collection fraction", "Collected electrons", 100, 0, 1, 150, 0, 150000);
        collected_electrons_hist = TH2F_run_plots_.GetHistogram(collected_electrons_name);
    }

    for (int effic = 0; effic < 1000; effic++) {
        double collection_efficiency = effic / 1000.;
        //int thermalised_electrons = CLHEP::RandPoisson::shoot(floor((double)thermalised_electrons * collection_efficiency));
        int thermalised_fraction = CLHEP::RandBinomial::shoot(thermalised_electrons, collection_efficiency);

        collected_electrons_hist->Fill(collection_efficiency, thermalised_fraction);
    } 
}
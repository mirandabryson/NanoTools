// -*- C++ -*-

// C++
#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TString.h"

// NanoCORE
#include "./NanoCORE/Nano.h"
#include "./NanoCORE/ElectronSelections.h"
#include "./NanoCORE/MuonSelections.h"
#include "./NanoCORE/SSSelections.h"

// Custom
#include "fakes.h"

// Namespaces
using namespace std;
using namespace tas;

FakesTree::FakesTree(int year_init) {
    // TTree
    t = new TTree("tree", "tree");
    // Event level
    b_event = t->Branch("event", &event, "event/I");
    b_event_ht = t->Branch("event_ht", &event_ht, "event_ht/F");
    b_event_met = t->Branch("event_met", &event_met, "event_met/F");
    b_event_deepCSV_btags_loose = t->Branch("event_deepCSV_btags_loose", 
                                            &event_deepCSV_btags_loose, 
                                            "event_deepCSV_btags_loose/I");
    b_event_deepCSV_btags_medium = t->Branch("event_deepCSV_btags_medium", 
                                             &event_deepCSV_btags_medium, 
                                             "event_deepCSV_btags_medium/I");
    b_event_deepCSV_btags_tight = t->Branch("event_deepCSV_btags_tight", 
                                            &event_deepCSV_btags_tight, 
                                            "event_deepCSV_btags_tight/I");
    b_event_deepJet_btags_loose = t->Branch("event_deepJet_btags_loose", 
                                            &event_deepJet_btags_loose, 
                                            "event_deepJet_btags_loose/I");
    b_event_deepJet_btags_medium = t->Branch("event_deepJet_btags_medium", 
                                             &event_deepJet_btags_medium, 
                                             "event_deepJet_btags_medium/I");
    b_event_deepJet_btags_tight = t->Branch("event_deepJet_btags_tight", 
                                            &event_deepJet_btags_tight, 
                                            "event_deepJet_btags_tight/I");
    // Object level
    b_lepton_id = t->Branch("lepton_id", &lepton_id, "lepton_id/I");
    b_lepton_pt = t->Branch("lepton_pt", &lepton_pt, "lepton_pt/F");
    b_lepton_pt_corr = t->Branch("lepton_pt_corr", &lepton_pt_corr, "lepton_pt_corr/F");
    b_lepton_eta = t->Branch("lepton_eta", &lepton_eta, "lepton_eta/F");
    b_lepton_phi = t->Branch("lepton_phi", &lepton_phi, "lepton_phi/F");
    b_lepton_mass = t->Branch("lepton_mass", &lepton_mass, "lepton_mass/F");
    b_is_tight = t->Branch("is_tight", &is_tight, "is_tight/O");
    b_is_loose = t->Branch("is_loose", &is_loose, "is_loose/O");
    b_unmatched = t->Branch("unmatched", &unmatched, "unmatched/O");
    b_heavy_mother = t->Branch("heavy_mother", &heavy_mother, "heavy_mother/O");
    b_light_mother = t->Branch("light_mother", &light_mother, "light_mother/O");
    // Other
    year = year_init;
}

void FakesTree::reset(bool obj_level_only) {

    if (!obj_level_only) {
        // Event level
        event = -999;
        event_ht = -999;
        event_met = -999;
        event_deepCSV_btags_loose  = -999;
        event_deepCSV_btags_medium = -999;
        event_deepCSV_btags_tight  = -999;
        event_deepJet_btags_loose  = -999;
        event_deepJet_btags_medium = -999;
        event_deepJet_btags_tight  = -999;
    }
    // Object level
    lepton_id = -999;
    lepton_pt = -999;
    lepton_pt_corr = -999;
    lepton_eta = -999;
    lepton_phi = -999;
    lepton_mass = -999;
    is_tight = -999;
    is_loose = -999;
    unmatched = false;
    heavy_mother = false;
    light_mother = false;

    return;
}

bool FakesTree::passesNonIsoTrigger(int lepton_pdg_id, int lepton_index) {

    if (abs(lepton_pdg_id) == 11) {
        float el_pt = Electron_pt().at(lepton_index);
        if (el_pt >= 15 && el_pt < 25) {
            return (HLT_Ele8_CaloIdM_TrackIdM_PFJet30());
        }
        else {
            return (HLT_Ele17_CaloIdM_TrackIdM_PFJet30());
        }
    }
    else if (abs(lepton_pdg_id) == 13) {
        float mu_pt = Muon_pt().at(lepton_index);
        if (mu_pt >= 10 && mu_pt < 25) {
            return (HLT_Mu8());
        }
        else {
            return (HLT_Mu17());
        }
    }
    else {
        return false;
    }

}

/**
 * Event-level function that stores event-level info on
 * event-level branches
 */
int FakesTree::fillEvtBranches() {

    event = nt.event();
    event_met = MET_pt();
    // DeepCSV working points
    float deepCSV_loose  = 0.1522;
    float deepCSV_medium = 0.8838;
    float deepCSV_tight  = 0.9693;
    // DeepJet working points
    float deepJet_loose  = 0.1522;
    float deepJet_medium = 0.4941;
    float deepJet_tight  = 0.8001;
    // DeepAlgo btag counting
    event_deepCSV_btags_loose  = 0;
    event_deepCSV_btags_medium = 0;
    event_deepCSV_btags_tight  = 0;
    event_deepJet_btags_loose  = 0;
    event_deepJet_btags_medium = 0;
    event_deepJet_btags_tight  = 0;
    for (unsigned int i = 0; i < nJet(); i++) {
        float deepCSV_disc = Jet_btagDeepB().at(i);
        if (deepCSV_disc >= deepCSV_loose) {
            event_deepCSV_btags_loose++;
        }
        if (deepCSV_disc >= deepCSV_medium) {
            event_deepCSV_btags_medium++;
        }
        if (deepCSV_disc >= deepCSV_tight) {
            event_deepCSV_btags_tight++;
        }
        float deepJet_disc = Jet_btagDeepFlavB().at(i);
        if (deepJet_disc >= deepJet_loose) {
            event_deepJet_btags_loose++;
        }
        if (deepJet_disc >= deepJet_medium) {
            event_deepJet_btags_medium++;
        }
        if (deepJet_disc >= deepJet_tight) {
            event_deepJet_btags_tight++;
        }
    }

    return 0;
}

/**
 * Object-level function that stores object-level info on
 * object-level branches (i.e. fake-by-fake)
 */
int FakesTree::fillBranches() {

    MonolepID monolep_id = whichGenMonolep();
    if (monolep_id == no_lepton) {
        // Uninteresting event
        return 0;
    }
    else if (monolep_id == single_mu) {
        // Truth-matched to single muon event -> electrons == fakes
        for (unsigned int i = 0; i < nElectron(); i++) {
            // bool ad_hoc_pt_cut = (Electron_pt().at(i) >= 15);
            // bool non_iso_trigger = passesNonIsoTrigger(Electron_pdgId().at(i), i);
            // if (ad_hoc_pt_cut && non_iso_trigger) {
            //     saveLepton(Electron_pdgId().at(i), i);
            // }
            saveLepton(Electron_pdgId().at(i), i);
        }
    }
    else if (monolep_id == single_el) {
        // Truth-matched to single electron event -> muons == fakes
        for (unsigned int i = 0; i < nMuon(); i++) {
            // bool non_iso_trigger = passesNonIsoTrigger(Muon_pdgId().at(i), i);
            // if (non_iso_trigger) {
            //     saveLepton(Muon_pdgId().at(i), i);
            // }
            saveLepton(Muon_pdgId().at(i), i);
        }
    }

    return 0;
}

void FakesTree::saveLepton(int lepton_pdg_id, int lepton_index) {

    if (abs(lepton_pdg_id) == 11) {
        lepton_id = lepton_pdg_id;
        lepton_pt = Electron_pt().at(lepton_index);
        lepton_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.07, 0.78, 8.0);
        lepton_eta = Electron_eta().at(lepton_index);
        lepton_phi = Electron_phi().at(lepton_index);
        lepton_mass = Electron_mass().at(lepton_index);
        is_tight = electronID(lepton_index, IDtight, year);
        is_loose = electronID(lepton_index, IDfakable, year);
        // Classify mother as 'heavy' if she is a b-quark or c-quark
        if (Electron_genPartFlav().at(lepton_index) == 5 
            || Electron_genPartFlav().at(lepton_index) == 4) {
            unmatched = false;
            heavy_mother = true;
            light_mother = false;
        }
        else if (Electron_genPartFlav().at(lepton_index) == 3) {
            unmatched = false;
            heavy_mother = false;
            light_mother = true;
        }
        else if (Electron_genPartFlav().at(lepton_index) == 0) {
            unmatched = true;
            heavy_mother = false;
            light_mother = false;
        }
        else {
            unmatched = false;
            heavy_mother = false;
            light_mother = false;
        }
    }
    else if (abs(lepton_pdg_id) == 13) {
        lepton_id = lepton_pdg_id;
        lepton_pt = Muon_pt().at(lepton_index);
        lepton_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.11, 0.74, 6.8);
        lepton_eta = Muon_eta().at(lepton_index);
        lepton_phi = Muon_phi().at(lepton_index);
        lepton_mass = Muon_mass().at(lepton_index);
        is_tight = muonID(lepton_index, IDtight, year);
        is_loose = muonID(lepton_index, IDfakable, year);
        // Classify mother as 'heavy' if she is a b-quark or c-quark
        if (Muon_genPartFlav().at(lepton_index) == 5 
            || Muon_genPartFlav().at(lepton_index) == 4) {
            unmatched = false;
            heavy_mother = true;
            light_mother = false;
        }
        else if (Muon_genPartFlav().at(lepton_index) == 3) {
            unmatched = false;
            heavy_mother = false;
            light_mother = true;
        }
        else if (Muon_genPartFlav().at(lepton_index) == 0) {
            unmatched = true;
            heavy_mother = false;
            light_mother = false;
        }
        else {
            unmatched = false;
            heavy_mother = false;
            light_mother = false;
        }
    }

    if (is_loose || is_tight) {
        t->Fill();
    }
    reset(true);

    return;
}

/**
 * Since we're using the tt-jets, single-lepton sample, we can find 
 * out exactly which lepton was produced for each such event, that
 * way we know exactly which leptons are imposters
 *
 * Returns monolepton ID code
 */
MonolepID FakesTree::whichGenMonolep() {

    // Check electrons
    int truth_matched_els = 0;
    for (unsigned int i = 0; i < nElectron(); i++) {
        // Only count prompt leptons from W boson
        bool is_prompt = (int(Electron_genPartFlav().at(i)) == 1);
        if (is_prompt) {
            int gen_index = Electron_genPartIdx().at(i);
            if (gen_index >= 0) {
                int gen_mother_index = GenPart_genPartIdxMother().at(gen_index);
                if (gen_mother_index >= 0) {
                    bool from_genW = (abs(GenPart_pdgId().at(gen_mother_index)) == 24);
                    if (from_genW) {
                        truth_matched_els++;
                    }
                }
            }
        }
    }

    // Check muons
    int truth_matched_mus = 0;
    for (unsigned int i = 0; i < nMuon(); i++) {
        // Only count prompt leptons from W boson
        bool is_prompt = (int(Muon_genPartFlav().at(i)) == 1);
        if (is_prompt) {
            int gen_index = Muon_genPartIdx().at(i);
            if (gen_index >= 0) {
                int gen_mother_index = GenPart_genPartIdxMother().at(gen_index);
                if (gen_mother_index >= 0) {
                    bool from_genW = (abs(GenPart_pdgId().at(gen_mother_index)) == 24);
                    if (from_genW) {
                        truth_matched_mus++;
                    }
                }
            }
        }
    }

    if (truth_matched_els == 1 && truth_matched_mus == 0) {
        return single_el;
    }
    else if (truth_matched_els == 0 && truth_matched_mus == 1) {
        return single_mu;
    }
    else {
        return no_lepton;
    }
}

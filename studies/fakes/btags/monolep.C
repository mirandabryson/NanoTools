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
#include "./NanoCORE/Config.h"
#include "./NanoCORE/ElectronSelections.h"
#include "./NanoCORE/MuonSelections.h"
#include "./NanoCORE/SSSelections.h"
// Custom
#include "monolep.h"

// Namespaces
using namespace std;
using namespace tas;

MonolepTree::MonolepTree() {
    // TTree
    t = new TTree("tree", "tree");
    /* Branches */
    // Event kinematics
    b_event = t->Branch("event", &event, "event/I");
    b_ht = t->Branch("ht", &ht, "ht/F");
    b_met = t->Branch("met", &met, "met/F");
    // DeepAlgo btags
    b_deepCSV_btags_loose = t->Branch("deepCSV_btags_loose", &deepCSV_btags_loose, "deepCSV_btags_loose/I");
    b_deepCSV_btags_medium = t->Branch("deepCSV_btags_medium", &deepCSV_btags_medium, "deepCSV_btags_medium/I");
    b_deepCSV_btags_tight = t->Branch("deepCSV_btags_tight", &deepCSV_btags_tight, "deepCSV_btags_tight/I");
    b_deepJet_btags_loose = t->Branch("deepJet_btags_loose", &deepJet_btags_loose, "deepJet_btags_loose/I");
    b_deepJet_btags_medium = t->Branch("deepJet_btags_medium", &deepJet_btags_medium, "deepJet_btags_medium/I");
    b_deepJet_btags_tight = t->Branch("deepJet_btags_tight", &deepJet_btags_tight, "deepJet_btags_tight/I");
    // Real lepton
    b_lepton_id = t->Branch("lepton_id", &lepton_id, "lepton_id/I");
    b_lepton_pt = t->Branch("lepton_pt", &lepton_pt, "lepton_pt/F");
    b_lepton_pt_corr = t->Branch("lepton_pt_corr", &lepton_pt_corr, "lepton_pt_corr/F");
    b_lepton_eta = t->Branch("lepton_eta", &lepton_eta, "lepton_eta/F");
    b_lepton_phi = t->Branch("lepton_phi", &lepton_phi, "lepton_phi/F");
    // Fake lepton
    b_fake_id = t->Branch("fake_id", &fake_id, "fake_id/I");
    b_fake_pt = t->Branch("fake_pt", &fake_pt, "fake_pt/F");
    b_fake_pt_corr = t->Branch("fake_pt_corr", &fake_pt_corr, "fake_pt_corr/F");
    b_fake_eta = t->Branch("fake_eta", &fake_eta, "fake_eta/F");
    b_fake_phi = t->Branch("fake_phi", &fake_phi, "fake_phi/F");
    b_fake_unmatched = t->Branch("fake_unmatched", &fake_unmatched, "fake_unmatched/O");
    b_fake_from_b = t->Branch("fake_from_b", &fake_from_b, "fake_from_b/O");
    b_fake_from_c = t->Branch("fake_from_c", &fake_from_c, "fake_from_c/O");
    b_fake_from_light = t->Branch("fake_from_light", &fake_from_light, "fake_from_light/O");

}

void MonolepTree::reset() {
    /* Reset branch values */
    // Event kinematics
    event = -999;
    ht = -999;
    met = -999;
    // DeepAlgo btags
    deepCSV_btags_loose  = -999;
    deepCSV_btags_medium = -999;
    deepCSV_btags_tight  = -999;
    deepJet_btags_loose  = -999;
    deepJet_btags_medium = -999;
    deepJet_btags_tight  = -999;
    // Real lepton
    lepton_id = -999;
    lepton_pt = -999;
    lepton_pt_corr = -999;
    lepton_eta = -999;
    lepton_phi = -999;
    // Fake lepton
    fake_id = -999;
    fake_pt = -999;
    fake_pt_corr = -999;
    fake_eta = -999;
    fake_phi = -999;
    fake_unmatched = false;
    fake_from_b = false;
    fake_from_c = false;
    fake_from_light = false;
    /* Reset internal values */
    lepton_jet_index = 999;
    fake_jet_index = 999;

    return;
}

bool MonolepTree::passesNonIsoTrigger(int lepton_pdg_id, int lepton_index) {

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

int MonolepTree::fillEvtBranches() {

    event = nt.event();
    met = MET_pt();
    // DeepCSV working points
    float deepCSV_loose  = 0.1522;
    float deepCSV_medium = 0.4941;
    float deepCSV_tight  = 0.8001;
    // DeepJet working points
    float deepJet_loose  = 0.0521;
    float deepJet_medium = 0.3033;
    float deepJet_tight  = 0.7489;
    // DeepAlgo btag counting
    deepCSV_btags_loose  = 0;
    deepCSV_btags_medium = 0;
    deepCSV_btags_tight  = 0;
    deepJet_btags_loose  = 0;
    deepJet_btags_medium = 0;
    deepJet_btags_tight  = 0;
    for (unsigned int i = 0; i < nJet(); i++) {
        // eta veto
        if (abs(Jet_eta().at(i)) > 2.4) continue;
        // pt veto
        if (Jet_pt().at(i) < 30) continue;
        // Lepton overlap veto
        if (i == fake_jet_index || i == lepton_jet_index) continue;
        float deepCSV_disc = Jet_btagDeepB().at(i);
        if (deepCSV_disc >= deepCSV_loose) {
            deepCSV_btags_loose++;
        }
        if (deepCSV_disc >= deepCSV_medium) {
            deepCSV_btags_medium++;
        }
        if (deepCSV_disc >= deepCSV_tight) {
            deepCSV_btags_tight++;
        }
        float deepJet_disc = Jet_btagDeepFlavB().at(i);
        if (deepJet_disc >= deepJet_loose) {
            deepJet_btags_loose++;
        }
        if (deepJet_disc >= deepJet_medium) {
            deepJet_btags_medium++;
        }
        if (deepJet_disc >= deepJet_tight) {
            deepJet_btags_tight++;
        }
    }

    return 0;
}

int MonolepTree::fillLepBranches() {

    MonolepID monolep_id = whichGenMonolep();
    if (monolep_id == no_lepton) {
        // Uninteresting event
        return 0;
    }
    else if (monolep_id == single_mu) {
        // Truth-matched to single muon event -> electrons == fakes
        int fake_el_index = 0;
        int n_tight_fakes = 0;
        for (unsigned int i = 0; i < nElectron(); i++) {
            if (electronID(i, IDtight, gconf.year)) {
                fake_el_index = i;
                n_tight_fakes++;
            }
        }
        if (n_tight_fakes == 1) {
            fake_jet_index = Electron_jetIdx().at(fake_el_index);
            saveLepton(Electron_pdgId().at(fake_el_index), fake_el_index, false);
        }
    }
    else if (monolep_id == single_el) {
        // Truth-matched to single electron event -> muons == fakes
        int fake_mu_index = 0;
        int n_tight_fakes = 0;
        for (unsigned int i = 0; i < nMuon(); i++) {
            if (muonID(i, IDtight, gconf.year)) {
                fake_mu_index = i;
                n_tight_fakes++;
            }
        }
        if (n_tight_fakes == 1) {
            fake_jet_index = Muon_jetIdx().at(fake_mu_index);
            saveLepton(Muon_pdgId().at(fake_mu_index), fake_mu_index, false);
        }
    }

    return 0;
}

void MonolepTree::saveLepton(int lepton_pdg_id, int lepton_index, bool is_prompt) {

    if (abs(lepton_pdg_id) == 11) {
        if (is_prompt) {
            // Kinematics
            lepton_id = lepton_pdg_id;
            lepton_pt = Electron_pt().at(lepton_index);
            lepton_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.07, 0.78, 8.0);
            lepton_eta = Electron_eta().at(lepton_index);
            lepton_phi = Electron_phi().at(lepton_index);
        }
        else {
            // Kinematics
            fake_id = lepton_pdg_id;
            fake_pt = Electron_pt().at(lepton_index);
            fake_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.07, 0.78, 8.0);
            fake_eta = Electron_eta().at(lepton_index);
            fake_phi = Electron_phi().at(lepton_index);
            // Lineage
            fake_unmatched = Electron_genPartFlav().at(lepton_index) == 0;
            fake_from_b = Electron_genPartFlav().at(lepton_index) == 5;
            fake_from_c = Electron_genPartFlav().at(lepton_index) == 4;
            fake_from_light = Electron_genPartFlav().at(lepton_index) == 3;
        }
    }
    else if (abs(lepton_pdg_id) == 13) {
        if (is_prompt) {
            // Kinematics
            lepton_id = lepton_pdg_id;
            lepton_pt = Muon_pt().at(lepton_index);
            lepton_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.11, 0.74, 6.8);
            lepton_eta = Muon_eta().at(lepton_index);
            lepton_phi = Muon_phi().at(lepton_index);
        }
        else {
            // Kinematics
            fake_id = lepton_pdg_id;
            fake_pt = Muon_pt().at(lepton_index);
            fake_pt_corr = coneCorrPt(lepton_pdg_id, lepton_index, 0.11, 0.74, 6.8);
            fake_eta = Muon_eta().at(lepton_index);
            fake_phi = Muon_phi().at(lepton_index);
            // Lineage
            fake_unmatched = Muon_genPartFlav().at(lepton_index) == 0;
            fake_from_b = Muon_genPartFlav().at(lepton_index) == 5;
            fake_from_c = Muon_genPartFlav().at(lepton_index) == 4;
            fake_from_light = Muon_genPartFlav().at(lepton_index) == 3;
        }
    }

    return;
}
    
/**
 * Since we're using the tt-jets, single-lepton sample, we can find 
 * out exactly which lepton was produced for each such event, that
 * way we know exactly which leptons are imposters
 *
 * Returns monolepton ID code and saves prompt lepton if it exists
 */
MonolepID MonolepTree::whichGenMonolep() {

    // Check electrons
    int truth_matched_els = 0;
    int prompt_el_index = -1;
    for (unsigned int i = 0; i < nElectron(); i++) {
        // Only count prompt leptons from W boson
        bool is_prompt = (int(Electron_genPartFlav().at(i)) == 1);
        if (is_prompt) {
            int gen_index = Electron_genPartIdx().at(i);
            if (gen_index < 0) continue;
            int gen_mother_index = GenPart_genPartIdxMother().at(gen_index);
            if (gen_mother_index < 0) continue;
            bool from_genW = (GenPart_pdgId().at(gen_mother_index) == 24);
            if (from_genW) {
                // Only count if W boson from top (NOT anti-top)
                int gen_grandmother_index = GenPart_genPartIdxMother().at(gen_mother_index);
                if (gen_grandmother_index < 0) continue;
                bool from_genW_fromTop = (GenPart_pdgId().at(gen_grandmother_index) == 6);
                if (from_genW_fromTop) {
                    truth_matched_els++;
                    prompt_el_index = i;
                }
            }
        }
    }

    // Check muons
    int truth_matched_mus = 0;
    int prompt_mu_index = -1;
    for (unsigned int i = 0; i < nMuon(); i++) {
        // Only count prompt leptons from W boson
        bool is_prompt = (int(Muon_genPartFlav().at(i)) == 1);
        if (is_prompt) {
            int gen_index = Muon_genPartIdx().at(i);
            if (gen_index < 0) continue;
            int gen_mother_index = GenPart_genPartIdxMother().at(gen_index);
            if (gen_mother_index < 0) continue;
            bool from_genW = (GenPart_pdgId().at(gen_mother_index) == 24);
            if (from_genW) {
                // Only count if W boson from top (NOT anti-top)
                int gen_grandmother_index = GenPart_genPartIdxMother().at(gen_mother_index);
                if (gen_grandmother_index < 0) continue;
                bool from_genW_fromTop = (GenPart_pdgId().at(gen_grandmother_index) == 6);
                if (from_genW_fromTop) {
                    truth_matched_mus++;
                    prompt_mu_index = i;
                }
            }
        }
    }

    if (truth_matched_els == 1 && truth_matched_mus == 0) {
        if (electronID(prompt_el_index, IDtight, gconf.year)) {
            lepton_jet_index = Electron_jetIdx().at(prompt_el_index);
            saveLepton(Electron_pdgId().at(prompt_el_index), prompt_el_index, true);
        }
        return single_el;
    }
    else if (truth_matched_els == 0 && truth_matched_mus == 1) {
        if (muonID(prompt_mu_index, IDtight, gconf.year)) {
            lepton_jet_index = Muon_jetIdx().at(prompt_mu_index);
            saveLepton(Muon_pdgId().at(prompt_mu_index), prompt_mu_index, true);
        }
        return single_mu;
    }
    else {
        return no_lepton;
    }
}

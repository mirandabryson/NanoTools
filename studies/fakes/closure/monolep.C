// -*- C++ -*-

// C++
#include <iostream>
#include <vector>
#include <algorithm>
// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TString.h"
// NanoCORE
#include "NanoCORE/Nano.h"
#include "NanoCORE/SSSelections.h"
// Custom
#include "monolep.h"
#include "utilities/muons.C"
#include "utilities/electrons.C"

// Namespaces
using namespace std;
using namespace tas;

MonolepTree::MonolepTree(TFile* new_tfile) {
    // TTree and TFile
    ttree = new TTree("tree", "tree");
    tfile = new_tfile;
    // Branches
    b_event = ttree->Branch("event", &event, "event/I");
    b_met = ttree->Branch("met", &met, "met/F");
    b_weight = ttree->Branch("weight", &weight, "weight/F");
    b_error = ttree->Branch("error", &error, "error/F");
    b_flav_weight = ttree->Branch("flav_weight", &flav_weight, "flav_weight/F");
    b_flav_error = ttree->Branch("flav_error", &flav_error, "flav_error/F");
    b_lepton_id = ttree->Branch("lepton_id", &lepton_id, "lepton_id/I");
    b_lepton_pt = ttree->Branch("lepton_pt", &lepton_pt, "lepton_pt/F");
    b_lepton_eta = ttree->Branch("lepton_eta", &lepton_eta, "lepton_eta/F");
    b_lepton_is_loose = ttree->Branch("lepton_is_loose", &lepton_is_loose, "lepton_is_loose/O");
    b_lepton_is_tight = ttree->Branch("lepton_is_tight", &lepton_is_tight, "lepton_is_loose/O");
    b_fake_id = ttree->Branch("fake_id", &fake_id, "fake_id/I");
    b_fake_pt = ttree->Branch("fake_pt", &fake_pt, "fake_pt/F");
    b_fake_pt_corr = ttree->Branch("fake_pt_corr", &fake_pt_corr, "fake_pt_corr/F");
    b_fake_eta = ttree->Branch("fake_eta", &fake_eta, "fake_eta/F");
    b_fake_gen_match = ttree->Branch("fake_gen_match", &fake_gen_match, "fake_gen_match/I");
    b_fake_is_loose = ttree->Branch("fake_is_loose", &fake_is_loose, "fake_is_loose/O");
    b_fake_is_tight = ttree->Branch("fake_is_tight", &fake_is_tight, "fake_is_loose/O");
    b_fake_unmatched = ttree->Branch("fake_unmatched", &fake_unmatched, "fake_unmatched/O");
    b_fake_from_b = ttree->Branch("fake_from_b", &fake_from_b, "fake_from_b/O");
    b_fake_from_c = ttree->Branch("fake_from_c", &fake_from_c, "fake_from_c/O");
    b_fake_from_light = ttree->Branch("fake_from_light", &fake_from_light, "fake_from_light/O");
    b_n_jets = ttree->Branch("n_jets", &n_jets, "n_jets/I");
    b_n_gen_bquarks = ttree->Branch("n_gen_bquarks", &n_gen_bquarks, "n_gen_bquarks/I");
    b_n_btags_deepCSV_loose = ttree->Branch("n_btags_deepCSV_loose", &n_btags_deepCSV_loose, "n_btags_deepCSV_loose/I");
    b_n_btags_deepCSV_medium = ttree->Branch("n_btags_deepCSV_medium", &n_btags_deepCSV_medium, "n_btags_deepCSV_medium/I");
    b_n_btags_deepCSV_tight = ttree->Branch("n_btags_deepCSV_tight", &n_btags_deepCSV_tight, "n_btags_deepCSV_tight/I");
    b_n_btags_deepJet_loose = ttree->Branch("n_btags_deepJet_loose", &n_btags_deepJet_loose, "n_btags_deepJet_loose/I");
    b_n_btags_deepJet_medium = ttree->Branch("n_btags_deepJet_medium", &n_btags_deepJet_medium, "n_btags_deepJet_medium/I");
    b_n_btags_deepJet_tight = ttree->Branch("n_btags_deepJet_tight", &n_btags_deepJet_tight, "n_btags_deepJet_tight/I");

}

void MonolepTree::resetBranches() {
    // Reset branch values
    event = -999;
    met = -999;
    weight = 1.0;
    error = 0.0;
    flav_weight = 1.0;
    flav_error = 0.0;
    lepton_id = -999;
    lepton_pt = -999;
    lepton_eta = -999;
    lepton_is_loose = false;
    lepton_is_tight = false;
    fake_id = -999;
    fake_pt = -999;
    fake_pt_corr = -999;
    fake_eta = -999;
    fake_gen_match = 0;
    fake_is_loose = false;
    fake_is_tight = false;
    fake_unmatched = false;
    fake_from_b = false;
    fake_from_c = false;
    fake_from_light = false;
    n_jets = 0;
    n_gen_bquarks = 0;
    n_btags_deepCSV_loose = -999;
    n_btags_deepCSV_medium = -999;
    n_btags_deepCSV_tight = -999;
    n_btags_deepJet_loose = -999;
    n_btags_deepJet_medium = -999;
    n_btags_deepJet_tight = -999;

    return;
}

int MonolepTree::fillBranches() {
    // Get reco leptons
    Leptons leptons = getLeptons();
    // Loop over leptons
    Lepton electron;
    Lepton muon;
    int n_els = 0;
    int n_mus = 0;
    for (auto& lepton : leptons) {
        if (lepton.idlevel() >= IDfakable) {
            if (lepton.is_el()) {
                electron = lepton;
                n_els++;
            }
            else if (lepton.is_mu()) {
                muon = lepton;
                n_mus++;
            }
        }
    }
    // Only accept events with ONE el && ONE mu
    if (n_els != 1 || n_mus != 1) {
        // Return without filling TTree
        return 0;
    }
    // Assign names
    Lepton prompt_lepton;
    Lepton fake_lepton;
    if (isPromptLepton(electron)) {
        prompt_lepton = electron;
        fake_lepton = muon;
    }
    else if (isPromptLepton(muon)) {
        prompt_lepton = muon;
        fake_lepton = electron;
    }
    else {
        // Return without filling TTree
        return 0;
    }
    // Fill appropriate branches only for tight prompts
    if (prompt_lepton.idlevel() == IDtight && fake_lepton.idlevel() >= IDfakable) {
        fillLeptonBranches(prompt_lepton, true);
        fillLeptonBranches(fake_lepton, false);
        // Apply FR if fake is loose
        if (fake_lepton.idlevel() == IDfakable) {
            // Overall fake rate
            auto result = getFakeRate(fake_lepton);
            float fr = result.first;
            float fr_err = result.second;
            weight *= fr/(1.0-fr);
            error = sqrt(error*error + fr_err*fr_err);
            // Gen-matched mother flavor fake rate
            auto flav_result = getFlavFakeRate(fake_lepton, fake_gen_match);
            float flav_fr = flav_result.first;
            float flav_fr_err = flav_result.second;
            flav_weight *= flav_fr/(1.0-flav_fr);
            flav_error = sqrt(flav_error*flav_error + flav_fr_err*flav_fr_err);
        }
    }
    else {
        // Return without filling TTree
        return 0;
    }

    // Store event information
    event = nt.event();
    met = MET_pt();
    n_jets = nJet();

    // Count gen-level b-quarks
    n_gen_bquarks = 0;
    for (unsigned int i = 0; i < nGenPart(); i++) {
        bool is_last_copy = (GenPart_statusFlags().at(i) & (1<<13)) == (1<<13);
        if (abs(GenPart_pdgId().at(i)) == 5 && is_last_copy) {
            n_gen_bquarks++;
        }
    }
    
    // DeepAlgo btag counting
    n_btags_deepCSV_loose  = 0;
    n_btags_deepCSV_medium = 0;
    n_btags_deepCSV_tight  = 0;
    n_btags_deepJet_loose  = 0;
    n_btags_deepJet_medium = 0;
    n_btags_deepJet_tight  = 0;
    // DeepCSV working points
    float deepCSV_loose  = 0.1522;
    float deepCSV_medium = 0.4941;
    float deepCSV_tight  = 0.8001;
    // DeepJet working points
    float deepJet_loose  = 0.0521;
    float deepJet_medium = 0.3033;
    float deepJet_tight  = 0.7489;
    // Loop over jets
    for (unsigned int i = 0; i < nJet(); i++) {
        // eta veto
        if (abs(Jet_eta().at(i)) > 2.4) continue;
        // pt veto
        if (Jet_pt().at(i) < 30) continue;
        // Lepton overlap veto
        if (i == prompt_lepton.idx()) continue;
        if (i == fake_lepton.idx()) continue;
        // DeepCSV counting
        float deepCSV_disc = Jet_btagDeepB().at(i);
        if (deepCSV_disc >= deepCSV_loose) {
            n_btags_deepCSV_loose++;
        }
        if (deepCSV_disc >= deepCSV_medium) {
            n_btags_deepCSV_medium++;
        }
        if (deepCSV_disc >= deepCSV_tight) {
            n_btags_deepCSV_tight++;
        }
        // DeepJet counting
        float deepJet_disc = Jet_btagDeepFlavB().at(i);
        if (deepJet_disc >= deepJet_loose) {
            n_btags_deepJet_loose++;
        }
        if (deepJet_disc >= deepJet_medium) {
            n_btags_deepJet_medium++;
        }
        if (deepJet_disc >= deepJet_tight) {
            n_btags_deepJet_tight++;
        }
    }

    fillTTree();

    return 0;
}

void MonolepTree::fillLeptonBranches(Lepton lepton, bool is_prompt) {
    // Save lepton
    if (is_prompt) {
        lepton_id = lepton.id();
        lepton_pt = lepton.pt();
        lepton_eta = lepton.eta();
        lepton_is_loose = (lepton.idlevel() >= IDfakable);
        lepton_is_tight = (lepton.idlevel() == IDtight);
    }
    else {
        fake_id = lepton.id();
        fake_pt = lepton.pt();
        if (lepton.is_el()) {
            fake_pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.07, 0.78, 8.0);
        }
        else if (lepton.is_mu()) {
            fake_pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.11, 0.74, 6.8);
        }
        if (lepton.is_el()) {
            fake_gen_match = Electron_genPartFlav().at(lepton.idx());
        }
        else if (lepton.is_mu()) {
            fake_gen_match = Muon_genPartFlav().at(lepton.idx());
        }
        fake_eta = lepton.eta();
        fake_is_loose = (lepton.idlevel() >= IDfakable);
        fake_is_tight = (lepton.idlevel() == IDtight);
        fake_unmatched = (fake_gen_match == 0);
        fake_from_b = (fake_gen_match == 5);
        fake_from_c = (fake_gen_match == 4);
        fake_from_light = (fake_gen_match == 3);
    }
    return;
}

void MonolepTree::fillTTree() {
    ttree->Fill();
    return;
}

void MonolepTree::writeTFile() {
    tfile->cd();
    ttree->Write();
    tfile->Close();
    return;
}

bool MonolepTree::isPromptLepton(Lepton lepton) {
    unsigned int idx = lepton.idx();
    bool is_el = lepton.is_el();
    int gen_part_flav = is_el ? Electron_genPartFlav().at(idx) : Muon_genPartFlav().at(idx);
    if (gen_part_flav == 1) {
        int gen_idx = is_el ? Electron_genPartIdx().at(idx) : Muon_genPartIdx().at(idx);
        if (gen_idx >= 0) {
            int gen_mother_idx = GenPart_genPartIdxMother().at(gen_idx);
            if (gen_mother_idx >= 0) {
                bool from_genW = (abs(GenPart_pdgId().at(gen_mother_idx)) == 24);
                if (from_genW) {
                    return true;
                }
            }
        }
    }

    return false;
}

pair<float, float> MonolepTree::getFakeRate(Lepton lepton) {
    float eta = lepton.eta();
    if (lepton.is_el()) {
        float pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.07, 0.78, 8.0);
        return {electronFakeRate(pt_corr, eta), electronFakeRateError(pt_corr, eta)};
    }
    else if (lepton.is_mu()) {
        float pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.11, 0.74, 6.8);
        return {muonFakeRate(pt_corr, eta), muonFakeRateError(pt_corr, eta)};
    }
    else {
        return {0.0, 0.0};
    }
}

pair<float, float> MonolepTree::getFlavFakeRate(Lepton lepton, int fake_gen_match) {
    if (fake_gen_match == 1) {
        return {0.0, 0.0};
    }
    float eta = lepton.eta();
    if (lepton.is_el()) {
        float pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.07, 0.78, 8.0);
        if (fake_gen_match == 5 || fake_gen_match == 4) {
            return {electronHeavyFakeRate(pt_corr, eta), electronHeavyFakeRateError(pt_corr, eta)};
        }
        else if (fake_gen_match == 0 || fake_gen_match == 3) {
            return {electronLightFakeRate(pt_corr, eta), electronLightFakeRateError(pt_corr, eta)};
        }
    }
    else if (lepton.is_mu()) {
        float pt_corr = coneCorrPt(lepton.id(), lepton.idx(), 0.11, 0.74, 6.8);
        if (fake_gen_match == 5 || fake_gen_match == 4) {
            return {muonHeavyFakeRate(pt_corr, eta), muonHeavyFakeRateError(pt_corr, eta)};
        }
        else if (fake_gen_match == 0 || fake_gen_match == 3) {
            return {muonLightFakeRate(pt_corr, eta), muonLightFakeRateError(pt_corr, eta)};
        }
    }
    else {
        return {0.0, 0.0};
    }

    return {0.0, 0.0};
}

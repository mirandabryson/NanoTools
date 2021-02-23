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
#include "./NanoCORE/SSSelections.h"
#include "./NanoCORE/ElectronSelections.h"
#include "./NanoCORE/MuonSelections.h"
// Custom
#include "nanohyps.h"

// Namespaces
using namespace std;
using namespace tas;

NanoHypsTree::NanoHypsTree() {
    // TTree
    t = new TTree("tree", "tree");
    // Branches
    b_leader_eta = t->Branch("leader_eta", &leader_eta, "leader_eta/F");
    b_lagger_isTriggerSafe = t->Branch("lagger_isTriggerSafe", &lagger_isTriggerSafe, "lagger_isTriggerSafe/O");
    b_leader_isTriggerSafe = t->Branch("leader_isTriggerSafe", &leader_isTriggerSafe, "leader_isTriggerSafe/O");
    b_lagger_id = t->Branch("lagger_id", &lagger_id, "lagger_id/I");
    b_leader_id = t->Branch("leader_id", &leader_id, "leader_id/I");
    b_leader_is_loose = t->Branch("leader_is_loose", &leader_is_loose, "leader_is_loose/O");
    b_lagger_is_loose = t->Branch("lagger_is_loose", &lagger_is_loose, "lagger_is_loose/O");
    b_leader_is_tight = t->Branch("leader_is_tight", &leader_is_tight, "leader_is_tight/O");
    b_lagger_is_tight = t->Branch("lagger_is_tight", &lagger_is_tight, "lagger_is_tight/O");
    b_lagger_eta = t->Branch("lagger_eta", &lagger_eta, "lagger_eta/F");
    b_hyp_class = t->Branch("hyp_class", &hyp_class, "hyp_class/I");
    b_leader_pt = t->Branch("leader_pt", &leader_pt, "leader_pt/F");
    b_lagger_pt = t->Branch("lagger_pt", &lagger_pt, "lagger_pt/F");
    b_is_dilepton_event = t->Branch("is_dilepton_event", &is_dilepton_event, "is_dilepton_event/O");
    b_event = t->Branch("event", &event, "event/I");

}

void NanoHypsTree::reset() {
    // Reset branch values
    leader_eta = -999;
    lagger_isTriggerSafe = false;
    leader_isTriggerSafe = false;
    lagger_id = -999;
    leader_id = -999;
    leader_is_loose = false;
    lagger_is_loose = false;
    leader_is_tight = false;
    lagger_is_tight = false;
    lagger_eta = -999;
    hyp_class = -999;
    leader_pt = -999;
    lagger_pt = -999;
    is_dilepton_event = false;
    event = -999;

    return;
}

int NanoHypsTree::fillBranches() {
    // Get hypothesis
    // bool verbose = (event == 134649108 || event == 209208961 || event == 209212074);
    bool verbose = false;
    if (verbose) cout << "Event: " << event << endl;
    auto leps = getLeptons();
    auto result = getBestHyp(leps, verbose);
    hyp_class = result.first;
    is_dilepton_event = (hyp_class > 0);
    Lepton leader = result.second.first;
    Lepton lagger = result.second.second;
    if (is_dilepton_event) {
        if (hyp_class == 6) {
            auto extraZ = makesResonance(leps, leader, lagger, 91., 15.);
            auto z_result = leps[extraZ.second];
            if (!isLeptonLevel(IDtight, leader.id(), leader.idx())) {
                is_dilepton_event = false;
                hyp_class = -1;
                return 0;
            }
            if (!isLeptonLevel(IDtight, lagger.id(), lagger.idx())) {
                is_dilepton_event = false;
                hyp_class = -1;
                return 0;
            }
            if (!isLeptonLevel(IDtight, z_result.id(), z_result.idx())) {
                is_dilepton_event = false;
                hyp_class = -1;
                return 0;
            }
        }
        if (leader.is_el()) {
            leader_isTriggerSafe = isTriggerSafe_noIso_v3(leader.idx());
        }
        if (lagger.is_el()) {
            lagger_isTriggerSafe = isTriggerSafe_noIso_v3(lagger.idx());
        }
        leader_is_loose = isLeptonLevel(IDfakable, leader.id(), leader.idx());
        lagger_is_loose = isLeptonLevel(IDfakable, lagger.id(), lagger.idx());
        leader_is_tight = isLeptonLevel(IDtight, leader.id(), leader.idx());
        lagger_is_tight = isLeptonLevel(IDtight, lagger.id(), lagger.idx());
        leader_pt = leader.pt();
        lagger_pt = lagger.pt();
        leader_eta = leader.eta();
        lagger_eta = lagger.eta();
        leader_id = leader.id();
        lagger_id = lagger.id();
    }

    return 0;
}
    

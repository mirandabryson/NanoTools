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
// Custom
#include "hyp.h"

// Namespaces
using namespace std;

HypTree::HypTree(int year_init) {
    // TTree
    t = new TTree("tree", "tree");
    // Branches
    b_is_dilepton_event = t->Branch("is_dilepton_event", &is_dilepton_event, "is_dilepton_event/O");
    b_event = t->Branch("event", &event, "event/I");
    b_hyp_class = t->Branch("hyp_class", &hyp_class, "hyp_class/I");
    TBranch* b_tight_pt = t->Branch("tight_pt", &tight_pt, "tight_pt/F");
    TBranch* b_loose_pt = t->Branch("loose_pt", &loose_pt, "loose_pt/F");
    // Other
    year = year_init;
}

void HypTree::reset() {
    // Reset branch values
    is_dilepton_event = false;
    event = -999;
    hyp_class = -999;
    leader_pt = -999;
    lagger_pt = -999;

    return;
}

int HypTree::fillBranches() {
    
    // Get hypothesis
    auto leps = getLeptons();
    auto result = getBestHyp(leps);
    int hyp_class = result.first;
    if (hyp_class < 0) {
        is_dilepton_event = false;
    }
    else {
        hyp_class = result.second;
        Lepton leader = result.second.first;
        Lepton lagger = result.second.second;
        leader_pt = leader.pt();
        lagger_pt = lagger.pt();
    }

    return 0;
}
    

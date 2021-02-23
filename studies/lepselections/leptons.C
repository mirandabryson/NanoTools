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
#include "../../NanoCORE/Nano.h"
#include "../../NanoCORE/ElectronSelections.h"
#include "../../NanoCORE/MuonSelections.h"
// Custom
#include "leptons.h"

// Namespaces
using namespace std;
using namespace tas;

LeptonsTree::LeptonsTree(int year_init) {
    // TTree
    t = new TTree("tree", "tree");
    // Branches
    b_lepton_pt = t->Branch("lepton_pt", &lepton_pt, "lepton_pt/F");
    b_lepton_eta = t->Branch("lepton_eta", &lepton_eta, "lepton_eta/F");
    b_lepton_phi = t->Branch("lepton_phi", &lepton_phi, "lepton_phi/F");
    b_event = t->Branch("event", &event, "event/I");
    b_lepton_id = t->Branch("lepton_id", &lepton_id, "lepton_id/I");
    // Other
    year = year_init;
}

void LeptonsTree::reset() {
    // Reset branch values
    lepton_pt = -999;
    lepton_eta = -999;
    lepton_phi = -999;
    event = -999;
    lepton_id = -999;

    return;
}

int LeptonsTree::fillBranches() {
    /* Your code here */
    for (unsigned int i = 0; i < nElectron(); i++) {
        if (electronID(i, IDtight, year)) {
            lepton_pt = Electron_pt().at(i);
            lepton_eta = Electron_eta().at(i);
            lepton_phi = Electron_phi().at(i);
            lepton_id = Electron_pdgId().at(i);
            t->Fill();
        }
    }
    for (unsigned int i = 0; i < nMuon(); i++) {
        if (muonID(i, IDtight, year)) {
            lepton_pt = Muon_pt().at(i);
            lepton_eta = Muon_eta().at(i);
            lepton_phi = Muon_phi().at(i);
            lepton_id = Muon_pdgId().at(i);
            t->Fill();
        }
    }
    return 0;
}
    

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
#include "sspairs.h"

// Namespaces
using namespace std;

SSpairsTree::SSpairsTree(int year_init) {
    // TTree and TFile
    t = new TTree("t", "t");
//    tfile = new_tfile;
    // Branches
    b_event = t->Branch("event", &event, "event/I");
    b_run = t->Branch("run", &run, "run/I");
    b_lumi = t->Branch("lumi", &lumi, "lumi/I");
    b_TotalEvents = t->Branch("TotalEvents", &TotalEvents, "TotalEvents/I");
    b_MinPt = t->Branch("MinPt", &MinPt, "MinPt/I");
    b_Loose = t->Branch("Loose", &Loose, "Loose/I");
    b_FO = t->Branch("FO", &FO, "FO/I");
    b_Tight = t->Branch("Tight", &Tight, "Tight/I");
    b_SSpairs = t->Branch("SSpairs", &SSpairs, "SSpairs/I");
    b_NoExtraZ = t->Branch("NoExtraZ", &NoExtraZ, "NoExtraZ/I");
    b_Min2Jets = t->Branch("Min2Jets", &Min2Jets, "Min2Jets/I");
    b_Nobtags = t->Branch("Nobtags", &Nobtags, "Nobtags/I");
    b_JetMass120 = t->Branch("JetMass120", &JetMass120, "JetMass120/I");
    b_JetMass500 = t->Branch("JetMass500", &JetMass500, "JetMass500/I");
    b_Min3Jets = t->Branch("Min3Jets", &Min3Jets, "Min3Jets/I");
    b_Min4Jets = t->Branch("Min4Jets", &Min4Jets, "Min4Jets/I");

}

void SSpairsTree::resetBranches() {
    // Reset branch values
    event = -999;
    run = -999;
    lumi = -999;

    TotalEvents = -999;
    MinPt = -999;
    Loose = -999;
    FO = -999;
    Tight = -999;
    SSpairs = -999;
    NoExtraZ = -999;
    Min2Jets = -999;

    Nobtags = -999;
    JetMass120 = -999;
    JetMass500 = -999;
    Min3Jets = -999;
    Min4Jets = -999;

    // Reset other values
    return;
}

int SSpairsTree::fillBranches() {

    return 0;
}

void SSpairsTree::fillTTree() {
    t->Fill();
    return;
}

void SSpairsTree::writeTFile() {
    tfile->cd();
    t->Write();
    tfile->Close();
    return;
}
    

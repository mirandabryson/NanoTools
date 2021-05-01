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
// Nano
#include "/home/users/ian/NanoTools/NanoCORE/Nano.h"
#include "/home/users/ian/NanoTools/NanoCORE/ElectronSelections.h"
#include "/home/users/ian/NanoTools/NanoCORE/MuonSelections.h"
#include "/home/users/ian/NanoTools/NanoCORE/SSSelections.h"
#include "/home/users/ian/NanoTools/NanoCORE/IsolationTools.h"
// Custom
#include "/home/users/ian/NanoTools/studies/sspairsNano/leppairs.h"

// Namespaces
using namespace std;
using namespace tas;

LeppairsTree::LeppairsTree(int year_init) {
    // TTree and TFile
    t = new TTree("t", "t");
//    tfile = new_tfile;
    // Branches
    b_run = t->Branch("run", &run, "run/I");
    b_lumi = t->Branch("lumi", &lumi, "lumi/I");
    b_event = t->Branch("event", &event, "event/I");

    b_jets_pt = t->Branch("jets_pt", &jets_pt);
    b_jets_eta = t->Branch("jets_eta", &jets_eta);
    b_jets_phi = t->Branch("jets_phi", &jets_phi);
    b_jets_id = t->Branch("jets_id", &jets_id);
    b_jets_btag = t->Branch("jets_btag", &jets_btag);

    b_lep1_id = t->Branch("lep1_id", &lep1_id, "lep1_id/I");
    b_lep1_idlevel = t->Branch("lep1_idlevel", &lep1_idlevel, "lep1_idlevel/I");
    b_lep1_iso = t->Branch("lep1_iso", &lep1_iso, "lep1_iso/O");
    b_lep1_multiiso = t->Branch("lep1_multiiso", &lep1_multiiso, "lep1_multiiso/O");
    b_lep1_miniiso = t->Branch("lep1_miniiso", &lep1_miniiso, "lep1_miniiso/F");
    b_lep1_ptrel = t->Branch("lep1_ptrel", &lep1_ptrel, "lep1_ptrel/F");
    b_lep1_ptratio = t->Branch("lep1_ptratio", &lep1_ptratio, "lep1_ptratio/F");
    b_lep1_isel = t->Branch("lep1_isel", &lep1_isel, "lep1_isel/O");
    b_lep1_ismu = t->Branch("lep1_ismu", &lep1_ismu, "lep1_ismu/O");
    b_lep1_isneg = t->Branch("lep1_isneg", &lep1_isneg, "lep1_isneg/O");
    b_lep1_p4 = t->Branch("lep1_p4", &lep1_p4);
    b_lep1_pt = t->Branch("lep1_pt", &lep1_pt, "lep1_pt/F");
    b_lep1_eta = t->Branch("lep1_eta", &lep1_eta, "lep1_eta/F");
    b_lep1_phi = t->Branch("lep1_phi", &lep1_phi, "lep1_phi/F");
    b_jet1_p4 = t->Branch("jet1_p4", &jet1_p4);
    b_jet1_pt = t->Branch("jet1_pt", &jet1_pt, "jet1_pt/F");
    b_jet1_eta = t->Branch("jet1_eta", &jet1_eta, "jet1_eta/F");
    b_jet1_phi = t->Branch("jet1_phi", &jet1_phi, "jet1_phi/F");

    b_lep2_id = t->Branch("lep2_id", &lep2_id, "lep2_id/I");
    b_lep2_idlevel = t->Branch("lep2_idlevel", &lep2_idlevel, "lep2_idlevel/I");
    b_lep2_iso = t->Branch("lep2_iso", &lep2_iso, "lep2_iso/O");
    b_lep2_multiiso = t->Branch("lep2_multiiso", &lep2_multiiso, "lep2_multiiso/O");
    b_lep2_miniiso = t->Branch("lep2_miniiso", &lep2_miniiso, "lep2_miniiso/F");
    b_lep2_ptrel = t->Branch("lep2_ptrel", &lep2_ptrel, "lep2_ptrel/F");
    b_lep2_ptratio = t->Branch("lep2_ptratio", &lep2_ptratio, "lep2_ptratio/F");
    b_lep2_isel = t->Branch("lep2_isel", &lep2_isel, "lep2_isel/O");
    b_lep2_ismu = t->Branch("lep2_ismu", &lep2_ismu, "lep2_ismu/O");
    b_lep2_isneg = t->Branch("lep2_isneg", &lep2_isneg, "lep2_isneg/O");
    b_lep2_p4 = t->Branch("lep2_p4", &lep2_p4);
    b_lep2_pt = t->Branch("lep2_pt", &lep2_pt, "lep2_pt/F");
    b_lep2_eta = t->Branch("lep2_eta", &lep2_eta, "lep2_eta/F");
    b_lep2_phi = t->Branch("lep2_phi", &lep2_phi, "lep2_phi/F");
    b_jet2_p4 = t->Branch("jet2_p4", &jet2_p4);
    b_jet2_pt = t->Branch("jet2_pt", &jet2_pt, "jet2_pt/F");
    b_jet2_eta = t->Branch("jet2_eta", &jet2_eta, "jet2_eta/F");
    b_jet2_phi = t->Branch("jet2_phi", &jet2_phi, "jet2_phi/F");

    b_pair_pt = t->Branch("pair_pt", &pair_pt, "pair_pt/F");
    b_pair_ss = t->Branch("pair_ss", &pair_ss, "pair_ss/O");
    b_pair_hyp = t->Branch("pair_hyp", &pair_hyp, "pair_hyp/I");

    b_lep3_id = t->Branch("lep3_id", &lep3_id, "lep3_id/I");
    b_lep3_idlevel = t->Branch("lep3_idlevel", &lep3_idlevel, "lep3_idlevel/I");
    b_lep3_iso = t->Branch("lep3_iso", &lep3_iso, "lep3_iso/O");
    b_lep3_multiiso = t->Branch("lep3_multiiso", &lep3_multiiso, "lep3_multiiso/O");
    b_lep3_miniiso = t->Branch("lep3_miniiso", &lep3_miniiso, "lep3_miniiso/F");
    b_lep3_ptrel = t->Branch("lep3_ptrel", &lep3_ptrel, "lep3_ptrel/F");
    b_lep3_ptratio = t->Branch("lep3_ptratio", &lep3_ptratio, "lep3_ptratio/F");
    b_lep3_isel = t->Branch("lep3_isel", &lep3_isel, "lep3_isel/O");
    b_lep3_ismu = t->Branch("lep3_ismu", &lep3_ismu, "lep3_ismu/O");
    b_lep3_isneg = t->Branch("lep3_isneg", &lep3_isneg, "lep3_isneg/O");
    b_lep3_p4 = t->Branch("lep3_p4", &lep3_p4);
    b_lep3_pt = t->Branch("lep3_pt", &lep3_pt, "lep3_pt/F");
    b_lep3_eta = t->Branch("lep3_eta", &lep3_eta, "lep3_eta/F");
    b_lep3_phi = t->Branch("lep3_phi", &lep3_phi, "lep3_phi/F");
    b_jet3_p4 = t->Branch("jet3_p4", &jet3_p4);
    b_jet3_pt = t->Branch("jet3_pt", &jet3_pt, "jet3_pt/F");
    b_jet3_eta = t->Branch("jet3_eta", &jet3_eta, "jet3_eta/F");
    b_jet3_phi = t->Branch("jet3_phi", &jet3_phi, "jet3_phi/F");

    b_lep4_id = t->Branch("lep4_id", &lep4_id, "lep4_id/I");
    b_lep4_idlevel = t->Branch("lep4_idlevel", &lep4_idlevel, "lep4_idlevel/I");
    b_lep4_iso = t->Branch("lep4_iso", &lep4_iso, "lep4_iso/O");
    b_lep4_multiiso = t->Branch("lep4_multiiso", &lep4_multiiso, "lep4_multiiso/O");
    b_lep4_miniiso = t->Branch("lep4_miniiso", &lep4_miniiso, "lep4_miniiso/F");
    b_lep4_ptrel = t->Branch("lep4_ptrel", &lep4_ptrel, "lep4_ptrel/F");
    b_lep4_ptratio = t->Branch("lep4_ptratio", &lep4_ptratio, "lep4_ptratio/F");
    b_lep4_isel = t->Branch("lep4_isel", &lep4_isel, "lep4_isel/O");
    b_lep4_ismu = t->Branch("lep4_ismu", &lep4_ismu, "lep4_ismu/O");
    b_lep4_isneg = t->Branch("lep4_isneg", &lep4_isneg, "lep4_isneg/O");
    b_lep4_p4 = t->Branch("lep4_p4", &lep4_p4);
    b_lep4_pt = t->Branch("lep4_pt", &lep4_pt, "lep4_pt/F");
    b_lep4_eta = t->Branch("lep4_eta", &lep4_eta, "lep4_eta/F");
    b_lep4_phi = t->Branch("lep4_phi", &lep4_phi, "lep4_phi/F");
    b_jet4_p4 = t->Branch("jet4_p4", &jet4_p4);
    b_jet4_pt = t->Branch("jet4_pt", &jet4_pt, "jet4_pt/F");
    b_jet4_eta = t->Branch("jet4_eta", &jet4_eta, "jet4_eta/F");
    b_jet4_phi = t->Branch("jet4_phi", &jet4_phi, "jet4_phi/F");

    // Other
    year = year_init;
}

void LeppairsTree::reset() {
    // Reset branch values
    run = 0;
    lumi = 0;
    event = -999;

    jets_pt.clear();
    jets_eta.clear();
    jets_phi.clear();
    jets_id.clear();
    jets_btag.clear();

    lep1_id = -999;
    lep1_idlevel = -999;
    lep1_iso = false;
    lep1_multiiso = false;
    lep1_miniiso = -999.;
    lep1_ptrel = -999.;
    lep1_ptratio = -999.;
    lep1_isel = false;
    lep1_ismu = false;
    lep1_isneg = false;
    lep1_p4 = LorentzVector();
    lep1_pt = -999.;
    lep1_eta = -999.;
    lep1_phi = -999.;
    jet1_p4 = LorentzVector();
    jet1_pt = -999.;
    jet1_eta = -999.;
    jet1_phi = -999.;

    lep2_id = -999;
    lep2_idlevel = -999;
    lep2_iso = false;
    lep2_multiiso = false;
    lep2_miniiso = -999.;
    lep2_ptrel = -999.;
    lep2_ptratio = -999.;
    lep2_isel = false;
    lep2_ismu = false;
    lep2_isneg = false;
    lep2_p4 = LorentzVector();
    lep2_pt = -999.;
    lep2_eta = -999.;
    lep2_phi = -999.;
    jet2_p4 = LorentzVector();
    jet2_pt = -999.;
    jet2_eta = -999.;
    jet2_phi = -999.;

    pair_pt = -999.;
    pair_ss = false;
    pair_hyp = -1;

    lep3_id = -999;
    lep3_idlevel = -999;
    lep3_iso = false;
    lep3_multiiso = false;
    lep3_miniiso = -999.;
    lep3_ptrel = -999.;
    lep3_ptratio = -999.;
    lep3_isel = false;
    lep3_ismu = false;
    lep3_isneg = false;
    lep3_p4 = LorentzVector();
    lep3_pt = -999.;
    lep3_eta = -999.;
    lep3_phi = -999.;
    jet3_p4 = LorentzVector();
    jet3_pt = -999.;
    jet3_eta = -999.;
    jet3_phi = -999.;

    lep4_id = -999;
    lep4_idlevel = -999;
    lep4_iso = false;
    lep4_multiiso = false;
    lep4_miniiso = -999.;
    lep4_ptrel = -999.;
    lep4_ptratio = -999.;
    lep4_isel = false;
    lep4_ismu = false;
    lep4_isneg = false;
    lep4_p4 = LorentzVector();
    lep4_pt = -999.;
    lep4_eta = -999.;
    lep4_phi = -999.;
    jet4_p4 = LorentzVector();
    jet4_pt = -999.;
    jet4_eta = -999.;
    jet4_phi = -999.;

    // Reset other values
    return;
}

void JetFinder(float dRcut, LorentzVector lep_p4, LorentzVector &jet_p4, float &jet_pt, float &jet_eta, float &jet_phi) {
    float deta;
    float dphi;
    float deltaR;

    for (unsigned int i = 0; i < Jet_p4().size(); i++) {
        deta = lep_p4.eta() - Jet_eta().at(i);
        dphi = lep_p4.phi() - Jet_phi().at(i);
        deltaR = sqrt(deta * deta + dphi * dphi);

        if (deltaR < dRcut) {
            jet_p4 = Jet_p4().at(i);
            jet_pt = Jet_pt().at(i);
            jet_eta = Jet_eta().at(i);
            jet_phi = Jet_phi().at(i);

            dRcut = deltaR;
        }
    }

    return;
}

bool highPt (Lepton lepa, Lepton lepb) {
    return (lepa.pt() > lepb.pt());
}

int LeppairsTree::fillBranches() {

    double e_mini_iso_cut = 0.12;
    double e_pt_ratio_cut = 0.80;
    double e_pt_rel_cut = 7.2;

    double u_mini_iso_cut = 0.16;
    double u_pt_ratio_cut = 0.76;
    double u_pt_rel_cut = 7.2;

    float dRcut = 0.4;

    run = nt.run();
    lumi = luminosityBlock();
    event = nt.event();

    for (unsigned int i = 0; i < Jet_pt().size(); i++) {
        jets_pt.push_back(Jet_pt().at(i));
        jets_eta.push_back(Jet_eta().at(i));
        jets_phi.push_back(Jet_phi().at(i));
        jets_id.push_back(Jet_jetId().at(i));
        jets_btag.push_back(Jet_btagDeepB().at(i));
    }

    Leptons leps;
    for (unsigned int i = 0; i < nElectron(); i++) {
        leps.push_back(Lepton(Electron_pdgId().at(i), i));
    }
    for (unsigned int i = 0; i < nMuon(); i++) {
        leps.push_back(Lepton(Muon_pdgId().at(i), i));
    }
    sort(leps.begin(), leps.end(), highPt);

    if (leps.size() >= 1) {
        lep1_id = leps[0].id();
        lep1_idlevel = leps[0].idlevel();
        lep1_isel = leps[0].is_el();
        lep1_ismu = leps[0].is_mu();
        if (lep1_isel) {
            lep1_multiiso = passesLeptonIso(leps[0].idx(), lep1_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep1_miniiso, lep1_ptratio, lep1_ptrel);
        } else if (lep1_ismu) {
            lep1_multiiso = passesLeptonIso(leps[0].idx(), lep1_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep1_miniiso, lep1_ptratio, lep1_ptrel);
        }
        lep1_p4 = leps[0].p4();
        lep1_pt = leps[0].pt();
        if (abs(lep1_id) == 11) {lep1_pt = lep1_pt/Electron_eCorr().at(leps[0].idx());}
        lep1_eta = leps[0].eta();
        lep1_phi = leps[0].phi();

        JetFinder(dRcut, lep1_p4, jet1_p4, jet1_pt, jet1_eta, jet1_phi);
    }


    if (leps.size() >= 2) {
        lep2_id = leps[1].id();
        lep2_idlevel = leps[1].idlevel();
        lep2_isel = leps[1].is_el();
        lep2_ismu = leps[1].is_mu();
        if (lep2_isel) {
            lep2_multiiso = passesLeptonIso(leps[1].idx(), lep2_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep2_miniiso, lep2_ptratio, lep2_ptrel);
        } else if (lep2_ismu) {
            lep2_multiiso = passesLeptonIso(leps[1].idx(), lep2_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep2_miniiso, lep2_ptratio, lep2_ptrel);
        }
        lep2_p4 = leps[1].p4();
        lep2_pt = leps[1].pt();
        if (abs(lep2_id) == 11) {lep2_pt = lep2_pt/Electron_eCorr().at(leps[1].idx());}
        lep2_eta = leps[1].eta();
        lep2_phi = leps[1].phi();
    
        JetFinder(dRcut, lep2_p4, jet2_p4, jet2_pt, jet2_eta, jet2_phi);
    }

    if (leps.size() >= 3) {
        lep3_id = leps[2].id();
        lep3_idlevel = leps[2].idlevel();
        lep3_isel = leps[2].is_el();
        lep3_ismu = leps[2].is_mu();
        if (lep3_isel) {
            lep3_multiiso = passesLeptonIso(leps[2].idx(), lep3_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep3_miniiso, lep3_ptratio, lep3_ptrel);
        } else if (lep3_ismu) {
            lep3_multiiso = passesLeptonIso(leps[2].idx(), lep3_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep3_miniiso, lep3_ptratio, lep3_ptrel);
        }
        lep3_p4 = leps[2].p4();
        lep3_pt = leps[2].pt();
        if (abs(lep3_id) == 11) {lep3_pt = lep3_pt/Electron_eCorr().at(leps[2].idx());}
        lep3_eta = leps[2].eta();
        lep3_phi = leps[2].phi();

        JetFinder(dRcut, lep3_p4, jet3_p4, jet3_pt, jet3_eta, jet3_phi);
    }

    if (leps.size() >= 4) {
        lep4_id = leps[3].id();
        lep4_idlevel = leps[3].idlevel();
        lep4_isel = leps[3].is_el();
        lep4_ismu = leps[3].is_mu();
        if (lep4_isel) {
            lep4_multiiso = passesLeptonIso(leps[3].idx(), lep4_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep4_miniiso, lep4_ptratio, lep4_ptrel);
        } else if (lep4_ismu) {
            lep4_multiiso = passesLeptonIso(leps[3].idx(), lep4_id, e_mini_iso_cut, e_pt_ratio_cut, e_pt_rel_cut, lep4_miniiso, lep4_ptratio, lep4_ptrel);
        }
        lep4_p4 = leps[3].p4();
        lep4_pt = leps[3].pt();
        if (abs(lep4_id) == 11) {lep4_pt = lep4_pt/Electron_eCorr().at(leps[3].idx());}
        lep4_eta = leps[3].eta();
        lep4_phi = leps[3].phi();

        JetFinder(dRcut, lep4_p4, jet4_p4, jet4_pt, jet4_eta, jet4_phi);
    }

    t->Fill();

    return 0;
}

void LeppairsTree::fillTTree() {
    t->Fill();
    return;
}

void LeppairsTree::writeTFile() {
    tfile->cd();
    t->Write();
    tfile->Close();
    return;
}

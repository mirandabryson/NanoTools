// -*- C++ -*-

// C++
#include <iostream>
#include <vector>
#include <string>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TString.h"

// CORE
#include "CORE/CMS3.h"
#include "CORE/ElectronSelections.h"
#include "CORE/MuonSelections.h"
#include "CORE/PhotonSelections.h"
#include "CORE/IsolationTools.h"
#include "CORE/TriggerSelections.h"
#include "CORE/MetSelections.h"

// Tools
#include "CORE/Tools/JetCorrector.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"

// Global Variables
DatasetInfoFromFile datasetInfoFromFile;
readMVA* globalEleMVAreader = 0;

// Custom
#include "./counter.C"
#include "./ElectronSelectionsCMS4.C"
#include "./MuonSelectionsCMS4.C"

// Namespaces
using namespace std;
using namespace tas;


int ScanChainCMS4(TChain* chain, TString id_level, TString flavor) {

    // Variables
    bool fast = true;
    int nEvents = -1;

    /* --> Initialization <-- */
    // Set gConf parameters
    gconf.year = 2016;
    gconf.cmssw_ver = 94;
    gconf.ea_version = 1;
    gconf.btag_disc_wp = 0.6324;
    gconf.WP_DEEPCSV_TIGHT  = 0.8958;
    gconf.WP_DEEPCSV_MEDIUM = 0.6324;
    gconf.WP_DEEPCSV_LOOSE  = 0.2219;
    gconf.multiiso_el_minireliso = 0.12;
    gconf.multiiso_el_ptratio = 0.80;
    gconf.multiiso_el_ptrel = 7.2;
    gconf.multiiso_mu_minireliso = 0.16;
    gconf.multiiso_mu_ptratio = 0.76;
    gconf.multiiso_mu_ptrel = 7.2;
    string jecEra = "Summer16_07Aug2017BCD_V11";
    string jecEraMC = "Summer16_07Aug2017_V11";
    // Initialize JECs
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
    jetcorr_filenames_25ns_MC_pfL1.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");

    FactorizedJetCorrector *jetCorrAG = nullptr;
    FactorizedJetCorrector *jetCorrAG_L1 = nullptr;
    FactorizedJetCorrector *jetCorrAG_L2L3 = nullptr;
    jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1);
    jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL2L3);
    jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1L2L3);
    // for closestJet() in CORE/IsolationTools.cc
    gconf.jet_corrector_L1 = jetCorrAG_L1;
    gconf.jet_corrector_L2L3 = jetCorrAG_L2L3;
    // Initialize MVA
    createAndInitMVA("./CORE", true, true, 80);

    /* --> File Loop <-- */
    // Get # events
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();
    if (nEvents >= 0) nEventsChain = nEvents;
    // Get list of files
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;
    // Loop over files
    while ( (currentFile = (TFile*)fileIter.Next()) ) {

        // Get File Content
        TFile *file = TFile::Open(currentFile->GetTitle());
        TTree *tree = (TTree*)file->Get("Events");
        if (fast) TTreeCache::SetLearnEntries(10);
        if (fast) tree->SetCacheSize(128*1024*1024);
        cms3.Init(tree);
        // Initialize counters
        vector<int> el_counts;
        vector<int> mu_counts;
        vector<string> el_cutflow_names;
        vector<string> mu_cutflow_names;
        // Loop over Events in current file
        cout << "START LEPTON COMPARISON" << endl;
        // cout << "cut,result,event,lumiblock" << endl;
        // START DEBUG
        cout << "cut,result,pt,event,lumiblock" << endl;
        // END DEBUG
        if (nEventsTotal >= nEventsChain) continue;
        unsigned int nEventsTree = tree->GetEntriesFast();
        for (unsigned int event = 0; event < nEventsTree; ++event) {

            // Get Event Content
            if (nEventsTotal >= nEventsChain) continue;
            if (fast) tree->LoadTree(event);
            cms3.GetEntry(event);
            ++nEventsTotal;

            /* --> Start Analysis Code <-- */
            if (flavor == "electron") {
                // Electrons
                bool no_el_counts = el_counts.empty();
                for (unsigned int i = 0; i < els_p4().size(); i++) {
                    // // Nano preselection
                    if (els_p4().at(i).pt() <= 5) {
                        continue;
                    }
                    Counter results;
                    if (id_level == "tight") {
                        results = isGoodElectronCMS4(i);
                    }
                    else if (id_level == "loose") {
                        results = isFakableElectronCMS4(i);
                    }
                    else {
                        cout << "Invalid ID level" << endl;
                        return 0;
                    }
                    vector<int> counts = results.counts;
                    el_cutflow_names = results.names;
                    for (unsigned int j = 0; j < counts.size(); j++) {
                        int count = counts.at(j);
                        if (no_el_counts) {
                            el_counts.push_back(count);
                        }
                        else {
                            el_counts[j] += count;
                        }
                    }
                    if (el_counts.size() == counts.size()) {
                        no_el_counts = false;
                    }
                    else {
                        cout << "Something went horribly wrong..." << endl;
                        return 0;
                    }
                }
            }
            if (flavor == "muon") {
                // Muons
                bool no_mu_counts = mu_counts.empty();
                for (unsigned int i = 0; i < mus_p4().size(); i++) {
                    vector<bool> POG_IDs;
                    POG_IDs.push_back(passesMuonPOG(muID::CutBasedIdLoose, i));
                    POG_IDs.push_back(passesMuonPOG(muID::SoftCutBasedId, i));
                    POG_IDs.push_back(passesMuonPOG(muID::SoftMvaId, i));
                    POG_IDs.push_back(passesMuonPOG(muID::CutBasedIdGlobalHighPt, i));
                    POG_IDs.push_back(passesMuonPOG(muID::CutBasedIdTrkHighPt, i));
                    bool POG_pass = false;
                    for (unsigned int k = 0; k < POG_IDs.size(); k++) {
                        if (POG_IDs.at(k) == true) {
                            POG_pass = true;
                            break;
                        }
                    }
                    if (!POG_pass) {
                        continue;
                    }
                    // // Nano preselection
                    if (mus_p4().at(i).pt() <= 3) {
                        continue;
                    }
                    Counter results;
                    if (id_level == "tight") {
                        results = isGoodMuonCMS4(i);
                    }
                    else if (id_level == "loose") {
                        results = isFakableMuonCMS4(i);
                    }
                    else {
                        cout << "Invalid ID level" << endl;
                        return 0;
                    }
                    vector<int> counts = results.counts;
                    mu_cutflow_names = results.names;
                    for (unsigned int j = 0; j < counts.size(); j++) {
                        int count = counts.at(j);
                        if (no_mu_counts) {
                            mu_counts.push_back(count);
                        }
                        else {
                            mu_counts[j] += count;
                        }
                    }
                    if (mu_counts.size() == counts.size()) {
                        no_mu_counts = false;
                    }
                    else {
                        cout << "Something went horribly wrong..." << endl;
                        return 0;
                    }
                }
            }
            /* --> END Analysis Code <-- */
        }

        // Print out
        cout << "START FILE SUMMARY" << endl;
        cout << "Events parsed: " << nEventsTotal << " events" << endl;
        if (flavor == "electron") {
            for (unsigned int i = 0; i < el_counts.size(); i++) {
                if (i > 0) {
                    cout << "&& ";
                }
                cout << el_cutflow_names.at(i) << ": " << endl;
                cout << "    --> OBJ LEVEL CUTFLOW <-- " << endl;
                cout << "    lep passed cut: " << el_counts.at(i) << " reco leptons" << endl;
            }
        }
        if (flavor == "muon") {
            for (unsigned int i = 0; i < mu_counts.size(); i++) {
                if (i > 0) {
                    cout << "&& ";
                }
                cout << mu_cutflow_names.at(i) << ": " << endl;
                cout << "    --> OBJ LEVEL CUTFLOW <-- " << endl;
                cout << "    lep passed cut: " << mu_counts.at(i) << " reco leptons" << endl;
            }
        }
  
        // Clean Up
        delete tree;
        file->Close();
    }
    if (nEventsChain != nEventsTotal) {
        cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
    }

    return 0;
}

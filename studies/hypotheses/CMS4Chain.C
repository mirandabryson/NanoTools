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

// Custom
#include "cms4hyps.h"

// Namespaces
using namespace std;
using namespace tas;

// Global Variables
DatasetInfoFromFile datasetInfoFromFile;

int CMS4Chain(TChain* chain, TString outName, bool verbose=false, bool fast=true, int nEvents=-1, string skimFilePrefix="test") {

    // Benchmark
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    /* --> Initialization <-- */
    // Initialize TFile
    TFile* f = new TFile(outName, "RECREATE");
    // Initialize TTree
    CMS4HypsTree* hyps_tree = new CMS4HypsTree();
    TTree* ht_ttree = hyps_tree->t;
    // Set gConf parameters
    string jecEra = "Fall17_17Nov2017B_V32";
    string jecEraMC = "Fall17_17Nov2017_V32";
    gconf.year = 2017;
    gconf.ea_version = 4;
    gconf.cmssw_ver = 94;
    gconf.btag_disc_wp = 0.4941;
    gconf.WP_DEEPCSV_TIGHT  = 0.8001;
    gconf.WP_DEEPCSV_MEDIUM = 0.4941;
    gconf.WP_DEEPCSV_LOOSE  = 0.1522;
    gconf.multiiso_el_minireliso = 0.07;
    gconf.multiiso_el_ptratio = 0.78;
    gconf.multiiso_el_ptrel = 8.0;
    gconf.multiiso_mu_minireliso = 0.11;
    gconf.multiiso_mu_ptratio = 0.74;
    gconf.multiiso_mu_ptrel = 6.8;
    gconf.SS_innerlayers = 0;
    // Initialize JECs
    createAndInitMVA("./CORE", true, true, 80);
    vector<string> jetcorr_filenames_25ns_MC_pfL1;
    vector<string> jetcorr_filenames_25ns_MC_pfL2L3;
    vector<string> jetcorr_filenames_25ns_MC_pfL1L2L3;
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
    createAndInitMVA("CORE", true, true, 80);

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

        // Loop over Events in current file
        if (nEventsTotal >= nEventsChain) continue;
        unsigned int nEventsTree = tree->GetEntriesFast();
        for (unsigned int event = 0; event < nEventsTree; ++event) {

            // Get Event Content
            if (nEventsTotal >= nEventsChain) continue;
            if (fast) tree->LoadTree(event);
            cms3.GetEntry(event);
            ++nEventsTotal;

            // Progress
            CMS3::progress( nEventsTotal, nEventsChain );

            /* --> Start Analysis Code <-- */
            hyps_tree->reset();
            // Fill event-level info
            hyps_tree->event = evt_event();
            hyps_tree->run = evt_run();
            hyps_tree->lumi = evt_lumiBlock();
            hyps_tree->fillBranches();
            ht_ttree->Fill();

            /* --> END Analysis Code <-- */
        }
  
        // Clean Up
        delete tree;
        file->Close();
    }
    if (nEventsChain != nEventsTotal) {
        cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
    }
  
    // Write tree
    f->cd();
    ht_ttree->Write();
    f->Close();

    // return
    bmark->Stop("benchmark");
    cout << endl;
    cout << nEventsTotal << " Events Processed" << endl;
    cout << "------------------------------" << endl;
    cout << "CPU  Time: " << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
    cout << "Real Time: " << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
    cout << endl;
    delete bmark;
    return 0;
}

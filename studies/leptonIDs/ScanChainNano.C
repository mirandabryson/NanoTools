#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"
#include "TCanvas.h"

#include "./NanoCORE/Nano.h"
#include "./NanoCORE/SSSelections.cc"

#include"./counter.C"
#include "./ElectronSelectionsNano.C"
#include "./MuonSelectionsNano.C"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using namespace tas;

int ScanChainNano(TChain *chain, TString id_level, TString flavor) {
    // Set configuration parameters
    gconf.year = 2016;
    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = chain->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    // File loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        // Open file
        TFile *file = TFile::Open(currentFile->GetTitle());
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        // TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
        nt.Init(tree);
        // Initialize counters
        vector<int> el_counts;
        vector<int> mu_counts;
        vector<string> el_cutflow_names;
        vector<string> mu_cutflow_names;
        // Event loop
        cout << "START LEPTON COMPARISON" << endl;
        // cout << "cut,result,event,lumiblock" << endl;
        // START DEBUG
        cout << "cut,result,pt,event,lumiblock" << endl;
        // END DEBUG
        for(unsigned int evt = 0; evt < tree->GetEntriesFast(); ++evt) {
            // Load event
            nt.GetEntry(evt);
            tree->LoadTree(evt);
            // Update progress
            nEventsTotal++;
            /* Analysis code */
            if (flavor == "electron") {
                // Electrons
                bool no_el_counts = el_counts.empty();
                for (unsigned int i = 0; i < nElectron(); i++) {
                    Counter results;
                    if (id_level == "tight") {
                        results = isGoodElectronNano(i);
                    }
                    else if (id_level == "loose") {
                        results = isFakableElectronNano(i);
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
                for (unsigned int i = 0; i < nMuon(); i++) {
                    Counter results;
                    if (id_level == "tight") {
                        results = isGoodMuonNano(i);
                    }
                    else if (id_level == "loose") {
                        results = isFakableMuonNano(i);
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
        } // END event loop

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

        // Clean up
        delete file;

    } // END file loop

    // Wrap up
    return 0;
}

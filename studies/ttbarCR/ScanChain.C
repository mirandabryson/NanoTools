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
#include "./NanoCORE/Config.h"
#include "./NanoCORE/SSSelections.cc"
#include "./NanoCORE/tqdm.h"
#include "./control.h"
#include "./HLTs.C"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace tas;

int ScanChain(TChain *ch, TString out_name, TString sample_name) {
    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
    tqdm bar; // progress bar
    // File loop
    cout << "STARTING FILE LOOP" << endl;
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        // Open file
        TFile *file = TFile::Open(currentFile->GetTitle());
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        // Imported TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
        nt.Init(tree);
        // Output
        TFile* control_tfile = new TFile(out_name, "RECREATE");
        // Custom TTree
        ControlTree* control_tree = new ControlTree(control_tfile);
        // Hists
        TH1F *n_events = new TH1F("n_events", "n_events", 1, 0, 1);
        control_tree->trackTH1F(n_events);
        // Event loop
        cout << "STARTING EVENT LOOP" << endl;
        for (unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            // Reset tree
            control_tree->resetBranches();
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);
            /* Analysis code */
			// HLTS
            bool passes_HLTs = checkHLTs(MuonElec_HLTs);
			if (passes_HLTs) {	
				control_tree->fillBranches();
			}
            if (!isData()) {
                n_events->Fill(0.5, genWeight());
            }
        } // END event loop

        // Clean up
        delete file;
        control_tree->writeTFile();

    } // END file loop

    // Wrap up
    bar.finish();
    return 0;
}

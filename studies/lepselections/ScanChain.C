#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"

#include "../../NanoCORE/Nano.h"
#include "../../NanoCORE/SSSelections.cc"
#include "../../NanoCORE/MetSelections.cc"
#include "../../NanoCORE/ElectronSelections.cc"
#include "../../NanoCORE/tqdm.h"
#include "leptons.h"

#include <iostream>
#include <iomanip>

#define SUM(vec) std::accumulate((vec).begin(), (vec).end(), 0);
#define SUM_GT(vec,num) std::accumulate((vec).begin(), (vec).end(), 0, [](float x,float y){return ((y > (num)) ? x+y : x); });
#define COUNT_GT(vec,num) std::count_if((vec).begin(), (vec).end(), [](float x) { return x > (num); });
#define COUNT_LT(vec,num) std::count_if((vec).begin(), (vec).end(), [](float x) { return x < (num); });

#define H1(name,nbins,low,high) TH1F *h_##name = new TH1F(#name,#name,nbins,low,high);

// #define DEBUG

struct debugger { template<typename T> debugger& operator , (const T& v) { cerr<<v<<" "; return *this; } } dbg;
#ifdef DEBUG
    #define debug(args...) do {cerr << #args << ": "; dbg,args; cerr << endl;} while(0)
#else
    #define debug(args...)
#endif

using namespace std;
using namespace tas;

int ScanChain(TChain *ch) {
    // Output
    TFile* out_tfile = new TFile("inclusive.root", "RECREATE");
    // Set configuration parameters
    gconf.year = 2017;
    // Custom TTree
    LeptonsTree* leptons_tree = new LeptonsTree(2017);
    TTree* lt_ttree = leptons_tree->t;
    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
    tqdm bar;
    // File loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        // Open file
        TFile *file = TFile::Open( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        // TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
        nt.Init(tree, 2017);
        // Event loop
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            leptons_tree->reset();
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);
            // Fill event-level info
            leptons_tree->event = nt.event();
            // Fill object-level branches
            leptons_tree->fillBranches();
        } // END event loop

        // Clean up
        delete file;

    } // END file loop
    
    // Wrap up
    bar.finish();
    out_tfile->cd();
    lt_ttree->Write();
    out_tfile->Close();

    return 0;
}

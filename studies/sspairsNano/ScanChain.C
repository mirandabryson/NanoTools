#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"

#include "/home/users/ian/NanoTools/NanoCORE/Nano.h"
#include "/home/users/ian/NanoTools/NanoCORE/SSSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MetSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/ElectronSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MuonSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/IsolationTools.cc"
#include "/home/users/ian/NanoTools/NanoCORE/tqdm.h"
#include "/home/users/ian/NanoTools/studies/sspairsNano/leppairs.h"

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

void write_debug(std::ofstream& inf, int r, int l, int e, int id, float eta, float phi, float pt /*, float ptratio*/) {
    inf << Form("%d, %d, %d, %d, %f, %f, %f", r, l, e, id, eta, phi, pt/*, ptratio*/) << endl;
    return;
}

int ScanChain(TChain *ch, TString option="", TString outputdir="../../../public_html/share/debug") {
    // Output
    TFile* out_tfile = new TFile("NANOdata.root", "RECREATE");

    // Debug files
    bool debug = true;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/lep_info_nano.dat", outputdir.Data()/*, ch->GetTitle()*/), ios::out);
    }

    // Set configuration parameters
    gconf.year = 2016;

    // Custom TTree
    LeppairsTree* leppairs_tree = new LeppairsTree(2016);
    TTree* lt_ttree = leppairs_tree->t;

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
//        nt.Init(tree, 2016);
        nt.SetYear(2016);
        nt.Init(tree);
        // Event loop
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            leppairs_tree->reset();
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);
            // Fill event-level info
            leppairs_tree->event = nt.event();

            if (debug and dfile.is_open()){
                Leptons unsortedleps;
    
                for (int i = 0; i < nElectron(); i++) {
                    unsortedleps.push_back(Lepton(Electron_pdgId().at(i), i));
                }
                for (int i = 0; i < nMuon(); i++) {
                    unsortedleps.push_back(Lepton(Muon_pdgId().at(i), i));
                }

                Leptons leps;
                while (unsortedleps.size() != 0) {
                    int mxptidx = -1;
                    float mxpt = 0.;
                    for (int i = 0; i < unsortedleps.size(); i++) {
                        if (abs(unsortedleps[i].id()) == 11) {
                            if (unsortedleps[i].pt()/Electron_eCorr().at(unsortedleps[i].idx()) > mxpt) {
                            mxptidx = i;
                            mxpt = unsortedleps[i].pt()/Electron_eCorr().at(unsortedleps[i].idx());
                            }
                        } else if (unsortedleps[i].pt() > mxpt) {
                            mxptidx = i;
                            mxpt = unsortedleps[i].pt();
                        }
                    }
                    leps.push_back(unsortedleps[mxptidx]);
                    unsortedleps.erase(unsortedleps.begin() + mxptidx);
                }

/*                Leptons leps = getLeptons();*/
                for (int i = 0; i < leps.size(); i++) {
                    write_debug(dfile, nt.run(), luminosityBlock(), nt.event(), leps[i].id(), leps[i].eta(), leps[i].phi(), leps[i].pt());
                }
            }

            // Fill object-level branches
            leppairs_tree->fillBranches();
        } // END event loop

        // Clean up
        delete file;

    } // END file loop

    // Wrap up
    bar.finish();
    out_tfile->cd();
    lt_ttree->Write();
    out_tfile->Close();
    if (dfile.is_open()) dfile.close();

    return 0;
}

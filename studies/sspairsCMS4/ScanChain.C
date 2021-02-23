#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"

// CORE
#include "/home/users/ian/VBSAna/common/CORE/CMS3.h"
#include "/home/users/ian/VBSAna/common/CORE/SSSelections.cc"
#include "/home/users/ian/VBSAna/common/CORE/MetSelections.cc"
#include "/home/users/ian/VBSAna/common/CORE/ElectronSelections.cc"
#include "/home/users/ian/VBSAna/common/CORE/MuonSelections.cc"
#include "/home/users/ian/VBSAna/common/CORE/IsolationTools.cc"

// Tools
#include "/home/users/ian/VBSAna/common/CORE/Tools/JetCorrector.h"
#include "/home/users/ian/VBSAna/common/CORE/Tools/goodrun.h"
#include "/home/users/ian/VBSAna/common/CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "/home/users/ian/VBSAna/common/CORE/Tools/datasetinfo/getDatasetInfo.h"

#include "/home/users/ian/NanoTools/NanoCORE/tqdm.h"
#include "/home/users/ian/NanoTools/studies/sspairsCMS4/leppairs.h"

#include <iostream>
#include <iomanip>

#include <fstream>
#include <string>

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

void write_debug(std::ofstream& inf, int r, int l, int e, int id, float eta, float phi, float pt) {
    inf << Form("%d, %d, %d, %d, %f, %f, %f", r, l, e, id, eta, phi, pt) << endl;
    return;
}

int ScanChain(TChain *ch, TString option="", TString outputdir="../../../public_html/share/debug") {
    // Output
    TFile* out_tfile = new TFile("CMS4data.root", "RECREATE");

    // Debug files
    bool debug = true;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/lep_info_cms4.dat", outputdir.Data()/*, ch->GetTitle()*/), ios::out);
    }

    // Set configuration parameters
    gconf.year = 2016;

    // JEC parameters
    string jecEra = "Summer16_07Aug2017BCD_V11";
    string jecEraMC = "Summer16_07Aug2017_V11";

    // Initialize JECs
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
    jetcorr_filenames_25ns_MC_pfL1.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back("../../../VBSAna/common/CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");

    FactorizedJetCorrector *jetCorrAG = nullptr;
    FactorizedJetCorrector *jetCorrAG_L1 = nullptr;
    FactorizedJetCorrector *jetCorrAG_L2L3 = nullptr;
    jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1);
    jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL2L3);
    jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1L2L3);

    // for closestJet() in CORE/IsolationTools.cc
    gconf.jet_corrector_L1 = jetCorrAG_L1;
    gconf.jet_corrector_L2L3 = jetCorrAG_L2L3;


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
//        cout << "File" << endl;
        TFile *file = TFile::Open( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        // TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
//        cms3.Init(tree, 2016);
//        cms3.SetYear(2016);
//        tree->Print();
//        cout << "Pre-Init" << endl;                
        cms3.Init(tree);
//        cout << "Post-Init" << endl;
        // Event loop
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
//            cout << "Event" << endl;
            leppairs_tree->reset();
            // Load event
            cms3.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);
            // Fill event-level info
            leppairs_tree->event = cms3.evt_event();
            // Fill debug info
            if (debug and dfile.is_open()){
                for (int i = 0; i < els_p4().size(); i++) {
                    if (els_p4().at(i).pt() <= 5) continue;
                    write_debug(dfile, evt_run(), evt_lumiBlock(), evt_event(), (-11 * els_charge().at(i)), els_p4().at(i).eta(), els_p4().at(i).phi(), els_p4().at(i).pt());
                }
                for (int i = 0; i < mus_p4().size(); i++) {
                    // Nano preselections
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
                if (!POG_pass) {continue;}
                if (mus_p4().at(i).pt() <= 3) {continue;}
                    write_debug(dfile, evt_run(), evt_lumiBlock(), evt_event(), (-13 * mus_charge().at(i)), mus_p4().at(i).eta(), mus_p4().at(i).phi(), mus_p4().at(i).pt());
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

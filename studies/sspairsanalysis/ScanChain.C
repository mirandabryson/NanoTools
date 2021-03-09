#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TMVA/Reader.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TRandom.h"


#include "/home/users/ian/NanoTools/NanoCORE/Nano.h"
#include "/home/users/ian/NanoTools/NanoCORE/SSSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MetSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/ElectronSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MuonSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/IsolationTools.cc"
//#include "/home/users/ian/NanoTools/studies/sspairsanalysis/sspairs.C"
#include "/home/users/ian/NanoTools/NanoCORE/tqdm.h"

#include <string>
#include <iostream>
#include <iomanip>

#define SUM(vec) std::accumulate((vec).begin(), (vec).end(), 0);
#define SUM_GT(vec,num) std::accumulate((vec).begin(), (vec).end(), 0, [](float x,float y){return ((y > (num)) ? x+y : x); });
#define COUNT_GT(vec,num) std::count_if((vec).begin(), (vec).end(), [](float x) { return x > (num); });
#define COUNT_LT(vec,num) std::count_if((vec).begin(), (vec).end(), [](float x) { return x < (num); });

#define H1(name,nbins,low,high) TH1F *h_##name = new TH1F(#name,#name,nbins,low,high);

using namespace std;
using namespace tas;

// Useful functions

// 2D histogram
struct HistCol2D {
    map<string, TH2D> in;
    HistCol2D(vector<string> regions, const string& name, int nbinsx, float lowx, float highx, int nbinsy, float lowy, float highy, vector<HistCol2D*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            in.emplace(region, TH2D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbinsx, lowx, highx, nbinsy, lowy, highy));
        }
        if (registry != nullptr) registry->push_back(this);
    }
    void Fill(const string& region, int id1, int id2, float valx, float valy, float weight) { in[region].Fill(valx, valy, weight); }
    void Write() {for (auto p : in) p.second.Write();}
};

// 1D histogram
struct HistCol {
    map<string, TH1D> in;
    map<string, TH1D> ee;
    map<string, TH1D> em;
    map<string, TH1D> mm;

    HistCol(vector<string> regions, const string& name, int nbins, const float* bins, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, bins));
            ee.emplace(region, TH1D((base_name + "_ee").c_str(), (base_title + " ee").c_str(), nbins, bins));
            em.emplace(region, TH1D((base_name + "_em").c_str(), (base_title + " em").c_str(), nbins, bins));
            mm.emplace(region, TH1D((base_name + "_mm").c_str(), (base_title + " mm").c_str(), nbins, bins));

        }
        if (registry != nullptr)
            registry->push_back(this);
    }

    HistCol(vector<string> regions, const string& name, int nbins, float low, float high, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, low, high));
            ee.emplace(region, TH1D((base_name + "_ee").c_str(), (base_title + " ee").c_str(), nbins, low, high));
            em.emplace(region, TH1D((base_name + "_em").c_str(), (base_title + " em").c_str(), nbins, low, high));
            mm.emplace(region, TH1D((base_name + "_mm").c_str(), (base_title + " mm").c_str(), nbins, low, high));
        }
        if (registry != nullptr)
            registry->push_back(this);
    }

    void Fill(const string& region, int id1, int id2, float val, float weight) {
        in[region].Fill(val, weight);

        if (abs(id1) == 11 and abs(id2) == 11) {
            ee[region].Fill(val, weight);
        } else if (abs(id1) == 13 and abs(id2) == 13) {
            mm[region].Fill(val, weight);
        } else if ((abs(id1) == 11 and abs(id2) == 13) or
                (abs(id1) == 13 and abs(id2) == 11)) {
            em[region].Fill(val, weight);
        } else {
            cout << "These ids are garbage: (" << id1 << ", " << id2 << ")\n";
        }
    }

    void Write() {
        for (auto p : in) p.second.Write();
        for (auto p : ee) p.second.Write();
        for (auto p : em) p.second.Write();
        for (auto p : mm) p.second.Write();
    }
};

// Update cut information, histograms
void cut_info (int &cut, int &count, HistCol &hist, string region, int lep1id, int lep2id, float weight) {
    hist.Fill(region, lep1id, lep2id, count, weight);
    cut++;
    count++;
    return;
}


// Debug tools
// #define DEBUG
struct debugger { template<typename T> debugger& operator , (const T& v) { cerr<<v<<" "; return *this; } } dbg;
#ifdef DEBUG
    #define debug(args...) do {cerr << #args << ": "; dbg,args; cerr << endl;} while(0)
#else
    #define debug(args...)
#endif


void write_debug(std::ofstream& inf, int r, int l, int e, int id, float eta, float phi, float pt /*, float ptratio*/) {
    inf << Form("%d, %d, %d, %d, %f, %f, %f", r, l, e, id, eta, phi, pt/*, ptratio*/) << endl;
    return;
}


// Looper begins
int ScanChain(TChain *ch, TString option="", TString outputdir="plots") {
    // Output
    TFile* out_tfile = new TFile("sspairs_data.root", "RECREATE");

    // Debug files
    bool debug = false;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/analysis_debug.dat", outputdir.Data()/*, ch->GetTitle()*/), ios::out);
    }
//  if (debug and dfile.is_open()){
//  }

    // Set configuration parameters
    gconf.year = 2016;

    // Custom TTree
//    SSpairsTree* sspairs_tree = new SSpairsTree(2016);
//    TTree* lt_ttree = sspairs_tree->t;

    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
    tqdm bar;

    // Regions of interest    
    vector<string> regions;
    regions.push_back("ssbr");
    regions.push_back("bkgdcuts");

    // Histograms of interest
    vector<HistCol*> registry;
    vector<HistCol2D*> registry2D;
    HistCol   h_counts         (regions, "count"            , 14, -0.5 ,14.5 ,  &registry);
    HistCol   h_nleps          (regions, "nleps"            ,  5, -0.5 , 4.5 ,  &registry);
    HistCol   h_njets          (regions, "njets"            , 13, -0.5 ,12.5 ,  &registry);
    HistCol   h_nbtags         (regions, "nbtags"           , 10, -0.5 , 9.5 ,  &registry);
    HistCol   h_mjj            (regions, "mjj"              , 50,    0 , 500 ,  &registry);

    // Variables
    float weight = 1;   

    // Tree branches
    int tree_counts = -1;

    TFile *  f1 = new TFile(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");
    f1->cd();

    TTree* out_tree = new TTree("t","t");
    out_tree->Branch("counts", &tree_counts);
 
    int surviveCuts [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    // File looper
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
//            sspairs_tree->resetBranches();
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);
            // Fill event-level info
//            sspairs_tree->event = nt.event();

            // Basic info
            int run = nt.run();
            int lumi = luminosityBlock();

            int counts = 1;
            cut_info(surviveCuts[0], counts, h_counts, "ssbr", 11, 11, weight);

            // Get leptons, coninue if not enought to make a pair
            Leptons leps = getLeptons();
            if (leps.size() <= 1) {continue;}



            // Make sure leptons have the required min pt
            float eptcut = 25;
            float uptcutleading = 25;
            float uptcuttrailing = 20;
            Leptons ptleps;
            for (int i = 0; i < leps.size(); i++) {
                if ((leps[i].is_el()) && (leps[i].pt() > eptcut)) {ptleps.push_back(leps[i]);}
                else if (ptleps.size() == 0) {
                    if ((leps[i].is_mu()) && (leps[i].pt() > uptcutleading)) {ptleps.push_back(leps[i]);}
                } else {
                    if ((leps[i].is_mu()) && (leps[i].pt() > uptcuttrailing)) {ptleps.push_back(leps[i]);}
                }
            }
            if (ptleps.size() < 2) {continue;} 
            cut_info(surviveCuts[1], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);

            // Tight leptons
            Leptons looseleps;
            for (int i = 0; i < ptleps.size(); i++) {
                if (ptleps[i].idlevel() >= SS::IDfakable) {looseleps.push_back(ptleps[i]);}
            }
            if (looseleps.size() < 2) {continue;}
            cut_info(surviveCuts[2], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);

            // Tight leptons
            Leptons tightleps;
            for (int i = 0; i < looseleps.size(); i++) {
                if (looseleps[i].idlevel() == SS::IDtight) {tightleps.push_back(ptleps[i]);}
            }
            int nleps = tightleps.size();
            if (nleps < 2) {continue;}
            cut_info(surviveCuts[3], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Make pairs of leptons
            vector<Hyp> pairs;
            Hyp tmppair;
            for (int i = 0; i < nleps; i++){
                for (int j = 0; j < nleps; j++){
                    tmppair.first = tightleps[i];
                    tmppair.second = tightleps[j];
                    pairs.push_back(tmppair);
                }
            }
            int npairs = pairs.size();


            // Sort pairs by opposite and same sign
            vector<Hyp> sspairs;
            vector<Hyp> ospairs;
            for (int i = 0; i < npairs; i++) {
                if ((pairs[i].first.charge() == pairs[i].second.charge())) {sspairs.push_back(pairs[i]);}
                else {ospairs.push_back(pairs[i]);}
            }
            if (sspairs.size() < 1) {continue;}
            cut_info(surviveCuts[4], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Made extra Z
            // Used code from getBestHyp
            float Zmass = 91.2;
            float Zcut = 15.0;
            bool extraZ = false;
            for (int i = 0; i < leps.size(); i++) {
                for (int j = i + 1; j < leps.size(); j++) {
                    auto zResult = makesResonance(leps, leps[i], leps[j], Zmass, Zcut);
                    extraZ = zResult.first >= 0;
                    if (extraZ) {break;}
                }
            }
            if (extraZ) {continue;}
            cut_info(surviveCuts[5], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Min 2 jets
            int njets;
            int nbtags;
            float ht;
            Jets jets;
            tie(njets, nbtags, ht) = getJetInfo(leps, jets);
            if (njets < 2) {continue;}
            cut_info(surviveCuts[6], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // No btags
            if (nbtags > 0) {continue;}
            cut_info(surviveCuts[7], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Make DiJet pairs
            vector<DiJet> dijets;
            DiJet tmpdijet;
            for (int i = 0; i < (njets - 1); i++){
                for (int j = 0; j < njets; j++){
                    tmpdijet.first = jets[i];
                    tmpdijet.second = jets[j];
                    dijets.push_back(tmpdijet);
                }
            }


            // Invarient DiJet Mass > 120
            vector<DiJet> jets120;
            for (int i = 0; i < dijets.size(); i++) {
                if (fabs((dijets[i].first.p4() + dijets[i].second.p4()).mass())) {jets120.push_back(dijets[i]);}
            }
            if (jets120.size() < 1) {continue;}
            cut_info(surviveCuts[8], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);




            // Create fill function
            int lep1id = ptleps[0].id();
            int lep2id = ptleps[1].id();
            auto fill_region = [&](const string& region, float weight) {
                if (std::find(regions.begin(), regions.end(), region) == regions.end()) return;
                // Fill all observables for a region
                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val, float extraweight=1.) {
                    h.Fill(region, lep1id, lep2id, val, weight*extraweight);
                };
                auto do_fill2D = [region, lep1id, lep2id, weight](HistCol2D& h, float valx, float valy) {
                    h.Fill(region, lep1id, lep2id, valx, valy, weight);
                };

            // My fills
                do_fill(h_nleps, nleps);
                do_fill(h_nbtags, nbtags);
                do_fill(h_njets, njets);

/*                int looseleg = -1;
                if (hyp_class == 2) {
                    looseleg = (lep1good ? 2 : 1);
                }
                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);*/
            };




            fill_region("ssbr", weight);

/*
 * End of Baseline Cuts
 */


            // Invarient DiJet Mass > 500
            vector<DiJet> jets500;
            for (int i = 0; i < jets120.size(); i++) {
                if (fabs((jets120[i].first.p4() + jets120[i].second.p4()).mass())) {jets500.push_back(jets120[i]);}
            }
            if (jets500.size() < 1) {continue;}
            cut_info(surviveCuts[9], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Min 3 jets
            if (njets < 3) {continue;}
            cut_info(surviveCuts[10], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);


            // Min 4 jets
            if (njets < 4) {continue;}
            cut_info(surviveCuts[11], counts, h_counts, "ssbr", ptleps[0].id(), ptleps[1].id(), weight);

            fill_region("bkgdcuts", weight);



            // Fill object-level branches
//            sspairs_tree->fillBranches();
        } // END event loop

        // Clean up
        delete file;

    } // END file loop

    // Print statements for debugging
    cout << endl;
    cout << "Debugs" << endl;
    printf ("\nTotal Events: %i \n", surviveCuts[0]);
    printf ("Pt Minimum: %i \n", surviveCuts[1]);
    printf ("Loose Selection: %i \n", surviveCuts[2]);
    printf ("Tight Selection: %i \n", surviveCuts[3]);
    printf ("SS Pairs: %i \n", surviveCuts[4]);
    printf ("Close to Z: %i \n", surviveCuts[5]);
    printf ("Min 2 Jets: %i \n", surviveCuts[6]);
    printf ("No bTags: %i \n", surviveCuts[7]);
    printf ("Invar Mass > 120: %i \n", surviveCuts[8]);
    printf ("Invar Mass > 500: %i \n", surviveCuts[9]);
    printf ("Min 3 Jets: %i \n", surviveCuts[10]);
    printf ("Min 4 Jets: %i \n", surviveCuts[11]);


    // Wrap up
    bar.finish();
    out_tfile->cd();
//    lt_ttree->Write();
    f1->cd();
    for (HistCol* coll : registry) coll->Write();
    for (HistCol2D* coll : registry2D) coll->Write();
    out_tfile->Close();
    f1->Close();
    if (dfile.is_open()) dfile.close();

    return 0;
}

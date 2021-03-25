#pragma GCC diagnostic ignored "-Wsign-compare"
// Various Root Functions
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

// Funtions used in analysis
#include "/home/users/ian/NanoTools/NanoCORE/Nano.h"
#include "/home/users/ian/NanoTools/NanoCORE/SSSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MetSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/ElectronSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/MuonSelections.cc"
#include "/home/users/ian/NanoTools/NanoCORE/IsolationTools.cc"
//#include "/home/users/ian/NanoTools/studies/sspairsanalysis/sspairs.h"
#include "/home/users/ian/NanoTools/NanoCORE/tqdm.h"

#include "/home/users/ian/VBSAna/analysis/misc/common_utils.h"

// Output
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
        }/* else {
            cout << "These ids are garbage: (" << id1 << ", " << id2 << ")\n";
        }*/
    }

    void Write() {
        for (auto p : in) p.second.Write();
        for (auto p : ee) p.second.Write();
        for (auto p : em) p.second.Write();
        for (auto p : mm) p.second.Write();
    }
};

// Update cut information, histograms
void cut_info (int &cut, float &wcut, int &count, HistCol &hist, string region, int lep1id, int lep2id, float weight) {
    hist.Fill(region, lep1id, lep2id, count, weight);
    cut++;
    wcut+=weight;
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

    bool doFlips = true;//options.Contains("doFlips");
    bool doFakes = true;//options.Contains("doFakes");
    
    // Debug files
    bool debug = false;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/analysis_debug.dat", outputdir.Data()/*, ch->GetTitle()*/), ios::out);
    }
//  if (debug and dfile.is_open()){
//  }

    // Set configuration parameters
//    gconf.year = 2016;

/*    int year;
    float lumiAG = 0.;
    bool is2016(false), is2017(false), is2018(false);
    if (options.Contains("Data2016")) {
        lumiAG = getLumi(2016);
        year = 2016;
        is2016 = true;
    } else 
*/



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

    // No title if using doall.C
    TFile *  f1 = new TFile(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");
    f1->cd();

    TTree* out_tree = new TTree("t","t");
    out_tree->Branch("counts", &tree_counts);
 
    int surviveCuts [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float weightedCut [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    // File looper
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        // Open file
        TFile *file = TFile::Open( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        string samplename(currentFile->GetTitle());
        cout << filename << endl;
        // TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
//        nt.Init(tree, 2016);
//        nt.SetYear(2017);
        nt.Init(tree);
        // Get "total" genWeight
        // Some nano files call it genWeight others Generator_weight
        int normalWeight = 0;
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            int weightMod = (nt.Generator_weight() > 0) ? 1 : -1;
//            int weightMod = (nt.genWeight() > 0) ? 1 : -1;
            normalWeight += weightMod;
        }

        // Gets xsec from file name
        vector<string> procs = {"ttHTobb", "TTWJetsToLNu", "TTZToLLNuNu", "TTJets_DiLept", "TTJets_SingleLeptFromT", "TTJets_SingleLeptFromTbar", "WpWpJJ_EWK", "WZTo3LNu", "VBSWmpWmpHToLNuLNu_TuneCP5"};
        vector<float>  xsecs = {   0.1279,         0.2043,        0.2529,          91.044,                   182.96,                      182.96,       0.0539,     4.4297,                   0.00001708};
        int nprocs = procs.size();

        float xsec;
        size_t found;
        bool procided = false;
        for (int i = 0; i < nprocs; i++) {
            found = samplename.find(procs[i]);
            if (found != string::npos) {cout << procs[i] << endl; xsec = xsecs[i]; cout << xsec << endl; procided = true; break;}
        }
        if (!procided) {cout << "Unknow Process, stopping" << endl; continue;}

        float lumi16 = 35.922;
        float lumi17 = 41.53;
        float lumi18 = 59.71;
        if (!isData()) {
            int wsgn = (nt.Generator_weight() > 0) ? 1 : -1;
//                int wsgn = (nt.genWeight() > 0) ? 1 : -1;
            weight = wsgn * xsec / normalWeight * 1000;
            if (nt.year() == 2016) {weight *= lumi16;}
            else if (nt.year() == 2017) {weight *= lumi17;}
            else if (nt.year() == 2018) {weight *= lumi18;}
            else {cout << "No year, skipping event" << endl; continue;}
        }

        cout << nt.year() << endl;
        
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
            int lumi = nt.luminosityBlock();

            int counts = 1;
//            cut_info(surviveCuts[0], weightedCut[0], scounts, h_counts, "ssbr", 11, 11, weight);

            // Get leptons, coninue if not enought to make a pair
            Leptons leps = getLeptons();
            if (leps.size() <= 1) {continue;}


            /* Hyp Definitions, Priority Order 3, 6, 2, 1, 4
             * 1: SS, loose-loose
             * 2: SS, tight-loose (or loose-tight)
             * 3: SS, tight-tight
             * 4: OS, tight-tight
             * 5: SS, inSituFR
             * 6: SS, tight-tight and fails Z-veto (lies! hyp_class==6 != lep1good and lep2good)
             */
            auto bestHyp = getBestHyp(leps, false);
            int hyp_class = bestHyp.first;
            Lepton hyplep1 = bestHyp.second.first;
            Lepton hyplep2 = bestHyp.second.second;
            int lep1hypid = bestHyp.second.first.id();
            int lep2hypid = bestHyp.second.second.id();

            cut_info(surviveCuts[0], weightedCut[0], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);
            

            // No hyp
            if (hyp_class == -1) {continue;}

            // Leptons in hypothesis make pt cut 
            float eptcut = 25;
            float uptcutleading = 25;
            float uptcuttrailing = 20;
            if (hyplep1.is_el()) {
                if (hyplep1.pt() < eptcut) {continue;}
            } else if (hyplep1.is_mu()) {
                if (hyplep1.pt() < uptcutleading) {continue;}
            }

            if (hyplep2.is_el()) {
                if (hyplep2.pt() < eptcut) {continue;}
            } else if (hyplep2.is_mu()) {
                if (hyplep2.pt() < uptcuttrailing) {continue;}
            }
            


            Leptons ptleps;
            for (int i = 0; i < leps.size(); i++) {
                if ((leps[i].is_el()) && (leps[i].pt() > eptcut)) {ptleps.push_back(leps[i]);}
                else if (ptleps.size() == 0) {
                    if ((leps[i].is_mu()) && (leps[i].pt() > uptcutleading)) {ptleps.push_back(leps[i]);}
                } else {
                    if ((leps[i].is_mu()) && (leps[i].pt() > uptcuttrailing)) {ptleps.push_back(leps[i]);}
                }
            }
//            if (ptleps.size() < 2) {continue;} 
            cut_info(surviveCuts[1], weightedCut[1], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);

            // At least loose-loose, not nessesarily ss
            vector<int> looseKeys = {3, 6, 2, 1, 4};
            if (find(looseKeys.begin(), looseKeys.end(), hyp_class) == looseKeys.end()) {continue;}
            cut_info(surviveCuts[2], weightedCut[2], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // At least tight-tight, not nessesarily ss
            vector<int> tightKeys = {3, 6, 4};
            if (find(tightKeys.begin(), tightKeys.end(), hyp_class) == tightKeys.end()) {continue;}
            cut_info(surviveCuts[3], weightedCut[3], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // Make pairs of leptons
/*            vector<Hyp> pairs;
            Hyp tmppair;
            for (int i = 0; i < nleps; i++){
                for (int j = 0; j < nleps; j++){
                    tmppair.first = tightleps[i];
                    tmppair.second = tightleps[j];
                    pairs.push_back(tmppair);
                }
            }
            int npairs = pairs.size();*/

            // SS
            vector<int> ssKeys = {3, 6};
            if (find(ssKeys.begin(), ssKeys.end(), hyp_class) == ssKeys.end()) {continue;}
            cut_info(surviveCuts[4], weightedCut[4], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);

            // Find 3rd lepton, (highest pt not part of the hyp pair)
//            Lepton lep3;
//            for (int i = 0; i < nleps; i++) {
//                if
//            }
/*
            // Fakes 
            bool class6Fake = false;
            if (doFakes) {
                if (hyp_class == 6) {
                    bool lep1_lowpt_veto = lep1pt < (abs(lep1id) == 11 ? 15 : 10);
                    bool lep2_lowpt_veto = lep2pt < (abs(lep2id) == 11 ? 15 : 10);
                    bool lep3_lowpt_veto = lep3pt < (abs(lep3id) == 11 ? 15 : 10);
                    int nfakes = 0;
                    if (ss::lep3_fo() and !ss::lep3_tight() and !lep3_lowpt_veto and lep1good and lep2good && lep3pt>min_pt_fake) {  // lep3 fake
                        float fr = fakeRate(year, lep3id, lep3ccpt, lep3eta, ht, analysis, new2016FRBins, !minPtFake18);
                        class6Fake = true;
                        nfakes++;
                        weight *= fr / (1-fr);
                    }
                    if (ss::lep2_fo() and !ss::lep2_tight() and !lep2_lowpt_veto and lep1good and lep3good && lep2pt>min_pt_fake) {  // lep2 fake
                        float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ht, analysis, new2016FRBins, !minPtFake18);
                        class6Fake = true;
                        nfakes++;
                        weight *= fr / (1-fr);
                    }
                    if (ss::lep1_fo() and !ss::lep1_tight() and !lep1_lowpt_veto and lep2good and lep3good && lep1pt>min_pt_fake) {  // lep1 fake
                        float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ht, analysis, new2016FRBins, !minPtFake18);
                        class6Fake = true;
                        nfakes++;
                        weight *= fr / (1-fr);
                    }
                    if (!class6Fake) {
                        continue; // No fakes!
                    }
                    if (nfakes == 2) weight *= -1;
                } else if (hyp_class == 1 or hyp_class == 2) {
                    bool foundGoodLoose = false;
                    if (ss::lep1_passes_id()==0 && lep1pt>min_pt_fake) {
                        float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ht, analysis, new2016FRBins, !minPtFake18);
                        weight *= fr/(1.-fr);
                        foundGoodLoose = true;
                    }
                    if (ss::lep2_passes_id()==0 && lep2pt>min_pt_fake) {
                        float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ht, analysis, new2016FRBins, !minPtFake18);
                        weight *= fr/(1.-fr);
                        foundGoodLoose = true;
                    }
                    if (!foundGoodLoose)
                        continue;
                    // subtract double FO (why is this?)
                    if (hyp_class == 1 && lep1pt>min_pt_fake && lep2pt>min_pt_fake) weight *= -1.;
                    // subtract prompt MC
                    if (isData and !ss::is_real_data()) weight *= -1.;
                    hyp_class = 3; // we've faked a SS Tight-Tight with a SS LL or SS TL
                    // Basically just update this so it gets put in the SR
                } else {
                    continue; // Not a fakeing hyp_class
                }
            }
*/

            //Flips
            if (doFlips) {
                // Why the arbitrary class change?
                if (hyp_class == 4) hyp_class = 3; // we've flipped an OS to a SS
                // else if (hyp_class == 6) class6Fake = true;
                else continue;
                float flipFact = 0.;
                if (abs(lep1hypid) == 11) {
                    float flr = flipRate(nt.year(), hyplep1.pt(), hyplep1.eta(), FTANA);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2hypid) == 11) {
                    float flr = flipRate(nt.year(), hyplep2.pt(), hyplep2.eta(), FTANA);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }


            // Made extra Z 
            if (hyp_class == 6) {continue;}
            cut_info(surviveCuts[5], weightedCut[5], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // Min 2 jets
            int njets;
            int nbtags;
            float ht;
            Jets jets;
            tie(njets, nbtags, ht) = getJetInfo(leps, jets);
            if (njets < 2) {continue;}
            cut_info(surviveCuts[6], weightedCut[6], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // No btags
            if (nbtags > 0) {continue;}
            cut_info(surviveCuts[7], weightedCut[7], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


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
            cut_info(surviveCuts[8], weightedCut[8], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);




            // Create fill function
            int lep1id = hyplep1.id();
            int lep2id = hyplep2.id();
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
//                do_fill(h_nleps, nleps);
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
            cut_info(surviveCuts[9], weightedCut[9], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // Min 3 jets
            if (njets < 3) {continue;}
            cut_info(surviveCuts[10], weightedCut[10], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);


            // Min 4 jets
            if (njets < 4) {continue;}
            cut_info(surviveCuts[11], weightedCut[11], counts, h_counts, "ssbr", lep1hypid, lep2hypid, weight);

            fill_region("bkgdcuts", weight);



            // Fill object-level branches
//            sspairs_tree->fillBranches();
        } // END event loop

        // Clean up
        delete file;

    } // END file loop

    // Print statements for debugging
    cout << endl;
    cout << "Debugs: Cut: Events, Weighted" << endl;
    printf ("\nTotal Events: %i, %f \n", surviveCuts[0], weightedCut[0]);
    printf ("Pt Minimum: %i, %f \n", surviveCuts[1], weightedCut[1]);
    printf ("Loose Selection: %i, %f \n", surviveCuts[2], weightedCut[2]);
    printf ("Tight Selection: %i, %f \n", surviveCuts[3], weightedCut[3]);
    printf ("SS Pairs: %i, %f \n", surviveCuts[4], weightedCut[4]);
    printf ("Close to Z: %i, %f \n", surviveCuts[5], weightedCut[5]);
    printf ("Min 2 Jets: %i, %f \n", surviveCuts[6], weightedCut[6]);
    printf ("No bTags: %i, %f \n", surviveCuts[7], weightedCut[7]);
    printf ("Invar Mass > 120: %i, %f \n", surviveCuts[8], weightedCut[8]);
    printf ("Invar Mass > 500: %i, %f \n", surviveCuts[9], weightedCut[9]);
    printf ("Min 3 Jets: %i, %f \n", surviveCuts[10], weightedCut[10]);
    printf ("Min 4 Jets: %i, %f \n", surviveCuts[11], weightedCut[11]);


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

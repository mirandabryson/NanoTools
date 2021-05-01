#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TH2F.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TMVA/Reader.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/tqdm.h"
#include <fstream>
#include <string>

#define MZ 91.2

using namespace std;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> Vec4;
// using namespace tas;
// float lumiAG = getLumi();
bool STOP_REQUESTED = false;
// float lumiAG = 36.3;
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

float calcDeltaPhi(float phi1, float phi2) {
    float dPhi = phi1 - phi2;
    while (dPhi  >  TMath::Pi()) dPhi -= 2*TMath::Pi();
    while (dPhi <= -TMath::Pi()) dPhi += 2*TMath::Pi();
    return fabs(dPhi);
}

float calcDeltaR(float eta1, float phi1, float eta2, float phi2) {
    return TMath::Sqrt(pow(calcDeltaPhi(phi1, phi2),2)+pow((eta1-eta2),2));
}

struct Jet {
    Jet () : v(LorentzVector(0,0,0,0)),islep(false),pt(0),eta(0),phi(0) {}
    Jet (LorentzVector v_) : v(v_),islep(false),pt(v.pt()),eta(v.eta()),phi(v.phi()) {}
    LorentzVector v;
    bool islep;
    float pt;
    float eta;
    float phi;
};

float calcMT(float pt1, float phi1, float pt2, float phi2) {
    return sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
}


float minDR(const Vec4 & lep , const vector<Vec4> & jet) {
    int size = (int)jet.size();
    //cout<<"jet size "<<size<<endl;
    float mindr = 999;
    if(size)for(int i=0;i<size;i++){
        float dr = calcDeltaR(lep.eta(), lep.phi(), jet[i].eta(), jet[i].phi());
        if(dr<mindr) mindr = dr;
    }
    return mindr;
}

float PtMaxEta(const vector<Vec4> & jet) {
    int size = (int)jet.size();
    //cout<<"jet size "<<size<<endl;
    float maxeta=0, eta=0;
    int index=0;
    for(int i=0;i<size;i++){
        eta = jet[i].eta();
        if(eta>maxeta){
            maxeta = eta;
            index = i;
        }
    }
    return jet[index].pt();
}

struct lepton {
    Vec4 v;
    int id;
    bool isgood;
    float miniiso;
    float dxy;
    float dz;
};

struct cutLepton {
//    cutLepton (string name_, LorentzVector v_, int id_, bool isGood_, bool isNeg_, bool passLoose_, bool passFO_, bool passTight_):
//        name(name_), v(v_), id(id_), isGood(isGood_), isNeg(isNeg_), passLoose(passLoose_), passFO(passFO_), passTight(passTight_) {pt = v.pt();}
    string name;
    LorentzVector v;
    int id;
    int mcid;
    int mc_motherid;
    bool isFake;
    bool isGood;
    int sign; //bool isNeg;
    bool passLoose;
    bool passFO;
    bool passTight;
    float pt;
    float eta;
    float phi;

    void print() {std::cout << this->name << ": pt = " << this->pt << ", eta = " << this->eta << ", is_good = " << this->isGood << ", isfo = " << this->passFO << std::endl;}
};

struct lepPair_t {
    Int_t lep1id;
    Int_t lep1mcid;
    Int_t lep1mc_motherid;
    Bool_t lep1isFake;
    Float_t lep1pt;
    Float_t lep1eta;
    Float_t lep1phi;

    Int_t lep2id;
    Int_t lep2mcid;
    Int_t lep2mc_motherid;
    Bool_t lep2isFake;
    Float_t lep2pt;
    Float_t lep2eta;
    Float_t lep2phi;

    string name;
    Int_t run;
    Int_t lumi;
    Int_t event;
};

bool lep_pt_sort(const cutLepton& lhs, const cutLepton& rhs) {
    return (lhs.pt > rhs.pt);
}

struct lepPair {
//    lepPair (cutLepton lepA_, cutLepton lepB_):
//        lepA(lepA_), lepB(lepB_) {sameSign = (lepA.isNeg() == lepB.isNeg());}
    cutLepton lepA;
    cutLepton lepB;
    bool sameSign;
    bool passLoose;
    bool passFO;
    bool passTight;
};

void pair_info(TTree* tree, lepPair_t& branch, lepPair& pair, TString filename) {
    branch.lep1id = pair.lepA.id;
    branch.lep1mcid = pair.lepA.mcid;
    branch.lep1mc_motherid = pair.lepA.mc_motherid;
    branch.lep1isFake = pair.lepA.isFake;
    branch.lep1pt = pair.lepA.pt;
    branch.lep1eta = pair.lepA.eta;
    branch.lep1phi = pair.lepA.phi;

    branch.lep2id = pair.lepB.id;
    branch.lep2mcid = pair.lepB.mcid;
    branch.lep2mc_motherid = pair.lepB.mc_motherid;
    branch.lep2isFake = pair.lepB.isFake;
    branch.lep2pt = pair.lepB.pt;
    branch.lep2eta = pair.lepB.eta;
    branch.lep2phi = pair.lepB.phi;

    branch.name = filename;
    branch.run = ss::run();
    branch.lumi = ss::lumi();
    branch.event = ss::event();

    tree->Fill();

    return;
}

float MOSSF(vector<lepton> lep) {
    int size = (int)lep.size();
    float mossf = 999999;
    float diff = 999999;
    if(size>=2)for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            if(lep[i].id*lep[j].id==-169||lep[i].id*lep[j].id==-121){
                float mass = (lep[i].v+lep[j].v).M();
                if(TMath::Abs(mass-MZ)<diff)
                {
                    mossf = mass;
                    diff = TMath::Abs(mass-MZ);
                }
            }
        }
    }

    return mossf;
}

float isr_reweight(bool useIsrWeight, int year, int nisrmatch, int sample) {
    if (!useIsrWeight) return 1;
    if (ss::is_real_data()) return 1;
    return isrWeight(year, nisrmatch, sample); // 10 is ttbar
}

float nb_reweight(int nbtags) {
    if (ss::is_real_data()) return 1;
    std::vector<float> weights = { 1.06, 0.96, 0.99, 1.29, 1.31 };
    // std::vector<float> weights = { 1.00, 1.00, 1.00, 1.29, 1.00 }; // FIXME only reweighting nb==3
    return weights[min(nbtags,4)];
}

void count_info(int& cut, int& count, HistCol& hist, string region, int lep1id, int lep2id, float weight) {
    cut++;
    count++;
    hist.Fill(region, lep1id, lep2id, count, weight);
    return;
}

void write_debug(std::ofstream& inf, int r, int l, int e, int c) {
    inf << Form("%d, %d, %d, %d", r, l, e, c) << endl;
    return;
}

void write_event(std::ofstream& inf, string data, float val) {
    if (data == "") {
        inf << endl;
    } else {
        inf << data << ": " << val << endl;
    }
    return;
}

int ScanChain(TChain *ch, TString options="", TString outputdir="outputs") {

    signal(SIGINT, [](int) {
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;
    bool doFakes = options.Contains("doFakes");
    bool doFlips = options.Contains("doFlips");
    bool useNonIsoTriggers = options.Contains("useNonIsoTriggers");
    bool quiet = options.Contains("quiet");
    bool minPtFake18 = options.Contains("minPtFake18");
    bool new2016FRBins = options.Contains("new2016FRBins");
    bool noISRWeights = options.Contains("noISRWeights");
    bool isData = options.Contains("doData");
    bool debug = options.Contains("debug");
    debug = false;
    //FG what is this for???
    //ana_t analysis = FTANA;
    //if (doSS) {
    ana_t analysis = SSANA;
    //}

    TString proc(ch->GetTitle());

    //cout<<"proc "<<proc<<endl;
    // bool useIsrWeight = proc.Contains("tt_");
    // FG: Don't think these are needed
    bool useIsrWeight = proc.Contains("tt_")
        or proc.Contains("ttw")
        or proc.Contains("ttz")
        or proc.Contains("tth");

    bool doScaleUnc = proc.Contains("tt_");
    if (doScaleUnc && !quiet)
        cout << "Calculating scale uncertainty for process " << proc << endl;

    // this isn't needed
    bool useTTBB = proc.Contains("tt_")
        or proc.Contains("ttw")
        or proc.Contains("ttz")
        or proc.Contains("tth");

    useTTBB = false;
    if (useTTBB and !quiet)
        cout << "Applying TTBB scale factors for process " << proc << endl;

    //cout<<"using isr "<<useIsrWeight<<" TTBB "<<useTTBB<<endl;
    if (noISRWeights)
        useIsrWeight = false;
    if (useIsrWeight and !quiet)
        cout << "Applying ISR weights to process " << proc << endl;

    // We may have derived the fake rate map throwing away leptons with pT<18 (e.g., 2017), so
    // we need to apply this cut here to be consistent
    //float min_pt_fake = minPtFake18 ? 18. : -1;
    float min_pt_fake = minPtFake18 ? 18. : -1;

    int year;
    float lumiAG = 0.;
    bool is2016(false), is2017(false), is2018(false);
    if (options.Contains("Data2016")) {
        lumiAG = getLumi(2016);
        year = 2016;
        is2016 = true;
    } else if (options.Contains("Data2017")) {
        lumiAG = useNonIsoTriggers ? 36.529: getLumi(2017);
        year = 2017;
        is2017 = true;
    } else if (options.Contains("Data2018")) {
        lumiAG = getLumi(2018);
        year = 2018;
        is2018 = true;
    } else {
        cout << "Need to specify year!\n";
        return -1;
    }
    // Clear already-seen list
    duplicate_removal::clear_list();

    // Used to determine which "era" a MC event is in
    TRandom *tr1 = new TRandom();

    if (!quiet) cout << "Working on " << proc << endl;

    // make debug files
//    std::ofstream dfile;
//    std::cout << "debug: " << debug << ", outputdir = " << outputdir.Data() << ", title = " << ch->GetTitle() << endl;
//    if (debug and not dfile.is_open()) {
//        dfile.open(Form("%s/debug_%s.dat", outputdir.Data(), ch->GetTitle()), ios::out);
//    } 

    vector<string> regions;// =
//    {
//        "ssbr"
//    };

    regions.push_back("ssbr");
    regions.push_back("bkgdcuts");
    regions.push_back("ssbr_deta_peak");
    regions.push_back("ssbr_dphi_peak");
    regions.push_back("ssbr_fake");

    vector<HistCol*> registry;
    vector<HistCol2D*> registry2D;
    HistCol   h_nbtags         (regions, "nbtags"           , 10, -0.5 , 9.5 ,  &registry);
    HistCol   h_type           (regions, "type"             , 8 , -0.5 , 7.5 ,  &registry);

// My hists
    HistCol   h_counts         (regions, "count"            , 14,  0.5 ,14.5 ,  &registry);
    HistCol   h_njets          (regions, "njets"            , 13, -0.5 ,12.5 ,  &registry);
    HistCol   h_mee            (regions, "mee"              , 50,    0 , 500 ,  &registry);
    HistCol   h_mjj            (regions, "mjj"              , 50,    0 , 500 ,  &registry);
    HistCol   h_dEta           (regions, "dEta"             , 50,  -10 ,  10 ,  &registry);
    HistCol   h_dPhi           (regions, "dPhi"             , 50,  -10 ,  10 ,  &registry);
    HistCol   h_dR             (regions, "dR"               , 50,    0 ,  10 ,  &registry);
    HistCol2D h_EtaPhi         (regions, "EtaPhi"           , 20,  -10 ,  10 , 20 , -10 , 10 , &registry2D);
    HistCol   h_pt1            (regions, "pt1"              , 50,    0 , 500 ,  &registry);
    HistCol   h_pt2            (regions, "pt2"              , 50,    0 , 500 ,  &registry);
    HistCol   h_nleps          (regions, "nleps"            ,  5, -0.5 , 4.5 ,  &registry);
    HistCol   h_pairMass       (regions, "pairMass"         , 50,    0 , 500 ,  &registry);
    HistCol   h_pairId         (regions, "pairId"           ,  3, -0.5 , 2.5 ,  &registry);
    HistCol   h_triId          (regions, "triId"            ,  4, -0.5 , 3.5 ,  &registry);
    HistCol   h_triMass        (regions, "triMass"          , 50,    0 , 500 ,  &registry);
    HistCol   h_triSign        (regions, "triSign"          ,  4, -0.5 , 3.5 ,  &registry);
    HistCol   h_met            (regions, "met"              , 50,    0 , 500 ,  &registry);
    HistCol   h_fakeid         (regions, "fakeid"           , 80, -40  , 40  ,  &registry);
    HistCol   h_fakemcid       (regions, "fakemcid"         , 80, -40  , 40  ,  &registry);
    HistCol   h_fakemcmotherid (regions, "fakemcmotherid"   , 80, -40  , 40  ,  &registry);
    HistCol   h_eta            (regions, "eta"              , 50,  -10 ,  10 ,  &registry);
    HistCol   h_phi            (regions, "phi"              , 50,  -10 ,  10 ,  &registry);

    // Declare a bunch of event variables to be filled below in the loop
    int nbtags;
    int type;
    float weight = 1.;

// My variables
    int counts;
    int njets;
    float mee;
    float mjj;
    float dEta;
    float dPhi;
    float dR;
    float pt1;
    float pt2;
    int nleps;
    float pairMass;
    int pairId;
    int triId;
    float triMass;
    int triSign;
    float met;
    int fakeid;
    int fakemcid;
    int fakemcmotherid;

    //add list of trees
    float tree_weight = -1;
    int tree_type = -1;
    int tree_nbtags = -1;

// My trees
    int tree_counts = -1;
    int tree_njets = -1;
    float tree_mee = -1;
    float tree_mjj = -1;
    float tree_dEta = -1;
    float tree_dPhi = -1;
    float tree_dR = -1;
    float tree_pt1 = -1;
    float tree_pt2 = -1;
    int tree_nleps = -1;
    int tree_pairId = -1;
    float tree_pairMass = -1;
    int tree_triId = -1;
    int tree_triSign = -1;
    float tree_triMass = -1;
    float tree_met = -1;
    int tree_fakeid = -1;
    int tree_fakemcid = -1;
    int tree_fakemcmotherid = -1;
    lepPair_t tree_lep;

    TFile *  f1 = new TFile(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");
    f1->cd();
    TTree* out_tree = new TTree("t","fortraining");

//    out_tree->Branch("weight", &tree_weight);
//    out_tree->Branch("nbtags", &tree_nbtags);
//    out_tree->Branch("type", &tree_type);

// My branches
//    out_tree->Branch("counts", &tree_counts);
//    out_tree->Branch("njets", &tree_njets);
//    out_tree->Branch("mee", &tree_mee);
//    out_tree->Branch("mjj", &tree_mjj);
//    out_tree->Branch("dEta", &tree_dEta);
//    out_tree->Branch("dPhi", &tree_dPhi);
//    out_tree->Branch("dR", &tree_dR);
//    out_tree->Branch("pt1", &tree_pt1);
//    out_tree->Branch("pt2", &tree_pt2);
//    out_tree->Branch("nleps", &tree_nleps);
//    out_tree->Branch("pairId", &tree_pairId);
//    out_tree->Branch("pairMass", &tree_pairMass);
//    out_tree->Branch("triId", &tree_triId);
//    out_tree->Branch("triMass", &tree_triMass);
//    out_tree->Branch("triSign", &tree_triSign);
//    out_tree->Branch("met", &tree_met);
//    out_tree->Branch("fakeid", &tree_fakeid);
//    out_tree->Branch("fakemcid", &tree_fakeid);
//    out_tree->Branch("fakemcmotherid", &tree_fakemcmotherid);
    out_tree->Branch("Lepton", &tree_lep);

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    tqdm bar;
    //  bar.set_theme_braille();

    int nFills = 0;
    int nErrors = 0;
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
//    bool event_info = true;
    bool event_info = false;
    int surviveCuts [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int nSSPairs [] = {0,0,0,0,0,0,0};
    int nGivenPair [] = {0,0,0,0,0,0};

    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        if (STOP_REQUESTED) break;
        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename(currentFile->GetTitle());

        auto tokens = filename.Tokenize("/");
        auto basename = ((TObjString*)(tokens->At(tokens->GetEntries()-1)))->String().Data();
        bar.set_label(basename);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            if (STOP_REQUESTED) break;
            samesign.GetEntry(event);
            nEventsTotal++;
            if (!quiet) bar.progress(nEventsTotal, nEventsChain);

            weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            // Simple cuts first to speed things up
            int lep1id = ss::lep1_id();
            int lep2id = ss::lep2_id();

            if (event_info) {
                if (ss::run() != 1) continue;
                if (ss::event() != 520894) continue;
                if (ss::lumi() != 446) continue;
//                if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), 0);
            }

            bool pass_trig =  ss::fired_trigger_ss();
            if (!pass_trig) continue;
            if (!ss::passes_met_filters()) continue;
            //
            // Reject duplicates
            if (ss::is_real_data()) {
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue;
            }

            // Save a bunch of event info for quick reference later
            met = ss::met();

            int lep1good = 0;
            int lep2good = 0;
            int lep3good = 0;
            int lep4good = 0;

            int lep1sign = 0;
            int lep2sign = 0;
            int lep3sign = 0;
            int lep4sign = 0;

            if(ss::lep1_passes_id()) {
                float pt_cutoff = 10;
                if (abs(ss::lep1_id() == 11)) {
                    pt_cutoff = 15;
            		}
                if (ss::lep1_p4().pt() > pt_cutoff) {
                    if (ss::lep1_id() == 11 || ss::lep1_id() == 13) {
                        lep1good = 1; //Good and negative
                        lep1sign = -1;
                    } else if (ss::lep1_id() == -11 || ss::lep1_id() == -13) {
                        lep1good = 2; //Good and positive
                        lep1sign = 1;
                    }
                }
            }
            if(ss::lep2_passes_id()) {
                float pt_cutoff = 10;
                if (abs(ss::lep2_id() == 11)) {
                    pt_cutoff = 15;
            		}
                if (ss::lep2_p4().pt() > pt_cutoff) {
                    if (ss::lep2_id() == 11 || ss::lep2_id() == 13) {
                        lep2good = 1; //Good and negative
                        lep2sign = -1;
                    } else if (ss::lep2_id() == -11 || ss::lep2_id() == -13) {
                        lep2good = 2; //Good and positive
                        lep2sign = 1;
                    }
                }
            }
            if(ss::lep3_passes_id()) {
                float pt_cutoff = 10;
                if (abs(ss::lep3_id() == 11)) {
                    pt_cutoff = 15;
            		}
                if (ss::lep3_p4().pt() > pt_cutoff) {
                    if (ss::lep3_id() == 11 || ss::lep3_id() == 13) {
                        lep3good = 1; //Good and negative
                        lep3sign = -1;
                    } else if (ss::lep3_id() == -11 || ss::lep3_id() == -13) {
                        lep3good = 2; //Good and positive
                        lep3sign = 1;
                    }
                }
            }
            if(ss::lep4_passes_id()) {
                float pt_cutoff = 10;
                    if (abs(ss::lep4_id() == 11)) {
                        pt_cutoff = 15;
            		}
                if (ss::lep4_p4().pt() > pt_cutoff) {
                    if (ss::lep4_id() == 11 || ss::lep4_id() == 13) {
                        lep4good = 1; //Good and negative
                        lep4sign = -1;
                    } else if (ss::lep4_id() == -11 || ss::lep4_id() == -13) {
                        lep4good = 2; //Good and positive
                        lep4sign = 1;
                    }
                }
            }

            /* hyp_class
             * 1: SS, loose-loose
             * 2: SS, tight-loose (or loose-tight)
             * 3: SS, tight-tight
             * 4: OS, tight-tight
             * 5: SS, inSituFR
             * 6: SS, tight-tight and fails Z-veto (lies! hyp_class==6 != lep1good and lep2good)
             */
            int hyp_class = ss::hyp_class();
            type = hyp_class;

//            int lep1good = ss::lep1_passes_id();
//            int lep2good = ss::lep2_passes_id();
//            int lep3good = ss::lep3_passes_id();
//            nleps = (lep3good) ? ((ss::lep4_passes_id() and (ss::lep4_p4().pt() > (abs(ss::lep4_id())==11 ? 15 : 10))) ? 4 : 3) : 2;
            vector<lepton> lep;
            lepton temp;
            temp.v = ss::lep1_p4(); temp.id = ss::lep1_id();
            lep.push_back(temp);
            temp.v = ss::lep2_p4(); temp.id = ss::lep2_id();
            lep.push_back(temp);

            float ht = ss::ht();
            int nisrmatch = ss::nisrMatch();
            float lep1ccpt = ss::lep1_coneCorrPt();
            float lep2ccpt = ss::lep2_coneCorrPt();
            float lep3ccpt = ss::lep3_coneCorrPt();
            float lep1eta = ss::lep1_p4().eta();
            float lep2eta = ss::lep2_p4().eta();
            float lep3eta = ss::lep3_p4().eta();
            float lep1pt = ss::lep1_p4().pt();
            float lep2pt = ss::lep2_p4().pt();
            float lep3pt = ss::lep3_p4().pt();
            int lep3id = ss::lep3_id();



            if (!ss::is_real_data()) {
                weight *= getTruePUw(year, ss::trueNumInt()[0]);
                if (lep1good) weight *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ht, FTANA);
                if (lep2good) weight *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ht, FTANA);
                if (lep3good) weight *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ht, FTANA);

                if (!lep3good) {
                    weight *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ht, analysis, 0);
                }
                weight *= ss::weight_btagsf();
                if (proc.Contains("ttw"))  weight *= isrWeight(year, nisrmatch, 1);
                if (proc.Contains("ttz"))  weight *= isrWeight(year, nisrmatch, 2);
                if (proc.Contains("tt_"))  weight *= isrWeight(year, nisrmatch, 10);

                if (year == 2016) weight *= ss::prefire2016_sf();
                if (year == 2017) weight *= ss::prefire2017_sf();
                if (useTTBB and (ss::extragenb() >= 2)) weight *= 1.7;
                weight *=ss::decayWSF();
            }

            // Create fill function
            auto fill_region = [&](const string& region, float weight) {
                if (std::find(regions.begin(), regions.end(), region) == regions.end()) return;
                // Fill all observables for a region
                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val, float extraweight=1.) {
                    h.Fill(region, lep1id, lep2id, val, weight*extraweight);
                };
                auto do_fill2D = [region, lep1id, lep2id, weight](HistCol2D& h, float valx, float valy) {
                    h.Fill(region, lep1id, lep2id, valx, valy, weight);
                };
                do_fill(h_nbtags, nbtags);

            // My fills
                do_fill(h_njets, njets);
                do_fill(h_pt1, pt1);
                do_fill(h_pt2, pt2);
                do_fill(h_nleps, nleps);
                do_fill(h_pairId, pairId);
                do_fill(h_pairMass, pairMass);
                do_fill(h_triId, triId);
                do_fill(h_triMass, triMass);
                do_fill(h_triSign, triSign);
                do_fill(h_met, met);

                int looseleg = -1;
                if (hyp_class == 2) {
                    looseleg = (lep1good ? 2 : 1);
                }
                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);
            };

            // Populate Leptons

            cutLepton lep1;
            cutLepton lep2;
            cutLepton lep3;
            cutLepton lep4;

            lep1.name = "Lep1";
            lep1.id = ss::lep1_id();
            lep1.mcid = ss::lep1_mc_id();
            lep1.mc_motherid = ss::lep1_mc_motherid();
            lep1.v = ss::lep1_p4();
            lep1.isGood = lep1good;
            lep1.sign = lep1sign;
            lep1.passLoose = ss::lep1_veto();
            lep1.passFO = ss::lep1_fo();
            lep1.passTight = ss::lep1_tight();
            lep1.pt = ss::lep1_p4().pt();
            lep1.eta = ss::lep1_p4().eta();
            lep1.phi = ss::lep1_p4().phi();

            lep2.name = "Lep2";
            lep2.id = ss::lep2_id();
            lep2.mcid = ss::lep2_mc_id();
            lep2.mc_motherid = ss::lep2_mc_motherid();
            lep2.v = ss::lep2_p4();
            lep2.isGood = lep2good;
            lep2.sign = lep2sign;
            lep2.passLoose = ss::lep2_veto();
            lep2.passFO = ss::lep2_fo();
            lep2.passTight = ss::lep2_tight();
            lep2.pt = ss::lep2_p4().pt();
            lep2.eta = ss::lep2_p4().eta();
            lep2.phi = ss::lep2_p4().phi();

            lep3.name = "Lep3";
            lep3.id = ss::lep3_id();
            lep3.mcid = ss::lep3_mcid();
            lep3.mc_motherid = ss::lep3_mc_motherid();
            lep3.v = ss::lep3_p4();
            lep3.isGood = lep3good;
            lep3.sign = lep3sign;
            lep3.passLoose = ss::lep3_veto();
            lep3.passFO = ss::lep3_fo();
            lep3.passTight = ss::lep3_tight();
            lep3.pt = ss::lep3_p4().pt();
            lep3.eta = ss::lep3_p4().eta();
            lep3.phi = ss::lep3_p4().phi();

            lep4.name = "Lep4";
            lep4.id = ss::lep4_id();
            lep4.mcid = ss::lep4_mcid();
            //lep 4 has no mother id in baby
            lep4.v = ss::lep4_p4();
            lep4.isGood = lep4good;
            lep4.sign = lep4sign;
            lep4.passLoose = ss::lep4_veto();
            lep4.passFO = ss::lep4_fo();
            lep4.passTight = ss::lep4_tight();
            lep4.pt = ss::lep4_p4().pt();
            lep4.eta = ss::lep4_p4().eta();
            lep4.phi = ss::lep4_p4().phi();

            cutLepton leptons[] = {lep1, lep2, lep3, lep4};

            vector<cutLepton> goodLeps;
            for (int i = 0; i < 4; i++) { 
                if (leptons[i].passTight && leptons[i].isGood) {
                    goodLeps.push_back(leptons[i]);
                }
            }

            nleps = goodLeps.size();
            sort(goodLeps.begin(), goodLeps.end(), &lep_pt_sort);

// All Events
            counts = 1;
            count_info(surviveCuts[0], counts, h_counts, "ssbr", lep1id, lep2id, weight);

            if (nleps < 2) {
//                if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
                continue;
            }

// Populate Pairs

            lepPair pair12;
            lepPair pair13;
            lepPair pair14;
            lepPair pair23;
            lepPair pair24;
            lepPair pair34;

            pair12.lepA = lep1;
            pair12.lepB = lep2;
            pair12.sameSign = pair12.lepA.sign == pair12.lepB.sign;
            pair12.passLoose = pair12.lepA.passLoose && pair12.lepB.passLoose;
            pair12.passFO = pair12.lepA.passFO && pair12.lepB.passFO;
            pair12.passTight = pair12.lepA.passTight && pair12.lepB.passTight;

            pair13.lepA = lep1;
            pair13.lepB = lep3;
            pair13.sameSign = pair13.lepA.sign == pair13.lepB.sign;
            pair13.passLoose = pair13.lepA.passLoose && pair13.lepB.passLoose;
            pair13.passFO = pair13.lepA.passFO && pair13.lepB.passFO;
            pair13.passTight = pair13.lepA.passTight && pair13.lepB.passTight;

            pair14.lepA = lep1;
            pair14.lepB = lep4;
            pair14.sameSign = pair14.lepA.sign == pair14.lepB.sign;
            pair14.passLoose = pair14.lepA.passLoose && pair14.lepB.passLoose;
            pair14.passFO = pair14.lepA.passFO && pair14.lepB.passFO;
            pair14.passTight = pair14.lepA.passTight && pair14.lepB.passTight;

            pair23.lepA = lep2;
            pair23.lepB = lep3;
            pair23.sameSign = pair23.lepA.sign == pair23.lepB.sign;
            pair23.passLoose = pair23.lepA.passLoose && pair23.lepB.passLoose;
            pair23.passFO = pair23.lepA.passFO && pair23.lepB.passFO;
            pair23.passTight = pair23.lepA.passTight && pair23.lepB.passTight;

            pair24.lepA = lep2;
            pair24.lepB = lep4;
            pair24.sameSign = pair24.lepA.sign == pair24.lepB.sign;
            pair24.passLoose = pair24.lepA.passLoose && pair24.lepB.passLoose;
            pair24.passFO = pair24.lepA.passFO && pair24.lepB.passFO;
            pair24.passTight = pair24.lepA.passTight && pair24.lepB.passTight;

            pair34.lepA = lep3;
            pair34.lepB = lep4;
            pair34.sameSign = pair34.lepA.sign == pair34.lepB.sign;
            pair34.passLoose = pair34.lepA.passLoose && pair34.lepB.passLoose;
            pair34.passFO = pair34.lepA.passFO && pair34.lepB.passFO;
            pair34.passTight = pair34.lepA.passTight && pair34.lepB.passTight;

            lepPair lepPairs[] = {pair12, pair13, pair14, pair23, pair24, pair34};

// pt Minimum

            vector<lepPair> ptPairs;
            for(int i = 0; i < 6; i++) {
                float pairPt[] = {lepPairs[i].lepA.pt, lepPairs[i].lepB.pt};
                std::sort(pairPt, pairPt + 1, std::greater<float>());
                if(pairPt[0] > 25 && pairPt[1] > 20) {
                    ptPairs.push_back(lepPairs[i]);
                }
            }
            if (ptPairs.size() == 0) {
//                if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
                continue;
            }
            count_info(surviveCuts[2], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// Loose Pairs

            vector<lepPair> loosePairs;
            for (int i =  0; i < ptPairs.size(); i++) {
                if (ptPairs[i].passLoose) {
                    loosePairs.push_back(ptPairs[i]);
                }
            }
            if (loosePairs.size() == 0) {
//               if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
               continue;
            }
            count_info(surviveCuts[3], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// FO Pairs

            vector<lepPair> FOPairs;
            for (int i = 0; i < loosePairs.size(); i++) {
                if(loosePairs[i].passFO) {
                    FOPairs.push_back(loosePairs[i]);
                }
            }
            if (FOPairs.size() == 0) {
//               if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
               continue;
            }
            count_info(surviveCuts[4], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// Tight Pairs

            vector<lepPair> tightPairs;
            for (int i = 0; i < FOPairs.size(); i++) {
                if(FOPairs[i].passTight) {
                   tightPairs.push_back(FOPairs[i]);
                }
            }
            if (tightPairs.size() == 0) {
//                if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
                continue;
            }
            count_info(surviveCuts[5], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// Same Sign Pairs

            vector<lepPair> ssPairs;
            vector<lepPair> osPairs;
            for (int i = 0; i < tightPairs.size(); i++) {
                if ((tightPairs[i].sameSign) && (tightPairs[i].lepA.sign != 0)) {
                    ssPairs.push_back(tightPairs[i]);
                    nGivenPair[i]++;
                } else if ((!tightPairs[i].sameSign) && (tightPairs[i].lepA.sign != 0)) {
                    osPairs.push_back(tightPairs[i]);
                }
            }
            nSSPairs[ssPairs.size()]++;
            if (ssPairs.size() == 0) {
//               if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
               continue;
            }
            count_info(surviveCuts[1], counts, h_counts, "ssbr", lep1id, lep2id, weight);

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

            //flips
            if (doFlips) {
                if (hyp_class == 4) hyp_class = 3; // we've flipped an OS to a SS
                // else if (hyp_class == 6) class6Fake = true;
                else continue;
                float flipFact = 0.;
                if (abs(lep1id) == 11) {
                    float flr = flipRate(year, lep1pt, lep1eta, FTANA);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2id) == 11) {
                    float flr = flipRate(year, lep2pt, lep2eta, FTANA);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }

            auto getmll = [](const Vec4& p1, const Vec4& p2, float ccpt1=-1, float ccpt2=-1) {
                /* Calculate dilepton mass with optional rescaling based on cone-corrected lepton pt */
                if (ccpt1 == -1) return (p1 + p2).M();
                else             return (p1*ccpt1/p1.pt() + p2*ccpt2/p2.pt()).M();
            };
            float m12 = getmll(ss::lep1_p4(), ss::lep2_p4(), lep1ccpt, lep2ccpt);
            float m13 = getmll(ss::lep1_p4(), ss::lep3_p4(), lep1ccpt, lep3ccpt);
            float m23 = getmll(ss::lep2_p4(), ss::lep3_p4(), lep2ccpt, lep3ccpt);
            float m3l = (ss::lep1_p4()+ss::lep2_p4()+ss::lep3_p4()).M();

            auto z_cand = [](int id1, int id2, float mll) {
                return abs(id1) == abs(id2) and  // Same flavor
                    id1*id2<0 and             // Opposite sign
                    fabs(mll - MZ) < 15;     // Z-mass window
            };
            bool zcand12 = z_cand(lep1id, lep2id, m12);
            bool zcand13 = z_cand(lep1id, lep3id, m13);
            bool zcand23 = z_cand(lep2id, lep3id, m23);
            float mllos = fabs(m13 - MZ) < fabs(m23 - MZ) ? m13 : m23;

// fills were defined here

// Z Boson

            vector<lepPair> zPairs;
            for (int i = 0; i < osPairs.size(); i++) {
                if (fabs((osPairs[i].lepA.v + osPairs[i].lepB.v).mass() - MZ) < 15)
                    zPairs.push_back(osPairs[i]);
            }
            if (ss::madeExtraZ()) {
//            if (zPairs.size() != 0) {
//                if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
                continue;
            }
            count_info(surviveCuts[6], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// Minimum of 2 Jets

//            njets = ss::njets();
// Get jet info
            njets = 0;
            float Pi = TMath::Pi();
//            bool lowMjj120 = true;
//            bool lowMjj500 = true;
//            bool tooCloseA;
//            bool tooCloseB;
            float deta;
            float dphi;
            float deltaR;

            std::vector<Jet> candJets;
            for (auto j : ss::jets()) {
                if (j.pt() < 40.) continue;
                candJets.push_back(Jet(j));
            }
            std::vector<Jet> goodJets;
            for (int i = 0; i < candJets.size(); i++) {
                bool islep = false;
                for (int j = 0; j < goodLeps.size(); j++){
                    deta = candJets[i].eta - goodLeps[j].eta;
                    dphi = candJets[i].phi - goodLeps[j].phi;
                    if (dphi > Pi) {
                       dphi -= 2 * Pi;
                    } else if (dphi <= -Pi){
                       dphi += 2 * Pi;
                    }
                    deltaR = sqrt(deta*deta + dphi*dphi);
                    if (deltaR < 0.4) {
                        islep = true;
                    }
                }
                if (islep) continue;
                goodJets.push_back(candJets[i]);
            }

            njets = goodJets.size();
            float max_mjj = 0.0;
            float candMjj;
            float jetdEta;
            float jetdPhi;
            float jetdR;
            Jet jetA;
            Jet jetB;
            if (event_info) {
//                write_event(dfile, "nJets", njets);
                for (int i = 0; i < njets; i++) {
//                    write_event(dfile, "Jet " + std::to_string(i + 1) + " pt", goodJets[i].pt);
//                    write_event(dfile, "Jet " + std::to_string(i + 1) + " mass", goodJets[i].v.mass());
                }
            }
            for (int i = 0; i < njets; i++) {
                for (int j = i + 1; j < njets; j++) {
                    candMjj = (goodJets[i].v + goodJets[j].v).mass();
                    if (candMjj > max_mjj) {
                        max_mjj = candMjj;
                        jetA = goodJets[i];
//                        std::cout << i << endl;
                        jetB = goodJets[j];
//                        std::cout << j << endl;
                    }
                }
            }

            if (njets < 2) {
//               if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
               continue;
            }
            count_info(surviveCuts[7], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// No btags

            nbtags = ss::nbtags();
            if (nbtags != 0) {
//               if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
               continue;
            }
            count_info(surviveCuts[8], counts, h_counts, "ssbr", lep1id, lep2id, weight);

// Jet mass > 120 GeV
          if (max_mjj < 120) {
//             if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
             continue;
          }
          count_info(surviveCuts[9], counts, h_counts, "ssbr", lep1id, lep2id, weight);

          if (goodLeps.size() == 2) {
              if ((abs(goodLeps[0].id) == abs(goodLeps[1].id)) && (abs(goodLeps[0].id) == 11)) {
                  pairId = 0;
              } else if ((abs(goodLeps[0].id) == abs(goodLeps[1].id)) && (abs(goodLeps[0].id) == 13)) {
                  pairId = 2;
              } else {
                  pairId = 1;
              }
              pairMass = (goodLeps[0].v + goodLeps[1].v).mass();
          }

          pt1 = goodLeps[0].pt;
          pt2 = goodLeps[1].pt;

          triId = -1;
          triSign = -1;
//                   ---, --+, -++, +++
          if (goodLeps.size() > 2) {
              triId++;
              triSign++;
              for (int i = 0; i < 3; i++) {
                  if (goodLeps[i].sign > 0) {
                      triSign++;
                  }
                  if (abs(goodLeps[i].id) == 13) {
                      triId++;
                  }
                  for (int j = i + 1; j < goodLeps.size(); j++) {
                      if (goodLeps[i].sign == -goodLeps[j].sign) {
                          h_triMass.Fill("ssbr", lep1id, lep2id, (goodLeps[i].v + goodLeps[j].v).mass(), weight);
                      }
                  }
              }
          }

/*
 * End of Baseline cuts
 */

         fill_region("ssbr", weight);
         nFills++;
         bool error = false;
         for (int i = 0; i < goodLeps.size(); i++) {
             if ((goodLeps[i].id == goodLeps[i].mcid) && (abs(goodLeps[i].mc_motherid) == 24)) {
                 goodLeps[i].isFake = false;
             } else if (abs(goodLeps[i].mc_motherid) != 24) {
                 goodLeps[i].isFake = true;
             } else {
                 nErrors++;
                 error = true;
//                 std::cout << "Unexpected Fake" << endl;
//                 goodLeps[i].isFake = true;
             }
         }
 
         if (error) continue;

         for (int i = 0; i < goodLeps.size(); i++) {
             for (int j = 0; j < ssPairs.size(); j++) {
                 if (goodLeps[i].name == ssPairs[j].lepA.name) {
                     ssPairs[j].lepA.isFake = goodLeps[i].isFake;
                 } else if (goodLeps[i].name == ssPairs[j].lepB.name) {
                     ssPairs[j].lepB.isFake = goodLeps[i].isFake;
                 }
             }
         }

         for (int i = 0; i < ssPairs.size(); i++) {
             if (ssPairs[i].lepA.isFake && !ssPairs[i].lepB.isFake) {
                 fakeid = ssPairs[i].lepA.id;
                 fakemcid = ssPairs[i].lepA.mcid;
                 fakemcmotherid = ssPairs[i].lepA.mc_motherid;
                 h_fakeid.Fill("ssbr_fake", lep1id, lep2id, fakeid, weight);
                 h_fakemcid.Fill("ssbr_fake", lep1id, lep2id, fakemcid, weight);
                 h_fakemcmotherid.Fill("ssbr_fake", lep1id, lep2id, fakemcmotherid, weight);
                 fill_region("ssbr_fake", weight);
                 h_pt1.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.pt, weight);
                 h_eta.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.eta, weight);
                 h_phi.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.phi, weight);

                 pair_info(out_tree, tree_lep, ssPairs[i], filename);

                 if ((abs(fakemcid) == 11) && (abs(fakemcmotherid) == 2212)) {
                    cout << filename << ", " << ss::run() << ", " << ss::lumi() << ", " << ss::event() << endl;
                 }

             } else if (!ssPairs[i].lepA.isFake && ssPairs[i].lepB.isFake) {

//                 fake_loader(ssPairs[i]);

                 fakeid = ssPairs[i].lepB.id;
                 fakemcid = ssPairs[i].lepB.mcid;
                 fakemcmotherid = ssPairs[i].lepB.mc_motherid;
                 h_fakeid.Fill("ssbr_fake", lep1id, lep2id, fakeid, weight);
                 h_fakemcid.Fill("ssbr_fake", lep1id, lep2id, fakemcid, weight);
                 h_fakemcmotherid.Fill("ssbr_fake", lep1id, lep2id, fakemcmotherid, weight);
                 fill_region("ssbr_fake", weight);
                 h_pt1.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.pt, weight);
                 h_eta.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.eta, weight);
                 h_phi.Fill("ssbr_fake", lep1id, lep2id, ssPairs[i].lepA.phi, weight);

                 pair_info(out_tree, tree_lep, ssPairs[i], filename);

                 if ((abs(fakemcid) == 11) && (abs(fakemcmotherid) == 2212)) {
                    cout << filename << ", " << ss::run() << ", " << ss::lumi() << ", " << ss::event() << endl;
                 }
             }
         }

         if (max_mjj < 500) {
//            if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
            continue;
         }
         count_info(surviveCuts[10], counts, h_counts, "ssbr", lep1id, lep2id, weight);

         if (njets < 3) {
//            if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
            continue;
         }
         count_info(surviveCuts[11], counts, h_counts, "ssbr", lep1id, lep2id, weight);

         if (njets < 4) {
//            if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), counts + 1);
            continue;
         }
         count_info(surviveCuts[12], counts, h_counts, "ssbr", lep1id, lep2id, weight);

//         if (debug and dfile.is_open()) write_debug(dfile, ss::run(), ss::lumi(), ss::event(), 30);

         fill_region("bkgdcuts", weight);

         }//event loop
        //delete tree;
        delete file;
    }//file loop

//Print statements for debugging
/*
    std::cout << endl;
    std::cout << "Fills: " << nFills << endl;
    std::cout << "Errors: " << nErrors << endl;
    nFills = 0;
    nErrors = 0;
    printf ("\nTotal Events: %i \n", surviveCuts[0]);
    printf ("Pt Minimum: %i \n", surviveCuts[2]);
    printf ("Loose Selection: %i \n", surviveCuts[3]);
    printf ("FO Selection: %i \n", surviveCuts[4]);
    printf ("Tight Selection: %i \n", surviveCuts[5]);
    printf ("SS Pairs: %i \n", surviveCuts[1]);
    printf ("Close to Z: %i \n", surviveCuts[6]);
    printf ("Min 2 Jets: %i \n", surviveCuts[7]);
    printf ("No bTags: %i \n", surviveCuts[8]);
    printf ("Invar Mass > 120: %i \n", surviveCuts[9]);
    printf ("Invar Mass > 500: %i \n", surviveCuts[10]);
    printf ("Min 3 Jets: %i \n", surviveCuts[11]);
    printf ("Min 4 Jets: %i \n", surviveCuts[12]);
    printf ("\n0 Pairs: %i \n", nSSPairs[0]);
    printf ("\n1 Pairs: %i \n", nSSPairs[1]);
    printf ("2 Pairs: %i \n", nSSPairs[2]);
    printf ("3 Pairs: %i \n", nSSPairs[3]);
    printf ("4 Pairs: %i \n", nSSPairs[4]);
    printf ("5 Pairs: %i \n", nSSPairs[5]);
    printf ("6 Pairs: %i \n", nSSPairs[6]);
    printf ("Total Pairs: %i \n", nSSPairs[1] + nSSPairs[2] + nSSPairs[3] + nSSPairs[4] + nSSPairs[5] + nSSPairs[6]);
    printf ("\n1.2 Pairs: %i \n", nGivenPair[0]);
    printf ("1.3 Pairs: %i \n", nGivenPair[1]);
    printf ("1.4 Pairs: %i \n", nGivenPair[2]);
    printf ("2.3 Pairs: %i \n", nGivenPair[3]);
    printf ("2.4 Pairs: %i \n", nGivenPair[4]);
    printf ("3.4 Pairs: %i \n", nGivenPair[5]);
    printf ("Total Pairs: %i \n", nGivenPair[0] + nGivenPair[1] + nGivenPair[2] + nGivenPair[3] + nGivenPair[4] + nGivenPair[5]);
*/
    //Reset cut counter
/*    for(int i = 0; i < 11; i++) {
        surviveCuts[i] = 0;
    }
    for(int i = 0; i < 6; i++) {
        nSSPairs[i] = 0;
    }
*/
    f1->cd();
    for (HistCol* coll : registry) coll->Write();
    for (HistCol2D* coll : registry2D) coll->Write();
    out_tree->Write();
    f1->Close();
//    if (dfile.is_open()) dfile.close();
    if (!quiet) cout << "\n Done!" << endl;
    return 0;
}

#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"

#include "../NanoCORE/Nano.h"
#include "../NanoCORE/MetSelections.h"
#include "../NanoCORE/Tools/JetCorrector.h"
#include "../NanoCORE/tqdm.h"
#include "../NanoCORE/Config.h"

#include <iostream>
#include <iomanip>

// #define DEBUG

struct debugger { template<typename T> debugger& operator , (const T& v) { cerr<<v<<" "; return *this; } } dbg;
#ifdef DEBUG
    #define debug(args...) do {cerr << #args << ": "; dbg,args; cerr << endl;} while(0)
#else
    #define debug(args...)
#endif

using namespace std;
using namespace tas;

int ScanChain(TChain *ch, int nevents_to_process=-1)
{

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
    tqdm bar;

    TFile* eff_output = new TFile("eff.root", "recreate");

    Double_t ptbins[18] = {20., 25., 30., 35., 40., 45., 50., 60., 70., 80., 90., 100., 120., 150., 200., 300., 400., 600.};
    Double_t etabins[8] = {0., 0.4, 0.8, 1.2, 1.6, 2., 2.4, 2.8};

    TString algo = "DeepJet";

    TH2D* h2_BTaggingEff_Denom_b = new TH2D("h2_BTaggingEff_Denom_b", TString::Format("b-tagging effiency, Denominator, MC, %s, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_Denom_c = new TH2D("h2_BTaggingEff_Denom_c", TString::Format("b-tagging effiency, Denominator, MC, %s loose, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_Denom_udsg = new TH2D("h2_BTaggingEff_Denom_udsg", TString::Format("b-tagging effiency, Denominator, MC, %s loose, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Num_b = new TH2D("h2_BTaggingEff_tight_Num_b", TString::Format("b-tagging effiency, Numerator, MC, %s tight, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Num_c = new TH2D("h2_BTaggingEff_tight_Num_c", TString::Format("b-tagging effiency, Numerator, MC, %s tight, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Num_udsg = new TH2D("h2_BTaggingEff_tight_Num_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s tight, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Num_b = new TH2D("h2_BTaggingEff_med_Num_b", TString::Format("b-tagging effiency, Numerator, MC, %s med, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Num_c = new TH2D("h2_BTaggingEff_med_Num_c", TString::Format("b-tagging effiency, Numerator, MC, %s med, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Num_udsg = new TH2D("h2_BTaggingEff_med_Num_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s med, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Num_b = new TH2D("h2_BTaggingEff_loose_Num_b", TString::Format("b-tagging effiency, Numerator, MC, %s loose, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Num_c = new TH2D("h2_BTaggingEff_loose_Num_c", TString::Format("b-tagging effiency, Numerator, MC, %s loose, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Num_udsg = new TH2D("h2_BTaggingEff_loose_Num_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s loose, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Eff_c = new TH2D("h2_BTaggingEff_med_Eff_c", TString::Format("b-tagging effiency, Numerator, MC, %s med, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Eff_b = new TH2D("h2_BTaggingEff_med_Eff_b", TString::Format("b-tagging effiency, Numerator, MC, %s med, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_med_Eff_udsg = new TH2D("h2_BTaggingEff_med_Eff_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s med, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Eff_c = new TH2D("h2_BTaggingEff_tight_Eff_c", TString::Format("b-tagging effiency, Numerator, MC, %s tight, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Eff_b = new TH2D("h2_BTaggingEff_tight_Eff_b", TString::Format("b-tagging effiency, Numerator, MC, %s tight, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_tight_Eff_udsg = new TH2D("h2_BTaggingEff_tight_Eff_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s tight, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Eff_c = new TH2D("h2_BTaggingEff_loose_Eff_c", TString::Format("b-tagging effiency, Numerator, MC, %s loose, c-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Eff_b = new TH2D("h2_BTaggingEff_loose_Eff_b", TString::Format("b-tagging effiency, Numerator, MC, %s loose, b-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);
    TH2D* h2_BTaggingEff_loose_Eff_udsg = new TH2D("h2_BTaggingEff_loose_Eff_udsg", TString::Format("b-tagging effiency, Numerator, MC, %s loose, udsg-jets, #eta vs p_{T}", algo.Data()).Data(), 17, ptbins, 7, etabins);

    h2_BTaggingEff_Denom_b->Sumw2();
    h2_BTaggingEff_Denom_c->Sumw2();
    h2_BTaggingEff_Denom_udsg->Sumw2();
    h2_BTaggingEff_tight_Num_b->Sumw2();
    h2_BTaggingEff_tight_Num_c->Sumw2();
    h2_BTaggingEff_tight_Num_udsg->Sumw2();
    h2_BTaggingEff_med_Num_b->Sumw2();
    h2_BTaggingEff_med_Num_c->Sumw2();
    h2_BTaggingEff_med_Num_udsg->Sumw2();
    h2_BTaggingEff_loose_Num_b->Sumw2();
    h2_BTaggingEff_loose_Num_c->Sumw2();
    h2_BTaggingEff_loose_Num_udsg->Sumw2();
    h2_BTaggingEff_med_Eff_c->Sumw2();
    h2_BTaggingEff_med_Eff_b->Sumw2();
    h2_BTaggingEff_med_Eff_udsg->Sumw2();
    h2_BTaggingEff_tight_Eff_c->Sumw2();
    h2_BTaggingEff_tight_Eff_b->Sumw2();
    h2_BTaggingEff_tight_Eff_udsg->Sumw2();
    h2_BTaggingEff_loose_Eff_c->Sumw2();
    h2_BTaggingEff_loose_Eff_b->Sumw2();
    h2_BTaggingEff_loose_Eff_udsg->Sumw2();

    while ( (currentFile = (TFile*)fileIter.Next()) )
    {
        TFile *file = TFile::Open( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());

        tree->SetCacheSize(32*1024*1024);
        tree->SetCacheLearnEntries(100);

        nt.Init(tree);

        // Set up the NanoCORE's common configuration service tool
        gconf.GetConfigs(nt.year());

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event)
        {

            nt.GetEntry(event);
            tree->LoadTree(event);

            nEventsTotal++;
            bar.progress(nEventsTotal, nEventsChain);

            int njets = Jet_pt().size();
            debug(njets);
            for (int i = 0; i < njets; i++)
            {
                float pt = Jet_pt()[i];
                float abseta = fabs(Jet_eta()[i]);
                float flavor = Jet_hadronFlavour()[i];
                float btagscore = Jet_btagDeepFlavB()[i];
                bool is_tight = btagscore > gconf.WP_DeepFlav_tight;
                bool is_medium = btagscore > gconf.WP_DeepFlav_medium;
                bool is_loose = btagscore > gconf.WP_DeepFlav_loose;

                if (flavor == 0)
                {
                    h2_BTaggingEff_Denom_udsg->Fill(pt, abseta);
                    if (is_tight)
                    {
                        h2_BTaggingEff_tight_Num_udsg->Fill(pt, abseta);
                    }
                    if (is_medium)
                    {
                        h2_BTaggingEff_med_Num_udsg->Fill(pt, abseta);
                    }
                    if (is_loose)
                    {
                        h2_BTaggingEff_loose_Num_udsg->Fill(pt, abseta);
                    }
                }
                else if (flavor == 4)
                {
                    h2_BTaggingEff_Denom_c->Fill(pt, abseta);
                    if (is_tight)
                    {
                        h2_BTaggingEff_tight_Num_c->Fill(pt, abseta);
                    }
                    if (is_medium)
                    {
                        h2_BTaggingEff_med_Num_c->Fill(pt, abseta);
                    }
                    if (is_loose)
                    {
                        h2_BTaggingEff_loose_Num_c->Fill(pt, abseta);
                    }
                }
                else if (flavor == 5)
                {
                    h2_BTaggingEff_Denom_b->Fill(pt, abseta);
                    if (is_tight)
                    {
                        h2_BTaggingEff_tight_Num_b->Fill(pt, abseta);
                    }
                    if (is_medium)
                    {
                        h2_BTaggingEff_med_Num_b->Fill(pt, abseta);
                    }
                    if (is_loose)
                    {
                        h2_BTaggingEff_loose_Num_b->Fill(pt, abseta);
                    }
                }
                else
                {
                    std::cout << "ERROR:: Should never be here!  The Jet_hadronFlavour is alwyas 0, 4, or 5." << std::endl;
                    abort();
                }

            }

            if (nevents_to_process > 0 and nEventsTotal > nevents_to_process) break;
        }
        if (nevents_to_process > 0 and nEventsTotal > nevents_to_process) break;

        delete file;

    }

    h2_BTaggingEff_tight_Eff_c->Divide(h2_BTaggingEff_tight_Num_c, h2_BTaggingEff_Denom_c, 1, 1, "B");
    h2_BTaggingEff_tight_Eff_b->Divide(h2_BTaggingEff_tight_Num_b, h2_BTaggingEff_Denom_b, 1, 1, "B");
    h2_BTaggingEff_tight_Eff_udsg->Divide(h2_BTaggingEff_tight_Num_udsg, h2_BTaggingEff_Denom_udsg, 1, 1, "B");
    h2_BTaggingEff_med_Eff_c->Divide(h2_BTaggingEff_med_Num_c, h2_BTaggingEff_Denom_c, 1, 1, "B");
    h2_BTaggingEff_med_Eff_b->Divide(h2_BTaggingEff_med_Num_b, h2_BTaggingEff_Denom_b, 1, 1, "B");
    h2_BTaggingEff_med_Eff_udsg->Divide(h2_BTaggingEff_med_Num_udsg, h2_BTaggingEff_Denom_udsg, 1, 1, "B");
    h2_BTaggingEff_loose_Eff_c->Divide(h2_BTaggingEff_loose_Num_c, h2_BTaggingEff_Denom_c, 1, 1, "B");
    h2_BTaggingEff_loose_Eff_b->Divide(h2_BTaggingEff_loose_Num_b, h2_BTaggingEff_Denom_b, 1, 1, "B");
    h2_BTaggingEff_loose_Eff_udsg->Divide(h2_BTaggingEff_loose_Num_udsg, h2_BTaggingEff_Denom_udsg, 1, 1, "B");

    eff_output->cd();

    h2_BTaggingEff_Denom_b->Write();
    h2_BTaggingEff_Denom_c->Write();
    h2_BTaggingEff_Denom_udsg->Write();
    h2_BTaggingEff_tight_Num_b->Write();
    h2_BTaggingEff_tight_Num_c->Write();
    h2_BTaggingEff_tight_Num_udsg->Write();
    h2_BTaggingEff_med_Num_b->Write();
    h2_BTaggingEff_med_Num_c->Write();
    h2_BTaggingEff_med_Num_udsg->Write();
    h2_BTaggingEff_loose_Num_b->Write();
    h2_BTaggingEff_loose_Num_c->Write();
    h2_BTaggingEff_loose_Num_udsg->Write();
    h2_BTaggingEff_med_Eff_c->Write();
    h2_BTaggingEff_med_Eff_b->Write();
    h2_BTaggingEff_med_Eff_udsg->Write();
    h2_BTaggingEff_tight_Eff_c->Write();
    h2_BTaggingEff_tight_Eff_b->Write();
    h2_BTaggingEff_tight_Eff_udsg->Write();
    h2_BTaggingEff_loose_Eff_c->Write();
    h2_BTaggingEff_loose_Eff_b->Write();
    h2_BTaggingEff_loose_Eff_udsg->Write();

    eff_output->Close();

    bar.finish();

    return 0;
}

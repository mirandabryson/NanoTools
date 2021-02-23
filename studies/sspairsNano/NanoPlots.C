#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "iostream"

void NanoPlots() {
    TFile f("NANOdata.root");
    TTree *t = (TTree*)f.Get("t");

    TCanvas *c1 = new TCanvas();
/*
    TH1D *lep1 = new TH1D();
    t->GetHistogram();
    lep1->Draw();
    c1->Print("lep1_multiiso.png");
*/

    // MultiIso
    t->Draw("lep1_multiiso");
//    TH1D *lep1 = c1->GetHIstorgram()
    c1->Print("nanoplots/lep1_multiiso.png");
    t->Draw("lep2_multiiso");
    c1->Print("nanoplots/lep2_multiiso.png");
    t->Draw("lep3_multiiso");
    c1->Print("nanoplots/lep3_multiiso.png");
    t->Draw("lep4_multiiso");
    c1->Print("nanoplots/lep4_multiiso.png");

    // MiniIso
    t->Draw("lep1_miniiso");
    c1->Print("nanoplots/lep1_miniiso.png");
    t->Draw("lep2_miniiso");
    c1->Print("nanoplots/lep2_miniiso.png");
    t->Draw("lep3_miniiso");
    c1->Print("nanoplots/lep3_miniiso.png");
    t->Draw("lep4_miniiso");
    c1->Print("nanoplots/lep4_miniiso.png");

    // PtRel
    t->Draw("lep1_ptrel");
    c1->Print("nanoplots/lep1_ptrel.png");
    t->Draw("lep2_ptrel");
    c1->Print("nanoplots/lep2_ptrel.png");
    t->Draw("lep3_ptrel");
    c1->Print("nanoplots/lep3_ptrel.png");
    t->Draw("lep4_ptrel");
    c1->Print("nanoplots/lep4_ptrel.png");

    // PtRatio
    t->Draw("lep1_ptratio");
    c1->Print("nanoplots/lep1_ptratio.png");
    t->Draw("lep2_ptratio");
    c1->Print("nanoplots/lep2_ptratio.png");
    t->Draw("lep3_ptratio");
    c1->Print("nanoplots/lep3_ptratio.png");
    t->Draw("lep4_ptratio");
    c1->Print("nanoplots/lep4_ptratio.png");

    // PtRatio
    t->Draw("lep1_pt");
    c1->Print("nanoplots/lep1_pt.png");
    t->Draw("lep2_pt");
    c1->Print("nanoplots/lep2_pt.png");
    t->Draw("lep3_pt");
    c1->Print("nanoplots/lep3_pt.png");
    t->Draw("lep4_pt");
    c1->Print("nanoplots/lep4_pt.png");
}

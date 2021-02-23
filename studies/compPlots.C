#include <vector>
#include "TString.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TFile.h"
#include <iostream>
using namespace std;

TString outdir = "/home/users/ian/public_html/share/test/";

void plotter(TTree *nano, TTree *cms4, TString lep, TString quant, int nbins, float lowbin, float highbin, bool log) {

    TString title = "lep" + lep + "_" + quant;
    if (log) {title += "_log";}

    TH1F* nanohist = new TH1F("nanohist", title, nbins, lowbin, highbin);
    nano->Draw("lep" + lep + "_" + quant + " >> nanohist", "((abs(lep" + lep + "_id) == 11) || (abs(lep" + lep + "_id) == 13))", "E");
    nanohist->Scale(1./nanohist->Integral());

    TH1F* cms4hist = new TH1F("cms4hist", title, nbins, lowbin, highbin);
    cms4->Draw("lep" + lep + "_" + quant + " >> cms4hist", "((abs(lep" + lep + "_id) == 11) || (abs(lep" + lep + "_id) == 13))", "E");
    cms4hist->Scale(1./cms4hist->Integral());

    TCanvas* c1 = new TCanvas("c1", "c1");
    c1->cd();
    c1->SetRightMargin(0.09);
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();
    pad1->cd();

    nanohist->Draw("HIST");
    nanohist->SetStats(0);

    if(log) {gPad->SetLogy();}

    cms4hist->Draw("HIST SAME");
    cms4hist->SetLineColor(kRed);

    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanohist, "Nano", "f");
    legend->AddEntry(cms4hist, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();

    c1->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd();

    TH1F *ratio = (TH1F*)nanohist->Clone("ratio");
    ratio->SetMinimum(0.5);
    ratio->SetMaximum(1.5);
    ratio->Divide(cms4hist);
    ratio->Draw();

    c1->SaveAs(outdir + title + ".png");

    delete nanohist;
    delete cms4hist;
    delete ratio;
    delete c1;
}   

void plotterManualRatio(TTree *nano, TTree *cms4, TString lep, int nbins, float lowbin, float highbin, bool log) {

    TString title = "lep" + lep + "_manual_ptratio";
    if (log) {title += "_log";}

    TH1F* nanohist = new TH1F("nanohist", title, nbins, lowbin, highbin);
    nano->Draw("lep" + lep + "_pt/jet" + lep + "_pt >> nanohist", "((abs(lep" + lep + "_id) == 11) || (abs(lep" + lep + "_id) == 13)) && ((jet" + lep + "_pt != -999) || (jet" + lep + "_pt != 0))", "E");
    nanohist->Scale(1./nanohist->Integral());

    TH1F* cms4hist = new TH1F("cms4hist", title, nbins, lowbin, highbin);
    cms4->Draw("lep" + lep + "_pt/jet" + lep + "_pt >> cms4hist", "((abs(lep" + lep + "_id) == 11) || (abs(lep" + lep + "_id) == 13)) && ((jet" + lep + "_pt != -999) || (jet" + lep + "_pt != 0))", "E");
    cms4hist->Scale(1./cms4hist->Integral());

    TCanvas* c1 = new TCanvas("c1", "c1");
    c1->cd();
    c1->SetRightMargin(0.09);
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();
    pad1->cd();

    nanohist->Draw("HIST");
    nanohist->SetStats(0);

    if(log) {gPad->SetLogy();}

    cms4hist->Draw("HIST SAME");
    cms4hist->SetLineColor(kRed);

    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanohist, "Nano", "f");
    legend->AddEntry(cms4hist, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();

    c1->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd();

    TH1F *ratio = (TH1F*)nanohist->Clone("ratio");
    ratio->SetMinimum(0.5);
    ratio->SetMaximum(1.5);
    ratio->Divide(cms4hist);
    ratio->Draw();

    c1->SaveAs(outdir + title + ".png");

    delete nanohist;
    delete cms4hist;
    delete ratio;
    delete c1;
}

void plotterLep(TTree *nano, TTree *cms4, TString lep, TString quant, int nbins, float lowbin, float highbin, bool log, TString lepId) {

    TString title = "lep" + lep;

    if (lepId == "11") {title += "_e_" + quant;}
    else if (lepId == "13") {title += "_mu_" + quant;}

    if (log) {title += "_log";}

    TH1F* nanohist = new TH1F("nanohist", title, nbins, lowbin, highbin);
    nano->Draw("lep" + lep + "_" + quant + " >> nanohist", "(abs(lep" + lep + "_id) == " + lepId + ") && ((jet" + lep + "_pt != -999) || (jet" + lep + "_pt != 0))", "E");
    nanohist->Scale(1./nanohist->Integral());

    TH1F* cms4hist = new TH1F("cms4hist", title, nbins, lowbin, highbin);
    cms4->Draw("lep" + lep + "_" + quant + " >> cms4hist", "(abs(lep" + lep + "_id) == " + lepId + ") && ((jet" + lep + "_pt != -999) || (jet" + lep + "_pt != 0))", "E");
    cms4hist->Scale(1./cms4hist->Integral());


    TCanvas* c1 = new TCanvas("c1", "c1");
    c1->cd();
    c1->SetRightMargin(0.09);
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();
    pad1->cd();

    nanohist->Draw("HIST");
    nanohist->SetStats(0);

    if(log) {gPad->SetLogy();}

    cms4hist->Draw("HIST SAME");
    cms4hist->SetLineColor(kRed);

    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanohist, "Nano", "f");
    legend->AddEntry(cms4hist, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();

    c1->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd();

    TH1F *ratio = (TH1F*)nanohist->Clone("ratio");
    ratio->SetMinimum(0.5);
    ratio->SetMaximum(1.5);
    ratio->Divide(cms4hist);
    ratio->Draw();

    c1->SaveAs(outdir + title + ".png");

    delete nanohist;
    delete cms4hist;
    delete ratio;
    delete c1;
}

void plotterJet(TTree *nano, TTree *cms4, TString jet, TString quant, int nbins, float lowbin, float highbin, bool log) {

    TString title = "jet" + jet + "_" + quant;
    if (log) {title += "_log";}

    TH1F* nanohist = new TH1F("nanohist", title, nbins, lowbin, highbin);
    nano->Draw("jet" + jet + "_" + quant + " >> nanohist", "(jet" + jet + "_pt != -999) || (jet" + jet + "_pt != 0)", "E");
    nanohist->Scale(1./nanohist->Integral());

    TH1F* cms4hist = new TH1F("cms4hist", title, nbins, lowbin, highbin);
    cms4->Draw("jet" + jet + "_" + quant + " >> cms4hist", "(jet" + jet + "_pt != -999) || (jet" + jet + "_pt != 0)", "E");
    cms4hist->Scale(1./cms4hist->Integral());

    TCanvas* c1 = new TCanvas("c1", "c1");
    c1->cd();
    c1->SetRightMargin(0.09);
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();
    pad1->cd();

    nanohist->Draw("HIST");
    nanohist->SetStats(0);

    if(log) {gPad->SetLogy();}

    cms4hist->Draw("HIST SAME");
    cms4hist->SetLineColor(kRed);

    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanohist, "Nano", "f");
    legend->AddEntry(cms4hist, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();

    c1->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd();

    TH1F *ratio = (TH1F*)nanohist->Clone("ratio");
    ratio->SetMinimum(0.5);
    ratio->SetMaximum(1.5);
    ratio->Divide(cms4hist);
    ratio->Draw();

    c1->SaveAs(outdir + title + ".png");

    delete nanohist;
    delete cms4hist;
    delete ratio;
    delete c1;
}

void plotterJets(TTree *nano, TTree *cms4) {

    TString titlept = "jet_pts";
    TString titleeta = "jet_etas";
    TString titlephi = "jet_phis";
    TString titlebtag = "jet_btags";

    std::vector<float> *jets_pt =  0;
    std::vector<float> *jets_eta =  0;
    std::vector<float> *jets_phi =  0;
    std::vector<float> *jets_btag =  0;

    TH1F* cms4pt = new TH1F("cms4pt", titlept, 100, 0, 100);
    TH1F* cms4eta = new TH1F("cms4eta", titleeta, 100, -4, 4);
    TH1F* cms4phi = new TH1F("cms4phi", titlephi, 100, -4, 4);
    TH1F* cms4btag = new TH1F("cms4btag", titlebtag, 100, 0, 1);
    TBranch *cms4br = 0;
    cms4->SetBranchAddress("jets_pt", &jets_pt, &cms4br);
    cms4->SetBranchAddress("jets_eta", &jets_eta, &cms4br);
    cms4->SetBranchAddress("jets_phi", &jets_phi, &cms4br);
    cms4->SetBranchAddress("jets_btag", &jets_btag, &cms4br);
    for (unsigned int i = 0; i < cms4->GetEntries(); i++) {
        cms4->GetEntry(i);
        for (unsigned int j = 0; j < jets_pt->size(); j++) {
            cms4pt->Fill(jets_pt->at(j));
            cms4eta->Fill(jets_eta->at(j));
            cms4phi->Fill(jets_phi->at(j));
            cms4btag->Fill(jets_btag->at(j));
        }
    }
    cms4pt->Draw();
    cms4pt->Scale(1./cms4pt->Integral());
    cms4eta->Draw();
    cms4eta->Scale(1./cms4eta->Integral());
    cms4phi->Draw();
    cms4phi->Scale(1./cms4phi->Integral());
    cms4btag->Draw();
    cms4btag->Scale(1./cms4btag->Integral());

    TH1F* nanopt = new TH1F("nanopt", titlept, 100, 0, 100);
    TH1F* nanoeta = new TH1F("nanoeta", titleeta, 100, -4, 4);
    TH1F* nanophi = new TH1F("nanophi", titlephi, 100, -4, 4);
    TH1F* nanobtag = new TH1F("nanobtag", titlebtag, 100, 0, 1);
    TBranch *nanobr = 0;
    nano->SetBranchAddress("jets_pt", &jets_pt, &nanobr);
    nano->SetBranchAddress("jets_eta", &jets_eta, &nanobr);
    nano->SetBranchAddress("jets_phi", &jets_phi, &nanobr);
    nano->SetBranchAddress("jets_btag", &jets_btag, &nanobr);
    for (unsigned int i = 0; i < nano->GetEntries(); i++) {
        nano->GetEntry(i);
        for (unsigned int j = 0; j < jets_pt->size(); j++) {
            nanopt->Fill(jets_pt->at(j));
            nanoeta->Fill(jets_eta->at(j));
            nanophi->Fill(jets_phi->at(j));
            nanobtag->Fill(jets_btag->at(j));
        }
    }
    nanopt->Draw();
    nanopt->Scale(1./nanopt->Integral());
    nanoeta->Draw();
    nanoeta->Scale(1./nanoeta->Integral());
    nanophi->Draw();
    nanophi->Scale(1./nanophi->Integral());
    nanobtag->Draw();
    nanobtag->Scale(1./nanobtag->Integral());

    TCanvas* cpt = new TCanvas("cpt", "cpt");
    cpt->cd();
    cpt->SetRightMargin(0.09);
    cpt->SetLeftMargin(0.15);
    cpt->SetBottomMargin(0.15);

    TPad *pad1pt = new TPad("pad1pt", "pad1pt", 0, 0.3, 1, 1.0);
    pad1pt->Draw();
    pad1pt->cd();

    nanopt->Draw("HIST");
    nanopt->SetStats(1);
    cms4pt->Draw("HIST SAMES");
    cms4pt->SetLineColor(kRed);
/*
    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanopt, "Nano", "f");
    legend->AddEntry(cms4pt, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();
*/
    cpt->cd();

    TPad *pad2pt = new TPad("pad2pt", "pad2pt", 0, 0.05, 1, 0.3);
    pad2pt->Draw();
    pad2pt->cd();

    TH1F *ratiopt = (TH1F*)nanopt->Clone("ratiopt");
    ratiopt->SetMinimum(0.5);
    ratiopt->SetMaximum(1.5);
    ratiopt->Divide(cms4pt);
    ratiopt->Draw();


    TCanvas* ceta = new TCanvas("ceta", "ceta");
    ceta->cd();
    ceta->SetRightMargin(0.09);
    ceta->SetLeftMargin(0.15);
    ceta->SetBottomMargin(0.15);

    TPad *pad1eta = new TPad("pad1eta", "pad1eta", 0, 0.3, 1, 1.0);
    pad1eta->Draw();
    pad1eta->cd();

    nanoeta->Draw("HIST");
    nanoeta->SetStats(1);
    cms4eta->Draw("HIST SAMES");
    cms4eta->SetLineColor(kRed);
/*
    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanoeta, "Nano", "f");
    legend->AddEntry(cms4eta, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();
*/
    ceta->cd();

    TPad *pad2eta = new TPad("pad2eta", "pad2eta", 0, 0.05, 1, 0.3);
    pad2eta->Draw();
    pad2eta->cd();

    TH1F *ratioeta = (TH1F*)nanoeta->Clone("ratioeta");
    ratioeta->SetMinimum(0.5);
    ratioeta->SetMaximum(1.5);
    ratioeta->Divide(cms4eta);
    ratioeta->Draw();


    TCanvas* cphi = new TCanvas("cphi", "cphi");
    cphi->cd();
    cphi->SetRightMargin(0.09);
    cphi->SetLeftMargin(0.15);
    cphi->SetBottomMargin(0.15);

    TPad *pad1phi = new TPad("pad1phi", "pad1phi", 0, 0.3, 1, 1.0);
    pad1phi->Draw();
    pad1phi->cd();

    nanophi->Draw("HIST");
    nanophi->SetStats(1);
    cms4phi->Draw("HIST SAMES");
    cms4phi->SetLineColor(kRed);
/*
    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanophi, "Nano", "f");
    legend->AddEntry(cms4phi, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();
*/
    cphi->cd();

    TPad *pad2phi = new TPad("pad2phi", "pad2phi", 0, 0.05, 1, 0.3);
    pad2phi->Draw();
    pad2phi->cd();

    TH1F *ratiophi = (TH1F*)nanophi->Clone("ratiophi");
    ratiophi->SetMinimum(0.5);
    ratiophi->SetMaximum(1.5);
    ratiophi->Divide(cms4phi);
    ratiophi->Draw();

    TCanvas* cbtag = new TCanvas("cbtag", "cbtag");
    cbtag->cd();
    cbtag->SetRightMargin(0.09);
    cbtag->SetLeftMargin(0.15);
    cbtag->SetBottomMargin(0.15);

    TPad *pad1btag = new TPad("pad1btag", "pad1btag", 0, 0.3, 1, 1.0);
    pad1btag->Draw();
    pad1btag->cd();

    nanobtag->Draw("HIST");
    nanobtag->SetStats(1);
    cms4btag->Draw("HIST SAMES");
    cms4btag->SetLineColor(kRed);
/*
    auto legend = new TLegend(0.9,0.9,0.68,0.8);
    legend->SetHeader("TTBar 2016","C");
    legend->AddEntry(nanobtag, "Nano", "f");
    legend->AddEntry(cms4btag, "CMS4", "f");
    legend->SetTextSize(0.025);
    legend->Draw();
*/
    cbtag->cd();

    TPad *pad2btag = new TPad("pad2btag", "pad2btag", 0, 0.05, 1, 0.3);
    pad2btag->Draw();
    pad2btag->cd();

    TH1F *ratiobtag = (TH1F*)nanobtag->Clone("ratiobtag");
    ratiobtag->SetMinimum(0.5);
    ratiobtag->SetMaximum(1.5);
    ratiobtag->Divide(cms4btag);
    ratiobtag->Draw();

    cpt->SaveAs(outdir + titlept + ".png");
    ceta->SaveAs(outdir + titleeta + ".png");
    cphi->SaveAs(outdir + titlephi + ".png");
    cbtag->SaveAs(outdir + titlebtag + ".png");

    titlept += "_log";
    titleeta += "_log";
    titlephi += "_log";
    titlebtag += "_log";

    cpt->cd();
    pad1pt->cd();
    gPad->SetLogy();
    nanopt->SetTitle(titlept);
    cms4pt->SetTitle(titlept);

    ceta->cd();
    pad1eta->cd();
    gPad->SetLogy();
    nanoeta->SetTitle(titleeta);
    cms4eta->SetTitle(titleeta);

    cphi->cd();
    pad1phi->cd();
    gPad->SetLogy();
    nanophi->SetTitle(titlephi);
    cms4phi->SetTitle(titlephi);

    cbtag->cd();
    pad1btag->cd();
    gPad->SetLogy();
    nanobtag->SetTitle(titlebtag);
    cms4btag->SetTitle(titlebtag);

    cpt->SaveAs(outdir + titlept + ".png");
    ceta->SaveAs(outdir + titleeta + ".png");
    cphi->SaveAs(outdir + titlephi + ".png");
    cbtag->SaveAs(outdir + titlebtag + ".png");

    delete nanopt;
    delete nanoeta;
    delete nanophi;
    delete nanobtag;
    delete cms4pt;
    delete cms4eta;
    delete cms4phi;
    delete cms4btag;
    delete ratiopt;
    delete ratioeta;
    delete ratiophi;
    delete ratiobtag;
    delete cpt;
    delete ceta;
    delete cphi;
    delete cbtag;
}

void plotterTest(TTree *nano, TTree *cms4, TString quant) {

    float nbins, lowbin, highbin;

    vector<TString> quants = {"pt", "ptratio"/*, "ptrel"*/};

    for (auto quant : quants) {
        if (quant == "pt") {
            nbins = 100;
            lowbin = 0;
            highbin = 200;
        } else if (quant == "ptratio") {
            nbins = 100;
            lowbin = 0;
            highbin = 1.2;
        } else if (quant == "ptrel") {
            nbins = 100;
            lowbin = 0;
            highbin = 100;
        }

        for (int lepnum = 4; lepnum < 5; lepnum++) {
            TString lep = to_string(lepnum);
            TString titlegood = "test_lep" + lep + "_" + quant + "_good";
            TString titlebad = "test_lep" + lep + "_" + quant + "_bad";
            TString title2d = "test_lep" + lep + "_ptratio_bad_matched_2d";

            TH2F* h2d = new TH2F("2D", title2d + "; CMS4; NANO", nbins, lowbin, highbin, nbins, lowbin, highbin);

            TH1F* nanohistgood = new TH1F("nanohistgood", titlegood, nbins, lowbin, highbin);
            TH1F* nanohistbad = new TH1F("nanohistbad", titlebad, nbins, lowbin, highbin);
            TString titlem = titlebad + "_matched";
            TString titleu = titlebad + "_unmatched";
            TH1F* nanohistm = new TH1F("nanohistm", titlem, nbins, lowbin, highbin);
            TH1F* nanohistu = new TH1F("nanohistu", titleu, nbins, lowbin, highbin);
            float nano_ratio = 0;
            float nano_quant = 0;
            int nano_lepid = 0;
            float nano_jetpt = 0;
            nano->SetBranchAddress("lep" + lep + "_ptratio", &nano_ratio);
            if (quant != "ptratio") {nano->SetBranchAddress("lep" + lep + "_" + quant, &nano_quant);}
            nano->SetBranchAddress("lep" + lep + "_" + "id", &nano_lepid);
            nano->SetBranchAddress("jet" + lep + "_" + "pt", &nano_jetpt);

            TH1F* cms4histgood = new TH1F("cms4histgood", titlegood, nbins, lowbin, highbin);
            TH1F* cms4histbad = new TH1F("cms4histbad", titlebad, nbins, lowbin, highbin);
            TH1F* cms4histm = new TH1F("cms4histm", titlem, nbins, lowbin, highbin);
            TH1F* cms4histu = new TH1F("cms4histu", titleu, nbins, lowbin, highbin);
            float cms4_ratio = 0;
            float cms4_quant = 0;
            int cms4_lepid = 0;
            float cms4_jetpt = 0;
            cms4->SetBranchAddress("lep" + lep + "_ptratio", &cms4_ratio);
            if (quant != "ptratio") {cms4->SetBranchAddress("lep" + lep + "_" + quant, &cms4_quant);}
            cms4->SetBranchAddress("lep" + lep + "_" + "id", &cms4_lepid);
            cms4->SetBranchAddress("jet" + lep + "_" + "pt", &cms4_jetpt);
            
            for (unsigned int i = 0; i < nano->GetEntries(); i++) {
                cms4->GetEntry(i);
                nano->GetEntry(i);

                if (nano_lepid != cms4_lepid) {continue;}

                int absid = abs(nano_lepid);
                if (!((absid == 11) || (absid == 13))) {continue;}

                if ((fabs(nano_ratio - cms4_ratio)/ nano_ratio) <= .05) {
                    if (quant != "ptratio") {
                        nanohistgood->Fill(nano_quant);
                        cms4histgood->Fill(cms4_quant);
                    } else {
                        nanohistgood->Fill(nano_ratio);
                        cms4histgood->Fill(cms4_ratio);
                    }
                } else {
                    if (quant != "ptratio") {
                        nanohistbad->Fill(nano_quant);
                        cms4histbad->Fill(cms4_quant);

                        if (nano_jetpt == -999) {
                            nanohistu->Fill(nano_quant);
                        } else {
                            nanohistm->Fill(nano_quant);
                        }

                        if (cms4_jetpt == -999) {
                            cms4histu->Fill(cms4_quant);
                        } else {
                            cms4histm->Fill(cms4_quant);
                        }

                    } else {
                        nanohistbad->Fill(nano_ratio);
                        cms4histbad->Fill(cms4_ratio);

                        if (nano_jetpt == -999) {
                            nanohistu->Fill(nano_ratio);
                        } else {
                            nanohistm->Fill(nano_ratio);
                        }

                        if (cms4_jetpt == -999) {
                            cms4histu->Fill(cms4_ratio);
                        } else {
                            cms4histm->Fill(cms4_ratio);
                        }

                        if ((cms4_jetpt != -999) && (nano_jetpt != -999)){
                            h2d->Fill(cms4_ratio, nano_ratio);
                        }
                    }                
                }
            }

            nanohistgood->Draw();
            nanohistgood->Scale(1./nanohistgood->Integral(0,nbins+1));
            cms4histgood->Draw();
            cms4histgood->Scale(1./cms4histgood->Integral(0,nbins+1));

            nanohistbad->Draw();
            nanohistbad->Scale(1./nanohistbad->Integral(0,nbins+1));
            cms4histbad->Draw();
            cms4histbad->Scale(1./cms4histbad->Integral(0,nbins+1));

            nanohistu->Draw();
            nanohistu->Scale(1./nanohistu->Integral(0,nbins+1));
            cms4histu->Draw();
            cms4histu->Scale(1./cms4histu->Integral(0,nbins+1));

            nanohistm->Draw();
            nanohistm->Scale(1./nanohistm->Integral(0,nbins+1));
            cms4histm->Draw();
            cms4histm->Scale(1./cms4histm->Integral(0,nbins+1));

            TCanvas* c2d = new TCanvas("c2d", "c2d");
            c2d->cd();
            c2d->SetRightMargin(0.09);
            c2d->SetLeftMargin(0.15);
            c2d->SetBottomMargin(0.15);
            c2d->cd();
            h2d->Draw("COLZ");

            c2d->SaveAs(outdir + title2d + ".png");

            TCanvas* cgood = new TCanvas("cgood", "cgood");
            cgood->cd();
            cgood->SetRightMargin(0.09);
            cgood->SetLeftMargin(0.15);
            cgood->SetBottomMargin(0.15);
            cgood->cd();

            TPad *pad1good = new TPad("pad1good", "pad1good", 0, 0.3, 1, 1.0);
            pad1good->Draw();
            pad1good->cd();

//            gStyle->SetOptStat("noume");
            nanohistgood->Draw("HIST");
            nanohistgood->SetStats(1);
            cms4histgood->Draw("HIST SAMES");
            cms4histgood->SetLineColor(kRed);

            auto legendgood = new TLegend(0.9,0.9,0.68,0.8);
            legendgood->SetHeader("TTBar 2016","C");
            legendgood->AddEntry(nanohistgood, "Nano", "f");
            legendgood->AddEntry(cms4histgood, "CMS4", "f");
            legendgood->SetTextSize(0.025);
//            legendgood->Draw();

            cgood->cd();

            TPad *pad2good = new TPad("pad2good", "pad2good", 0, 0.05, 1, 0.3);
            pad2good->Draw();
            pad2good->cd();

            TH1F *ratiogood = (TH1F*)nanohistgood->Clone("ratiogood");
            ratiogood->SetMinimum(0.5);
            ratiogood->SetMaximum(1.5);
            ratiogood->Divide(cms4histgood);
            ratiogood->Draw();

            TCanvas* cbad = new TCanvas("cbad", "cbad");
            cbad->cd();
            cbad->SetRightMargin(0.09);
            cbad->SetLeftMargin(0.15);
            cbad->SetBottomMargin(0.15);

            TPad *pad1bad = new TPad("pad1bad", "pad1bad", 0, 0.3, 1, 1.0);
            pad1bad->Draw();
            pad1bad->cd();

            nanohistbad->Draw("HIST");
            nanohistbad->SetStats(1);
            cms4histbad->Draw("HIST SAMES");
            cms4histbad->SetLineColor(kRed);

            auto legendbad = new TLegend(0.9,0.9,0.68,0.8);
            legendbad->SetHeader("TTBar 2016","C");
            legendbad->AddEntry(nanohistbad, "Nano", "f");
            legendbad->AddEntry(cms4histbad, "CMS4", "f");
            legendbad->SetTextSize(0.025);
//            legendbad->Draw();

            cbad->cd();

            TPad *pad2bad = new TPad("pad2bad", "pad2bad", 0, 0.05, 1, 0.3);
            pad2bad->Draw();
            pad2bad->cd();

            TH1F *ratiobad = (TH1F*)nanohistbad->Clone("ratiobad");
            ratiobad->SetMinimum(0.5);
            ratiobad->SetMaximum(1.5);
            ratiobad->Divide(cms4histbad);
            ratiobad->Draw();


            TCanvas* cu = new TCanvas("cu", "cu");
            cu->cd();
            cu->SetRightMargin(0.09);
            cu->SetLeftMargin(0.15);
            cu->SetBottomMargin(0.15);

            TPad *pad1u = new TPad("pad1u", "pad1u", 0, 0.3, 1, 1.0);
            pad1u->Draw();
            pad1u->cd();

            nanohistu->Draw("HIST");
            nanohistu->SetStats(1);
            cms4histu->Draw("HIST SAMES");
            cms4histu->SetLineColor(kRed);

            auto legendu = new TLegend(0.9,0.9,0.68,0.8);
            legendu->SetHeader("TTBar 2016","C");
            legendu->AddEntry(nanohistu, "Nano", "f");
            legendu->AddEntry(cms4histu, "CMS4", "f");
            legendu->SetTextSize(0.025);
//            legendu->Draw();

            cu->cd();

            TPad *pad2u = new TPad("pad2u", "pad2u", 0, 0.05, 1, 0.3);
            pad2u->Draw();
            pad2u->cd();

            TH1F *ratiou = (TH1F*)nanohistu->Clone("ratiou");
            ratiou->SetMinimum(0.5);
            ratiou->SetMaximum(1.5);
            ratiou->Divide(cms4histu);
            ratiou->Draw();


            TCanvas* cm = new TCanvas("cm", "cm");
            cm->cd();
            cm->SetRightMargin(0.09);
            cm->SetLeftMargin(0.15);
            cm->SetBottomMargin(0.15);

            TPad *pad1m = new TPad("pad1m", "pad1m", 0, 0.3, 1, 1.0);
            pad1m->Draw();
            pad1m->cd();

            nanohistm->Draw("HIST");
            nanohistm->SetStats(1);
            cms4histm->Draw("HIST SAMES");
            cms4histm->SetLineColor(kRed);

            auto legendm = new TLegend(0.9,0.9,0.68,0.8);
            legendm->SetHeader("TTBar 2016","C");
            legendm->AddEntry(nanohistm, "Nano", "f");
            legendm->AddEntry(cms4histm, "CMS4", "f");
            legendm->SetTextSize(0.025);
//            legendm->Draw();

            cm->cd();

            TPad *pad2m = new TPad("pad2m", "pad2m", 0, 0.05, 1, 0.3);
            pad2m->Draw();
            pad2m->cd();

            TH1F *ratiom = (TH1F*)nanohistm->Clone("ratiom");
            ratiom->SetMinimum(0.5);
            ratiom->SetMaximum(1.5);
            ratiom->Divide(cms4histm);
            ratiom->Draw();


            cgood->SaveAs(outdir + titlegood + ".png");
            cbad->SaveAs(outdir + titlebad + ".png");
            cu->SaveAs(outdir + titleu + ".png");
            cm->SaveAs(outdir + titlem + ".png");

            titlegood += "_log";
            titlebad += "_log";
            titleu += "_log";
            titlem += "_log";
            cgood->cd();
            pad1good->cd();
            gPad->SetLogy();
            nanohistgood->SetTitle(titlegood);
            cms4histgood->SetTitle(titlegood);

            cbad->cd();
            pad1bad->cd();
            gPad->SetLogy();
            nanohistbad->SetTitle(titlebad);
            cms4histbad->SetTitle(titlebad);

            cu->cd();
            pad1u->cd();
            gPad->SetLogy();
            nanohistu->SetTitle(titleu);
            cms4histu->SetTitle(titleu);

            cm->cd();
            pad1m->cd();
            gPad->SetLogy();
            nanohistm->SetTitle(titlem);
            cms4histm->SetTitle(titlem);

            cgood->SaveAs(outdir + titlegood + ".png");
            cbad->SaveAs(outdir + titlebad + ".png");
            cu->SaveAs(outdir + titleu + ".png");
            cm->SaveAs(outdir + titlem + ".png");

            delete h2d;
            delete nanohistgood;
            delete nanohistbad;
            delete nanohistu;
            delete nanohistm;
            delete cms4histgood;
            delete cms4histbad;
            delete cms4histm;
            delete cms4histu;
            delete ratiogood;
            delete ratiobad;
            delete ratiou;
            delete ratiom;
            delete cgood;
            delete cbad;
            delete cm;
            delete cu;
            delete c2d;

        }
    }
}

int compPlots() {
    TFile NANO("/home/users/ian/NanoTools/studies/sspairsNano/NANOdata.root");
    TTree *nanot = (TTree*)NANO.Get("t");

    TFile CMS4("/home/users/ian/NanoTools/studies/sspairsCMS4/CMS4data.root");
    TTree *cms4t = (TTree*)CMS4.Get("t");

//    plotterTest(nanot, cms4t, "pt");

//------------------------------------------------------------------------------//
/*
    plotterGood(nanot, cms4t, "1", "ptrel", 100, 0, 100, true);
    plotterGood(nanot, cms4t, "2", "ptrel", 100, 0, 100, true);
    plotterGood(nanot, cms4t, "3", "ptrel", 100, 0, 100, true);
    plotterGood(nanot, cms4t, "4", "ptrel", 100, 0, 100, true);

    plotterGood(nanot, cms4t, "1", "ptrel", 100, 0, 100, false);
    plotterGood(nanot, cms4t, "2", "ptrel", 100, 0, 100, false);
    plotterGood(nanot, cms4t, "3", "ptrel", 100, 0, 100, false);
    plotterGood(nanot, cms4t, "4", "ptrel", 100, 0, 100, false);

//------------------------------------------------------------------------------//

    plotterBad(nanot, cms4t, "1", "ptrel", 100, 0, 100, true);
    plotterBad(nanot, cms4t, "2", "ptrel", 100, 0, 100, true);
    plotterBad(nanot, cms4t, "3", "ptrel", 100, 0, 100, true);
    plotterBad(nanot, cms4t, "4", "ptrel", 100, 0, 100, true);

    plotterBad(nanot, cms4t, "1", "ptrel", 100, 0, 100, false);
    plotterBad(nanot, cms4t, "2", "ptrel", 100, 0, 100, false);
    plotterBad(nanot, cms4t, "3", "ptrel", 100, 0, 100, false);
    plotterBad(nanot, cms4t, "4", "ptrel", 100, 0, 100, false);

//------------------------------------------------------------------------------//

    plotterGood(nanot, cms4t, "1", "pt", 100, 0, 200, true);
    plotterGood(nanot, cms4t, "2", "pt", 100, 0, 200, true);
    plotterGood(nanot, cms4t, "3", "pt", 100, 0, 200, true);
    plotterGood(nanot, cms4t, "4", "pt", 100, 0, 200, true);

    plotterGood(nanot, cms4t, "1", "pt", 100, 0, 200, false);
    plotterGood(nanot, cms4t, "2", "pt", 100, 0, 200, false);
    plotterGood(nanot, cms4t, "3", "pt", 100, 0, 200, false);
    plotterGood(nanot, cms4t, "4", "pt", 100, 0, 200, false);

//------------------------------------------------------------------------------//

    plotterBad(nanot, cms4t, "1", "pt", 100, 0, 200, true);
    plotterBad(nanot, cms4t, "2", "pt", 100, 0, 200, true);
    plotterBad(nanot, cms4t, "3", "pt", 100, 0, 200, true);
    plotterBad(nanot, cms4t, "4", "pt", 100, 0, 200, true);

    plotterBad(nanot, cms4t, "1", "pt", 100, 0, 200, false);
    plotterBad(nanot, cms4t, "2", "pt", 100, 0, 200, false);
    plotterBad(nanot, cms4t, "3", "pt", 100, 0, 200, false);
    plotterBad(nanot, cms4t, "4", "pt", 100, 0, 200, false);

//------------------------------------------------------------------------------//

    plotter(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, true);
    plotter(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, true);
    plotter(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, true);
    plotter(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, true);

    plotter(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, false);
    plotter(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, false);
    plotter(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, false);
    plotter(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, false);

//------------------------------------------------------------------------------//

    plotter(nanot, cms4t, "1", "ptrel", 100, 0, 100, true);
    plotter(nanot, cms4t, "2", "ptrel", 100, 0, 100, true);
    plotter(nanot, cms4t, "3", "ptrel", 100, 0, 100, true);
    plotter(nanot, cms4t, "4", "ptrel", 100, 0, 100, true);

    plotter(nanot, cms4t, "1", "ptrel", 100, 0, 100, false);
    plotter(nanot, cms4t, "2", "ptrel", 100, 0, 100, false);
    plotter(nanot, cms4t, "3", "ptrel", 100, 0, 100, false);
    plotter(nanot, cms4t, "4", "ptrel", 100, 0, 100, false);

//------------------------------------------------------------------------------//

    plotter(nanot, cms4t, "1", "pt", 100, 0, 200, true);
    plotter(nanot, cms4t, "2", "pt", 100, 0, 200, true);
    plotter(nanot, cms4t, "3", "pt", 100, 0, 200, true);
    plotter(nanot, cms4t, "4", "pt", 100, 0, 200, true);

    plotter(nanot, cms4t, "1", "pt", 100, 0, 200, false);
    plotter(nanot, cms4t, "2", "pt", 100, 0, 200, false);
    plotter(nanot, cms4t, "3", "pt", 100, 0, 200, false);
    plotter(nanot, cms4t, "4", "pt", 100, 0, 200, false);

//------------------------------------------------------------------------------//

    plotterManualRatio(nanot, cms4t, "1", 100, 0, 1.2, true);
    plotterManualRatio(nanot, cms4t, "2", 100, 0, 1.2, true);
    plotterManualRatio(nanot, cms4t, "3", 100, 0, 1.2, true);
    plotterManualRatio(nanot, cms4t, "4", 100, 0, 1.2, true);

    plotterManualRatio(nanot, cms4t, "1", 100, 0, 1.2, false);
    plotterManualRatio(nanot, cms4t, "2", 100, 0, 1.2, false);
    plotterManualRatio(nanot, cms4t, "3", 100, 0, 1.2, false);
    plotterManualRatio(nanot, cms4t, "4", 100, 0, 1.2, false);

//------------------------------------------------------------------------------//

    plotterJet(nanot, cms4t, "1", "pt", 100, 0, 200, true);
    plotterJet(nanot, cms4t, "2", "pt", 100, 0, 200, true);
    plotterJet(nanot, cms4t, "3", "pt", 100, 0, 200, true);
    plotterJet(nanot, cms4t, "4", "pt", 100, 0, 200, true);

    plotterJet(nanot, cms4t, "1", "pt", 100, 0, 200, false);
    plotterJet(nanot, cms4t, "2", "pt", 100, 0, 200, false);
    plotterJet(nanot, cms4t, "3", "pt", 100, 0, 200, false);
    plotterJet(nanot, cms4t, "4", "pt", 100, 0, 200, false);

//------------------------------------------------------------------------------//

    plotterLep(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, true, "11");
    plotterLep(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, true, "11");
    plotterLep(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, true, "11");
    plotterLep(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, true, "11");

    plotterLep(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, false, "11");
    plotterLep(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, false, "11");
    plotterLep(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, false, "11");
    plotterLep(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, false, "11");

//------------------------------------------------------------------------------//

    plotterLep(nanot, cms4t, "1", "pt", 100, 0, 200, true, "11");
    plotterLep(nanot, cms4t, "2", "pt", 100, 0, 200, true, "11");
    plotterLep(nanot, cms4t, "3", "pt", 100, 0, 200, true, "11");
    plotterLep(nanot, cms4t, "4", "pt", 100, 0, 200, true, "11");

    plotterLep(nanot, cms4t, "1", "pt", 100, 0, 200, false, "11");
    plotterLep(nanot, cms4t, "2", "pt", 100, 0, 200, false, "11");
    plotterLep(nanot, cms4t, "3", "pt", 100, 0, 200, false, "11");
    plotterLep(nanot, cms4t, "4", "pt", 100, 0, 200, false, "11");

//------------------------------------------------------------------------------//

    plotterLep(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, true, "13");
    plotterLep(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, true, "13");
    plotterLep(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, true, "13");
    plotterLep(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, true, "13");

    plotterLep(nanot, cms4t, "1", "ptratio", 120, 0, 1.2, false, "13");
    plotterLep(nanot, cms4t, "2", "ptratio", 120, 0, 1.2, false, "13");
    plotterLep(nanot, cms4t, "3", "ptratio", 120, 0, 1.2, false, "13");
    plotterLep(nanot, cms4t, "4", "ptratio", 120, 0, 1.2, false, "13");

//------------------------------------------------------------------------------//

    plotterLep(nanot, cms4t, "1", "pt", 100, 0, 200, true, "13");
    plotterLep(nanot, cms4t, "2", "pt", 100, 0, 200, true, "13");
    plotterLep(nanot, cms4t, "3", "pt", 100, 0, 200, true, "13");
    plotterLep(nanot, cms4t, "4", "pt", 100, 0, 200, true, "13");

    plotterLep(nanot, cms4t, "1", "pt", 100, 0, 200, false, "13");
    plotterLep(nanot, cms4t, "2", "pt", 100, 0, 200, false, "13");
    plotterLep(nanot, cms4t, "3", "pt", 100, 0, 200, false, "13");
    plotterLep(nanot, cms4t, "4", "pt", 100, 0, 200, false, "13");

//------------------------------------------------------------------------------//
*/
    plotterJets(nanot, cms4t);

    return 0;
}

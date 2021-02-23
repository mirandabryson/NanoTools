#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

void write_debug(std::ofstream& inf, int e, int id, float neta, float nphi, float npt, float ceta, float cphi, float cpt) {
    inf << Form("%d, %d, %f, %f, %f, %f, %f, %f", e, id, neta, nphi, npt, ceta, cphi, cpt) << endl;
    return;
}

TString outdir = "/home/users/ian/public_html/share/debug/";

int comptree() {
    // Debug files
    bool debug = true;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/lep_comp.dat", outdir.Data()), ios::out);
    }

    TFile NANO("/home/users/ian/NanoTools/studies/sspairsNano/NANOdata.root");
    TTree *nanot = (TTree*)NANO.Get("t");

    TFile CMS4("/home/users/ian/NanoTools/studies/sspairsCMS4/CMS4data.root");
    TTree *cms4t = (TTree*)CMS4.Get("t");

    float nanoentries = nanot->GetEntries();
    float cms4entries = cms4t->GetEntries();

    if (nanoentries != cms4entries) {cout << "Unequal Trees, abort" << endl;}

    int i = 0;

    while (i < nanoentries) {

        float etadiff = 0.005;
        float phidiff = 0.005;

        int nevent;
        int cevent;

        float nanoeta1;
        float nanoeta2;
        float nanoeta3;
        float nanoeta4;

        float cms4eta1;
        float cms4eta2;
        float cms4eta3;
        float cms4eta4;

        float nanophi1;
        float nanophi2;
        float nanophi3;
        float nanophi4;

        float cms4phi1;
        float cms4phi2;
        float cms4phi3;
        float cms4phi4;

        float nanopt1;
        float nanopt2;
        float nanopt3;
        float nanopt4;

        float cms4pt1;
        float cms4pt2;
        float cms4pt3;
        float cms4pt4;

        nanot->SetBranchAddress("event", &nevent);
        cms4t->SetBranchAddress("event", &cevent);

        nanot->SetBranchAddress("lep1_eta", &nanoeta1);
        nanot->SetBranchAddress("lep2_eta", &nanoeta2);
        nanot->SetBranchAddress("lep3_eta", &nanoeta3);
        nanot->SetBranchAddress("lep4_eta", &nanoeta4);

        cms4t->SetBranchAddress("lep1_eta", &cms4eta1);
        cms4t->SetBranchAddress("lep2_eta", &cms4eta2);
        cms4t->SetBranchAddress("lep3_eta", &cms4eta3);
        cms4t->SetBranchAddress("lep4_eta", &cms4eta4);

        nanot->SetBranchAddress("lep1_phi", &nanophi1);
        nanot->SetBranchAddress("lep2_phi", &nanophi2);
        nanot->SetBranchAddress("lep3_phi", &nanophi3);
        nanot->SetBranchAddress("lep4_phi", &nanophi4);

        cms4t->SetBranchAddress("lep1_phi", &cms4phi1);
        cms4t->SetBranchAddress("lep2_phi", &cms4phi2);
        cms4t->SetBranchAddress("lep3_phi", &cms4phi3);
        cms4t->SetBranchAddress("lep4_phi", &cms4phi4);

        nanot->SetBranchAddress("lep1_pt", &nanopt1);
        nanot->SetBranchAddress("lep2_pt", &nanopt2);
        nanot->SetBranchAddress("lep3_pt", &nanopt3);
        nanot->SetBranchAddress("lep4_pt", &nanopt4);

        cms4t->SetBranchAddress("lep1_pt", &cms4pt1);
        cms4t->SetBranchAddress("lep2_pt", &cms4pt2);
        cms4t->SetBranchAddress("lep3_pt", &cms4pt3);
        cms4t->SetBranchAddress("lep4_pt", &cms4pt4);

        nanot->GetEntry(i);
        cms4t->GetEntry(i);

        if (nevent != cevent) {
            cout << "Different Events, continuing" << endl;
            continue;
        }

        if (debug and dfile.is_open()){
            if (abs(nanoeta1 - cms4eta1) >= etadiff) {write_debug(dfile, nevent, 1, nanoeta1, nanophi1, nanopt1, cms4eta1, cms4phi1, cms4pt1);}
            else if (abs(nanophi1 - cms4phi1) >= phidiff) {write_debug(dfile, nevent, 1, nanoeta1, nanophi1, nanopt1, cms4eta1, cms4phi1, cms4pt1);} 

            if (abs(nanoeta2 - cms4eta2) >= etadiff) {write_debug(dfile, nevent, 2, nanoeta2, nanophi2, nanopt2, cms4eta2, cms4phi2, cms4pt2);}
            else if (abs(nanophi2 - cms4phi2) >= phidiff) {write_debug(dfile, nevent, 2, nanoeta2, nanophi2, nanopt2, cms4eta2, cms4phi2, cms4pt2);} 

            if (abs(nanoeta3 - cms4eta3) >= etadiff) {write_debug(dfile, nevent, 3, nanoeta3, nanophi3, nanopt3, cms4eta3, cms4phi3, cms4pt3);}
            else if (abs(nanophi3 - cms4phi3) >= phidiff) {write_debug(dfile, nevent, 3, nanoeta3, nanophi3, nanopt3, cms4eta3, cms4phi3, cms4pt3);} 

            if (abs(nanoeta4 - cms4eta4) >= etadiff) {write_debug(dfile, nevent, 4, nanoeta4, nanophi4, nanopt4, cms4eta4, cms4phi4, cms4pt4);}
            else if (abs(nanophi4 - cms4phi4) >= phidiff) {write_debug(dfile, nevent, 4, nanoeta4, nanophi4, nanopt4, cms4eta4, cms4phi4, cms4pt4);} 
        }
        i++;
    }

    if (dfile.is_open()) dfile.close();
    return 0;
}

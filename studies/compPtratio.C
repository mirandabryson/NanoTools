#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

void write_debug(std::ofstream& inf, int e, int id, float nptratio, float cptratio, float nleppt, float cleppt, float njetpt, float cjetpt) {
    inf << Form("%d, %d, %f, %f, %f, %f, %f, %f", e, id, nptratio, cptratio, nleppt, cleppt, njetpt, cjetpt) << endl;
    return;
}

TString outdir = "/home/users/ian/public_html/share/debug/";

int compPtratio() {
    // Debug files
    bool debug = true;
    ofstream dfile;
    if (debug and not dfile.is_open()) {
        dfile.open(Form("%s/ptratio_comp.dat", outdir.Data()), ios::out);
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

        int nevent;
        int cevent;

        float nanolepeta1;
        float nanolepeta2;
        float nanolepeta3;
        float nanolepeta4;

        float cms4lepeta1;
        float cms4lepeta2;
        float cms4lepeta3;
        float cms4lepeta4;

        float nanolepphi1;
        float nanolepphi2;
        float nanolepphi3;
        float nanolepphi4;

        float cms4lepphi1;
        float cms4lepphi2;
        float cms4lepphi3;
        float cms4lepphi4;

        float nanoleppt1;
        float nanoleppt2;
        float nanoleppt3;
        float nanoleppt4;

        float cms4leppt1;
        float cms4leppt2;
        float cms4leppt3;
        float cms4leppt4;

        float nanojeteta1;
        float nanojeteta2;
        float nanojeteta3;
        float nanojeteta4;

        float cms4jeteta1;
        float cms4jeteta2;
        float cms4jeteta3;
        float cms4jeteta4;

        float nanojetphi1;
        float nanojetphi2;
        float nanojetphi3;
        float nanojetphi4;

        float cms4jetphi1;
        float cms4jetphi2;
        float cms4jetphi3;
        float cms4jetphi4;

        float nanojetpt1;
        float nanojetpt2;
        float nanojetpt3;
        float nanojetpt4;

        float cms4jetpt1;
        float cms4jetpt2;
        float cms4jetpt3;
        float cms4jetpt4;

        float nanoptratio1;
        float nanoptratio2;
        float nanoptratio3;
        float nanoptratio4;

        float cms4ptratio1;
        float cms4ptratio2;
        float cms4ptratio3;
        float cms4ptratio4;

        nanot->SetBranchAddress("event", &nevent);
        cms4t->SetBranchAddress("event", &cevent);

        nanot->SetBranchAddress("lep1_ptratio", &nanoptratio1);
        nanot->SetBranchAddress("lep2_ptratio", &nanoptratio2);
        nanot->SetBranchAddress("lep3_ptratio", &nanoptratio3);
        nanot->SetBranchAddress("lep4_ptratio", &nanoptratio4);

        cms4t->SetBranchAddress("lep1_ptratio", &cms4ptratio1);
        cms4t->SetBranchAddress("lep2_ptratio", &cms4ptratio2);
        cms4t->SetBranchAddress("lep3_ptratio", &cms4ptratio3);
        cms4t->SetBranchAddress("lep4_ptratio", &cms4ptratio4);

        nanot->SetBranchAddress("lep1_eta", &nanolepeta1);
        nanot->SetBranchAddress("lep2_eta", &nanolepeta2);
        nanot->SetBranchAddress("lep3_eta", &nanolepeta3);
        nanot->SetBranchAddress("lep4_eta", &nanolepeta4);

        cms4t->SetBranchAddress("lep1_eta", &cms4lepeta1);
        cms4t->SetBranchAddress("lep2_eta", &cms4lepeta2);
        cms4t->SetBranchAddress("lep3_eta", &cms4lepeta3);
        cms4t->SetBranchAddress("lep4_eta", &cms4lepeta4);

        nanot->SetBranchAddress("lep1_phi", &nanolepphi1);
        nanot->SetBranchAddress("lep2_phi", &nanolepphi2);
        nanot->SetBranchAddress("lep3_phi", &nanolepphi3);
        nanot->SetBranchAddress("lep4_phi", &nanolepphi4);

        cms4t->SetBranchAddress("lep1_phi", &cms4lepphi1);
        cms4t->SetBranchAddress("lep2_phi", &cms4lepphi2);
        cms4t->SetBranchAddress("lep3_phi", &cms4lepphi3);
        cms4t->SetBranchAddress("lep4_phi", &cms4lepphi4);

        nanot->SetBranchAddress("lep1_pt", &nanoleppt1);
        nanot->SetBranchAddress("lep2_pt", &nanoleppt2);
        nanot->SetBranchAddress("lep3_pt", &nanoleppt3);
        nanot->SetBranchAddress("lep4_pt", &nanoleppt4);

        cms4t->SetBranchAddress("lep1_pt", &cms4leppt1);
        cms4t->SetBranchAddress("lep2_pt", &cms4leppt2);
        cms4t->SetBranchAddress("lep3_pt", &cms4leppt3);
        cms4t->SetBranchAddress("lep4_pt", &cms4leppt4);

        nanot->SetBranchAddress("jet1_eta", &nanolepeta1);
        nanot->SetBranchAddress("jet2_eta", &nanolepeta2);
        nanot->SetBranchAddress("jet3_eta", &nanolepeta3);
        nanot->SetBranchAddress("jet4_eta", &nanolepeta4);

        cms4t->SetBranchAddress("jet1_eta", &cms4lepeta1);
        cms4t->SetBranchAddress("jet2_eta", &cms4lepeta2);
        cms4t->SetBranchAddress("jet3_eta", &cms4lepeta3);
        cms4t->SetBranchAddress("jet4_eta", &cms4lepeta4);

        nanot->SetBranchAddress("jet1_phi", &nanolepphi1);
        nanot->SetBranchAddress("jet2_phi", &nanolepphi2);
        nanot->SetBranchAddress("jet3_phi", &nanolepphi3);
        nanot->SetBranchAddress("jet4_phi", &nanolepphi4);

        cms4t->SetBranchAddress("jet1_phi", &cms4lepphi1);
        cms4t->SetBranchAddress("jet2_phi", &cms4lepphi2);
        cms4t->SetBranchAddress("jet3_phi", &cms4lepphi3);
        cms4t->SetBranchAddress("jet4_phi", &cms4lepphi4);

        nanot->SetBranchAddress("jet1_pt", &nanoleppt1);
        nanot->SetBranchAddress("jet2_pt", &nanoleppt2);
        nanot->SetBranchAddress("jet3_pt", &nanoleppt3);
        nanot->SetBranchAddress("jet4_pt", &nanoleppt4);

        cms4t->SetBranchAddress("jet1_pt", &cms4leppt1);
        cms4t->SetBranchAddress("jet2_pt", &cms4leppt2);
        cms4t->SetBranchAddress("jet3_pt", &cms4leppt3);
        cms4t->SetBranchAddress("jet4_pt", &cms4leppt4);

        nanot->GetEntry(i);
        cms4t->GetEntry(i);

        if (nevent != cevent) {
            cout << "Different Events, continuing" << endl;
            continue;
        }

        float ptratiodiff = 0.005;

        if (debug and dfile.is_open()){
            if (abs(nanoptratio1 - cms4ptratio1) >= ptratiodiff) {write_debug(dfile, nevent, 1, nanoptratio1, cms4ptratio1, nanoleppt1, cms4leppt1, nanojetpt1, cms4jetpt1);}
            if (abs(nanoptratio2 - cms4ptratio2) >= ptratiodiff) {write_debug(dfile, nevent, 2, nanoptratio2, cms4ptratio2, nanoleppt2, cms4leppt2, nanojetpt2, cms4jetpt2);}
            if (abs(nanoptratio3 - cms4ptratio3) >= ptratiodiff) {write_debug(dfile, nevent, 3, nanoptratio3, cms4ptratio3, nanoleppt3, cms4leppt3, nanojetpt3, cms4jetpt3);}
            if (abs(nanoptratio4 - cms4ptratio4) >= ptratiodiff) {write_debug(dfile, nevent, 4, nanoptratio4, cms4ptratio4, nanoleppt4, cms4leppt4, nanojetpt4, cms4jetpt4);}
        }
        i++;
    }

    if (dfile.is_open()) dfile.close();
    return 0;
}

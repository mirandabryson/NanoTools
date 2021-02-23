#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"
#include "TCanvas.h"

#include "./NanoCORE/Nano.h"
#include "./NanoCORE/SSSelections.cc"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace tas;

struct Events {
    int gen_1lepton;
    int gen_1mu;
    int gen_1el;
    int gen_1mu1el;
    int gen_1mu1el_fiducial;
    int gen_1mu1el_fiducial_OS;
    int gen_1mu1el_fiducial_OS_2bquark;
    int reco_geq1mu;
    int reco_geq1el;
    int reco_1mu;
    int reco_1el;
    int reco_1mu1el;
    int reco_1mu1el_OS;
    int reco_1mu1el_OS_HLT;
    int reco_1mu1el_OS_HLT_2btags;
};


bool inFiducialLepRegion(float pt, float eta) {
    return (pt > 25 && fabs(eta) < 2.5);
}

bool inFiducialJetRegion(float pt, float eta) {
    return (pt > 30 && fabs(eta) < 2.5);
}

void analysis(Events &events, TString sample_name, bool isData) {
    // Generator particle counters
    int num_gen_els = 0;
    int num_gen_mus = 0;
    // Generator particles in fiducial region counters
    int num_gen_fiducial_els = 0;
    int num_gen_fiducial_mus = 0;
    int num_gen_fiducial_bquarks = 0;
    // Properties of el and mu
    int gen_el_id;
    int gen_mu_id;
    // Loop over generator record
    for (unsigned int i = 0; i < nGenPart(); i++) {
        // This gen particle
        int this_id = GenPart_pdgId().at(i);
        int this_pt = GenPart_pt().at(i);
        int this_eta = GenPart_eta().at(i);
        // This gen particle's mother
        int mother_idx = GenPart_genPartIdxMother().at(i);
        if (mother_idx < 0) {
            continue;
        }
        int mother_id = GenPart_pdgId().at(mother_idx);
        // Look for bquarks from t/tbar
        if (abs(mother_id) == 6) {
            if (abs(this_id) == 5 && inFiducialJetRegion(this_pt, this_eta)) {
                num_gen_fiducial_bquarks++;
            }
        }
        // This gen particle's grandmother
        int grandmother_idx = GenPart_genPartIdxMother().at(mother_idx);
        if (grandmother_idx < 0) {
            continue;
        }
        int grandmother_id = GenPart_pdgId().at(grandmother_idx);
        // Look for leptons from W+/W- from t/tbar
        if (abs(mother_id) == 24 && abs(grandmother_id) == 6) {
            if (abs(this_id) == 11) {
                num_gen_els++;
                gen_el_id = this_id;
                if (inFiducialLepRegion(this_pt, this_eta)) {
                    num_gen_fiducial_els++;
                }
            }
            else if (abs(this_id) == 13) {
                num_gen_mus++;
                gen_mu_id = this_id;
                if (inFiducialLepRegion(this_pt, this_eta)) {
                    num_gen_fiducial_mus++;
                }
            }
        }
    }
    // Check counters
    bool one_gen_el = (num_gen_els == 1);
    bool one_gen_mu = (num_gen_mus == 1);
    bool one_gen_fiducial_el = (num_gen_fiducial_els == 1);
    bool one_gen_fiducial_mu = (num_gen_fiducial_mus == 1);
    bool two_gen_fiducial_bquarks = (num_gen_fiducial_bquarks == 2);
    bool OS_gen_fiducial_1el1mu = false;
    if (one_gen_fiducial_el && one_gen_fiducial_mu) {
        OS_gen_fiducial_1el1mu = (gen_el_id*gen_mu_id < 0);
    }

    if (num_gen_els > 0 || num_gen_mus > 0) {
        events.gen_1lepton++;
    }
    if (one_gen_el) {
        events.gen_1el++;
    }
    if (one_gen_mu) {
        events.gen_1mu++;
    }
    if (one_gen_el && one_gen_mu) {
        events.gen_1mu1el++;
    }
    if (one_gen_fiducial_el && one_gen_fiducial_mu) {
        events.gen_1mu1el_fiducial++;
    }
    if (OS_gen_fiducial_1el1mu) {
        events.gen_1mu1el_fiducial_OS++;
        if (two_gen_fiducial_bquarks) {
            events.gen_1mu1el_fiducial_OS_2bquark++;
        }
    }
    if (!OS_gen_fiducial_1el1mu || !two_gen_fiducial_bquarks) {
        // Don't increment any reco counts
        return;
    }

    /* NOTE:
     * Beyond here, looking at e-mu events with 2 bquarks all within fiducial
     * region 
     */

    // Reco particle counters
    int num_reco_els = 0;
    int num_reco_mus = 0;
    int num_reco_btags = 0;
    // Properties of el and mu
    Lepton reco_el;
    Lepton reco_mu;
    // Loop over reco leptons
    Leptons leptons = getLeptons();
    for (unsigned int i = 0; i < leptons.size(); i++) {
        Lepton lep = leptons.at(i);
        if (lep.pt() < 25) {
            continue;
        }
        if (lep.is_el()) {
			bool tight_el_id = (Electron_cutBased()[lep.idx()] == 4);
            if (tight_el_id) {
                num_reco_els++;
                reco_el = lep;
            }
        }
        if (lep.is_mu()) {
			bool med_mu_id = (Muon_mediumId()[lep.idx()] == 1);
            bool loose_mu_iso = (Muon_pfRelIso04_all()[lep.idx()] < 0.25);
            if (med_mu_id && loose_mu_iso) {
                num_reco_mus++;
                reco_mu = lep;
            }
        }
    }
	// 2016 legacy wp	
	float tight_working_point = 0.7221;	
	if (!isData) {
		// 102X MC wp	
		tight_working_point = 0.7264;	
	}
    // Loop over reco jets
    for (unsigned int i = 0; i < nJet(); i++) {
        if (!inFiducialJetRegion(Jet_pt().at(i), Jet_eta().at(i))) {
            continue;
        }
        if (num_reco_els > 0 && num_reco_mus > 0) {
            bool el_overlap = (Electron_jetIdx().at(reco_el.idx()) == i);
            bool mu_overlap = (Muon_jetIdx().at(reco_mu.idx()) == i);
            if (el_overlap || mu_overlap) {
                continue;
            }
        }
        if (Jet_btagDeepFlavB().at(i) >= tight_working_point) {
            num_reco_btags++;
        }
    }
    // Check HLTs
    bool passes_HLTs = true;
    if (!isData) {
        passes_HLTs = (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() 
                       || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() 
                       || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() 
                       || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ());
    }
    else if (sample_name.Contains("Run2016H")) {
        passes_HLTs = (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() 
                       || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ());
    }
    else if (sample_name.Contains("Run2016")) {
        passes_HLTs = (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() 
                       || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL());
    }
    // Check counters
    if (num_reco_els > 0) {
        events.reco_geq1el++;
        if (num_reco_els == 1) {
            events.reco_1el++;
        }
    }
    if (num_reco_mus > 0) {
        events.reco_geq1mu++;
        if (num_reco_mus == 1) {
            events.reco_1mu++;
        }
    }
    if (num_reco_els == 1 && num_reco_mus == 1) {
        events.reco_1mu1el++;
        if (reco_el.charge() != reco_mu.charge()) {
            events.reco_1mu1el_OS++;
            if (passes_HLTs) {
                events.reco_1mu1el_OS_HLT++;
                if (num_reco_btags == 2) {
                    events.reco_1mu1el_OS_HLT_2btags++;
                }
            }
        }
    }
    
    return;
}

int ScanChain(TChain *ch, TString sample_name, bool isData) {
    // Set configuration parameters
    gconf.year = 2016;
    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
	float xsec = 831.76;
    // File loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        // Open file
        TFile *file = TFile::Open(currentFile->GetTitle());
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        // TTree configuration
        tree->SetCacheSize(128*1024*1024);
        tree->SetCacheLearnEntries(100);
        auto psRead = new TTreePerfStats("readPerf", tree);
        nt.Init(tree);
        // Event counters (initialized to 0)
        Events events = {};
        // Event loop
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            /* Analysis code */
            analysis(events, sample_name, isData);
        } // END event loop

        // Print out
        cout << "gen_1lepton " << events.gen_1lepton << endl;
        cout << "gen_1mu " << events.gen_1mu << endl;
        cout << "gen_1el " << events.gen_1el << endl;
        cout << "gen_1mu1el " << events.gen_1mu1el << endl;
        cout << "gen_1mu1el_fiducial " << events.gen_1mu1el_fiducial << endl;
        cout << "gen_1mu1el_fiducial_OS " << events.gen_1mu1el_fiducial_OS << endl;
        cout << "gen_1mu1el_fiducial_OS_2bquark " << events.gen_1mu1el_fiducial_OS_2bquark << endl;
        cout << "reco_geq1mu " << events.reco_geq1mu << endl;
        cout << "reco_geq1el " << events.reco_geq1el << endl;
        cout << "reco_1mu " << events.reco_1mu << endl;
        cout << "reco_1el " << events.reco_1el << endl;
        cout << "reco_1mu1el " << events.reco_1mu1el << endl;
        cout << "reco_1mu1el_OS " << events.reco_1mu1el_OS << endl;
        cout << "reco_1mu1el_OS_HLT " << events.reco_1mu1el_OS_HLT << endl;
        cout << "reco_1mu1el_OS_HLT_2btags " << events.reco_1mu1el_OS_HLT_2btags << endl;

        // Clean up
        delete file;

    } // END file loop

    // Wrap up
    return 0;
}

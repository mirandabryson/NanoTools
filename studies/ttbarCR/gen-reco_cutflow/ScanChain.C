#pragma GCC diagnostic ignored "-Wsign-compare"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeCache.h"
#include "TTreeCacheUnzip.h"
#include "TTreePerfStats.h"

#include "./NanoCORE/Nano.h"
#include "./NanoCORE/SSSelections.cc"
#include "./scalefactors.C"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace tas;

void analysis(int &gen_lept_event, int &gen_elec_event_t, int &gen_mu_event_t, int &gen_one_and_one_t, int &gen_opp_ch_t, int &gen_two_b_t, int &lept_event, int &elec_event_t, int &mu_event_t, int &one_and_one_t, int &opp_ch_t, int &pass_hlt_t, int &two_btag_t, float &elecID_w, float &muID_w, float &muIso_w, bool isData){
	// Iter over GenParticles
	int numGenMu = 0;
	int numGenElec = 0;
	int numGenb = 0;
	int gen_el_id = 0;
	int gen_mu_id = 0;
	for (unsigned int i = 0; i < nGenPart(); i++) {
		int m_index = abs(GenPart_genPartIdxMother().at(i));
		if(abs(GenPart_pdgId().at(m_index)) == 24){
			if(abs(GenPart_pdgId().at(i)) == 13 && GenPart_pt().at(i) > 25) {
				numGenMu++;
				gen_mu_id = i;
			}
			else if(abs(GenPart_pdgId().at(i)) == 11 && GenPart_pt().at(i) > 25) {
				numGenElec++;
				gen_el_id = i;
			}	
		}
		else if(abs(GenPart_pdgId().at(m_index)) == 6){
			if(abs(GenPart_pdgId().at(i)) == 5 && fabs(GenPart_eta().at(i)) < 2.5 && GenPart_pt().at(i) > 30){
				numGenb++;
			}
		}
	}
	// Bools	
	bool gen_elec_event = false;
	bool gen_mu_event = false;
	bool gen_one_and_one = false;
	bool gen_opp_ch = false;
	bool gen_two_b = false;
	// Conditionals
	if (numGenElec > 0 || numGenMu > 0) {gen_lept_event++;}

	if (numGenElec == 1) {gen_elec_event = true;}
	
	if (numGenMu == 1) {gen_mu_event = true;}

	if (numGenElec == 1 && numGenMu == 1) {gen_one_and_one = true;}

	if (gen_one_and_one && (GenPart_pdgId().at(gen_mu_id) * GenPart_pdgId().at(gen_el_id) < 0)) {gen_opp_ch = true;}
	
	//cout << GenPart_pdgId().at(gen_mu_id) * GenPart_pdgId().at(gen_el_id) << endl;	
	if (gen_opp_ch && numGenb == 2) {gen_two_b = true;}	
	//if (numGenb == 2){gen_two_b = true;}
	// Iter over Reco Leptons	
	Leptons leptons = getLeptons();
	int numElec = 0;
	int numMu = 0;
	Lepton elec;
	Lepton mu;
	for (unsigned int i = 0; i < leptons.size(); i++) {		
		if (leptons[i].pt() < 25) {
			continue;
		}
		if(leptons[i].is_el()) {
			bool goodElectron = (Electron_cutBased()[leptons[i].idx()] == 4);
			if (goodElectron) {
				elec = leptons[i];
				numElec++; 
			}
		}
		else if(leptons[i].is_mu()) {
			bool goodMuon = (Muon_mediumId()[leptons[i].idx()] == 1 && Muon_pfRelIso04_all()[leptons[i].idx()] < 0.25);
			if (goodMuon) {
				mu = leptons[i];
				numMu++;
			}
		}
	}
	// Bools
	bool elec_event = false;
	bool mu_event = false;
	bool one_and_one = false;
	bool opp_ch = false;
	bool pass_hlt = false;
	bool two_btag = false;
	// Conditionals
	
	if (numElec > 0 || numMu > 0) {lept_event++;}

	if (numElec == 1) {elec_event = true;}
	
	if (numMu == 1) {mu_event = true;}

	if (numElec == 1 && numMu == 1) {one_and_one = true;}
	
	if (one_and_one && (elec.charge() == -mu.charge())) {opp_ch = true;}
	
	if (opp_ch && (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ())) {pass_hlt = true;}

	// 2016 legacy wp	
	float tight_working_point = 0.7221;	
	if (!isData) {
		// 102X MC wp	
		tight_working_point = 0.7264;	
	}
	int num_tagged_b_tight = 0;	
	// Iter Over Jets
	
	for (unsigned int i = 0; i < nJet(); i++) {
		if(fabs(Jet_eta().at(i)) > 2.4) continue;
		if(Jet_pt().at(i) < 30) continue;
		if (numElec != 0 && numMu != 0){
			if(Jet_jetId().at(i) == Electron_jetIdx().at(elec.idx()) || Jet_jetId().at(i) == Muon_jetIdx().at(mu.idx())) continue;
		}
		if(Jet_btagDeepFlavB().at(i) >= tight_working_point) {
			num_tagged_b_tight++;
		}
	}

	if (pass_hlt && (num_tagged_b_tight == 2)) {two_btag = true;}
	//if (num_tagged_b_tight == 2) {two_btag = true;}
	// Update Global Count
	if (gen_elec_event) {gen_elec_event_t++;}
	if (gen_mu_event) {gen_mu_event_t++;}
	if (gen_one_and_one) {gen_one_and_one_t++;}
	if (gen_opp_ch) {gen_opp_ch_t++;}
	if (gen_two_b) {gen_two_b_t++;}
	if (elec_event) {elec_event_t++;}
	if (mu_event) {mu_event_t++;}
	if (one_and_one) {one_and_one_t++;}
	if (opp_ch) {opp_ch_t++;}
	if (pass_hlt) {pass_hlt_t++;}
	if (two_btag) {two_btag_t++;}

	float xsec_w = 831.76 * 16357.0 / 6492749;
	float el_w = elecIDScaleFactors(elec.pt(), elec.eta());
	float mu_w = muonMediumIDScaleFactors(mu.pt(), mu.eta());
	float mu_is_w = muonLooseIsoMediumIDScaleFactors(mu.pt(), mu.eta());
	if (two_btag) {
		elecID_w += xsec_w * el_w;
		muID_w += xsec_w * el_w * mu_w;
		muIso_w += xsec_w * el_w * mu_w * mu_is_w;
	}
	
}        


int ScanChain(TChain *ch, bool isData) {
    // Output
    // Set configuration parameters
    gconf.year = 2016;
    // Initialize looper variables
    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);
	// Non Cutflow Vars
	float xsec = 831.76;
	float int_lum = 16357.0;
	// Cutflow Vars
	int gen_lept_event = 0;
	int gen_elec_event_t = 0;
	int gen_mu_event_t = 0;
	int gen_one_and_one_t = 0;
	int gen_opp_ch_t = 0;
	int gen_two_b_t = 0;
	int lept_event = 0;
	int elec_event_t = 0;
	int mu_event_t = 0;
	int one_and_one_t = 0;
	int opp_ch_t = 0;
	int pass_hlt_t = 0;
	int two_btag_t = 0;
	float elecID_w = 0;
	float muID_w = 0;
	float muIso_w = 0;

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
        // Event loop
        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            // Load event
            nt.GetEntry(event);
            tree->LoadTree(event);
            // Update progress
            nEventsTotal++;
            /* Analysis code */
			analysis(gen_lept_event, gen_elec_event_t, gen_mu_event_t, gen_one_and_one_t, gen_opp_ch_t, gen_two_b_t, lept_event, elec_event_t, mu_event_t, one_and_one_t, opp_ch_t, pass_hlt_t, two_btag_t, elecID_w, muID_w, muIso_w, isData);		
			} // END event loop
        // Clean up
        delete file;
    } // END file loop
	// Print Cutflow Total
	cout << "Total Number of Events: " << nEventsTotal << endl;
	cout << "Number of Gen Lepton Events: " << gen_lept_event << endl;
	cout << "Num Gen Elec Events: " << gen_elec_event_t << endl;
	cout << "Num Gen Mu Events: " << gen_mu_event_t << endl;
	cout << "Num Gen One Elec && One Mu Events: " << gen_one_and_one_t << endl;
	cout << "Num Gen Opposite Sign Events: " << gen_opp_ch_t << endl;
	cout << "Num Gen 2 b Events: " << gen_two_b_t << endl;
	cout << "Number of Lepton Events: " << lept_event << endl;
	cout << "Num Elec Events: " << elec_event_t << endl;
	cout << "Num Mu Events: " << mu_event_t << endl;
	cout << "Num One Elec && One Mu Events: " << one_and_one_t << endl;
	cout << "Num Opposite Sign Events: " << opp_ch_t << endl;
	cout << "Num Passed HLTs Events: " << pass_hlt_t << endl;
	cout << "Num 2 bTag Events: " << two_btag_t << endl;
	cout << "Num elecID_w: " << elecID_w << endl;
	cout << "Num muID_w: " << muID_w << endl;
	cout << "Num muIso_w: " << muIso_w << endl;
 
    return 0;
}

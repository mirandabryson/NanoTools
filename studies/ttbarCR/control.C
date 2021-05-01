// C++
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TString.h"
// NanoCORE
#include "./NanoCORE/Config.h"
#include "./NanoCORE/SSSelections.h"
#include "./NanoCORE/Tools/btagsf/BTagCalibrationStandalone.h"
#include "./NanoCORE/Tools/btagsf/BTagCalibrationStandalone.cc"
// Header
#include "control.h"
#include "scalefactors/all.h"


using namespace std;
using namespace tas;

ControlTree::ControlTree(TFile* new_tfile) {
	// TTree and TFile
    ttree = new TTree("tree", "tree");
    tfile = new_tfile;
    // Event
    b_num_pvs = ttree->Branch("num_pvs", &num_pvs, "num_pvs/I");
	b_met = ttree->Branch("met", &met, "met/F");
	b_ht = ttree->Branch("ht", &ht, "ht/F");
    // Leptons
	b_leading_lep_id = ttree->Branch("leading_lep_id", &leading_lep_id, "leading_lep_id/I");
	b_leading_lep_pt = ttree->Branch("leading_lep_pt", &leading_lep_pt, "leading_lep_pt/F");
	b_leading_lep_eta = ttree->Branch("leading_lep_eta", &leading_lep_eta, "leading_lep_eta/F");
	b_leading_lep_phi = ttree->Branch("leading_lep_phi", &leading_lep_phi, "leading_lep_phi/F");
	b_trailing_lep_id = ttree->Branch("trailing_lep_id", &trailing_lep_id, "trailing_lep_id/I");
	b_trailing_lep_pt = ttree->Branch("trailing_lep_pt", &trailing_lep_pt, "trailing_lep_pt/F");
	b_trailing_lep_eta = ttree->Branch("trailing_lep_eta", &trailing_lep_eta, "trailing_lep_eta/F");
	b_trailing_lep_phi = ttree->Branch("trailing_lep_phi", &trailing_lep_phi, "trailing_lep_phi/F");
	b_dilep_mass = ttree->Branch("dilep_mass", &dilep_mass, "dilep_mass/F");
    // Jets
    TBranch* b_leading_vbs_jet_mc_origin = ttree->Branch("leading_vbs_jet_mc_origin", 
                                                         &leading_vbs_jet_mc_origin, 
                                                         "leading_vbs_jet_mc_origin/I");
    TBranch* b_leading_vbs_jet_P = ttree->Branch("leading_vbs_jet_P", 
                                                 &leading_vbs_jet_P, 
                                                 "leading_vbs_jet_P/F");
    TBranch* b_leading_vbs_jet_pt = ttree->Branch("leading_vbs_jet_pt", 
                                                  &leading_vbs_jet_pt, 
                                                  "leading_vbs_jet_pt/F");
    TBranch* b_leading_vbs_jet_eta = ttree->Branch("leading_vbs_jet_eta", 
                                                   &leading_vbs_jet_eta, 
                                                   "leading_vbs_jet_eta/F");
    TBranch* b_leading_vbs_jet_phi = ttree->Branch("leading_vbs_jet_phi", 
                                                   &leading_vbs_jet_phi, 
                                                   "leading_vbs_jet_phi/F");
    TBranch* b_trailing_vbs_jet_mc_origin = ttree->Branch("trailing_vbs_jet_mc_origin", 
                                                          &trailing_vbs_jet_mc_origin, 
                                                          "trailing_vbs_jet_mc_origin/I");
    TBranch* b_trailing_vbs_jet_P = ttree->Branch("trailing_vbs_jet_P", 
                                                  &trailing_vbs_jet_P, 
                                                  "trailing_vbs_jet_P/F");
    TBranch* b_trailing_vbs_jet_pt = ttree->Branch("trailing_vbs_jet_pt", 
                                                   &trailing_vbs_jet_pt, 
                                                   "trailing_vbs_jet_pt/F");
    TBranch* b_trailing_vbs_jet_eta = ttree->Branch("trailing_vbs_jet_eta", 
                                                    &trailing_vbs_jet_eta, 
                                                    "trailing_vbs_jet_eta/F");
    TBranch* b_trailing_vbs_jet_phi = ttree->Branch("trailing_vbs_jet_phi", 
                                                   &trailing_vbs_jet_phi, 
                                                   "trailing_vbs_jet_phi/F");
	b_vbs_dijet_mass = ttree->Branch("vbs_dijet_mass", &vbs_dijet_mass, "vbs_dijet_mass/F");
	b_jet_is_vbs = ttree->Branch("jet_is_vbs", &jet_is_vbs);
	b_jet_pt = ttree->Branch("jet_pt", &jet_pt);
	b_jet_eta = ttree->Branch("jet_eta", &jet_eta);
	b_jet_phi = ttree->Branch("jet_phi", &jet_phi);
	b_jet_mc_origin = ttree->Branch("jet_mc_origin", &jet_mc_origin);
	b_jet_pu_id = ttree->Branch("jet_pu_id", &jet_pu_id);
	b_jet_pu_id_disc = ttree->Branch("jet_pu_id_disc", &jet_pu_id_disc);
	b_jet_n_em_Efrac = ttree->Branch("jet_n_em_Efrac", &jet_n_em_Efrac);
	b_jet_n_he_Efrac = ttree->Branch("jet_n_he_Efrac", &jet_n_he_Efrac);
	b_jet_mu_Efrac = ttree->Branch("jet_mu_Efrac", &jet_mu_Efrac);
	b_jet_ch_em_Efrac = ttree->Branch("jet_ch_em_Efrac", &jet_ch_em_Efrac);
	b_jet_ch_he_Efrac = ttree->Branch("jet_ch_he_Efrac", &jet_ch_he_Efrac);
	b_jet_ch_pv0_Efrac = ttree->Branch("jet_ch_pv0_Efrac", &jet_ch_pv0_Efrac);
	b_jet_ch_pv1_Efrac = ttree->Branch("jet_ch_pv1_Efrac", &jet_ch_pv1_Efrac);
	b_jet_ch_pv2_Efrac = ttree->Branch("jet_ch_pv2_Efrac", &jet_ch_pv2_Efrac);
	b_jet_ch_pv3_Efrac = ttree->Branch("jet_ch_pv3_Efrac", &jet_ch_pv3_Efrac);
	b_num_jets = ttree->Branch("num_jets", &num_jets, "num_jets/I");
	b_num_btags_tight = ttree->Branch("num_btags_tight", &num_btags_tight, "num_btags_tight/I");
    // Weights
	b_gen_weight = ttree->Branch("gen_weight", &gen_weight, "gen_weight/F");
	b_mc_weight = ttree->Branch("mc_weight", &mc_weight, "mc_weight/F");
	b_mc_loose_btag_weight = ttree->Branch("mc_loose_btag_weight", 
                                           &mc_loose_btag_weight, 
                                           "mc_loose_btag_weight/F");
	b_mc_medium_btag_weight = ttree->Branch("mc_medium_btag_weight", 
                                            &mc_medium_btag_weight, 
                                            "mc_medium_btag_weight/F");
	b_mc_tight_btag_weight = ttree->Branch("mc_tight_btag_weight", 
                                           &mc_tight_btag_weight, 
                                           "mc_tight_btag_weight/F");
    if (!isData()) {
        // Initialize calibration objects for b-tagging scale factors
        string csv_path;
        if (year() == 2016) {
            csv_path = "./NanoCORE/Tools/btagsf/csv/DeepJet_2016LegacySF_V1.csv";
        }
        else if (year() == 2017) {
            csv_path = "./NanoCORE/Tools/btagsf/csv/DeepJet_94XSF_V4_B_F.csv";
        }
        else if (year() == 2018) {
            csv_path = "./NanoCORE/Tools/btagsf/csv/DeepJet_102XSF_V3.csv";
        }
        else {
            throw std::runtime_error("ControlTree::INIT: Error - invalid year");
        }
        // CSV object
        deepjet_csv = BTagCalibration("csvv1", csv_path);
        // Loose reader
        deepjet_loose_reader = BTagCalibrationReader(BTagEntry::OP_LOOSE, "central");
        deepjet_loose_reader.load(deepjet_csv, BTagEntry::FLAV_B, "comb");
        // Medium reader
        deepjet_medium_reader = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central");
        deepjet_medium_reader.load(deepjet_csv, BTagEntry::FLAV_B, "comb");
        // Tight reader
        deepjet_tight_reader = BTagCalibrationReader(BTagEntry::OP_TIGHT, "central");
        deepjet_tight_reader.load(deepjet_csv, BTagEntry::FLAV_B, "comb");
    }
}

void ControlTree::resetBranches() {
	// Event
    num_pvs = -999;
	met = -999;
	ht = -999;
    // Leptons
	leading_lep_id = -999;
	leading_lep_pt = -999;
	leading_lep_eta = -999;
	leading_lep_phi = -999;
	trailing_lep_id = -999;
	trailing_lep_pt = -999;
	trailing_lep_eta = -999;
	trailing_lep_phi = -999;
    dilep_mass = -999;
    // Jets
    leading_vbs_jet_mc_origin = -999;
    leading_vbs_jet_P = -999;
    leading_vbs_jet_pt = -999;
    leading_vbs_jet_eta = -999;
    leading_vbs_jet_phi = -999;
    trailing_vbs_jet_mc_origin = -999;
    trailing_vbs_jet_P = -999;
    trailing_vbs_jet_pt = -999;
    trailing_vbs_jet_eta = -999;
    trailing_vbs_jet_phi = -999;
    vbs_dijet_mass = -999;
    jet_is_vbs.clear();
    jet_pt.clear();
    jet_eta.clear();
    jet_phi.clear();
    jet_mc_origin.clear();
    jet_pu_id.clear();
    jet_pu_id_disc.clear();
    jet_n_em_Efrac.clear();
    jet_n_he_Efrac.clear();
    jet_mu_Efrac.clear();
    jet_ch_em_Efrac.clear();
    jet_ch_he_Efrac.clear();
    jet_ch_pv0_Efrac.clear();
    jet_ch_pv1_Efrac.clear();
    jet_ch_pv2_Efrac.clear();
    jet_ch_pv3_Efrac.clear();
    num_jets = 0;
    num_btags_tight = 0;
    // Weights
	gen_weight = 1.;
	mc_weight = 1.;
	mc_loose_btag_weight = 1.;
	mc_medium_btag_weight = 1.;
	mc_tight_btag_weight = 1.;
}

bool ControlTree::jetLeptonOverlap(int jet_idx, Lepton lep) {
    int lep_jet_idx = -1;
    if (lep.is_el()) { lep_jet_idx = Electron_jetIdx().at(lep.idx()); }
    else if (lep.is_mu()) { lep_jet_idx = Muon_jetIdx().at(lep.idx()); }
    else { return false; }
    return (lep_jet_idx == jet_idx);
}

void ControlTree::fillBranches() {
	// Get Leptons
	Leptons leptons = getLeptons();
	// Iter Over Leptons 
    Lepton leading_lep;
    Lepton trailing_lep;
    int num_tight_leptons = 0;
    int num_fakable_leptons = 0;
	for (unsigned int i = 0; i < leptons.size(); i++) {		
        Lepton lep = leptons.at(i);
        if (lep.pt() < 10.) {
            continue;
        }
        if (lep.idlevel() == SS::IDtight) {
            // Tight cut on electron rel iso
            if (lep.is_el() && Electron_miniPFRelIso_all().at(lep.idx()) > 0.05) {
                continue;
            }
            // Count and sort tight leptons
            num_tight_leptons++;
            if (lep.pt() > leading_lep.pt()) {
                trailing_lep = leading_lep;
                leading_lep = lep;
            }
            else if (lep.pt() > trailing_lep.pt()) {
                trailing_lep = lep;
            }
        }
        // Count loose&!tight leptons
        if (lep.idlevel() == SS::IDfakable) {
            num_fakable_leptons++;
        }
	}
    // Require dilepton events
    if (num_tight_leptons != 2) { return; }
    // Opposite flavor
    if (abs(leading_lep.id()) == abs(trailing_lep.id())) { return; }
    // Opposite sign
	if (leading_lep.charge() == trailing_lep.charge()) { return; }
    // Veto surviving events with any loose&!tight leptons
    if (num_fakable_leptons > 0) { return; }
    // Make pt cuts on leading/trailing leptons
    if (leading_lep.pt() < 25.) { return; }
    if (trailing_lep.is_el() && trailing_lep.pt() < 25.) { return; }
    else if (trailing_lep.is_mu() && trailing_lep.pt() < 20.) { return; }

    // Run b-tagging procedure and find VBS jet candidates
    float loose_working_point;	
    float medium_working_point;
    float tight_working_point;
    if (year() == 2016) {
        loose_working_point = 0.0614;	
        medium_working_point = 0.3093;	
        tight_working_point = 0.7221;	
    }
    else if (year() == 2017) {
        loose_working_point = 0.0521;
        medium_working_point = 0.3033;
        tight_working_point = 0.7489;
    }
    else if (year() == 2018) {
        loose_working_point = 0.0494;
        medium_working_point = 0.2770;
        tight_working_point = 0.7264;
    }
    else {
        throw std::runtime_error("ControlTree::fillBranches: Error - invalid year");
        return;
    }
	int num_tagged_b_loose = 0;	
	int num_tagged_b_medium = 0;
	int num_tagged_b_tight = 0;	
	// Iter Over Jets
    double sf = 1.0; // placeholder for btag sf (MC only)
    vector<unsigned int> good_jets;
    vector<VBSJetCand> vbs_jet_cands;
	for (unsigned int i = 0; i < nJet(); i++) {
        // Require tight jet ID
        if (!(Jet_jetId().at(i) & (1 << 2))) { continue; }
        // Check other jet properties
        bool vbs_jet_candidate = true;
        bool taggable = true;
        bool countable = true;
		if (fabs(Jet_eta().at(i)) >= 2.4) { 
            taggable = false; 
        }
		if (Jet_pt().at(i) <= 30) {
            taggable = false;
            vbs_jet_candidate = false;
            countable = false;
        }
        // Lepton overlap removal
		if (jetLeptonOverlap(i, leading_lep) || jetLeptonOverlap(i, trailing_lep)) {
            taggable = false;
            vbs_jet_candidate = false;
            countable = false;
        }
        // START: b-tagging procedure
        if (taggable) {
            // Get jet pt for sfs (MC only)
            float mc_jet_pt = (Jet_pt().at(i) < 1000.) ? Jet_pt().at(i) : 999.;
            // Check DeepJet working points
            float deepjet_disc = Jet_btagDeepFlavB().at(i);
            if (deepjet_disc >= loose_working_point) {
                num_tagged_b_loose++;
                if (!isData()) {
                    sf = deepjet_loose_reader.eval(BTagEntry::FLAV_B,
                                                   Jet_eta().at(i),
                                                   mc_jet_pt,
                                                   deepjet_disc);
                    assert (sf > 0.);
                    mc_loose_btag_weight *= sf;
                }
            }
            if (deepjet_disc >= medium_working_point) {
                num_tagged_b_medium++;
                if (!isData()) {
                    sf = deepjet_medium_reader.eval(BTagEntry::FLAV_B,
                                                    Jet_eta().at(i),
                                                    mc_jet_pt,
                                                    deepjet_disc);
                    assert (sf > 0.);
                    mc_medium_btag_weight *= sf;
                }
            }
            if (deepjet_disc >= tight_working_point) {
                vbs_jet_candidate = false;
                num_tagged_b_tight++;
                if (!isData()) {
                    sf = deepjet_tight_reader.eval(BTagEntry::FLAV_B,
                                                   Jet_eta().at(i),
                                                   mc_jet_pt,
                                                   deepjet_disc);
                    assert (sf > 0.);
                    mc_tight_btag_weight *= sf;
                }
            }
        }
        // END: b-tagging procedure
        if (countable) { 
            if (vbs_jet_candidate) { vbs_jet_cands.push_back(VBSJetCand(i)); }
            good_jets.push_back(i); 
        }
	}

    // Veto events with < 2 btags
    if (num_tagged_b_tight < 2) {
        return;
    }

    // VBS candidate search procedure
    VBSJetCand leading_jet;
    VBSJetCand trailing_jet;
    if (vbs_jet_cands.size() == 0) {
        // Veto these events
        return;
    }
    else if (vbs_jet_cands.size() == 1) {
        // Veto these events
        return;
    }
    else if (vbs_jet_cands.size() == 2) {
        VBSJetCand jet1 = vbs_jet_cands.at(0);
        VBSJetCand jet2 = vbs_jet_cands.at(1);
        // Sort into leading and trailing jets
        if (jet1.pt() > jet2.pt()) {
            leading_jet = jet1;
            trailing_jet = jet2;
        }
        else {
            leading_jet = jet2;
            trailing_jet = jet1;
        }
    }
    else {
        // Determine if all jets have positive or negative eta
        bool all_eta_gt0 = true;
        bool all_eta_lt0 = true;
        for (unsigned int i = 0; i < vbs_jet_cands.size(); i++) {
            VBSJetCand jet = vbs_jet_cands.at(i);
            if (jet.eta() > 0) {
                all_eta_lt0 = false;
            }
            else if (jet.eta() < 0) {
                all_eta_gt0 = false;
            }
        }
        if (all_eta_gt0 || all_eta_lt0) {
            VBSJetCand largest_P_jet;
            VBSJetCand next_largest_P_jet;
            // Find two jets with largest P
            for (unsigned int i = 0; i < vbs_jet_cands.size(); i++) {
                VBSJetCand jet = vbs_jet_cands.at(i);
                if (jet.p4().P() > largest_P_jet.p4().P()) {
                    next_largest_P_jet = largest_P_jet;
                    largest_P_jet = jet;
                }
                else if (jet.p4().P() > next_largest_P_jet.p4().P()) {
                    next_largest_P_jet = jet;
                }
            }
            // Sort into leading and trailing jets
            if (largest_P_jet.pt() > next_largest_P_jet.pt()) {
                leading_jet = largest_P_jet;
                trailing_jet = next_largest_P_jet;
            }
            else {
                leading_jet = next_largest_P_jet;
                trailing_jet = largest_P_jet;
            }
        }
        else {
            VBSJetCand largest_P_neg_eta_jet;
            VBSJetCand largest_P_pos_eta_jet;
            // Find jet with largest P in each eta region (positive, negative)
            for (unsigned int i = 0; i < vbs_jet_cands.size(); i++) {
                VBSJetCand jet = vbs_jet_cands.at(i);
                if (jet.eta() < 0) {
                    if (jet.p4().P() > largest_P_neg_eta_jet.p4().P()) {
                        largest_P_neg_eta_jet = jet;
                    }
                }
                if (jet.eta() > 0) {
                    if (jet.p4().P() > largest_P_pos_eta_jet.p4().P()) {
                        largest_P_pos_eta_jet = jet;
                    }
                }
            }
            // Sort into leading and trailing jets
            if (largest_P_pos_eta_jet.pt() > largest_P_neg_eta_jet.pt()) {
                leading_jet = largest_P_pos_eta_jet;
                trailing_jet = largest_P_neg_eta_jet;
            }
            else {
                leading_jet = largest_P_neg_eta_jet;
                trailing_jet = largest_P_pos_eta_jet;
            }
        }
    }

    // Event
    num_pvs = PV_npvsGood();
	met = MET_pt();
    ht = 0.; // Set in loop over good jets below
    // Leptons
	leading_lep_id = leading_lep.id();
	leading_lep_pt = leading_lep.pt();
	leading_lep_eta = leading_lep.eta();
	leading_lep_phi = leading_lep.phi();
	trailing_lep_id = trailing_lep.id();
	trailing_lep_pt = trailing_lep.pt();
	trailing_lep_eta = trailing_lep.eta();
	trailing_lep_phi = trailing_lep.phi();
    dilep_mass = (leading_lep.p4()+trailing_lep.p4()).M();
    // Jets
    leading_vbs_jet_mc_origin = leading_jet.mc_origin();
    leading_vbs_jet_P = leading_jet.p4().P();
    leading_vbs_jet_pt = leading_jet.pt();
    leading_vbs_jet_eta = leading_jet.eta();
    leading_vbs_jet_phi = leading_jet.phi();
    trailing_vbs_jet_mc_origin = trailing_jet.mc_origin();
    trailing_vbs_jet_P = trailing_jet.p4().P();
    trailing_vbs_jet_pt = trailing_jet.pt();
    trailing_vbs_jet_eta = trailing_jet.eta();
    trailing_vbs_jet_phi = trailing_jet.phi();
    vbs_dijet_mass = (leading_jet.p4()+trailing_jet.p4()).M();
    for (unsigned int i = 0; i < good_jets.size(); i++) {
        int jet_idx = good_jets.at(i);
        ht += Jet_pt().at(jet_idx);
        jet_is_vbs.push_back(jet_idx == leading_jet.idx() || jet_idx == trailing_jet.idx());
        jet_pt.push_back(Jet_pt().at(jet_idx));
        jet_eta.push_back(Jet_eta().at(jet_idx));
        jet_phi.push_back(Jet_phi().at(jet_idx));
        if (!isData()) { jet_mc_origin.push_back(Jet_partonFlavour().at(jet_idx)); }
        else { jet_mc_origin.push_back(-999); }
        jet_pu_id.push_back(Jet_puId().at(jet_idx));
        jet_pu_id_disc.push_back(Jet_puIdDisc().at(jet_idx));
        jet_n_em_Efrac.push_back(Jet_neEmEF().at(jet_idx));
        jet_n_he_Efrac.push_back(Jet_neHEF().at(jet_idx));
        jet_mu_Efrac.push_back(Jet_muEF().at(jet_idx));
        jet_ch_em_Efrac.push_back(Jet_chEmEF().at(jet_idx));
        jet_ch_he_Efrac.push_back(Jet_chHEF().at(jet_idx));
        jet_ch_pv0_Efrac.push_back(Jet_chFPV0EF().at(jet_idx));
        jet_ch_pv1_Efrac.push_back(Jet_chFPV1EF().at(jet_idx));
        jet_ch_pv2_Efrac.push_back(Jet_chFPV2EF().at(jet_idx));
        jet_ch_pv3_Efrac.push_back(Jet_chFPV3EF().at(jet_idx));
    }
	num_jets = good_jets.size();
    num_btags_tight = num_tagged_b_tight;
	// Weights
	if (!isData()) {
        gen_weight = genWeight();
        mc_weight = 1.;
        if (year() == 2016) {
            mc_weight *= for2016::leptonScaleFactor(leading_lep_id, leading_lep_pt, leading_lep_eta, ht);
            mc_weight *= for2016::leptonScaleFactor(trailing_lep_id, trailing_lep_pt, trailing_lep_eta, ht);
        }
        else if (year() == 2017) {
            mc_weight *= for2017::leptonScaleFactor(leading_lep_id, leading_lep_pt, leading_lep_eta, ht);
            mc_weight *= for2017::leptonScaleFactor(trailing_lep_id, trailing_lep_pt, trailing_lep_eta, ht);
        }
        else if (year() == 2018) {
            mc_weight *= for2018::leptonScaleFactor(leading_lep_id, leading_lep_pt, leading_lep_eta, ht);
            mc_weight *= for2018::leptonScaleFactor(trailing_lep_id, trailing_lep_pt, trailing_lep_eta, ht);
        }
        else {
            throw std::runtime_error("ControlTree::fillBranches: Error - invalid year");
            return;
        }
	}
	fillTTree();	
	return;
}

void ControlTree::trackTH1F(TH1F* new_hist) {
    th1fs.push_back(new_hist);
    return;
}

void ControlTree::fillTTree() {
    ttree->Fill();
	return;
}

void ControlTree::writeTFile() {
    tfile->cd();
    ttree->Write();
    for (unsigned int i = 0; i < th1fs.size(); i++) {
        th1fs.at(i)->Write();
    }
    tfile->Close();
	return;
}

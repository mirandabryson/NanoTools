#include <vector>
#include "./NanoCORE/Nano.h"
#include "./NanoCORE/SSSelections.h"
#include "./NanoCORE/Tools/btagsf/BTagCalibrationStandalone.h"

#ifndef CONTROL_H
#define CONTROL_H

struct VBSJetCand {
    // Constructor
    VBSJetCand(unsigned int idx = 0) : idx_(idx) {
        pt_ = nt.Jet_pt().at(idx);
        eta_ = nt.Jet_eta().at(idx);
        phi_ = nt.Jet_phi().at(idx);
        mass_ = nt.Jet_mass().at(idx);
        p4_ = nt.Jet_p4().at(idx);
        mc_origin_ = -999;
        if (!nt.isData()) {
            mc_origin_ = nt.Jet_partonFlavour().at(idx);
        }
    }
    // Accessors
    LorentzVector p4() { return p4_; }
    float pt() { return pt_; }
    float eta() { return eta_; }
    float phi() { return phi_; }
    float mass() { return mass_; }
    unsigned int idx() { return idx_; }
    int mc_origin() { return mc_origin_; }
    private:
        // Attributes
        LorentzVector p4_;
        float pt_;
        float eta_;
        float phi_;
        float mass_;
        unsigned int idx_;
        int mc_origin_ = -999;
};

class ControlTree {

    // Event
    TBranch* b_num_pvs;
	TBranch* b_met;
	TBranch* b_ht;
    // Leptons
	TBranch* b_leading_lep_id;
	TBranch* b_leading_lep_pt;
	TBranch* b_leading_lep_eta;
	TBranch* b_leading_lep_phi;
	TBranch* b_trailing_lep_id;
	TBranch* b_trailing_lep_pt;
	TBranch* b_trailing_lep_eta;
	TBranch* b_trailing_lep_phi;
	TBranch* b_dilep_mass;
    // Jets
    TBranch* b_leading_vbs_jet_mc_origin;
    TBranch* b_leading_vbs_jet_pt;
    TBranch* b_leading_vbs_jet_eta;
    TBranch* b_leading_vbs_jet_phi;
    TBranch* b_trailing_vbs_jet_mc_origin;
    TBranch* b_trailing_vbs_jet_pt;
    TBranch* b_trailing_vbs_jet_eta;
    TBranch* b_trailing_vbs_jet_phi;
    TBranch* b_vbs_dijet_mass;
    TBranch* b_jet_is_vbs;
    TBranch* b_jet_pt;
    TBranch* b_jet_eta;
    TBranch* b_jet_phi;
    TBranch* b_jet_mc_origin;
    TBranch* b_jet_pu_id;
    TBranch* b_jet_pu_id_disc;
    TBranch* b_jet_n_em_Efrac;
    TBranch* b_jet_n_he_Efrac;
    TBranch* b_jet_mu_Efrac;
    TBranch* b_jet_ch_em_Efrac;
    TBranch* b_jet_ch_he_Efrac;
    TBranch* b_jet_ch_pv0_Efrac;
    TBranch* b_jet_ch_pv1_Efrac;
    TBranch* b_jet_ch_pv2_Efrac;
    TBranch* b_jet_ch_pv3_Efrac;
    TBranch* b_num_jets;
    TBranch* b_num_btags_tight;
    // Weights
	TBranch* b_gen_weight;
	TBranch* b_mc_weight;
	TBranch* b_mc_loose_btag_weight;
	TBranch* b_mc_medium_btag_weight;
	TBranch* b_mc_tight_btag_weight;

	public:
	    // ROOT I/O	
		TTree* ttree;
        TFile* tfile;
        vector<TH1F*> th1fs;
        // Calibrations
        BTagCalibration deepjet_csv;
        BTagCalibrationReader deepjet_loose_reader;
        BTagCalibrationReader deepjet_medium_reader;
        BTagCalibrationReader deepjet_tight_reader;
        // Event
        int num_pvs;
		float met;
		float ht;
        // Leptons
		int leading_lep_id;
		float leading_lep_pt;
		float leading_lep_eta;
		float leading_lep_phi;
		int trailing_lep_id;
		float trailing_lep_pt;
		float trailing_lep_eta;
		float trailing_lep_phi;
		float dilep_mass;
        // Jets
		int leading_vbs_jet_mc_origin;
		float leading_vbs_jet_P;
		float leading_vbs_jet_pt;
		float leading_vbs_jet_eta;
		float leading_vbs_jet_phi;
		int trailing_vbs_jet_mc_origin;
		float trailing_vbs_jet_P;
		float trailing_vbs_jet_pt;
		float trailing_vbs_jet_eta;
		float trailing_vbs_jet_phi;
        float vbs_dijet_mass;
        vector<bool> jet_is_vbs;
        vector<float> jet_pt;
        vector<float> jet_eta;
        vector<float> jet_phi;
        vector<int> jet_mc_origin;
        vector<int> jet_pu_id;
        vector<float> jet_pu_id_disc;
        vector<float> jet_n_em_Efrac;
        vector<float> jet_n_he_Efrac;
        vector<float> jet_mu_Efrac;
        vector<float> jet_ch_em_Efrac;
        vector<float> jet_ch_he_Efrac;
        vector<float> jet_ch_pv0_Efrac;
        vector<float> jet_ch_pv1_Efrac;
        vector<float> jet_ch_pv2_Efrac;
        vector<float> jet_ch_pv3_Efrac;
		int num_jets;
		int num_btags_tight;
        // Weights
		float gen_weight;
		float mc_weight;		
		float mc_loose_btag_weight;		
		float mc_medium_btag_weight;		
		float mc_tight_btag_weight;		
		//Constructor
		ControlTree(TFile* new_tfile);
		
		//Methods
		void resetBranches();
        bool jetLeptonOverlap(int jet_idx, Lepton lep);
		void fillBranches();
        void trackTH1F(TH1F* new_hist);
		void fillTTree();
		void writeTFile();
};

#endif

#ifndef FAKES_H
#define FAKES_H

enum MonolepID {
    no_lepton = -1,
    single_el = 0,
    single_mu = 1
};

class FakesTree {

    /**
     * Object-level (lepton-by-lepton) TTree for storing -fake_ lepton
     * information
     */
    
    /* Initialize branches */
    // Event level
    TBranch* b_event;
    TBranch* b_event_ht;
    TBranch* b_event_met;
    TBranch* b_event_deepCSV_btags_loose;
    TBranch* b_event_deepCSV_btags_medium;
    TBranch* b_event_deepCSV_btags_tight;
    TBranch* b_event_deepJet_btags_loose;
    TBranch* b_event_deepJet_btags_medium;
    TBranch* b_event_deepJet_btags_tight;
    // Object level
    TBranch* b_lepton_id;
    TBranch* b_lepton_pt;
    TBranch* b_lepton_pt_corr;
    TBranch* b_lepton_eta;
    TBranch* b_lepton_phi;
    TBranch* b_lepton_mass;
    TBranch* b_is_tight;
    TBranch* b_is_loose;
    TBranch* b_unmatched;
    TBranch* b_heavy_mother;
    TBranch* b_light_mother;

    public:

        // TTree
        TTree *t;
        // Target file
        TFile* f;

        /* Initialize branch values */
        // Event level
        int event;
        float event_ht;
        float event_met;
        int event_deepCSV_btags_loose;
        int event_deepCSV_btags_medium;
        int event_deepCSV_btags_tight;
        int event_deepJet_btags_loose;
        int event_deepJet_btags_medium;
        int event_deepJet_btags_tight;
        // Object level
        int lepton_id;
        float lepton_pt;
        float lepton_pt_corr;
        float lepton_eta;
        float lepton_phi;
        float lepton_mass;
        bool is_tight;
        bool is_loose;
        bool unmatched;
        bool heavy_mother;
        bool light_mother;
        // Other
        int year;

        /* Methods */
        // Constructor
        FakesTree(int year_init);
        // Reset variables
        void reset(bool obj_level_only);
        // Fillers & cuts
        bool passesNonIsoTrigger(int lepton_pdg_id, int lepton_index);
        int fillEvtBranches();
        int fillBranches();
        // Other
        void saveLepton(int lepton_id, int lepton_index);
        MonolepID whichGenMonolep();

};

#endif

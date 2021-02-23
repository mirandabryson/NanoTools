// -*- C++ -*-

// C++
#include <iostream>
#include <vector>
// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TString.h"
// CMS4
#include "CORE/CMS3.h"
#include "CORE/SSSelections.h"
// Custom
#include "cms4hyps.h"

// Namespaces
using namespace std;
using namespace tas;

CMS4HypsTree::CMS4HypsTree() {
    // TTree
    t = new TTree("tree", "tree");
    // Branches
    b_leader_eta = t->Branch("leader_eta", &leader_eta, "leader_eta/F");
    b_lagger_isTriggerSafe = t->Branch("lagger_isTriggerSafe", &lagger_isTriggerSafe, "lagger_isTriggerSafe/O");
    b_leader_isTriggerSafe = t->Branch("leader_isTriggerSafe", &leader_isTriggerSafe, "leader_isTriggerSafe/O");
    b_lagger_id = t->Branch("lagger_id", &lagger_id, "lagger_id/I");
    b_leader_id = t->Branch("leader_id", &leader_id, "leader_id/I");
    b_lagger_idx = t->Branch("lagger_idx", &lagger_idx, "lagger_idx/i");
    b_leader_idx = t->Branch("leader_idx", &leader_idx, "leader_idx/i");
    b_leader_is_loose = t->Branch("leader_is_loose", &leader_is_loose, "leader_is_loose/O");
    b_lagger_is_loose = t->Branch("lagger_is_loose", &lagger_is_loose, "lagger_is_loose/O");
    b_leader_is_tight = t->Branch("leader_is_tight", &leader_is_tight, "leader_is_tight/O");
    b_lagger_is_tight = t->Branch("lagger_is_tight", &lagger_is_tight, "lagger_is_tight/O");
    b_lagger_eta = t->Branch("lagger_eta", &lagger_eta, "lagger_eta/F");
    b_hyp_class = t->Branch("hyp_class", &hyp_class, "hyp_class/I");
    b_leader_pt = t->Branch("leader_pt", &leader_pt, "leader_pt/F");
    b_lagger_pt = t->Branch("lagger_pt", &lagger_pt, "lagger_pt/F");
    b_is_dilepton_event = t->Branch("is_dilepton_event", &is_dilepton_event, "is_dilepton_event/O");
    b_event = t->Branch("event", &event, "event/I");
    b_run = t->Branch("run", &run, "run/I");
    b_lumi = t->Branch("lumi", &lumi, "lumi/F");

}

void CMS4HypsTree::reset() {
    // Reset branch values
    leader_eta = -999;
    lagger_isTriggerSafe = false;
    leader_isTriggerSafe = false;
    lagger_id = -999;
    leader_id = -999;
    lagger_idx = 999;
    leader_idx = 999;
    leader_is_loose = false;
    lagger_is_loose = false;
    leader_is_tight = false;
    lagger_is_tight = false;
    lagger_eta = -999;
    hyp_class = -999;
    leader_pt = -999;
    lagger_pt = -999;
    is_dilepton_event = false;
    event = -999;
    run = -999;
    lumi = -999;

    return;
}

int CMS4HypsTree::fillBranches() {
    if (hyp_type().size() < 1) {
        is_dilepton_event = false;
    }
    else {
        // bool verbose = (event == 134649108 || event == 209208961 || event == 209212074);
        bool verbose = false;
        if (verbose) cout << "Event: " << event << endl;
        hyp_result_t best_hyp_info = chooseBestHyp(verbose);
        hyp_class = best_hyp_info.hyp_class;
        int best_hyp_index = best_hyp_info.best_hyp;
        is_dilepton_event = (hyp_class > 0);
        if (is_dilepton_event) {
            if (hyp_class == 6) {
                auto z_result = makesExtraZ(best_hyp_index);
                if (!isGoodLepton(hyp_ll_id().at(best_hyp_index), hyp_ll_index().at(best_hyp_index))) {
                    is_dilepton_event = false;
                    hyp_class = -1;
                    return 0;
                }
                if (!isGoodLepton(hyp_lt_id().at(best_hyp_index), hyp_lt_index().at(best_hyp_index))) {
                    is_dilepton_event = false;
                    hyp_class = -1;
                    return 0;
                }
                if (!isGoodLepton(z_result.id, z_result.idx)) {
                    is_dilepton_event = false;
                    hyp_class = -1;
                    return 0;
                }
            }
            auto leader_p4 = hyp_lt_p4().at(best_hyp_index);
            auto lagger_p4 = hyp_ll_p4().at(best_hyp_index);
            leader_id = hyp_lt_id().at(best_hyp_index);
            lagger_id = hyp_ll_id().at(best_hyp_index);
            leader_idx = hyp_lt_index().at(best_hyp_index);
            lagger_idx = hyp_ll_index().at(best_hyp_index);
            if (abs(leader_id) == 11) {
                leader_isTriggerSafe = isTriggerSafenoIso_v1(leader_idx);
            }
            if (abs(lagger_id) == 11) {
                lagger_isTriggerSafe = isTriggerSafenoIso_v1(lagger_idx);
            }
            leader_is_loose = isDenominatorLepton(leader_id, leader_idx);
            lagger_is_loose = isDenominatorLepton(lagger_id, lagger_idx);
            leader_is_tight = isGoodLepton(leader_id, leader_idx);
            lagger_is_tight = isGoodLepton(lagger_id, lagger_idx);
            leader_pt = leader_p4.pt();
            lagger_pt = lagger_p4.pt();
            leader_eta = leader_p4.eta();
            lagger_eta = lagger_p4.eta();
        }
    }
    return 0;
}
    

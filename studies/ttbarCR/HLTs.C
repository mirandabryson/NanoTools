#include "TString.h"
#include "./NanoCORE/Nano.h"
#include "./NanoCORE/Config.h"

using namespace tas;
using namespace std;

enum HLTSet {
    MuonElec_HLTs = 0,
    DoubleMuon_HLTs = 1,
    DoubleElec_HLTs = 2
};

bool checkHLTs(HLTSet HLT_set) {
    // e-mu HLTs
    bool HLT_mu23_el12_noDZ;
    try {
        HLT_mu23_el12_noDZ = HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL();
    }
    catch (runtime_error) {
        HLT_mu23_el12_noDZ = false;
    }
    bool HLT_mu8_el23_noDZ;
    try {
        HLT_mu8_el23_noDZ = HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL();
    }
    catch (runtime_error) {
        HLT_mu8_el23_noDZ = false;
    }
    bool HLT_mu23_el12_DZ;
    try {
        HLT_mu23_el12_DZ = HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ();
    }
    catch (runtime_error) {
        HLT_mu23_el12_DZ = false;
    }
    bool HLT_mu8_el23_DZ;
    try {
        HLT_mu8_el23_DZ = HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ();
    }
    catch (runtime_error) {
        HLT_mu8_el23_DZ = false;
    }
    // Select set of HLTs based on year using year in global config
    bool passes_HLTs = false;
    if (year() == 2016) {
        if (HLT_set == MuonElec_HLTs) {
            passes_HLTs = (HLT_mu23_el12_noDZ 
                           || HLT_mu8_el23_noDZ 
                           || HLT_mu23_el12_DZ 
                           || HLT_mu8_el23_DZ);
        }
    }
    else if (year() == 2017) {
        if (HLT_set == MuonElec_HLTs) {
            passes_HLTs = (HLT_mu23_el12_DZ || HLT_mu8_el23_DZ);
        }
    }
    else if (year() == 2018) {
        if (HLT_set == MuonElec_HLTs) {
            passes_HLTs = (HLT_mu23_el12_DZ || HLT_mu8_el23_DZ);
        }
    }
    else {
        throw std::runtime_error("HLTs.C::checkHLTs: Error - Invalid year");
    }
    return passes_HLTs;
}

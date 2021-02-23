#include "./NanoCORE/Nano.h"
#include "./NanoCORE/IsolationTools.h"
#include "./NanoCORE/ElectronSelections.h"

using namespace std;
using namespace tas;

// 2016
Counter isGoodElectronNano(unsigned int idx) {
    Counter c;
    // CMS4: if (els_p4().at(idx).pt() < 10.) return false;
    c.newCut("pt > 10");
    if (Electron_pt().at(idx)/Electron_eCorr().at(idx) < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (!isTriggerSafenoIso_v1(elIdx)) return false;
    c.newCut("isTriggerSafe_noIso");
    if (!isTriggerSafeNoIso(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (els_exp_innerlayers().at(elIdx) > 1) return false;
    c.newCut("lostHits == 0");
    if (int(Electron_lostHits().at(idx) > 0)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (globalEleMVAreader->passesElectronMVAid(elIdx, SS_veto_noiso_v5) == 0) return false;
    c.newCut("MVA(2016_veto_noIso_v5)");
    if (!passesElectronMVA(idx, veto_noIso_2016, 2016)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
    c.newCut("dxy < 0.05");
    if (fabs(Electron_dxy().at(idx)) >= 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (globalEleMVAreader->passesElectronMVAid(elIdx, SS_fo_noiso_v5) == 0) return false;
    c.newCut("MVA(2016_fo_noIso_v5)");
    if (!passesElectronMVA(idx, fakable_noIso_2016, 2016)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
    c.newCut("etaSC <= 2.5");
    if (fabs(Electron_eta().at(idx) + Electron_deltaEtaSC().at(idx)) > 2.5) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (els_conv_vtx_flag().at(elIdx)) return false;
    c.newCut("convVeto == true");
    if (!Electron_convVeto().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (threeChargeAgree(elIdx)==0) return false;
    c.newCut("tightCharge != 0 && tightCharge != 1");
    if (Electron_tightCharge().at(idx) == 0 || Electron_tightCharge().at(idx) == 1) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
    c.newCut("|dz| < 0.01");
    if (fabs(Electron_dz().at(idx)) >= 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
    c.newCut("|sip3d| < 4");
    if (fabs(Electron_sip3d().at(idx)) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (globalEleMVAreader->passesElectronMVAid(elIdx, SS_medium_noiso_v5) == 0) return false;
    c.newCut("MVA(2016_medium_noIso_v5)");
    if (!passesElectronMVA(idx, medium_noIso_2016, 2016)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: return passMultiIso(11, elIdx, gconf.multiiso_el_minireliso, gconf.multiiso_el_ptratio, gconf.multiiso_el_ptrel, gconf.ea_version, 2);
    c.newCut("passes iso");
    if (!passesLeptonIso(idx, 11, 0.12, 0.80, 7.2)) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

Counter isFakableElectronNano(unsigned int idx) {
    Counter c;
    // CMS4: if (els_p4().at(idx).pt() < 10.) return false;
    c.newCut("pt < 10");
    if (Electron_pt().at(idx)/Electron_eCorr().at(idx) < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (!isTriggerSafenoIso_v1(elIdx)) return false;
    c.newCut("isTriggerSafe_noIso");
    if (!isTriggerSafeNoIso(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (globalEleMVAreader->passesElectronMVAid(elIdx, SS_veto_noiso_v5) == 0) return false;
    c.newCut("MVA(2016_veto_noIso_v5)");
    if (!passesElectronMVA(idx, veto_noIso_2016, 2016)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
    c.newCut("etaSC <= 2.5");
    if (fabs(Electron_eta().at(idx) + Electron_deltaEtaSC().at(idx)) > 2.5) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (els_conv_vtx_flag().at(elIdx)) return false;
    c.newCut("convVeto == true");
    if (!Electron_convVeto().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (els_exp_innerlayers().at(elIdx) > 1) return false;
    c.newCut("lostHits == 0");
    if (int(Electron_lostHits().at(idx) > 0)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (threeChargeAgree(elIdx)==0) return false;
    c.newCut("tightCharge != 0 && tightCharge != 1");
    if (Electron_tightCharge().at(idx) == 0 || Electron_tightCharge().at(idx) == 1) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
    c.newCut("dxy < 0.05");
    if (fabs(Electron_dxy().at(idx)) >= 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
    c.newCut("|sip3d| < 4");
    if (fabs(Electron_sip3d().at(idx)) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
    c.newCut("|dz| < 0.01");
    if (fabs(Electron_dz().at(idx)) >= 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: if (globalEleMVAreader->passesElectronMVAid(idx, SS_fo_looseMVA_noiso_v5) == 0) return false;
    c.newCut("MVA(2016_fo_looseMVA_noIso_v5)");
    if (!passesElectronMVA(idx, fakable_noIso_looseMVA_2016, 2016)) {
        c.fail();
    }
    else {
        c.pass();
    }
    // CMS4: return (elMiniRelIsoCMS3_EA(idx, gconf.ea_version) >= 0.40);
    c.newCut("passes rel iso");
    if (Electron_miniPFRelIso_all().at(idx) >= 0.40) { 
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

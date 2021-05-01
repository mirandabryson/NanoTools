#include "./CORE/CMS3.h"
#include "./CORE/ElectronSelections.h"
#include "./CORE/IsolationTools.h"

using namespace std;
using namespace tas;

// 2016
Counter isGoodElectronCMS4(unsigned int idx) {
    Counter c;
    c.newCut("pt > 10");
    if (els_p4().at(idx).pt() < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("isTriggerSafe_noIso");
    if (!isTriggerSafenoIso_v1(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("lostHits == 0");
    if (els_exp_innerlayers().at(idx) > 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("MVA(2016_veto_noIso_v5)");
    if (globalEleMVAreader->passesElectronMVAid(idx, SS_veto_noiso_v5) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("dxy < 0.05");
    if (fabs(els_dxyPV().at(idx)) >= 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("MVA(2016_fo_noIso_v5)");
    if (globalEleMVAreader->passesElectronMVAid(idx, SS_fo_noiso_v5) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("etaSC <= 2.5");
    if (fabs(els_etaSC().at(idx)) > 2.5) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("convVeto == true");
    if (els_conv_vtx_flag().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("tightCharge != 0 && tightCharge != 1");
    if (threeChargeAgree(idx) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dz| < 0.01");
    if (fabs(els_dzPV().at(idx)) >= 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(els_ip3d().at(idx))/els_ip3derr().at(idx) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("MVA(2016_medium_noIso_v5)");
    if (globalEleMVAreader->passesElectronMVAid(idx, SS_medium_noiso_v5) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes iso");
    if (!passMultiIso(11, idx, gconf.multiiso_el_minireliso, gconf.multiiso_el_ptratio, gconf.multiiso_el_ptrel, gconf.ea_version, 2)) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

Counter isFakableElectronCMS4(unsigned int idx) {
    Counter c;
    c.newCut("pt > 10");
    if (els_p4().at(idx).pt() < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("isTriggerSafe_noIso");
    if (!isTriggerSafenoIso_v1(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("MVA(SS_veto_noiso_v5)");
    if (globalEleMVAreader->passesElectronMVAid(idx, SS_veto_noiso_v5) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("etaSC <= 2.5");
    if (fabs(els_etaSC().at(idx)) > 2.5) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("convVeto == true");
    if (els_conv_vtx_flag().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("lostHits == 0");
    if (els_exp_innerlayers().at(idx) > 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("tightCharge != 0 && tightCharge != 1");
    if (threeChargeAgree(idx)==0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("dxy < 0.05");
    if (fabs(els_dxyPV().at(idx)) >= 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(els_ip3d().at(idx))/els_ip3derr().at(idx) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("dz < 0.1");
    if (fabs(els_dzPV().at(idx)) >= 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("MVA(SS_fo_looseMVA_noiso_v5)");
    if (globalEleMVAreader->passesElectronMVAid(idx, SS_fo_looseMVA_noiso_v5) == 0) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes rel iso");
    if (elMiniRelIsoCMS3_EA(idx, gconf.ea_version) >= 0.40) { 
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

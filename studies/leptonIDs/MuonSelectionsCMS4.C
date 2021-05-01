#include "./CORE/CMS3.h"
#include "./CORE/MuonSelections.h"
#include "./CORE/IsolationTools.h"

using namespace std;
using namespace tas;

// 2016
Counter isGoodMuonCMS4(unsigned int idx) {
    Counter c;
    c.newCut("pt < 10");
    if (mus_p4().at(idx).pt() < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|eta| <= 2.4");
    if (fabs(mus_p4().at(idx).eta()) > 2.4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dxy| <= 0.05");
    if (fabs(mus_dxyPV().at(idx)) > 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dz| <= 0.1");
    if (fabs(mus_dzPV().at(idx)) > 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes loose POG");
    if (!isLooseMuonPOG(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(mus_ip3d().at(idx))/mus_ip3derr().at(idx) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pt_err/pt < 0.2");
    if (mus_ptErr().at(idx)/mus_trk_p4().at(idx).pt() >= 0.2) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes medium POG");
    if (!isMediumMuonPOG(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pass iso");
    if (!passMultiIso(13, idx, gconf.multiiso_mu_minireliso, gconf.multiiso_mu_ptratio, gconf.multiiso_mu_ptrel, gconf.ea_version, 2)) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

Counter isFakableMuonCMS4(unsigned int idx) {
    Counter c;
    c.newCut("pt < 10");
    if (mus_p4().at(idx).pt() < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|eta| <= 2.4");
    if (fabs(mus_p4().at(idx).eta()) > 2.4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dxy| <= 0.05");
    if (fabs(mus_dxyPV().at(idx)) > 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dz| <= 0.1");
    if (fabs(mus_dzPV().at(idx)) > 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes loose POG");
    if (!isLooseMuonPOG(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(mus_ip3d().at(idx))/mus_ip3derr().at(idx) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pt_err/pt < 0.2");
    if (mus_ptErr().at(idx)/mus_trk_p4().at(idx).pt() >= 0.2) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pass medium POG");
    if (!isMediumMuonPOG(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pass rel iso");
    if (muMiniRelIsoCMS3_EA(idx, gconf.ea_version) > 0.40) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

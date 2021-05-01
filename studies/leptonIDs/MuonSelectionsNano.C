#include "./NanoCORE/Nano.h"
#include "./NanoCORE/IsolationTools.h"
#include "./NanoCORE/MuonSelections.h"

using namespace std;
using namespace tas;

// 2016
Counter isGoodMuonNano(unsigned int idx) {
    Counter c;
    c.newCut("pt < 10");
    if (Muon_pt().at(idx) < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|eta| <= 2.4");
    if (fabs(Muon_eta().at(idx)) > 2.4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dxy| <= 0.05");
    if (fabs(Muon_dxy().at(idx)) > 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dz| <= 0.1");
    if (fabs(Muon_dz().at(idx)) > 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes loose POG");
    if (!Muon_looseId().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(Muon_sip3d().at(idx)) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pt_err/pt < 0.2");
    if (Muon_ptErr().at(idx)/Muon_pt().at(idx) >= 0.2) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes medium POG");
    if (!Muon_mediumId().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pass iso");
    if (!passesLeptonIso(idx, 13, 0.16, 0.76, 7.2)) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

Counter isFakableMuonNano(unsigned int idx) {
    Counter c;
    c.newCut("pt < 10");
    if (Muon_pt().at(idx) < 10.) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|eta| <= 2.4");
    if (fabs(Muon_eta().at(idx)) > 2.4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dxy| <= 0.05");
    if (fabs(Muon_dxy().at(idx)) > 0.05) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|dz| <= 0.1");
    if (fabs(Muon_dz().at(idx)) > 0.1) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes loose POG");
    if (!Muon_looseId().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("|sip3d| < 4");
    if (fabs(Muon_sip3d().at(idx)) >= 4) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pt_err/pt < 0.2");
    if (Muon_ptErr().at(idx)/Muon_pt().at(idx) >= 0.2) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("passes medium POG");
    if (!Muon_mediumId().at(idx)) {
        c.fail();
    }
    else {
        c.pass();
    }
    c.newCut("pass rel iso");
    if (Muon_miniPFRelIso_all().at(idx) > 0.40) {
        c.fail();
    }
    else {
        c.pass();
    }

    return c;
}

#include "SSSelections.h"
#include "Config.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "Math/VectorUtil.h"

using namespace tas;

bool lepsort (Lepton i,Lepton j) {return ( i.pt()>j.pt() );}
bool jetptsort (Jet i,Jet j) { return (i.pt()>j.pt()); }

Leptons getLeptons() {
    Leptons leptons;
    auto mupts = Muon_pt();
    for (unsigned int imu = 0; imu < mupts.size(); imu++) {
        if (mupts[imu] < 5) { continue; }
        leptons.push_back(Lepton(Muon_pdgId()[imu], imu));
    }
    auto epts = Electron_pt();
    for (unsigned int iel = 0; iel < epts.size(); iel++) {
        if (epts[iel] < 7) { continue; }
        leptons.push_back(Lepton(Electron_pdgId()[iel], iel));
    }
    return leptons;
}

// check if lepton passes pt,eat cuts
bool pass_lep_pt_eta(Lepton &lep) {
    float minpt = lep.is_el() ? 25. : 20.;
    float maxeta = lep.is_el() ? 2.5 : 2.4;
    if (lep.pt() < minpt || fabs(lep.eta()) > maxeta) return false;
    return true;
}

std::tuple<int, int, float> getJetInfo(Leptons &leps, int variation) {
    int njets = 0;
    float ht = 0;
    int nbtags = 0;
    auto jetpts = Jet_pt();
    vector<float> discs = Jet_btagDeepFlavB();
    for (unsigned int ijet = 0; ijet < jetpts.size(); ijet += 1) {
        float pt = jetpts[ijet];
        if (!(Jet_jetId()[ijet] & (1 << 1))) { continue; }
        // Clean against up to 2 fakable electrons
        if (Jet_electronIdx1()[ijet] >= 0) {
            bool skip = false;
            for (auto &lep : leps) {
                if (lep.is_el() && (Jet_electronIdx1()[ijet] == (int)(lep.idx())) && lep.idlevel() >= SS::IDfakable) {
                    skip = true;
                    break;
                }
                if (skip) { break; }
                if (Jet_electronIdx2()[ijet] >= 0) {
                    if (lep.is_el() && (Jet_electronIdx2()[ijet] == (int)(lep.idx())) && lep.idlevel() >= SS::IDfakable) {
                        skip = true;
                        break;
                    }
                    if (skip) { break; }
                }
            }
            if (skip) { continue; }
        }
        // Clean against up to 2 fakable muons
        if (Jet_muonIdx1()[ijet] >= 0) {
            bool skip = false;
            for (auto &lep : leps) {
                if (lep.is_mu() && (Jet_muonIdx1()[ijet] == (int)(lep.idx())) && lep.idlevel() >= SS::IDfakable) {
                    skip = true;
                    break;
                }
                if (skip) { break; }
                if (Jet_muonIdx2()[ijet] >= 0) {
                    if (lep.is_mu() && (Jet_muonIdx2()[ijet] == (int)(lep.idx())) && lep.idlevel() >= SS::IDfakable) {
                        skip = true;
                        break;
                    }
                    if (skip) { break; }
                }
            }
            if (skip) { continue; }
        }
        if (fabs(Jet_eta()[ijet]) > 2.4) { continue; }
        if (Jet_jetId()[ijet] <= 1) { continue; }
        if (pt > 25. && discs[ijet] > 0.2770) { nbtags += 1; }
        if (pt < 40) { continue; }
        ht += pt;
        njets++;
    }
    return std::make_tuple(njets, nbtags, ht);
}

std::tuple<int, int, float> getCleanJetInfo(Leptons &leps, int variation) {
    float btagCut = 0;
    if(nt.year()==2016){ btagCut = 0.3093; }
    else if (nt.year()==2017){ btagCut = 0.3033; }
    else if (nt.year()==2018){ btagCut = 0.2770; }
    int njets = 0;
    float ht = 0;
    int nbtags = 0;
    auto jetpts = Jet_pt();
    vector<float> discs = Jet_btagDeepFlavB();
    vector<int> jetsToClean;
    for (unsigned int ijet = 0; ijet < jetpts.size(); ijet += 1) {
        float pt = jetpts[ijet];
        if (!(Jet_jetId()[ijet] & (1 << 1))) { continue; }
        if (fabs(Jet_eta()[ijet]) > 2.4) { continue; }
        if ((pt > 25. && discs[ijet] > btagCut)||(pt > 40)) { jetsToClean.push_back(ijet); }
    }
    //once we've saved the indices of jets that pass kinematic requirements, time to clean them
    for (auto &lep : leps) {
        float mindr = 0.41;
        int minidx = -1;
        //find jet closest to lepton if within deltaR of 0.4
        for (unsigned int j = 0; j < jetsToClean.size(); j += 1){
            int iGoodJet = jetsToClean[j];
            float dr = ROOT::Math::VectorUtil::DeltaR(lep.p4(),Jet_p4()[iGoodJet]);
            if (dr < mindr){
                mindr = dr;
                minidx = j;
            }
        }
        if (minidx>=0) { jetsToClean.erase( jetsToClean.begin()+minidx ); }
    }
    //ok, now jetsToClean no longer contains jets that are closest to a lepton if deltaR < 0.4
    //now we can use this information to count our btags and jets
    for (unsigned int j = 0; j < jetsToClean.size(); j += 1){
        int iJet = jetsToClean[j];
        float pt = jetpts[iJet];
        float bdisc = discs[iJet];
        if (pt > 25. && bdisc > btagCut) { nbtags++; }
        if (pt < 40) { continue; }
        ht += pt;
        njets++;
    }
    return std::make_tuple(njets, nbtags, ht);
}

std::pair<int, int> makesResonance(Leptons &leps, Lepton lep1, Lepton lep2, float mass, float window) {
    // return {which lepton (1,2), and index of resonance partner}
    for (auto &lep : leps) {
        if (lep.is_el()) {
            if (!(lep1.is_el() || lep2.is_el())) { continue; }
            if ((lep.idx() == lep1.idx() && lep1.is_el()) || (lep.idx() == lep2.idx() && lep2.is_el())) { continue; }
            if (fabs(lep.eta()) > 2.4) { continue; }
            if (lep.pt() < 7) { continue; }
            if (lep.idlevel() < SS::IDveto) { continue; }
            if (lep1.is_el() && (lep1.id() * lep.id() < 0) && (fabs((lep1.p4() + lep.p4()).M() - mass) < window)) {
                return {1, lep.idx()};
            }
            if (lep2.is_el() && (lep2.id() * lep.id() < 0) && (fabs((lep2.p4() + lep.p4()).M() - mass) < window)) {
                return {2, lep.idx()};
            }
        } else {
            if (!(lep1.is_mu() || lep2.is_mu())) { continue; }
            if ((lep.idx() == lep1.idx() && lep1.is_mu()) || (lep.idx() == lep2.idx() && lep2.is_mu())) { continue; }
            if (fabs(lep.eta()) > 2.4) { continue; }
            if (lep.pt() < 5) { continue; }
            if (lep.idlevel() < SS::IDveto) { continue; }
            if (lep1.is_mu() && (lep1.id() * lep.id() < 0) && (fabs((lep1.p4() + lep.p4()).M() - mass) < window)) {
                return {1, lep.idx()};
            }
            if (lep2.is_mu() && (lep2.id() * lep.id() < 0) && (fabs((lep2.p4() + lep.p4()).M() - mass) < window)) {
                return {2, lep.idx()};
            }
        }
    }
    for (unsigned int iel = 0; iel < Electron_pt().size(); iel++) {
        if ((iel == lep1.idx() && lep1.is_el()) || (iel == lep2.idx() && lep2.is_el())) continue;
        if (fabs(Electron_eta()[iel]) > 2.4) continue;
        if (fabs(Electron_pt()[iel]) < 7) continue;
        if (!SS::electronID(iel, SS::IDveto, year())) continue;
        if (lep1.is_el() && (lep1.id() * Electron_pdgId()[iel] < 0) &&
            (fabs((lep1.p4() + Electron_p4()[iel]).M() - mass) < window)) {
            return {1, iel};
        }
        if (lep2.is_el() && (lep2.id() * Electron_pdgId()[iel] < 0) &&
            (fabs((lep2.p4() + Electron_p4()[iel]).M() - mass) < window)) {
            return {2, iel};
        }
    }
    for (unsigned int imu = 0; imu < Muon_pt().size(); imu++) {
        if ((imu == lep1.idx() && lep1.is_mu()) || (imu == lep2.idx() && lep2.is_mu())) continue;
        if (fabs(Muon_eta()[imu]) > 2.4) continue;
        if (fabs(Muon_pt()[imu]) < 5) continue;
        if (!SS::muonID(imu, SS::IDveto, year())) continue;
        if (lep1.is_mu() && (lep1.id() * Muon_pdgId()[imu] < 0) &&
            (fabs((lep1.p4() + Muon_p4()[imu]).M() - mass) < window)) {
            return {1, imu};
        }
        if (lep2.is_mu() && (lep2.id() * Muon_pdgId()[imu] < 0) &&
            (fabs((lep2.p4() + Muon_p4()[imu]).M() - mass) < window)) {
            return {2, imu};
        }
    }
    return {-1, -1};
}

std::pair<int, Hyp> getBestHyp(Leptons &leptons, bool verbose) {
    int hyp_class = -1;
    Hyp best_hyp;
    if (leptons.size() < 2) return {hyp_class, best_hyp};
    vector<Hyp> hyp1s;
    vector<Hyp> hyp2s;
    vector<Hyp> hyp3s;
    vector<Hyp> hyp4s;
    vector<Hyp> hyp6s;
    for (unsigned int i = 0; i < leptons.size(); i++) {
        for (unsigned int j = i + 1; j < leptons.size(); j++) {
            // DEBUG
            auto &lep1 = leptons[i];
            auto &lep2 = leptons[j];
            if (verbose) {
                cout << "I can read the gconf year. It's " << year() << " ok?? Get off my back." << endl;
                bool DEBUG_isss = lep1.charge() == lep2.charge();
                auto DEBUG_z_result = makesResonance(leptons, lep1, lep2, 91., 15.);
                auto DEBUG_gammastar_result = makesResonance(leptons, lep1, lep2, 0., 12.);
                bool DEBUG_extraZ = DEBUG_z_result.first >= 0;
                bool DEBUG_extraGammaStar = DEBUG_gammastar_result.first >= 0;
                cout << "hyp " << i << " leptons: " << lep2.id() << " " << lep2.pt() << " (idx: " << lep2.idx() << ") "
                     << lep1.id() << " " << lep1.pt() << " (idx: " << lep1.idx() << ")" << endl;
                cout << "   isss: " << DEBUG_isss << endl;
                cout << "   extraZ: " << DEBUG_extraZ << endl;
                cout << "   extraG: " << DEBUG_extraGammaStar << endl;
                cout << "   invt mass: " << (lep2.p4() + lep1.p4()).M() << endl;
                cout << "   passes eta: "
                     << ((lep2.is_el() ? fabs(lep2.eta()) < 2.5 : fabs(lep2.eta()) < 2.4) &&
                         (lep1.is_el() ? fabs(lep1.eta()) < 2.5 : fabs(lep1.eta()) < 2.4))
                     << " etas are " << lep2.eta() << " and " << lep1.eta() << endl;
                cout << "   passes hypsFromFirstGoodVertex: "
                     << "missing?" << endl;
                cout << "   lepton with pT " << lep2.pt() << " passes numer,denom id: " << (lep2.idlevel() == SS::IDtight)
                     << "," << (lep2.idlevel() >= SS::IDfakable) << endl;
                cout << "   lepton with pT " << lep1.pt() << " passes numer,denom id: " << (lep1.idlevel() == SS::IDtight)
                     << "," << (lep1.idlevel() >= SS::IDfakable) << endl;
                cout << "   lowMassVeto: " << ((lep2.p4() + lep1.p4()).M() < 8) << endl;
            }
            if (lep1.idlevel() < SS::IDfakable || lep2.idlevel() < SS::IDfakable) { continue; }
            bool isss = lep1.charge() == lep2.charge();
            int ntight = (lep1.idlevel() == SS::IDtight) + (lep2.idlevel() == SS::IDtight);
            int nloose = (lep1.idlevel() == SS::IDfakable) + (lep2.idlevel() == SS::IDfakable);
            if (lep1.is_el()) {
                if (lep1.pt() < 15. || fabs(lep1.eta()) > 2.5) { continue; }
            } else {
                if (lep1.pt() < 10. || fabs(lep1.eta()) > 2.4) { continue; }
            }
            if (lep2.is_el()) {
                if (lep2.pt() < 15. || fabs(lep2.eta()) > 2.5) { continue; }
            } else {
                if (lep2.pt() < 10. || fabs(lep2.eta()) > 2.4) { continue; }
            }
            // Veto SS ee or any OSSF, with mll < 12
            if ((isss && lep1.is_el() && lep2.is_el()) || (lep1.id() == -lep2.id())) {
                if ((lep1.p4() + lep2.p4()).M() < 12.) { continue; }
            }
            auto z_result = makesResonance(leptons, lep1, lep2, 91., 15.);
            auto gammastar_result = makesResonance(leptons, lep1, lep2, 0., 12.);
            bool extraZ = z_result.first >= 0;
            bool extraGammaStar = gammastar_result.first >= 0;
            int DEBUG_hyp_class = -1;
            if ((extraZ || extraGammaStar) && isss) {
                DEBUG_hyp_class = 6;
                if (lep1.pt() > lep2.pt())
                    hyp6s.push_back({lep1, lep2});
                else
                    hyp6s.push_back({lep2, lep1});
            } else if (ntight == 2 && isss) {
                DEBUG_hyp_class = 3;
                if (lep1.pt() > lep2.pt())
                    hyp3s.push_back({lep1, lep2});
                else
                    hyp3s.push_back({lep2, lep1});
            } else if (nloose == 2 && isss) {
                DEBUG_hyp_class = 1;
                if (lep1.pt() > lep2.pt())
                    hyp1s.push_back({lep1, lep2});
                else
                    hyp1s.push_back({lep2, lep1});
            } else if (ntight == 1 && nloose == 1 && isss) {
                DEBUG_hyp_class = 2;
                if (lep1.pt() > lep2.pt())
                    hyp2s.push_back({lep1, lep2});
                else
                    hyp2s.push_back({lep2, lep1});
            } else if (ntight == 2 && !isss) {
                DEBUG_hyp_class = 4;
                if (lep1.pt() > lep2.pt())
                    hyp4s.push_back({lep1, lep2});
                else
                    hyp4s.push_back({lep2, lep1});
            }
            if (verbose) cout << "hyp #" << i << " hyp_class: " << DEBUG_hyp_class << endl;
        }
    }
    vector<Hyp> hyps;
    // Priority order - 3,6,2,1,4
    if (hyp3s.size() > 0) {
        hyps = hyp3s;
        hyp_class = 3;
    } else if (hyp6s.size() > 0) {
        hyps = hyp6s;
        hyp_class = 6;
    } else if (hyp2s.size() > 0) {
        hyps = hyp2s;
        hyp_class = 2;
    } else if (hyp1s.size() > 0) {
        hyps = hyp1s;
        hyp_class = 1;
    } else if (hyp4s.size() > 0) {
        hyps = hyp4s;
        hyp_class = 4;
    }
    if ((hyp_class <= 0) || (hyps.size() < 1)) return {hyp_class, best_hyp};
    if (hyps.size() == 1) {
        best_hyp = hyps[0];
    } else if (hyps.size() > 1) {
        best_hyp = hyps[0];
        for (unsigned int i = 1; i < hyps.size(); i++) {
            Hyp hyp = hyps[i];
            if (hyp.first.is_mu() + hyp.second.is_mu() > best_hyp.first.is_mu() + best_hyp.second.is_mu()) {
                best_hyp = hyp;
            } else if (hyp.first.is_mu() + hyp.second.is_mu() == best_hyp.first.is_mu() + best_hyp.second.is_mu()) {
                if (hyp.first.pt() + hyp.second.pt() > best_hyp.first.pt() + best_hyp.second.pt()) { best_hyp = hyp; }
            }
        }
    }
    return {hyp_class, best_hyp};
}

void dumpLeptonProperties(Lepton lep) {
    std::cout << lep << std::endl;
    int i = lep.idx();
    if (lep.is_el()) {
        std::cout << "  -- etaSC: " << Electron_eta()[i] + Electron_deltaEtaSC()[i] << std::endl;
        std::cout << "  -- mva: " << Electron_mvaFall17V1noIso()[i] << std::endl;
        std::cout << "  -- lostHits: " << Electron_lostHits()[i] << std::endl;
        std::cout << "  -- miniRelIso: " << Electron_miniPFRelIso_all()[i] << std::endl;
        std::cout << "  -- ptRatio: " << 1/(Electron_jetRelIso()[i] + 1) << std::endl;
        std::cout << "  -- ptRel: " << Electron_jetPtRelv2()[i] << std::endl;
    } else {
        std::cout << "  -- miniRelIso: " << Muon_miniPFRelIso_all()[i] << std::endl;
        std::cout << "  -- ptRatio: " << 1/(Muon_jetRelIso()[i] + 1) << std::endl;
        std::cout << "  -- ptRel: " << Muon_jetPtRelv2()[i] << std::endl;
    }
}

bool isLeptonLevel(SS::IDLevel idlevel, int id, int idx) {
    if (abs(id) == 11) {
        return SS::electronID(idx, idlevel, year());
    } else if (abs(id) == 13) {
        return SS::muonID(idx, idlevel, year());
    } else {
        return false;
    }
}

SS::IDLevel whichLeptonLevel(int id, int idx) {
    if (isLeptonLevel(SS::IDveto, id, idx)) {
        if (isLeptonLevel(SS::IDfakable, id, idx)) {
            if (isLeptonLevel(SS::IDtight, id, idx)) {
                return SS::IDtight;
            } else {
                return SS::IDfakable;
            }
        } else {
            return SS::IDveto;
        }
    } else {
        return SS::IDdefault;
    }
}

std::vector<bool> cleanJets(Jets &jets, Leptons &leps) {
        std::vector<bool> ret;
        for (unsigned int idx=0; idx<jets.size();idx++) {ret.push_back(1);}
        for (auto lep : leps) {
            float mindr = 0.41;
            int minidx=-1;
            for (unsigned int idx=0; idx<jets.size(); idx++) {
                float dr = ROOT::Math::VectorUtil::DeltaR(lep.p4(),jets[idx].p4());
                if (dr < mindr) {
                    mindr = dr;
                    minidx = idx;
                }
            }
            if (minidx>=0) {ret[minidx]=0;}
        }
        return ret;
}

std::vector<bool> cleanJets(Leptons &leps) {
        std::vector<bool> ret;
        Jets jets;
        for (unsigned int idx=0; idx<nt.nJet();idx++) {
            ret.push_back(1);
            jets.push_back(Jet(idx));
        }
        for (auto lep : leps) {
            float mindr = 0.41;
            int minidx=-1;
            for (unsigned int idx=0; idx<jets.size(); idx++) {
                float dr = ROOT::Math::VectorUtil::DeltaR(lep.p4(),jets[idx].p4());
                if (dr < mindr) {
                    mindr = dr;
                    minidx = idx;
                }
            }
            if (minidx>=0) {ret[minidx]=0;}
        }
        return ret;
}

std::pair<Jets, Jets> getJets(float min_jet_pt, float min_bjet_pt) {
    Jets jets_;
    Jets bjets_;
    for (unsigned int idx=0; idx<nt.nJet();idx++){
        Jet jet(idx);
        if (std::fabs(jet.eta()) > 2.4) continue;
        if (jet.pt() < min_bjet_pt) continue;
        if (!jet.passJetId()) continue;
        if (jet.pt() > min_jet_pt) jets_.push_back(jet);
        if (jet.isBtag()) bjets_.push_back(jet);
    }
    return std::make_pair(jets_,bjets_);
}

std::pair<Jets, Jets> getJets(std::vector<Lepton> &leps, float min_jet_pt, float min_bjet_pt) {
    Jets  jets_;
    // get all jets passing kinematics (pt,eta) and jet ID
    for (unsigned int idx=0; idx<nt.nJet();idx++){
        Jet jet(idx);
        if (std::fabs(jet.eta()) > 2.4) continue;
        if (jet.pt() < min_bjet_pt) continue;
        if (!jet.passJetId()) continue;
        if (jet.pt() > min_jet_pt) jets_.push_back(jet);
        else if (jet.pt() > min_bjet_pt && jet.isBtag()) jets_.push_back(jet);
    }

    Jets ret_jets_;
    Jets ret_bjets_;
    // remove jets overlapping with our selected leptons
    std::vector<bool> jet_flags = cleanJets(jets_,leps);
    for (unsigned int idx=0; idx<jet_flags.size(); idx++) {
        if (!jet_flags[idx]) continue;
        if (jets_[idx].pt() > min_jet_pt) ret_jets_.push_back(jets_[idx]);
        if (jets_[idx].pt() > min_bjet_pt && jets_[idx].isBtag()) ret_bjets_.push_back(jets_[idx]);
    }

    return std::make_pair(ret_jets_,ret_bjets_);
}

Genparts getGenparts() {
    Genparts genparts;
    auto genpartpts = GenPart_pt();
    for (unsigned int igp = 0; igp < genpartpts.size(); igp++) {
        genparts.push_back(Genpart(GenPart_pdgId()[igp], igp));
    }
    return genparts;
}

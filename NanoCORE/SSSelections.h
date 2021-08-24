#ifndef SSSELECTIONS_H
#define SSSELECTIONS_H
#include "Nano.h"
#include "Base.h"
#include "IsolationTools.h"

SS::IDLevel whichLeptonLevel(int id, int idx);
bool isLeptonLevel(SS::IDLevel idlevel, int id, int idx);

struct Lepton {
    Lepton(int id = 0, unsigned int idx = 0) : id_(id), idx_(idx) {
        if (id != 0) {
            isData_ = nt.isData();
            pt_ = (abs(id_) == 11 ? nt.Electron_pt()[idx_] : nt.Muon_pt()[idx_]);
            eta_ = (abs(id_) == 11 ? nt.Electron_eta()[idx_] : nt.Muon_eta()[idx_]);
            phi_ = (abs(id_) == 11 ? nt.Electron_phi()[idx_] : nt.Muon_phi()[idx_]);
            mass_ = (abs(id_) == 11 ? nt.Electron_mass()[idx_] : nt.Muon_mass()[idx_]);
            p4_ = (abs(id_) == 11 ? nt.Electron_p4()[idx_] : nt.Muon_p4()[idx_]);
            miniIso_ = (abs(id_) == 11 ? nt.Electron_miniPFRelIso_all()[idx_] : nt.Muon_miniPFRelIso_all()[idx_]);
            dxy_ = (abs(id_) == 11 ? nt.Electron_dxy()[idx_] : nt.Muon_dxy()[idx_]);
            dz_ = (abs(id_) == 11 ? nt.Electron_dz()[idx_] : nt.Muon_dz()[idx_]);
            if (!isData_){
                genPartFlav_ = (abs(id_) == 11 ? nt.Electron_genPartFlav()[idx_] : nt.Muon_genPartFlav()[idx_]);
                int mcidx = (abs(id_) == 11 ? nt.Electron_genPartIdx()[idx_] : nt.Muon_genPartIdx()[idx_]);
                mcid_ = nt.GenPart_pdgId()[mcidx];
            }
            idlevel_ = whichLeptonLevel(id_, idx_);
            if (nt.year() == 2016){
                if(abs(id_)==11){
                    ptrel_cut = 7.2;
                    miniIso_cut = 0.12;
                    ptRatio_cut = 0.80;
                }else if(abs(id_)==13){
                    ptrel_cut = 7.2;
                    miniIso_cut = 0.12;
                    ptRatio_cut = 0.76;
                }
            }else{
                if(abs(id_)==11){
                    ptrel_cut = 8.0;
                    miniIso_cut = 0.07;
                    ptRatio_cut = 0.78;
                }else if(abs(id_)==13){
                    ptrel_cut = 6.8;
                    miniIso_cut = 0.11;
                    ptRatio_cut = 0.74;
                }
            }
            coneCorrPt_ = coneCorrPt(id_, idx_, miniIso_cut, ptRatio_cut, ptrel_cut);

        }
    }
    void set_idlevel(int idlevel) { idlevel_ = idlevel; }
    unsigned int idx() const { return idx_; }
    int id() const { return id_; }
    int absid() { return abs(id_); }
    int is_el() { return abs(id_) == 11; }
    int is_mu() { return abs(id_) == 13; }
    int charge() { return -1 * id_ / abs(id_); }
    int idlevel() { return idlevel_; }
    LorentzVector p4() { return p4_; }
    float conecorrpt() { return coneCorrPt_; }
    float pt() { return pt_; }
    float eta() { return eta_; }
    float phi() { return phi_; }
    float miniIso() { return miniIso_;}
    float dxy() { return dxy_;}
    float dz() { return dz_;}
    bool is_loose() {return (idlevel_==SS::IDLevel::IDfakable || idlevel_==SS::IDLevel::IDtight);}
    bool is_tight() {return (idlevel_==SS::IDLevel::IDtight);}
    bool operator==(const Lepton& other) {return ((this->id_ == other.id()) && (this->idx_==other.idx()));}
    int mcid() { if (!isData_) {return mcid_;} else {return -1;} }
    int genPartFlav() { if (!isData_) {return genPartFlav_;} else {return -1;} }
    bool isFake() { if (!isData_) {return (genPartFlav_!=1 && genPartFlav_!=15);} else {return 0;} }
    bool isFlip() { if (!isData_) {return (id_==-1*mcid_);} else {return 0;} }

  private:
    int id_;
    int genPartFlav_ = -1;
    int mcid_ = -1;
    float pt_ = 0.;
    float eta_ = 0.;
    float phi_ = 0.;
    float mass_ = 0.;
    LorentzVector p4_;
    float miniIso_ = -1.;
    float dxy_ = -1.;
    float dz_ = -1.;
    unsigned int idx_;
    int idlevel_ = SS::IDdefault;
    bool isData_;
    float ptrel_cut = 0;
    float miniIso_cut = 0;
    float ptRatio_cut = 0;
    float coneCorrPt_ = 0;
                
};

typedef std::pair<Lepton, Lepton> Hyp;
typedef std::vector<std::pair<Lepton, Lepton> > Hyps;
typedef std::vector<Lepton> Leptons;

std::ostream &operator<<(std::ostream &os, Lepton &lep) {
    std::string lepstr = (abs(lep.id()) == 11) ? "Electron" : "Muon";
    return os << "<" << lepstr << " id=" << std::showpos << setw(3) << lep.id() << std::noshowpos << ", idx=" << setw(2)
              << lep.idx() << ", level=" << lep.idlevel() << ", (pT,eta)="
              << "(" << lep.pt() << "," << lep.eta() << ")>";
}
template <typename T1, typename T2> std::ostream &operator<<(std::ostream &os, std::pair<T1, T2> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

vector<Lepton> getLeptons();
Leptons getTightLeptons();
Leptons getLooseLeptons();
std::tuple<int, int, float> getJetInfo(vector<Lepton> &leps, int variation = 0);
std::tuple<int, int, float> getCleanJetInfo(vector<Lepton> &leps, int variation = 0);
std::pair<int, int> makesResonance(Leptons &leps, Lepton lep1, Lepton lep2, float mass, float window);
std::pair<int, Hyp> getBestHyp(vector<Lepton> &leptons, bool verbose);
void dumpLeptonProperties(Lepton lep);
std::pair<int, Leptons> getBestHypFCNC(Leptons &leps, bool verbose);
std::pair<int,int> get_n_mu_el(Leptons &leps);

struct Jet {
    Jet(unsigned int idxx = 0) : idx_(idxx) {
        isData_ = nt.isData();
        id_ = nt.Jet_jetId()[idx_];
        pt_ = nt.Jet_pt()[idx_];
        if(!isData_){
            pt_jesup_ = nt.Jet_pt_jesTotalUp()[idx_];
            pt_jesdown_ = nt.Jet_pt_jesTotalDown()[idx_];
        }
        eta_ = nt.Jet_eta()[idx_];
        phi_ = nt.Jet_phi()[idx_];
        p4_ = nt.Jet_p4()[idx_];
    }
    int id() { return id_; }
    bool is_btag() { return false; } // { return ((pt_ > 25.0) && (Jet_btagDeepB()[idx_] > 0.4941)); }
    unsigned int idx() const { return idx_; }
    LorentzVector p4() { return p4_; }
    float pt() { return pt_; }
    float pt_jesup() { if(!isData_) {return pt_jesup_;} else {return -1;} }
    float pt_jesdown() { if(!isData_) {return pt_jesdown_;} else {return -1;} }
    float eta() { return eta_; }
    float phi() { return phi_; }
    float bdisc() {return nt.Jet_btagDeepFlavB()[idx_];}
    bool isBtag() {
        if (nt.year()==2016) {return bdisc()>0.3093;}
        else if (nt.year()==2017) {return bdisc()>0.3033;}
        else if (nt.year()==2018) {return bdisc()>0.2770;}
        else {return false;}
    }
    int hadronFlavor() {return nt.Jet_hadronFlavour()[idx_];}
    int partonFlavor() {return nt.Jet_partonFlavour()[idx_];}
    bool passJetId() {return id_>1;}
    bool operator==(const Jet& other) {return (idx_==other.idx());}

  private:
    int id_;
    float pt_ = 0.;
    float pt_jesup_ = 0.;
    float pt_jesdown_ = 0.;
    float eta_ = 0.;
    float phi_ = 0.;
    LorentzVector p4_;
    unsigned int idx_;
    bool isData_;
};
typedef std::pair<Jet, Jet> DiJet;
typedef std::vector<Jet> Jets;

// perform lepton-jet overlap removal
std::vector<bool> cleanJets(Leptons &leps);
std::vector<bool> cleanJets(Jets &jets, Leptons &leps);

// get all good jets and bjets
// applies kinematic (eta,pt) cuts, jet ID, and b-tagging
std::pair<Jets, Jets> getJets(float min_jet_pt=40., float min_bjet_pt=25., int jesVar = 0);
std::pair<Jets, Jets> getJets(Leptons &leps,float min_jet_pt=40., float min_bjet_pt=25., int jesVar = 0);

//Sorting functions
bool lepsort (Lepton i,Lepton j);
bool jetptsort (Jet i,Jet j);

bool pass_lep_pt_eta(Lepton &lep);

#endif

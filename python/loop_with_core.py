#!/usr/bin/env python
from tqdm import tqdm
import ROOT as r
import os

ch = r.TChain("Events")
#ch.Add("root://redirector.t2.ucsd.edu///store/user/namin/nanoaod/TTJets_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18NanoAODv5-Nano1June2019_102X_upgrade2018_realistic_v19-v1/8E0C8306-DC0D-0548-BA7C-D0698140DF28.root");
#ch.Add("root://redirector.t2.ucsd.edu///store/mc/RunIISummer16NanoAODv7/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/NANOAODSIM/PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/260000/6393A45C-BFE8-6948-87A7-5D478B9ECA57.root");
ch.Add('file:///home/users/fgolf/forYash/dyjets_RunIISummer16NanoAOD.root')

r.gSystem.Load('../NanoCORE/NANO_CORE.so')
for include in ["Nano.cc", "SSSelections.cc", "MetSelections.cc"]:
    r.gInterpreter.ProcessLine('#include "../NanoCORE/%s"' % include)

# CORE functions will be ROOT object members after we gSystem.Load CORE
from ROOT import nt, passesMETfilters, gconf

gconf.year = 2018

nt.Init(ch)
N = ch.GetEntries()

h1 = r.TH1F("h1","",100,0,200)

def process_event(ievent):
    nt.GetEntry(ievent)

    if not passesMETfilters(False): return

    for imu,pt in enumerate(nt.Muon_pt()):
        if pt < 25: continue
        h1.Fill(pt)


for ievent in tqdm(range(ch.GetEntries())):
    process_event(ievent)

print(h1.GetMean())

c1 = r.TCanvas()
h1.Draw()
c1.SaveAs("plot.pdf")
os.system("which ic && ic plot.pdf")

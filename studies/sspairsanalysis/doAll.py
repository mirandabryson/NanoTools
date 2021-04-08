#!/usr/bin/env python
import os
import sys
sys.path.insert(1,os.getcwd()+'/utils')
import pyrun as pyrun
import argparse
import fnmatch
import operator
import glob
import ast
import time
import ROOT as r
r.gROOT.SetBatch()
#r.gROOT.ProcessLine(".L ../misc/class_files/v8.02/SS.cc+")
r.gROOT.ProcessLine(".L /home/users/ian/NanoTools/NanoCORE/NANO_CORE.so")
#r.gROOT.ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc++")
r.gROOT.ProcessLine(".L /home/users/ian/NanoTools/studies/sspairsanalysis/ScanChain.C++")
#r.gROOT.ProcessLine(".L findHeader.C+")

years_to_consider = [
#    2016,
    2017,
#    2018,
    ]
procs_to_consider = [ # FIXME
    # Nano procs with computed xsecs
#    "TTJets_DiLept",
#    "TTJets_SingleLeptFromT",
#    "TTJets_SingleLeptFromTbar",
#    "TTWJetsToLNu",
#    "TTZToLLNuNu",
    "WZTo3LNu",
#    "WpWpJJ_EWK",
#    "ttHTobb",

    #VBSWmpWmpHToLNuLNu has many samples, not in every year
#    "VBSWmpWmpHToLNuLNu_TuneCP5",




    #Old proc names
#    "qqww",
#    "wz",
#
#    "fakes_mc",
#    "TTSL",
#    "WJets_HT",
#
#    "Invs" 
#
#    "flips_mc",
#    "TTDL",
#    "DY_high",
#    "DY_low",
#
#    "xg",
#    "TGext",
#    "TTGdilep",
#    "TTGsinglelepbar",
#    "TTGsinglelep",
#    "WGToLNuGext",
#    "ZG",
#
#    "rares",
#    "GGHtoZZto4L",
#    "TWZ",
#    "TZQ",
#    "VHtoNonBB",
#    "WWDPS",
#    "WWW",
#    "WWZ",
#    "WZG",
#    "WWG",
#    "WZZ",
#    "ZZ",
#    "ZZZ",
#    "TTTJ",
#    "TTTW",
#    "TTWnlo",
#    "TTHtoNonBB",
#    "TTZnlo",
#    "TTZLOW",
#    "TTHH",
#    "TTWH",
#    "TTWW",
#    "TTWZ",
#    "TTZH",
#    "TTZZ",
#
#    "vbs_hww",
#
#    "data"
#    "fakes",
#    "fakes_mc_ml",
#    "flips",
#    "wz",
#    "fcnc",
    ]


# Paths to the samples by year
basedirs ={
    2016: "/home/users/ian/NanoTools/studies/sspairsanalysis/samples/RunIISummer16NanoAOD/",
    2017: "/home/users/ian/NanoTools/studies/sspairsanalysis/samples/RunIIFall17NanoAOD/",
    2018: "/home/users/ian/NanoTools/studies/sspairsanalysis/samples/RunIIAutumn18NanoAOD/",
    }


outputdir = "Outputs"


# Commands to be passed to ScanChain.C by year
options = {
    # for SS
    #2016: "  doSS Data2016 new2016FRBins ReadBDT quiet ",
    #2017: "  doSS Data2017 ReadBDT quiet ",
    #2018: "  doSS Data2018 ReadBDT quiet ",
    2016: "  doSS Data2016 new2016FRBins quiet debug",
    2017: "  doSS Data2017 quiet debug",
    2018: "  doSS Data2018 quiet debug",
    }

def make_objs(fpatts=[],options="",treename="Events"):
    if type(fpatts) == str: fpatts = [fpatts]
    ch = r.TChain(treename)
    for fpatt in fpatts:
        n = ch.Add(fpatt)
        print(fpatt, n)
    return {"ch": ch, "options": options}


# Paths to samples, by year, by process
chs = {
    2016: {
        "TTJets_DiLept": make_objs(basedirs[2016]+"TTJets_DiLept.root", options=options[2016]),
        "TTJets_SingleLeptFromT": make_objs(basedirs[2016]+"TTJets_SingleLeptFromT.root", options=options[2016]),
        "TTJets_SingleLeptFromTbar": make_objs(basedirs[2016]+"TTJets_SingleLeptFromTbar.root", options=options[2016]),
 #       "TTWJetsToLNu": make_objs(basedirs[2016]+"TTWJetsToLNu/*.root", options=options[2016]),
#        "TTZToLLNuNu": make_objs(basedirs[2016]+"TTZToLLNuNu/*.root", options=options[2016]),
        "WZTo3LNu": make_objs(basedirs[2016]+"WZTo3LNu.root", options=options[2016]),
        "WpWpJJ_EWK": make_objs(basedirs[2016]+"WpWpJJ_EWK.root", options=options[2016]),
        "ttHTobb": make_objs(basedirs[2016]+"ttHTobb.root", options=options[2016]),



#        "data": make_objs(basedirs[2016]+"Data*.root", options=options[2016]),
#                basedirs[2016]+"TTHtoNonBB.root",
#                ] , options=options[2016]+" doFakes doData"),
        "fakes_mc": make_objs([
                basedirs[2016]+"TTSLtop.root",
                basedirs[2016]+"TTSLtopbar.root",
                basedirs[2016]+"WJets_HT*.root",
                ] , options=options[2016]+ " doTruthFake"),
        "TTSL": make_objs([
		basedirs[2016]+"TTSLtop.root",
		basedirs[2016]+"TTSLtopbar.root",
		] , options=options[2016]+ " doTruthFake"),
	"WJets_HT": make_objs(basedirs[2016]+"WJets_HT*.root", options=options[2016] + " doTruthFake"),

        "flips": make_objs(basedirs[2016]+"Data*.root", options=options[2016]+" doFlips"),        
        ## right now, flips_mc and fakes_mc_ml are the same.  FIX!!
        "flips_mc": make_objs([
                basedirs[2016]+"TTDL.root",
                basedirs[2016]+"DY_high.root",
                basedirs[2016]+"DY_low.root",
                ] ,options=options[2016]),
        "TTDL": make_objs(basedirs[2016]+"TTDL.root", options=options[2016]),
        "DY_high": make_objs(basedirs[2016]+"DY_high.root", options=options[2016]),
        "DY_low": make_objs(basedirs[2016]+"DY_low.root", options=options[2016]),

        "fakes_mc_ml": make_objs([
                basedirs[2016]+"TTDL.root",
                basedirs[2016]+"DY_high.root",
                basedirs[2016]+"DY_low.root",
                ] ,options=options[2016]+ " doTruthFake"),
        "xg": make_objs([
                basedirs[2016]+"TGext.root",
                basedirs[2016]+"TTGdilep.root",
                basedirs[2016]+"TTGsinglelepbar.root",
                basedirs[2016]+"TTGsinglelep.root",
                basedirs[2016]+"WGToLNuGext.root",
                basedirs[2016]+"ZG.root",
                ],options=options[2016] + " doXgamma "),
        "TGext": make_objs(basedirs[2016]+"TGext.root", options=options[2016] + " doXgamma "),
        "TTGdilep": make_objs(basedirs[2016]+"TTGdilep.root", options=options[2016] + " doXgamma "),
        "TTGsinglelepbar": make_objs(basedirs[2016]+"TTGsinglelepbar.root", options=options[2016] + " doXgamma "),
        "TTGsinglelep": make_objs(basedirs[2016]+"TTGsinglelep.root", options=options[2016] + " doXgamma "),
        "WGToLNuGext": make_objs(basedirs[2016]+"WGToLNuGext.root", options=options[2016] + " doXgamma "),
        "ZG": make_objs(basedirs[2016]+"ZG.root", options=options[2016] + " doXgamma "),

        "qqww": make_objs(basedirs[2016]+"QQWW.root",options=options[2016]),
        "wz":   make_objs(basedirs[2016]+"WZ.root",options=options[2016]),
        "rares": make_objs([
                basedirs[2016]+"GGHtoZZto4L.root",
                basedirs[2016]+"TWZ.root",
                basedirs[2016]+"TZQ.root",
                basedirs[2016]+"VHtoNonBB.root",
                basedirs[2016]+"WWDPS.root",
                basedirs[2016]+"WWW.root",
                basedirs[2016]+"WWZ.root",
                basedirs[2016]+"WZG.root",
                basedirs[2016]+"WWG.root",
                basedirs[2016]+"WZZ.root",
                basedirs[2016]+"ZZ.root",
                basedirs[2016]+"ZZZ.root",
                basedirs[2016]+"TTTJ.root",
                basedirs[2016]+"TTTW.root",
                basedirs[2016]+"TTWnlo.root",
                basedirs[2016]+"TTHtoNonBB.root",
                basedirs[2016]+"TTZnlo.root",
                basedirs[2016]+"TTZLOW.root",
                basedirs[2016]+"TTHH.root",
                basedirs[2016]+"TTWH.root",
                basedirs[2016]+"TTWW.root",
                basedirs[2016]+"TTWZ.root",
                basedirs[2016]+"TTZH.root",
                basedirs[2016]+"TTZZ.root",
                ],options=options[2016]),
        "GGHtoZZto4L": make_objs(basedirs[2016]+"GGHtoZZto4L.root", options=options[2016]),
        "TWZ": make_objs(basedirs[2016]+"TWZ.root", options=options[2016]),
        "TZQ": make_objs(basedirs[2016]+"TZQ.root", options=options[2016]),
        "VHtoNonBB": make_objs(basedirs[2016]+"VHtoNonBB.root", options=options[2016]),
        "WWDPS": make_objs(basedirs[2016]+"WWDPS.root", options=options[2016]),
        "WWW": make_objs(basedirs[2016]+"WWW.root", options=options[2016]),
        "WWZ": make_objs(basedirs[2016]+"WWZ.root", options=options[2016]),
        "WZG": make_objs(basedirs[2016]+"WZG.root", options=options[2016]),
        "WWG": make_objs(basedirs[2016]+"WWG.root", options=options[2016]),
        "WZZ": make_objs(basedirs[2016]+"WZZ.root", options=options[2016]),
        "ZZ": make_objs(basedirs[2016]+"ZZ.root", options=options[2016]),
        "ZZZ": make_objs(basedirs[2016]+"ZZZ.root", options=options[2016]),
        "TTTJ": make_objs(basedirs[2016]+"TTTJ.root", options=options[2016]),
        "TTTW": make_objs(basedirs[2016]+"TTTW.root", options=options[2016]),
        "TTWnlo": make_objs(basedirs[2016]+"TTWnlo.root", options=options[2016]),
        "TTHtoNonBB": make_objs(basedirs[2016]+"TTHtoNonBB.root", options=options[2016]),
        "TTZnlo": make_objs(basedirs[2016]+"TTZnlo.root", options=options[2016]),
        "TTZLOW": make_objs(basedirs[2016]+"TTZLOW.root", options=options[2016]),
        "TTHH": make_objs(basedirs[2016]+"TTHH.root", options=options[2016]),
        "TTWH": make_objs(basedirs[2016]+"TTWH.root", options=options[2016]),
        "TTWW": make_objs(basedirs[2016]+"TTWW.root", options=options[2016]),
        "TTWZ": make_objs(basedirs[2016]+"TTWZ.root", options=options[2016]),
        "TTZH": make_objs(basedirs[2016]+"TTZH.root", options=options[2016]),
        "TTZZ": make_objs(basedirs[2016]+"TTZZ.root", options=options[2016]),

        "fcnc_hut": make_objs(basedirs[2016]+"FCNC_hut*.root", options=options[2016]),
        "fcnc_hct": make_objs(basedirs[2016]+"FCNC_hct*.root", options=options[2016]),
        },


    # Start of 2017 paths
    2017: {
        "TTJets_DiLept": make_objs(basedirs[2017]+"TTJets_DiLept.root", options=options[2017]),
        "TTJets_SingleLeptFromT": make_objs(basedirs[2017]+"TTJets_SingleLeptFromT.root", options=options[2017]),
        "TTJets_SingleLeptFromTbar": make_objs(basedirs[2017]+"TTJets_SingleLeptFromTbar.root", options=options[2017]),
        "TTWJetsToLNu": make_objs(basedirs[2017]+"TTWJetsToLNu.root", options=options[2017]),
        "TTZToLLNuNu": make_objs(basedirs[2017]+"TTZToLLNuNu.root", options=options[2017]),
        "WZTo3LNu": make_objs(basedirs[2017]+"WZTo3LNu/*.root", options=options[2017]),
        "WpWpJJ_EWK": make_objs(basedirs[2017]+"WpWpJJ_EWK.root", options=options[2017]),
        "ttHTobb": make_objs(basedirs[2017]+"ttHTobb.root", options=options[2017]),



        "data": make_objs(basedirs[2017]+"Data*.root", options=options[2017]),
        "fakes": make_objs([
                basedirs[2017]+"Data*.root",
                basedirs[2017]+"TTWnlo.root",
                basedirs[2017]+"TTZnlo.root",
                basedirs[2017]+"TTHtoNonBB.root",
                ] , options=options[2017]+" doFakes doData"),
        "fakes_mc": make_objs([
                basedirs[2017]+"TTSLtop.root",
                basedirs[2017]+"TTSLtopbar.root",
                basedirs[2017]+"WJets_HT*.root",
                ] , options=options[2017]+ " doTruthFake"),
        "TTSL": make_objs([
		basedirs[2017]+"TTSLtop.root",
		basedirs[2017]+"TTSLtopbar.root",
		] , options=options[2017]+ " doTruthFake"),
        "WJets_HT": make_objs(basedirs[2017]+"WJets_HT*.root", options=options[2017] + " doTruthFake"),

        "flips": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFlips"),        
        ## right now, flips_mc and fakes_mc_ml are the same.  FIX!!
        "flips_mc": make_objs([
                basedirs[2017]+"TTDL.root",
                basedirs[2017]+"DY_high.root",
                basedirs[2017]+"DY_low.root",
                ] ,options=options[2017]),
        "TTDL": make_objs(basedirs[2017]+"TTDL.root", options=options[2017]),
        "DY_high": make_objs(basedirs[2017]+"DY_high.root", options=options[2017]),
        "DY_low": make_objs(basedirs[2017]+"DY_low.root", options=options[2017]),

        "fakes_mc_ml": make_objs([
                basedirs[2017]+"TTDL.root",
                basedirs[2017]+"DY_high.root",
                basedirs[2017]+"DY_low.root",
                ] ,options=options[2017]+ " doTruthFake"),
        "xg": make_objs([
                basedirs[2017]+"TGext.root",
                basedirs[2017]+"TTGdilep.root",
                basedirs[2017]+"TTGsinglelepbar.root",
                basedirs[2017]+"TTGsinglelep.root",
                basedirs[2017]+"WGToLNuGext.root",
                basedirs[2017]+"ZG.root",
                ],options=options[2017] + " doXgamma "),
        "TGext": make_objs(basedirs[2017]+"TGext.root", options=options[2017] + " doXgamma "),
        "TTGdilep": make_objs(basedirs[2017]+"TTGdilep.root", options=options[2017] + " doXgamma "),
        "TTGsinglelepbar": make_objs(basedirs[2017]+"TTGsinglelepbar.root", options=options[2017] + " doXgamma "),
        "TTGsinglelep": make_objs(basedirs[2017]+"TTGsinglelep.root", options=options[2017] + " doXgamma "),
        "WGToLNuGext": make_objs(basedirs[2017]+"WGToLNuGext.root", options=options[2017] + " doXgamma "),
        "ZG": make_objs(basedirs[2017]+"ZG.root", options=options[2017] + " doXgamma "),

        "qqww": make_objs(basedirs[2017]+"QQWW.root",options=options[2017]),
        "wz":   make_objs(basedirs[2017]+"WZ.root",options=options[2017]),
        "rares": make_objs([
                basedirs[2017]+"GGHtoZZto4L.root",
                basedirs[2017]+"TWZ.root",
                basedirs[2017]+"TZQ.root",
                basedirs[2017]+"VHtoNonBB.root",
                basedirs[2017]+"WWDPS.root",
                basedirs[2017]+"WWW.root",
                basedirs[2017]+"WWZ.root",
                basedirs[2017]+"WZG.root",
                basedirs[2017]+"WWG.root",
                basedirs[2017]+"WZZ.root",
                basedirs[2017]+"ZZ.root",
                basedirs[2017]+"ZZZ.root",
                basedirs[2017]+"TTTJ.root",
                basedirs[2017]+"TTTW.root",
                basedirs[2017]+"TTWnlo.root",
                basedirs[2017]+"TTHtoNonBB.root",
                basedirs[2017]+"TTZnlo.root",
                basedirs[2017]+"TTZLOW.root",
                basedirs[2017]+"TTHH.root",
                basedirs[2017]+"TTWH.root",
                basedirs[2017]+"TTWW.root",
                basedirs[2017]+"TTWZ.root",
                basedirs[2017]+"TTZH.root",
                basedirs[2017]+"TTZZ.root",
                ],options=options[2017]),
        "GGHtoZZto4L": make_objs(basedirs[2017]+"GGHtoZZto4L.root", options=options[2017]),
        "TWZ": make_objs(basedirs[2017]+"TWZ.root", options=options[2017]),
        "TZQ": make_objs(basedirs[2017]+"TZQ.root", options=options[2017]),
        "VHtoNonBB": make_objs(basedirs[2017]+"VHtoNonBB.root", options=options[2017]),
        "WWDPS": make_objs(basedirs[2017]+"WWDPS.root", options=options[2017]),
        "WWW": make_objs(basedirs[2017]+"WWW.root", options=options[2017]),
        "WWZ": make_objs(basedirs[2017]+"WWZ.root", options=options[2017]),
        "WZG": make_objs(basedirs[2017]+"WZG.root", options=options[2017]),
        "WWG": make_objs(basedirs[2017]+"WWG.root", options=options[2017]),
        "WZZ": make_objs(basedirs[2017]+"WZZ.root", options=options[2017]),
        "ZZ": make_objs(basedirs[2017]+"ZZ.root", options=options[2017]),
        "ZZZ": make_objs(basedirs[2017]+"ZZZ.root", options=options[2017]),
        "TTTJ": make_objs(basedirs[2017]+"TTTJ.root", options=options[2017]),
        "TTTW": make_objs(basedirs[2017]+"TTTW.root", options=options[2017]),
        "TTWnlo": make_objs(basedirs[2017]+"TTWnlo.root", options=options[2017]),
        "TTHtoNonBB": make_objs(basedirs[2017]+"TTHtoNonBB.root", options=options[2017]),
        "TTZnlo": make_objs(basedirs[2017]+"TTZnlo.root", options=options[2017]),
        "TTZLOW": make_objs(basedirs[2017]+"TTZLOW.root", options=options[2017]),
        "TTHH": make_objs(basedirs[2017]+"TTHH.root", options=options[2017]),
        "TTWH": make_objs(basedirs[2017]+"TTWH.root", options=options[2017]),
        "TTWW": make_objs(basedirs[2017]+"TTWW.root", options=options[2017]),
        "TTWZ": make_objs(basedirs[2017]+"TTWZ.root", options=options[2017]),
        "TTZH": make_objs(basedirs[2017]+"TTZH.root", options=options[2017]),
        "TTZZ": make_objs(basedirs[2017]+"TTZZ.root", options=options[2017]),

        "fcnc_hut": make_objs(basedirs[2017]+"FCNC_hut*.root", options=options[2017]),
        "fcnc_hct": make_objs(basedirs[2017]+"FCNC_hct*.root", options=options[2017]),
        },

    # Start of 2018 paths
    2018: {
        "TTJets_DiLept": make_objs(basedirs[2018]+"TTJets_DiLept.root", options=options[2018]),
#        "TTJets_SingleLeptFromT": make_objs(basedirs[2018]+"TTJets_SingleLeptFromT/*.root", options=options[2018]),
        "TTJets_SingleLeptFromTbar": make_objs(basedirs[2018]+"TTJets_SingleLeptFromTbar.root", options=options[2018]),
        "TTWJetsToLNu": make_objs(basedirs[2018]+"TTWJetsToLNu.root", options=options[2018]),
        "TTZToLLNuNu": make_objs(basedirs[2018]+"TTZToLLNuNu.root", options=options[2018]),
        "WZTo3LNu": make_objs(basedirs[2018]+"WZTo3LNu.root", options=options[2018]),
#        "WpWpJJ_EWK": make_objs(basedirs[2018]+"WpWpJJ_EWK/*.root", options=options[2018]),
        "ttHTobb": make_objs(basedirs[2018]+"ttHTobb.root", options=options[2018]),



        "data": make_objs(basedirs[2018]+"Data*.root", options=options[2018]),
        "fakes": make_objs([
                basedirs[2018]+"Data*.root",
                basedirs[2018]+"TTWnlo.root",
                basedirs[2018]+"TTZnlo.root",
                basedirs[2018]+"TTHtoNonBB.root",
                ] , options=options[2018]+" doFakes doData"),
        "fakes_mc": make_objs([
                basedirs[2018]+"TTSLtop.root",
                basedirs[2018]+"TTSLtopbar.root",
                basedirs[2018]+"WJets_HT*.root",
                ] , options=options[2018]+ " doTruthFake"),
        "TTSL": make_objs([
		basedirs[2018]+"TTSLtop.root",
		basedirs[2018]+"TTSLtopbar.root",
		] , options=options[2018]+ " doTruthFake"),
        "WJets_HT": make_objs(basedirs[2018]+"WJets_HT*.root", options=options[2018] + " doTruthFake"),

        "flips": make_objs(basedirs[2018]+"Data*.root", options=options[2018]+" doFlips"),        
        ## right now, flips_mc and fakes_mc_ml are the same.  FIX!!
        "flips_mc": make_objs([
                basedirs[2018]+"TTDL.root",
                basedirs[2018]+"DY_high.root",
                basedirs[2018]+"DY_low.root",
                ] ,options=options[2018]),
        "TTDL": make_objs(basedirs[2018]+"TTDL.root", options=options[2018]),
        "DY_high": make_objs(basedirs[2018]+"DY_high.root", options=options[2018]),
        "DY_low": make_objs(basedirs[2018]+"DY_low.root", options=options[2018]),

        "fakes_mc_ml": make_objs([
                basedirs[2018]+"TTDL.root",
                basedirs[2018]+"DY_high.root",
                basedirs[2018]+"DY_low.root",
                ] ,options=options[2018]+ " doTruthFake"),
        "xg": make_objs([
                basedirs[2018]+"TGext.root",
                basedirs[2018]+"TTGdilep.root",
                basedirs[2018]+"TTGsinglelepbar.root",
                basedirs[2018]+"TTGsinglelep.root",
                basedirs[2018]+"WGToLNuGext.root",
                basedirs[2018]+"ZG.root",
                ],options=options[2018] + " doXgamma "),
        "TGext": make_objs(basedirs[2018]+"TGext.root", options=options[2018] + " doXgamma "),
        "TTGdilep": make_objs(basedirs[2018]+"TTGdilep.root", options=options[2018] + " doXgamma "),
        "TTGsinglelepbar": make_objs(basedirs[2018]+"TTGsinglelepbar.root", options=options[2018] + " doXgamma "),
        "TTGsinglelep": make_objs(basedirs[2018]+"TTGsinglelep.root", options=options[2018] + " doXgamma "),
        "WGToLNuGext": make_objs(basedirs[2018]+"WGToLNuGext.root", options=options[2018] + " doXgamma "),
        "ZG": make_objs(basedirs[2018]+"ZG.root", options=options[2018] + " doXgamma "),

        "qqww": make_objs(basedirs[2018]+"QQWW.root",options=options[2018]),
        "wz":   make_objs(basedirs[2018]+"WZ.root",options=options[2018]),
        "rares": make_objs([
                basedirs[2018]+"GGHtoZZto4L.root",
                basedirs[2018]+"TWZ.root",
                basedirs[2018]+"TZQ.root",
                basedirs[2018]+"VHtoNonBB.root",
                basedirs[2018]+"WWDPS.root",
                basedirs[2018]+"WWW.root",
                basedirs[2018]+"WWZ.root",
                basedirs[2018]+"WZG.root",
                basedirs[2018]+"WWG.root",
                basedirs[2018]+"WZZ.root",
                basedirs[2018]+"ZZ.root",
                basedirs[2018]+"ZZZ.root",
                basedirs[2018]+"TTTJ.root",
                basedirs[2018]+"TTTW.root",
                basedirs[2018]+"TTWnlo.root",
                basedirs[2018]+"TTHtoNonBB.root",
                basedirs[2018]+"TTZnlo.root",
                basedirs[2018]+"TTZLOW.root",
                basedirs[2018]+"TTHH.root",
                basedirs[2018]+"TTWH.root",
                basedirs[2018]+"TTWW.root",
                basedirs[2018]+"TTWZ.root",
                basedirs[2018]+"TTZH.root",
                basedirs[2018]+"TTZZ.root",
                ],options=options[2018]),
        "GGHtoZZto4L": make_objs(basedirs[2018]+"GGHtoZZto4L.root", options=options[2018]),
        "TWZ": make_objs(basedirs[2018]+"TWZ.root", options=options[2018]),
        "TZQ": make_objs(basedirs[2018]+"TZQ.root", options=options[2018]),
        "VHtoNonBB": make_objs(basedirs[2018]+"VHtoNonBB.root", options=options[2018]),
        "WWDPS": make_objs(basedirs[2018]+"WWDPS.root", options=options[2018]),
        "WWW": make_objs(basedirs[2018]+"WWW.root", options=options[2018]),
        "WWZ": make_objs(basedirs[2018]+"WWZ.root", options=options[2018]),
        "WZG": make_objs(basedirs[2018]+"WZG.root", options=options[2018]),
        "WWG": make_objs(basedirs[2018]+"WWG.root", options=options[2018]),
        "WZZ": make_objs(basedirs[2018]+"WZZ.root", options=options[2018]),
        "ZZ": make_objs(basedirs[2018]+"ZZ.root", options=options[2018]),
        "ZZZ": make_objs(basedirs[2018]+"ZZZ.root", options=options[2018]),
        "TTTJ": make_objs(basedirs[2018]+"TTTJ.root", options=options[2018]),
        "TTTW": make_objs(basedirs[2018]+"TTTW.root", options=options[2018]),
        "TTWnlo": make_objs(basedirs[2018]+"TTWnlo.root", options=options[2018]),
        "TTHtoNonBB": make_objs(basedirs[2018]+"TTHtoNonBB.root", options=options[2018]),
        "TTZnlo": make_objs(basedirs[2018]+"TTZnlo.root", options=options[2018]),
        "TTZLOW": make_objs(basedirs[2018]+"TTZLOW.root", options=options[2018]),
        "TTHH": make_objs(basedirs[2018]+"TTHH.root", options=options[2018]),
        "TTWH": make_objs(basedirs[2018]+"TTWH.root", options=options[2018]),
        "TTWW": make_objs(basedirs[2018]+"TTWW.root", options=options[2018]),
        "TTWZ": make_objs(basedirs[2018]+"TTWZ.root", options=options[2018]),
        "TTZH": make_objs(basedirs[2018]+"TTZH.root", options=options[2018]),
        "TTZZ": make_objs(basedirs[2018]+"TTZZ.root", options=options[2018]),

        "fcnc_hut": make_objs(basedirs[2018]+"FCNC_hut*.root", options=options[2018]),
        "fcnc_hct": make_objs(basedirs[2018]+"FCNC_hct*.root", options=options[2018]),
        },

#    18812: {
#        "Invs": make_objs(basedirs[18812]+"merged_ntuple_*.root", options=options[18812]),        
#    },
}

# Change chain titles to proc_year so that we output the right root file name
for year in chs:
    [obj["ch"].SetTitle("{}_{}".format(proc,year)) for proc,obj in chs[year].items()]

def run_chain((index,info)):
    ch, options, outputdir = info
    ret = r.ScanChain(ch, options, outputdir)
    return index, ret

to_run = []
for year in years_to_consider:
    for proc,obj in chs[year].items():
        if procs_to_consider and (proc not in procs_to_consider): continue
        to_run.append([obj["ch"], obj["options"], outputdir])

os.system("mkdir -p {}".format(outputdir))

os.nice(10)
runner = pyrun.Runner(nproc=min(len(to_run),25), func=run_chain, dot_type=2)
runner.add_args(to_run)
runner.run()

#os.system("cd {}".format(outputdir))
#for proc in procs_to_consider:
#    try:
#        os.system("rm histos_{}_run2.root".format(proc))
#    except:
#        print "New run 2 file"
#    os.system("hadd histos_{}_run2.root histos_{}_2016.root histos_{}_2017.root histos_{}_2018.root".format(proc))


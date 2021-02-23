#! /usr/bin/env python
from argparse import ArgumentParser, RawTextHelpFormatter
import ROOT
ROOT.SetMemoryPolicy(ROOT.kMemoryStrict)
import glob

def doAll(input_file, id_level, flavor, cms4=False, nano=False):
    if nano and cms4:
        print("You can't have both! Choose CMS4 OR Nano.")
        return
    if nano:
        # Load .so files
        ROOT.gROOT.ProcessLine(".L ./NanoCORE/NANO_CORE.so");
        ROOT.gROOT.ProcessLine(".L ScanChainNano.C+");
        # Make TChain
        tchain = ROOT.TChain("Events")
        tchain.Add(input_file)
        # Run ScanChain
        ROOT.ScanChainNano(tchain, id_level, flavor)
    elif cms4:
        # Load .so files
        ROOT.gROOT.ProcessLine(".L ./CORE/CMS3_CORE.so");
        ROOT.gROOT.ProcessLine(".L ScanChainCMS4.C+");
        # Make TChain
        tchain = ROOT.TChain("Events")
        tchain.Add(input_file)
        # Run ScanChain
        ROOT.ScanChainCMS4(tchain, id_level, flavor)
    else:
        print("No looper specified")

    return

if __name__ == "__main__":
    # CLI
    argparser = ArgumentParser(
        description='Run looper',
        formatter_class=RawTextHelpFormatter
    )
    # Input file path
    argparser.add_argument(
        "--input_file", 
        type=str, 
        default="",
        help="Input file path"
    )
    # ID level
    argparser.add_argument(
        "--id_level", 
        type=str, 
        default="tight",
        help="ID level (tight, loose)"
    )
    # ID level
    argparser.add_argument(
        "--flavor", 
        type=str, 
        default="tight",
        help="Lepton flavor (electron, muon)"
    )
    # CMS4
    argparser.add_argument(
        "--nano", 
        action="store_true",
        default=True,
        help="Run NanoAOD looper"
    )
    # Nano
    argparser.add_argument(
        "--cms4", 
        action="store_true",
        default=False,
        help="Run CMS4 looper"
    )
    # Get args
    args = argparser.parse_args()

    doAll(
        args.input_file, 
        args.id_level, 
        args.flavor, 
        args.cms4, 
        args.nano
    )

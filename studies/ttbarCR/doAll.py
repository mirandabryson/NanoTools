#! /usr/bin/env python
from argparse import ArgumentParser, RawTextHelpFormatter
import ROOT
ROOT.SetMemoryPolicy(ROOT.kMemoryStrict)
import glob
import json

def doAll(input_file, output_file, sample_name):
    # Load .so files
    ROOT.gROOT.ProcessLine(".L ./NanoCORE/NANO_CORE.so");
    ROOT.gROOT.ProcessLine(".L ./control.C+");
    ROOT.gROOT.ProcessLine(".L ScanChain.C+");
    # Get sample information (only used for MC)
    xsec = 0
    n_events_total = 0
    # Make TChain
    tchain = ROOT.TChain("Events")
    tchain.Add(input_file)
    # Run ScanChain
    ROOT.ScanChain(
        tchain, 
        output_file, 
        sample_name
    )

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
    # Output file path
    argparser.add_argument(
        "--output_file", 
        type=str, 
        default="",
        help="Output file path"
    )
    # Sample name
    argparser.add_argument(
        "--sample_name", 
        type=str, 
        default="",
        help="Sample name"
    )
    # Get args
    args = argparser.parse_args()

    doAll(args.input_file, args.output_file, args.sample_name)

#! /usr/bin/env python
from argparse import ArgumentParser, RawTextHelpFormatter
import ROOT
ROOT.SetMemoryPolicy(ROOT.kMemoryStrict)
import glob

def doAll(input_file, sample_name, is_data):
    # Load .so files
    ROOT.gROOT.ProcessLine(".L ./NanoCORE/NANO_CORE.so");
    ROOT.gROOT.ProcessLine(".L ScanChain.C+");
    # Parse samples
    # Make TChain
    tchain = ROOT.TChain("Events")
    tchain.Add(input_file)
    # Run ScanChain
    ROOT.ScanChain(tchain, sample_name, is_data)

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
    # Sample name
    argparser.add_argument(
        "--sample_name", 
        type=str, 
        default="",
        help="Sample name"
    )
    # Dilepton
    argparser.add_argument(
        "--is_data", 
        action="store_true",
        default=False,
        help="Input is not simulation"
    )
    # Get args
    args = argparser.parse_args()

    doAll(args.input_file, args.sample_name, args.is_data)

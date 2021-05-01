# Copies skims to local directory and makes them into a format that works with the looper
import os
import sys
import datetime
import subprocess
from distutils.dir_util import copy_tree

# Location of files to be copied from, broken into parts to help organize
source_dir = "/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v9"
sample = "/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1_NANOAODSIM_v9"

# Location for files to be copied to
target_dir = "/home/users/ian/NanoTools/studies/sspairsanalysis/samples"

# Updates target location based on year of sample, aborts if not found
if (sample.count("RunIISummer16NanoAOD")):
    target_dir += "/RunIISummer16NanoAOD"
elif (sample.count("RunIIFall17NanoAOD")):
    target_dir += "/RunIIFall17NanoAOD"
elif (sample.count("RunIIAutumn18NanoAOD")): 
    target_dir += "/RunIIAutumn18NanoAOD"
else:
    sys.exit("Unknown year, aborting")

target_dir += "/test"
# Create log file
f = open(target_dir + "/transfer_log.txt", "w")
f.write("\nDate: " + str(datetime.datetime.now()))
f.write("\nSource: " + source_dir + sample)
f.write("\nTarget: " + target_dir)

# Need to find a way to print to terminal and log file
#sys.stdout = f
#sys.stderr = f

os.mkdir(target_dir);
#print("Copying files")
#copy_tree(source_dir+sample, target_dir)

f.close()
print("Done")

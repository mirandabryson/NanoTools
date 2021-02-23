import os

from metis.Sample import DBSSample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask

# Avoid spamming too many short jobs to condor
# Less dileptn pairs = faster = more input files per job
def split_func(dsname):
    if any(x in dsname for x in [
        "/W","/Z","/TTJets","/DY","/ST",
        ]):
        return 5
    elif "Run201" in dsname:
        return 7
    else:
        return 2

if __name__ == "__main__":

    # Specify a dataset name and a short name for the output root file on nfs
    # 2016
    # sample_map = {
    #     "/MuonEG/Run2016B-02Apr2020_ver1-v1/NANOAOD": "MuonEG2016B-ver1_baby",
    #     "/MuonEG/Run2016B-02Apr2020_ver2-v1/NANOAOD": "MuonEG2016B-ver2_baby",
    #     "/MuonEG/Run2016C-02Apr2020-v1/NANOAOD": "MuonEG2016C_baby",
    #     "/MuonEG/Run2016D-02Apr2020-v1/NANOAOD": "MuonEG2016D_baby",
    #     "/MuonEG/Run2016E-02Apr2020-v2/NANOAOD": "MuonEG2016E_baby",
    #     "/MuonEG/Run2016F-02Apr2020-v1/NANOAOD": "MuonEG2016F_baby",
    #     "/MuonEG/Run2016G-02Apr2020-v1/NANOAOD": "MuonEG2016G_baby",
    #     "/MuonEG/Run2016H-02Apr2020-v1/NANOAOD": "MuonEG2016H_baby"
    # }
    # 2017
    sample_map = {
        "/MuonEG/Run2017B-02Apr2020-v1/NANOAOD": "MuonEG2017B_baby",
        "/MuonEG/Run2017C-02Apr2020-v1/NANOAOD": "MuonEG2017C_baby",
        "/MuonEG/Run2017D-02Apr2020-v1/NANOAOD": "MuonEG2017D_baby",
        "/MuonEG/Run2017E-02Apr2020-v1/NANOAOD": "MuonEG2017E_baby",
        "/MuonEG/Run2017F-02Apr2020-v1/NANOAOD": "MuonEG2017F_baby"
    }
    # 2018
    # sample_map = {
    #     "/MuonEG/Run2018A-02Apr2020-v1/NANOAOD": "MuonEG2018A_baby",
    #     "/MuonEG/Run2018B-02Apr2020-v1/NANOAOD": "MuonEG2018B_baby",
    #     "/MuonEG/Run2018C-02Apr2020-v1/NANOAOD": "MuonEG2018C_baby",
    #     "/MuonEG/Run2018D-02Apr2020-v1/NANOAOD": "MuonEG2018D_baby"
    # }
    # Submission tag
    tag = "baby_v3-0-0"
    # Loop over samples
    for dsname, shortname in sample_map.items():
        task = CondorTask(
            sample=DBSSample(
                dataset=dsname,
            ),
            files_per_output=1,
            output_name="output.root",
            tag=tag,
            condor_submit_params={"use_xrootd":True},
            cmssw_version="CMSSW_10_2_18",
            input_executable="inputs/condor_executable_metis.sh", # your condor executable here
            tarfile="inputs/package.tar.xz", # your tarfile with assorted goodies here
            special_dir="ttbarCR/babies/MuonEG", # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        # Straightforward logic
        if not task.complete():
            task.process()
        else:
            if not merge_task.complete():
                merge_task.process()


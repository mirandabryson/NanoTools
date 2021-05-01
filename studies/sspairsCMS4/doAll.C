{

    gROOT->ProcessLine(".L /home/users/ian/VBSAna/common/CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L /home/users/ian/NanoTools/studies/sspairsCMS4/leppairs.C++");
    gROOT->ProcessLine(".L /home/users/ian/NanoTools/studies/sspairsCMS4/ScanChain.C++");
    TChain *ch = new TChain("Events");

    // ch->Add("/hadoop/cms/store/user/jguiang/ProjectMetis/VBSWWHLEP_TuneCP5_13TeV-madgraph-pythia8_checkv4_07_2_100KE_PRIVATE_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM__v2/output_1.root");
//    ch->Add("/hadoop/cms/store/user/jguiang/ProjectMetis/VBSWWH_TuneCP5_13TeV_madgraph_pythia8_checkv4_01_2_100KE_PRIVATE_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM__v2/output_1.root");
    ch->Add("/hadoop/cms/store/user/jguiang/validation/cms4/ttbar_semileptonic_RunIISummer16CMS4_0.root");
    ScanChain(ch);
}

{
  gROOT->ProcessLine(".L ./NanoCORE/NANO_CORE.so");
  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *tchain1 = new TChain("Events"); 
  tchain1->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_0.root");
  ScanChain(tchain1, false);

  TChain *tchain2 = new TChain("Events"); 
  tchain2->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_1.root");
  ScanChain(tchain2, false);

  TChain *tchain3 = new TChain("Events"); 
  tchain3->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_2.root");
  ScanChain(tchain3, false);

  TChain *tchain4 = new TChain("Events"); 
  tchain4->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_3.root");
  ScanChain(tchain4, false);

  TChain *tchain5 = new TChain("Events"); 
  tchain5->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_4.root");
  ScanChain(tchain5, false);

  TChain *tchain6 = new TChain("Events"); 
  tchain6->Add("/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_5.root");
  ScanChain(tchain6, false);

}

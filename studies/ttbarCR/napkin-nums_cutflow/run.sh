echo "running 1"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_0.root --sample_name=TTJets2016 > counts/counts_0.txt
echo "running 2"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_1.root --sample_name=TTJets2016 > counts/counts_1.txt
echo "running 3"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_2.root --sample_name=TTJets2016 > counts/counts_2.txt
echo "running 4"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_3.root --sample_name=TTJets2016 > counts/counts_3.txt
echo "running 5"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_4.root --sample_name=TTJets2016 > counts/counts_4.txt
echo "running 6"
python doAll.py --input_file=/hadoop/cms/store/user/jguiang/ttbarCR/mc/ttbar/TTJets2016_inclusive_NANOAODSIMv7_5.root --sample_name=TTJets2016 > counts/counts_5.txt

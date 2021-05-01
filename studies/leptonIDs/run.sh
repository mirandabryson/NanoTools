echo "running electron selections"
echo "running tight over cms4"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_CMS4_0.root \
    --id_level=tight \
    --flavor=electron \
    --cms4 > cms4_tight_elec_cutflow.txt
echo "running tight over nano"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_NanoAODv7_0.root \
    --id_level=tight \
    --flavor=electron \
    --nano > nano_tight_elec_cutflow.txt
echo "running loose over cms4"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_CMS4_0.root \
    --id_level=loose \
    --flavor=electron \
    --cms4 > cms4_loose_elec_cutflow.txt
echo "running loose over nano"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_NanoAODv7_0.root \
    --id_level=loose \
    --flavor=electron \
    --nano > nano_loose_elec_cutflow.txt

echo "running muon selections"
echo "running tight over cms4"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_CMS4_0.root \
    --id_level=tight \
    --flavor=muon \
    --cms4 > cms4_tight_muon_cutflow.txt
echo "running tight over nano"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_NanoAODv7_0.root \
    --id_level=tight \
    --flavor=muon \
    --nano > nano_tight_muon_cutflow.txt
echo "running loose over cms4"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_CMS4_0.root \
    --id_level=loose \
    --flavor=muon \
    --cms4 > cms4_loose_muon_cutflow.txt
echo "running loose over nano"
python doAll.py \
    --input_file=samples/ttbar_semileptonic_NanoAODv7_0.root \
    --id_level=loose \
    --flavor=muon \
    --nano > nano_loose_muon_cutflow.txt

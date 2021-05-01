import os
basedir = "/home/users/ian/NanoTools/studies/sspairsanalysis/samples" 
years = ["/RunIISummer16NanoAOD", "/RunIIFall17NanoAOD", "/RunIIAutumn18NanoAOD"]

f = open("/home/users/ian/NanoTools/studies/sspairsanalysis/NormalWeight.txt", "w")
f.write("\"Normal Weights\" computed for each sample")

for year in years:
    f.write("\n" + year)
    tmpdirs = os.listdir(basedir + year)
    samples = []
    for tmpdir in tmpdirs:
        if tmpdir.count('.root'):
            continue
        else:
            samples.append(tmpdir)

    for sample in samples:
        f.write("\n" + sample + ": ")
        
        outputs = os.listdir(basedir + year + "/" + sample)
        logs = []

        for output in outputs:
            if output.count('.root'):
                continue
            elif output.count('skimmed'):
                continue
            elif (output == 'nevents.txt'):
                continue
            else:
                logs.append(output)

        NormalWeight = 0

        for log in logs:
            tmpfile = open(basedir + year + "/" + sample + "/" + log)
            logweight = tmpfile.readlines()
            #print(log)
            #print(int(logweight[3]))
            NormalWeight += int(logweight[3])
            tmpfile.close

        f.write(str(NormalWeight))
    f.write("\n")

f.close()

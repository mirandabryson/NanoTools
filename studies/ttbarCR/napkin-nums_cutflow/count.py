import glob

count_files = glob.glob("counts/counts_*.txt")

counts = {}
ordered_names = []
for i, count_file in enumerate(count_files):
    with open(count_file, "r") as f_in:
        for line in f_in.readlines():
            name, count = line.split()
            if i == 0:
                counts[name] = int(count)
                ordered_names.append(name)
            else:
                counts[name] += int(count)

for i, name in enumerate(ordered_names):
    count = counts[name]
    print("{0}: {1}".format(name, count))

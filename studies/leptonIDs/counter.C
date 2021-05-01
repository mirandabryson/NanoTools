#include <vector>
#include <string>

struct Counter {
    vector<int> counts;
    vector<string> names;
    string staged_cut;
    void newCut(string name) {
        staged_cut = name;
    }
    void pass() {
        names.push_back(staged_cut);
        if (counts.empty()) {
            counts.push_back(1);
        }
        else {
            counts.push_back(counts.back());
        }
        staged_cut = "";
    }
    void fail() {
        names.push_back(staged_cut);
        counts.push_back(0);
        staged_cut = "";
    }
};

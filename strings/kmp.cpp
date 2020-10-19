#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

class KMP {
    public:
    KMP() {
        this->prefF = {};
    }

    void prefixFunc(string s) {
        /**
         * Computes the prefix function for the given string 
         * which is defined as an array π of length n, where π[i] 
         * is the length of the longest proper prefix of the 
         * substring s[0…i] which is also a suffix of this substring.
        */
        int n = s.size();
        prefF = vector<int>(n);
        prefF[0] = 0;
        for (int i = 1; i < n; ++i) {
            int j = i;
            while (prefF[j-1]) {
                j = prefF[j-1];
                if (s[j] == s[i]) {
                    prefF[i] = j+1;
                    break;
                }
            }
            if (!prefF[i] && prefF[j-1] == 0) {
                prefF[i] = int(s[i] == s[0]);
            }
        }
    }

    vector<int> run(string text, string pattern) {
        /**
         * Given a text and a pattern, we want to 
         * find and display the positions of all 
         * occurrences of the string s in the text t.
        */
        int n = pattern.size();
        string separator = "#";
        string concat = pattern + separator + text;
        prefixFunc(concat);
        vector<int> res;
        for (int i = n+1; i < concat.size(); ++i) {
            if (prefF[i] == n) {
                res.emplace_back(i - 2*n);
            }
        }
        return res;
    }

    vector<int> getprefF(){
        return this->prefF;
    }

    private:
    vector<int> prefF;
    
};

int main() {
    KMP kmp = KMP();
    string text, pattern;
    cin >> text >> pattern;
    vector<int> res = kmp.run(text, pattern);
    cout << res.size() << "\n";
    return 0;
}
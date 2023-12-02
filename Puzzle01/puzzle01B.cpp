#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::string;
using std::vector;

int sum(const vector<int>& vec);
int start_num(const string& str, string::iterator iter);

int main() {
    ifstream infile("inputB.txt");
    string line;
    vector<int> nums;

    while (getline(infile, line, '\n')) {
        string num = "";
        for (auto i = line.begin(); i != line.end(); ++i) {
            int read;
            if (*i >= '0' && *i <= '9') {
                if (num.empty()) {
                    num.push_back(*i);   // digit character
                    num.push_back(' ');  // placeholder for 2nd digit
                } else {
                    num.back() = *i;  // replace
                }
            } else if ((read = start_num(line, i)) != -1) {
                if (num.empty()) {
                    num.push_back((char)(read + '0'));  // interpreted word val
                    num.push_back(' ');  // placeholder for 2nd digit
                } else {
                    num.back() = (char)(read + '0');  // replace
                }
            }
        }

        if (num.back() == ' ') {
            num.back() = num.front();
        }

        nums.push_back(stoi(num));
    }

    cout << sum(nums) << endl;

    infile.close();

    return 0;
}

int sum(const vector<int>& vec) {
    int sum = 0;

    for (auto i : vec) {
        sum += i;
    }

    return sum;
}

int start_num(const string& str, string::iterator iter) {
    vector<string> num_names = {
        "zero", "one", "two", "three", "four",
        "five", "six", "seven", "eight", "nine"
    };

    int res = -1;
    bool found = false;

    for (auto n = num_names.begin(); !found && n != num_names.end(); ++n) {
        const string& name = *n;

        if (str.find(name, iter - str.begin()) == (size_t)(iter - str.begin())) {
            found = true;
            res = (int)(n - num_names.begin());
        }
    }

    return res;
}
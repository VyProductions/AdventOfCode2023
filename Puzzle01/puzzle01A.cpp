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

int main() {
    ifstream infile("inputA.txt");
    string line;
    vector<int> nums;

    while (getline(infile, line, '\n')) {
        string num = "";
        for (auto i = line.begin(); i != line.end(); ++i) {
            if (*i >= '0' && *i <= '9') {
                if (num.empty()) {
                    num.push_back(*i);
                    num.push_back(' ');  // placeholder for 2nd digit
                } else {
                    num.back() = *i;  // replace
                }
            }
        }

        if (num.back() == ' ') {
            num.back() = num.front();
        }

        nums.push_back(stoi(num));
    }

    cout << sum(nums) << endl;

    return 0;
}

int sum(const vector<int>& vec) {
    int sum = 0;

    for (auto i : vec) {
        sum += i;
    }

    return sum;
}
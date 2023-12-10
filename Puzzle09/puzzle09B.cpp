#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

bool all_zeroes(vector<long>::iterator first, vector<long>::iterator last);

int main() {
    ifstream infile("inputB.txt");  // Input file handler
    vector<vector<long>> history;   // Rows of history data
    long data_point;                // Individual environment data input value
    size_t row = 0;                 // Which entry is being processed
    long extrapolate_sum = 0;       // Sum of extrapolated data for each entry

    // for each row,
    while (infile.peek() != '\n') {
        history.push_back(vector<long>{});

        // read each data point into the history entry for that row (reversed)
        while (infile.peek() != '\n') {
            infile >> data_point;
            history.at(row).insert(history.at(row).begin(), data_point);
        }

        infile.ignore();  // '\n'
        ++row;
    }

    // dynamic programming implementation for extrapolation:
    // each sub-computed row is one element shorter than the previous, and the
    // last element in each sub-computation is all that matters for
    // extrapolation, so this allows everything to happen within the same vector
    for (auto& entry : history) {
        size_t last_pos = entry.size() - 1;

        while (!all_zeroes(entry.begin(), entry.begin() + last_pos + 1)) {
            for (size_t i = 0; i < last_pos; ++i) {
                entry.at(i) = entry.at(i) - entry.at(i+1);
            }

            --last_pos;
        }

        // extrapolate by passing the difference from front to back
        // the entry is reversed, so this take the first value of each
        // sub-computation starting with the most recent one and iterates toward
        // the first value of the initial entry to predict the previous data
        long difference = entry.at(0);

        for (size_t i = 1; i < entry.size(); ++i) {
            difference = entry.at(i) - difference;
        }

        extrapolate_sum += difference;
    }

    cout << extrapolate_sum << endl;

    infile.close();

    return 0;
}

bool all_zeroes(vector<long>::iterator first, vector<long>::iterator last) {
    bool zeroes = *first == 0;

    while (zeroes && first != last) {
        zeroes = *first++ == 0;
    }

    return zeroes;
}

/*
    10 | 9 10 20 47 106 220 423 776 1406 2579 4829 9202 17771 34815 69626 143262 304678 668562 1501903 3416589 7785735
    -1 | 1 10 27 59 114 203 353 630 1173 2250 4373 8569 17044 34811 73636 161416 363884 833341 1914686 4369146
     2 | 9 17 32 55 89 150 277 543 1077 2123 4196 8475 17767 38825 87780 202468 469457 1081345 2454460
     7 | 8 15 23 34 61 127 266 534 1046 2073 4279 9292 21058 48955 114688 266989 611888 1373115
     1 | 7 8 11 27 66 139 268 512 1027 2206 5013 11766 27897 65733 152301 344899 761227
     6 | 1 3 16 39 73 129 244 515 1179 2807 6753 16131 37836 86568 192598 416328
    -5 | 2 13 23 34 56 115 271 664 1628 3946 9378 21705 48732 106030 223730
     7 | 11 10 11 22 59 156 393 964 2318 5432 12327 27027 57298 117700
     4 | -1 1 11 37 97 237 571 1354 3114 6895 14700 30271 60402
    -5 | 2 10 26 60 140 334 783 1760 3781 7805 15571 30131
     7 | 8 16 34 80 194 449 977 2021 4024 7766 14560
     1 | 8 18 46 114 255 528 1044 2003 3742 6794
     7 | 10 28 68 141 273 516 959 1739 3052
     3 | 18 40 73 132 243 443 780 1313
    15 | 22 33 59 111 200 337 533
     7 | 11 26 52 89 137 196
     4 | 15 26 37 48 59
    11 | 11 11 11 11
     0 | 0 0 0
*/

#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

int main() {
    ifstream infile("inputA.txt");  // Input file handler
    vector<vector<long>> history;   // Rows of history data
    long data_point;                // Individual environment data input value
    size_t row = 0;                 // Which entry is being processed
    long extrapolate_sum = 0;       // Sum of extrapolated data for each entry

    // for each row,
    while (infile.peek() != '\n') {
        history.push_back(vector<long>{});

        // read each data point into the history entry for that row
        while (infile.peek() != '\n') {
            infile >> data_point;
            history.at(row).push_back(data_point);
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

        while (entry.at(last_pos - 1) != 0) {
            for (size_t i = 0; i < last_pos; ++i) {
                entry.at(i) = entry.at(i + 1) - entry.at(i);
            }

            --last_pos;
        }

        // extrapolate by summing differences after last sub-computation
        long row_sum = 0;

        for (const long& data : entry) {
            row_sum += data;
        }

        extrapolate_sum += row_sum;
    }

    cout << extrapolate_sum << endl;

    infile.close();

    return 0;
}

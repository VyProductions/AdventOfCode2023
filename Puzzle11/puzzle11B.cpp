#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::to_string;
using std::vector;

struct pos_t {
    int row;  // Row coordinate of position in a 2d array
    int col;  // Column coordinate of position in a 2d array

    string operator()() const {
        return string{"("} + to_string(row) + "," + to_string(col) + ")";
    }

    bool operator==(const pos_t& rhs) const {
        return row == rhs.row && col == rhs.col;
    }
};

template <class T>
inline T min(T a, T b) {
    return a < b ? a : b;
}

template <class T>
inline T max(T a, T b) {
    return a > b ? a : b;
}

int main() {
    ifstream infile("inputB.txt");  // Input file handler
    vector<vector<char>> universe;  // 2D grid for all the galaxies in the input
    vector<pos_t> galaxy_pos;       // Positions of galaxies in the
    vector<bool> row_galaxies;      // Determines if nth row has a galaxy
    vector<bool> col_galaxies;      // Determines if nth col has a galaxy
    pos_t pos {0, 0};               // Traversal position for input
    size_t length_sum = 0;          // Sum of path lengths between galaxies

    while (infile.peek() != '\n') {
        pos.col = 0;

        universe.push_back({});
        row_galaxies.push_back(false);

        while (infile.peek() != '\n') {
            universe.at(pos.row).push_back((char)infile.get());

            row_galaxies.at(pos.row) = (
                row_galaxies.at(pos.row) || universe.at(pos.row).back() == '#'
            );

            if (pos.row == 0) {
                col_galaxies.push_back(false);
            }

            col_galaxies.at(pos.col) = (
                col_galaxies.at(pos.col) || universe.at(pos.row).back() == '#'
            );

            if (universe.at(pos.row).back() == '#') {
                galaxy_pos.push_back(pos);
            }

            ++pos.col;
        }

        ++pos.row;

        infile.ignore();  // '\n'
    }

    // get path length between ith and jth position
    //  - uses taxicab distance formula (x_dist + y_dist)
    for (size_t i = 0; i < galaxy_pos.size() - 1; ++i) {
        pos_t& i_pos = galaxy_pos.at(i);  // Reference to first galaxy position

        // get all unprocessed paths from current galaxy to all other galaxies
        for (size_t j = i + 1; j < galaxy_pos.size(); ++j) {
            // Reference to other galaxy position
            pos_t& j_pos = galaxy_pos.at(j);

            // Top left point to simplify distance arithmetic
            pos_t top_left = {
                min(i_pos.row, j_pos.row), min(i_pos.col, j_pos.col)
            };

            // Bottom right point to simplify distance arithmetic
            pos_t bottom_right = {
                max(i_pos.row, j_pos.row), max(i_pos.col, j_pos.col)
            };

            // count empty rows between i_pos and j_pos
            size_t rows_between = 0;

            for (int r = top_left.row + 1; r < bottom_right.row; ++r) {
                if (!row_galaxies.at(r)) {
                    // 1e6 times larger per empty row
                    // we already count the first blank row in the initial
                    // distance between galaxies, so add 999'999 to the count
                    rows_between += 999'999;
                }
            }

            // count empty cols between i_pos and j_pos
            size_t cols_between = 0;

            for (int c = top_left.col + 1; c < bottom_right.col; ++c) {
                if (!col_galaxies.at(c)) {
                    // 1e6 times larger per empty column
                    // we already count the first blank column in the initial
                    // distance between galaxies, so add 999'999 to the count
                    cols_between += 999'999;
                }
            }

            // compute taxicab distance while factoring in extra spaces
            size_t path_length = (
                (bottom_right.row - top_left.row) +  // row distance
                (bottom_right.col - top_left.col) +  // column distance
                rows_between + cols_between          // spaces from expansion
            );

            length_sum += path_length;
        }
    }

    cout << length_sum << endl;

    return 0;
}

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::to_string;
using std::unordered_map;

const int DIM = 140;  // Size of square data array for input storage

struct gear_t {
    int adjCount;   // Number of adjacencies
    string adj[6];  // Max of 6 adjacencies:
                    //  Top Left, T. Right, Left, Right, Bottom Left, B. Right
};

bool hasAdjacentGear(char data[][DIM + 2], int r, int c, int& R, int& C);

int main() {
    ifstream infile("inputB.txt");  // Input file handler

    // 140 x 140 array with 1-width buffer ring
    char data[DIM + 2][DIM + 2] {};

    // keep track of gears and adjacencies (gear postion str -> gear_t instance)
    unordered_map<string, gear_t> gears;

    for (int r = 1; r <= DIM; ++r) {
        for (int c = 1; c <= DIM; ++c) {
            if (infile.peek() != '.') {
                infile >> data[r][c];
            } else {
                infile.ignore();
            }
        }
        infile.ignore();  // '\n'
    }

    for (int r = 1; r <= DIM; ++r) {
        bool adj = false;  // Assume characters are not adjacent until they are
        string res = "";   // Formed string from horizontally adjacent numbers
        int c = 1;         // Column index
        int rg;            // Row of gear
        int cg;            // Column of gear

        while (c <= DIM) {
            // find leading num
            while (
                c <= DIM && (data[r][c] < '0' || data[r][c] > '9')
            ) {
                ++c;
            }

            // form string from nums until next non num
            while (
                c <= DIM && data[r][c] >= '0' && data[r][c] <= '9'
            ) {
                adj = adj || hasAdjacentGear(data, r, c, rg, cg);
                res += data[r][c];
                ++c;
            }

            if (adj) {
                // row/col position of gear
                string gear_pos = to_string(rg) + "," + to_string(cg);
                // amount of adjacent numbers to gear
                int& count = gears[gear_pos].adjCount;

                gears[gear_pos].adj[count] = res;
                count++;
            }

            // reset string
            res = "";

            // reset adjacency detection
            adj = false;
        }
    }

    // Running total of gear values
    int sum = 0;

    for (auto& [pos, gear] : gears) {
        // If gear has exactly two adjacent numbers,
        // add the gear value (ratio 1 * ratio 2) to the sum
        if (gear.adjCount == 2) {
            sum += stoi(gear.adj[0]) * stoi(gear.adj[1]);
        }
    }

    cout << sum << endl;

    infile.close();

    return 0;
}

bool hasAdjacentGear(char data[][DIM + 2], int r, int c, int& R, int& C) {
    // Adjacency defined within the 8 surrounding cells of a center cell for a
    // 3x3 grid. Adjaceny does not exist for numbers in the same row.
    return (data[r-1][c-1] == '*' && (R = r-1) && (C = c-1)) ||
           (data[r-1][c]   == '*' && (R = r-1) && (C = c  )) ||
           (data[r-1][c+1] == '*' && (R = r-1) && (C = c+1)) ||
           (data[r][c-1]   == '*' && (R = r  ) && (C = c-1)) ||
           (data[r][c+1]   == '*' && (R = r  ) && (C = c+1)) ||
           (data[r+1][c-1] == '*' && (R = r+1) && (C = c-1)) ||
           (data[r+1][c]   == '*' && (R = r+1) && (C = c  )) ||
           (data[r+1][c+1] == '*' && (R = r+1) && (C = c+1));
}

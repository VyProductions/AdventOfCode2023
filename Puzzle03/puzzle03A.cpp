#include <fstream>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

const int DIM = 140;  // Size of square data array for input storage

bool hasAdjacent(char data[][DIM + 2], int r, int c);

int main() {
    ifstream infile("inputA.txt");  // Input file handler

    // 140 x 140 array with 1-width buffer ring
    char data[DIM + 2][DIM + 2] {};

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

    // Running total of part numbers
    int sum = 0;

    for (int r = 1; r <= DIM; ++r) {
        bool adj = false;  // Assume characters are not adjacent until they are
        string res = "";   // Formed string from horizontally adjacent numbers
        int c = 1;         // Column index

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
                adj = adj || hasAdjacent(data, r, c);
                res += data[r][c];
                ++c;
            }

            // convert string to int and add to sum if a number was formed, but
            // only if it has adjacent symbols
            if (!res.empty() && adj) {
                sum += stoi(res);
            }

            // reset string
            res = "";

            // reset adjacency detection
            adj = false;
        }
    }

    cout << sum << endl;

    infile.close();

    return 0;
}

bool hasAdjacent(char data[][DIM + 2], int r, int c) {
    // Adjacency defined within the 8 surrounding cells of a center cell for a
    // 3x3 grid. Adjaceny does not exist for numbers in the same row.
    return data[r-1][c-1] != 0 ||
           data[r-1][c]   != 0 ||
           data[r-1][c+1] != 0 ||
           (data[r][c-1] != 0 && (data[r][c-1] < '0' || data[r][c-1] > '9')) ||
           (data[r][c+1] != 0 && (data[r][c+1] < '0' || data[r][c+1] > '9')) ||
           data[r+1][c-1] != 0 ||
           data[r+1][c]   != 0 ||
           data[r+1][c+1] != 0;
}

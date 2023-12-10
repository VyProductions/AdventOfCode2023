#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::list;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

const int DIM = 140;  // Max dimension of input board data

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

template <>
struct std::hash<pos_t> {
    size_t operator()(const pos_t& pos) const {
        return std::hash<string>()(pos());
    }
};

list<pos_t> get_neighbors(const char pipe_board[][DIM + 2], const pos_t& pos);

int main() {
    ifstream infile("inputA.txt");         // Input file handler
    char pipe_board[DIM + 2][DIM + 2] {};  // 2d array for pipe characters
    pos_t cow_position;                    // Position of cow in pipe board
    pos_t pos = {1, 1};                    // Traversal position for input
    size_t max_dist = 0;                   // Max distance away from cow in pipe
    pos_t max_pos;                         // Position of pipe with max distance

    // read input into pipe_board
    while (infile.peek() != '\n') {
        pos.col = 1;

        while (infile.peek() != '\n') {
            if (infile.peek() != '.') {
                infile >> pipe_board[pos.row][pos.col];

                if (pipe_board[pos.row][pos.col] == 'S') {
                    cow_position = pos;
                }
            } else {
                pipe_board[pos.row][pos.col] = ' ';
                infile.ignore();  // '.'
            }

            pos.col++;
        }

        pos.row++;
        infile.ignore();  // '\n'
    }

    // link all the pipes together around the cow

    // pair an unprocessed position to its distance from the cow
    vector<pair<pos_t, size_t>> unprocessed_pipes {{cow_position, 0}};

    // determines if a pipe has been processed or not from the cow's pipe loop
    unordered_map<pos_t, bool> pipe_state {{cow_position, false}};

    // maps a pipe's position to its distance from the cow
    unordered_map<pos_t, size_t> pipe_distances {{cow_position, 0}};

    while (!unprocessed_pipes.empty()) {
        pos_t  curr_pos  = unprocessed_pipes.begin()->first;
        size_t curr_dist = unprocessed_pipes.begin()->second;

        // move current position from unprocessed to processed
        unprocessed_pipes.erase(unprocessed_pipes.begin());
        pipe_state[curr_pos] = true;

        // add the neighbors which are not aleady processed to the unproc list
        list<pos_t> neighbors = get_neighbors(pipe_board, curr_pos);

        for (const pos_t& neigh : neighbors) {
            // update distance based on best between current pos and
            // possibly other pipe connection that has already been proc'd

            // reference to distance from cow to neighbor pipe
            size_t& neigh_dist = pipe_distances[neigh];

            // if distance has not been computed, or if the distance from this
            // pipe to the neighbor is closer to the cow
            if (neigh_dist == 0 || neigh_dist > curr_dist + 1) {
                // update the neighbor pipe's distance
                neigh_dist = curr_dist + 1;
            }

            // if the neighbor hasn't yet been processed,
            // add it to the unprocessed list
            if (!pipe_state[neigh]) {
                unprocessed_pipes.push_back({neigh, pipe_distances[neigh]});
                sort(
                    unprocessed_pipes.begin(), unprocessed_pipes.end(),
                    [] (pair<pos_t, size_t>& a, pair<pos_t, size_t>& b) {
                        return a.second < b.second;
                    }
                );
            }
        }
    }

    // compute maximum distance from cow
    for (auto& [position, dist] : pipe_distances) {
        if (max_dist < dist) {
            max_dist = dist;
            max_pos = position;
        }
    }

    // print board and max distance
    cout << "  Cow is at " << cow_position() << endl;

    for (int i = 1; i < pos.row; ++i) {
        cout << "  ";
        for (int j = 1; j < pos.col; ++j) {
            if (pipe_state[{i, j}]) {
                cout << pipe_board[i][j];
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    cout << "  Max distance: " << max_dist << ", at position " + max_pos()
         << endl;

    infile.close();

    return 0;
}

list<pos_t> get_neighbors(const char pipe_board[][DIM + 2], const pos_t& pos) {
    list<pos_t> neighbors;  // List of valid adjacent pipes to given position

    #define CHECK_NORTH() {\
        if (\
            pipe_board[pos.row - 1][pos.col] == 'S' ||\
            pipe_board[pos.row - 1][pos.col] == '|' ||\
            pipe_board[pos.row - 1][pos.col] == '7' ||\
            pipe_board[pos.row - 1][pos.col] == 'F'\
        ) {\
            neighbors.push_back({pos.row - 1, pos.col});\
        }\
    }

    #define CHECK_SOUTH() {\
        if (\
            pipe_board[pos.row + 1][pos.col] == 'S' ||\
            pipe_board[pos.row + 1][pos.col] == '|' ||\
            pipe_board[pos.row + 1][pos.col] == 'L' ||\
            pipe_board[pos.row + 1][pos.col] == 'J'\
        ) {\
            neighbors.push_back({pos.row + 1, pos.col});\
        }\
    }

    #define CHECK_EAST() {\
        if (\
            pipe_board[pos.row][pos.col + 1] == 'S' ||\
            pipe_board[pos.row][pos.col + 1] == '-' ||\
            pipe_board[pos.row][pos.col + 1] == 'J' ||\
            pipe_board[pos.row][pos.col + 1] == '7'\
        ) {\
            neighbors.push_back({pos.row, pos.col + 1});\
        }\
    }

    #define CHECK_WEST() {\
        if (\
            pipe_board[pos.row][pos.col - 1] == 'S' ||\
            pipe_board[pos.row][pos.col - 1] == '-' ||\
            pipe_board[pos.row][pos.col - 1] == 'L' ||\
            pipe_board[pos.row][pos.col - 1] == 'F'\
        ) {\
            neighbors.push_back({pos.row, pos.col - 1});\
        }\
    }

    switch (pipe_board[pos.row][pos.col]) {
        case 'S':
            // check four sides for the two valid pipes
            CHECK_NORTH();
            CHECK_SOUTH();
            CHECK_EAST();
            CHECK_WEST();
            break;
        case '|':
            // check two sides for the two valid pipes
            CHECK_NORTH();
            CHECK_SOUTH();
            break;
        case '-':
            // check two sides for the two valid pipes
            CHECK_EAST();
            CHECK_WEST();
            break;
        case 'L':
            // check two sides for the two valid pipes
            CHECK_NORTH();
            CHECK_EAST();
            break;
        case 'J':
            // check two sides for the two valid pipes
            CHECK_NORTH();
            CHECK_WEST();
            break;
        case '7':
            // check two sides for the two valid pipes
            CHECK_SOUTH();
            CHECK_WEST();
            break;
        case 'F':
            // check two sides for the two valid pipes
            CHECK_SOUTH();
            CHECK_EAST();
            break;
        default:
            break;
    }

    return neighbors;
}

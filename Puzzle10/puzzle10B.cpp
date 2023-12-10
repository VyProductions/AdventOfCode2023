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

list<pos_t> pipeneighbors(char pipe_board[][2 * DIM + 3], const pos_t& pos);
list<pos_t> spaceneighbors(
    char pipe_board[][2 * DIM + 3], const pos_t& pos, const pos_t& bound_pt
);

int main() {
    ifstream infile("inputB.txt");  // Input file handler

    char pipe_board[2 * DIM + 3][2 * DIM + 3] {};  // 2d array for pipe chars
    pos_t cow_position;   // Position of cow in pipe board
    pos_t pos = {2, 2};   // Traversal position for input

    // read input into pipe_board
    while (infile.peek() != '\n') {
        pos.col = 2;

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

            pos.col += 2;
        }

        pos.row += 2;
        infile.ignore();  // '\n'
    }

    // link all the pipes together around the cow

    // listo of unprocessed pipe positions
    list<pos_t> unprocessed_pipes {cow_position};

    // determines if a pipe has been processed or not from the cow's pipe loop
    unordered_map<pos_t, bool> pipe_state {{cow_position, false}};

    while (!unprocessed_pipes.empty()) {
        pos_t curr_pos = unprocessed_pipes.front();

        // move current position from unprocessed to processed
        unprocessed_pipes.pop_front();
        pipe_state[curr_pos] = true;

        // add the neighbors which are not aleady processed to the unproc list
        list<pos_t> neighbors = pipeneighbors(pipe_board, curr_pos);

        for (const pos_t& neigh : neighbors) {
            // if the neighbor hasn't yet been processed,
            // add it to the unprocessed list
            if (!pipe_state[neigh]) {
                unprocessed_pipes.push_back(neigh);
            }
        }
    }

    // replace pipe characters that are not in the pipe loop with spaces
    for (int i = 2; i < pos.row; i += 2) {
        for (int j = 2; j < pos.col; j += 2) {
            pos_t p = {i, j};

            if (!pipe_state[p]) {
                pipe_board[i][j] = ' ';
            }
        }
    }

    // flood-fill to find outside values

    // list of unprocessed space positions
    list<pos_t> unprocessed_spaces {{0, 0}};

    // state constants for the space_state mapping
    enum space_state {SP_VOID, SP_UNPROCESSED, SP_PROCESSED};

    // determines if a space is outside the pipe loop or not
    unordered_map<pos_t, int> space_state {{{0, 0}, SP_UNPROCESSED}};

    while (!unprocessed_spaces.empty()) {
        pos_t curr_pos = unprocessed_spaces.front();

        // move current position from unprocessed to processed
        unprocessed_spaces.pop_front();
        space_state[curr_pos] = SP_PROCESSED;

        // add the neighbors which are not already processed to the unproc list
        list<pos_t> neighbors = spaceneighbors(pipe_board, curr_pos, pos);

        for (const pos_t& neigh : neighbors) {
            // if the neighbor hasn't yet been seen,
            // add it to the unprocessed list
            if (space_state[neigh] == SP_VOID) {
                unprocessed_spaces.push_back(neigh);
                space_state[neigh] = SP_UNPROCESSED;
            }
        }
    }

    // print board and details
    size_t inside_count = 0;   // Number of spaces inside the pipe loop
    size_t outside_count = 0;  // Number of spaces outside the pipe loop

    for (int i = 2; i < pos.row; i += 2) {
        cout << "  ";
        for (int j = 2; j < pos.col; j += 2) {
            pos_t p = {i, j};
            const char& ch = pipe_board[i][j];

            if (ch == ' ' && space_state[p] == SP_VOID) {
                ++inside_count;
                cout << 'I';
            } else if (ch == ' ' && space_state[p] == SP_PROCESSED) {
                ++outside_count;
                cout << 'O';
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "Outside: " << outside_count << endl;
    cout << "Inside:  " << inside_count << endl;

    infile.close();

    return 0;
}

list<pos_t> pipeneighbors(char pipe_board[][2 * DIM + 3], const pos_t& pos) {
    list<pos_t> neighbors;  // List of valid adjacent pipes to given position

    #define CHECK_NORTH() {\
        if (\
            pipe_board[pos.row - 2][pos.col] == 'S' ||\
            pipe_board[pos.row - 2][pos.col] == '|' ||\
            pipe_board[pos.row - 2][pos.col] == '7' ||\
            pipe_board[pos.row - 2][pos.col] == 'F'\
        ) {\
            neighbors.push_back({pos.row - 2, pos.col});\
            pipe_board[pos.row - 1][pos.col] = '|';\
        }\
    }

    #define CHECK_SOUTH() {\
        if (\
            pipe_board[pos.row + 2][pos.col] == 'S' ||\
            pipe_board[pos.row + 2][pos.col] == '|' ||\
            pipe_board[pos.row + 2][pos.col] == 'L' ||\
            pipe_board[pos.row + 2][pos.col] == 'J'\
        ) {\
            neighbors.push_back({pos.row + 2, pos.col});\
            pipe_board[pos.row + 1][pos.col] = '|';\
        }\
    }

    #define CHECK_EAST() {\
        if (\
            pipe_board[pos.row][pos.col + 2] == 'S' ||\
            pipe_board[pos.row][pos.col + 2] == '-' ||\
            pipe_board[pos.row][pos.col + 2] == 'J' ||\
            pipe_board[pos.row][pos.col + 2] == '7'\
        ) {\
            neighbors.push_back({pos.row, pos.col + 2});\
            pipe_board[pos.row][pos.col + 1] = '-';\
        }\
    }

    #define CHECK_WEST() {\
        if (\
            pipe_board[pos.row][pos.col - 2] == 'S' ||\
            pipe_board[pos.row][pos.col - 2] == '-' ||\
            pipe_board[pos.row][pos.col - 2] == 'L' ||\
            pipe_board[pos.row][pos.col - 2] == 'F'\
        ) {\
            neighbors.push_back({pos.row, pos.col - 2});\
            pipe_board[pos.row][pos.col - 1] = '-';\
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

list<pos_t> spaceneighbors(
    char pipe_board[][2 * DIM + 3], const pos_t& pos, const pos_t& bound_pt
) {
    list<pos_t> neighbors;  // List of adjacent spaces to given position

    // offsets from given position to find 
    pos_t neigh_offs[4] {
        {-1, 0}, {1, 0}, {0, 1}, {0, -1}
    };

    // check four sides for up to 4 possible neighboring spaces
    for (int i = 0; i < 4; ++i) {
        const pos_t& offs = neigh_offs[i];         // Reference to pos offset
        const int check_row = pos.row + offs.row;  // Compute target row
        const int check_col = pos.col + offs.col;  // Compute target col

        // if the offset position is in range of the pipe board's active area
        if (
            check_row >= 0 && check_row <= bound_pt.row &&
            check_col >= 0 && check_col <= bound_pt.col
        ) {
            // if the neighboring character is not a pipe / is a space
            if (
                pipe_board[check_row][check_col] == ' ' ||
                pipe_board[check_row][check_col] == '\0'
            ) {
                neighbors.push_back({check_row, check_col});
            }
        }
    }

    return neighbors;
}

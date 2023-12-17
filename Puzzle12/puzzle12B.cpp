#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

struct record_t {
    string         states;   // Known/unknown state of each spring
    vector<size_t> lengths;  // Continuous damaged spring group lengths
};

static unordered_map<
    string,  // Record state string
    unordered_map<
        string,  // Record lengths in textual representation
        pair<bool, size_t>  // (initialized, permutation_count) value for memo
    >
> perm_memo;  // Memoization for recursively counting permutations

istream& operator>>(istream& in, record_t& record);
size_t perm(string str, vector<size_t> lens);

int main() {
    ifstream infile("inputB.txt");  // Input file handler
    record_t record;                // Record instance for input
    size_t permute_sum = 0;         // Possible valid permutations of records

    while (infile >> record) {
        size_t count = perm(record.states, record.lengths);
        permute_sum += count;
    }
    cout << permute_sum << endl;

    return 0;
}

istream& operator>>(istream& in, record_t& record) {
    // clear record
    record.states.clear();
    record.lengths.clear();

    // read spring states
    while (in && in.peek() != ' ') {
        char ch = (char)in.get();

        if (ch != '.' || record.states.back() != '.') {
            record.states.push_back(ch);
        }
    }

    // read continuity lengths
    size_t len;  // Length of continuous damage spring group

    do {
        in.ignore();  // ' ', or ',' before number
        in >> len;
        record.lengths.push_back(len);
    } while (in && in.peek() != '\n');

    in.ignore();  // '\n'

    // create duplicates
    string init_str = record.states;
    vector<size_t> lens = record.lengths;

    for (int i = 0; i < 4; ++i) {
        record.states += "?" + init_str;

        for (auto& length : lens) {
            record.lengths.push_back(length);
        }
    }

    return in;
}

size_t perm(string str, vector<size_t> lens) {
    // match found, count this as 1 permutation
    if (str.empty() && lens.empty()) return 1;

    // mismatch found, discard this permutation
    if (str.empty() && !lens.empty()) return 0;

    string len_str = "";  // Textual representation of length vector content
    char sep[2] = "";     // Separator between length values in string

    // turn length vector into a string
    for (size_t len : lens) {
        len_str += sep + to_string(len);
        *sep = ',';
    }

    // Memo retrieved using record's state and lengths as an identifier
    pair<bool, size_t>& memo = perm_memo[str][len_str];

    // memo exists, return pre-computed result
    if (memo.first) return memo.second;

    // if the state string still has content, but there are no more lengths
    if (!str.empty() && lens.empty()) {
        for (char ch : str) {
            // if the state string has any remaining '#' characters,
            if (ch == '#') {
                // discard this permutation
                memo = {true, 0};
                return 0;
            }
        }

        // otherwise, count this as 1 permutation
        memo = {true, 1};
        return 1;
    }

    // cout << string(depth * 2, ' ') + str << ' ';
    // for (auto len : lens) cout << len << ' ';
    // cout << endl;

    size_t& len = lens.front();
    char ch = str.front();
    size_t sum = 0;

    switch (ch) {
        case '.':  // skip to the next character
            sum += perm(string(str.begin() + 1, str.end()), lens);
            break;
        case '?':  // check by replacing unknown with both '.' and '#'
            sum += perm("." + string(str.begin() + 1, str.end()), lens);
            sum += perm("#" + string(str.begin() + 1, str.end()), lens);
            break;
        case '#':
            {
                // Check if the next 'len' characters matches the next length
                bool valid = str.size() >= len;
                for (size_t i = 0; valid && i < len; ++i) {
                    // impossible to have 'len' '#' characters with a '.' in it
                    valid = str.at(i) != '.';
                }

                // not the right amount of '#' would make this permutation invalid
                valid = valid && (str.size() == len || str.at(len) != '#');

                if (!valid) {
                    // discard this permutation
                    memo = {true, 0};
                    return 0;
                }

                // Where to begin the next recursion in the string relative to
                // 'len' characters ahead.
                // If there are more characters, the 'len + 1'th character must
                // be a '.', so we skip it
                size_t offs = str.size() == len ? 0 : 1;

                // recurse after the '#' set at front of state string,
                // and without the first length value in the lengths vector
                sum += perm(
                    string(str.begin() + len + offs, str.end()),
                    vector<size_t>(lens.begin() + 1, lens.end())
                );
            }
            break;
        default:
            break;
    }

    memo = {true, sum};

    return sum;
}
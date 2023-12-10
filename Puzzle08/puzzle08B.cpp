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
using std::unordered_map;
using std::vector;

struct node_t {
    string label;  // Node name
    string left;   // Name of node after a left-traversal
    string right;  // Name of node after a right-traversal
};

istream& operator>>(istream& in, node_t& node);
size_t LCM(vector<size_t> numbers);
vector<pair<size_t, size_t>> prime_factor(size_t number);
size_t next_prime(size_t prime);
bool is_prime(size_t number);

int main() {
    ifstream infile("inputB.txt");        // Input file handler
    unordered_map<string, node_t> graph;  // Nodes to get from 'XXA' to 'XXZ' by
    string directions;                    // R/L directions to locate 'XXZ'
    node_t node;                          // A node to populate from the input
    vector<string> start_labels;          // Labels for nodes ending in 'A'
    vector<string> end_labels;            // Labels for nodes ending in 'Z'
    vector<string> travel_labels;         // Labels during graph traversal
    vector<size_t> path_lengths;          // Length of traversal to end node

    // read directions
    infile >> directions;

    // read nodes
    while (infile >> node) {
        graph[node.label] = node;

        // collect all starting nodes of the form 'XXA'
        if (node.label.at(2) == 'A') {
            start_labels.push_back(node.label);
        // collect all ending nodes of the form 'XXZ'
        } else if (node.label.at(2) == 'Z') {
            end_labels.push_back(node.label);
        }
    }

    size_t dir_idx ;        // Index into the direction string (idx % size())
    size_t step_count;      // How many steps it took to get from 'AAA' to 'ZZZ'

    // count how many steps each starting node takes to get to any end node
    // using the direction string
    for (size_t i = 0; i < start_labels.size(); ++i) {
        string& label = start_labels.at(i);  // Reference to a start node label
        dir_idx = 0;
        step_count = 0;

        // get start -> end step length
        while (label.at(2) != 'Z') {
            const char& which = directions.at(dir_idx);  // L/R direction value

            if (which == 'L') {
                label = graph[label].left;
            } else {
                label = graph[label].right;
            }

            ++step_count;
            dir_idx = (dir_idx + 1) % directions.size();
        }

        // store length for Least Common Multiple computation
        path_lengths.push_back(step_count);
    }

    cout << LCM(path_lengths) << endl;

    return 0;
}

istream& operator>>(istream& in, node_t& node) {
    in >> node.label;              // "ABC"
    in.ignore(4);                  // " = ("
    getline(in, node.left, ',');   // "DEF"; ignore ','
    in.ignore();                   // ' '
    getline(in, node.right, ')');  // "GHI"; ignore ')'

    return in;
}

size_t LCM(vector<size_t> numbers) {
    size_t result = 1;

    // Each factor with the highest power so far for the LCM computation
    unordered_map<size_t, size_t> mult_targets;

    // collect factors and powers
    for (const size_t& num : numbers) {
        // Prime factorization of each number
        vector<pair<size_t, size_t>> factors = prime_factor(num);

        for (auto& [factor, power] : factors) {
            if (mult_targets[factor] < power) {
                mult_targets[factor] = power;
            }
        }
    }

    // compute LCM by multiplying greatest factor^power instances together
    for (auto& [factor, power] : mult_targets) {
        for (size_t i = 0; i < power; ++i) {
            result *= factor;
        }
    }

    return result;
}

vector<pair<size_t, size_t>> prime_factor(size_t number) {
    vector<pair<size_t, size_t>> factors;  // Collection of prime factors

    while (number != 1) {
        size_t i = 2;  // First prime to check for divisibility

        while (i <= number) {
            if (number % i == 0) {
                // If factor already exists, increase its power by 1
                if (!factors.empty() && factors.back().first == i) {
                    factors.back().second++;
                // Otherwise, add it to the list
                } else {
                    factors.push_back({i, 1});
                }

                number /= i;
                i = factors.back().first;  // repeat last prime validated
            } else {
                i = next_prime(i);
            }
        }
    }

    return factors;
}

size_t next_prime(size_t prime) {
    size_t result = prime == 2 ? 3 : prime + 2;

    while (!is_prime(result)) {
        result += 2;
    }

    return result;
}

bool is_prime(size_t number) {
    bool primality = number % 2 != 0;

    for (size_t i = 3; primality && i * i < number; i += 2) {
        primality = number % i != 0;
    }

    return primality;
}

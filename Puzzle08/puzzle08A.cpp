#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::string;
using std::unordered_map;

struct node_t {
    string label;  // Node name
    string left;   // Name of node after a left-traversal
    string right;  // Name of node after a right-traversal
};

istream& operator>>(istream& in, node_t& node);

int main() {
    ifstream infile("inputA.txt");        // Input file handler
    unordered_map<string, node_t> graph;  // Nodes to get from 'AAA' to 'ZZZ' by
    string directions;                    // R/L directions to locate 'ZZZ'
    node_t node;                          // A node to populate from the input

    // read directions
    infile >> directions;

    // read nodes
    while (infile >> node) {
        graph[node.label] = node;
    }

    string start = "AAA";   // Name of starting node
    size_t dir_idx = 0;     // Index into the direction string (idx % size())
    size_t step_count = 0;  // How many steps it took to get from 'AAA' to 'ZZZ'

    while (start != "ZZZ") {
        const char& which = directions.at(dir_idx);

        if (which == 'L') {
            start = graph[start].left;
        } else {
            start = graph[start].right;
        }

        ++step_count;
        dir_idx = (dir_idx + 1) % directions.size();  // Wrap back around at end
    }

    cout << step_count << endl;

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

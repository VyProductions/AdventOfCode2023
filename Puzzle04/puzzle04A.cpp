#include <fstream>
#include <iostream>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::unordered_map;

int main() {
    ifstream infile("inputA.txt");  // Input file handler
    int score_sum = 0;  // Sum of card scores per scratch ticket

    while (infile.peek() == 'C') {
        unordered_map<int, bool> held_map;  // O(1) lookup for held scratch nums
        int matching = 0;  // Number of matching scratches on current card
        int scratch;       // Scratch number

        infile.ignore(20, ':');  // "Game #:"
        infile.ignore();         // ' ' after ':'

        // read held scratcher values
        while (infile.peek() != '|') {
            infile >> scratch;
            held_map[scratch] = true;  // mark scratch as held

            infile.ignore();  // ' ' after scratcher value
        }

        infile.ignore();  // '|' between held and winning scratches

        // read winning values and update matches as found
        while (infile.peek() != '\n') {
            infile >> scratch;
            matching += held_map.contains(scratch);
        }

        // update score
        score_sum += (matching ? 0b1 << (matching - 1) : 0);

        infile.ignore();  // '\n' at end of line
    }

    cout << score_sum << endl;

    return 0;
}
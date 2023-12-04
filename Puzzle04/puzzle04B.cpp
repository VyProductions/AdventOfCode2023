#include <fstream>
#include <iostream>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::pair;
using std::unordered_map;

int main() {
    ifstream infile("inputB.txt");  // Input file handler
    int card_count = 0;             // Duplicate card counts

    // Count map for duplicate copies of cards
    // (card num -> copies)
    unordered_map<int, int> card_map;

    while (infile.peek() == 'C') {
        unordered_map<int, bool> held_map;  // O(1) lookup for held scratch nums
        int matching = 0;  // Number of matching scratches on current card
        int scratch;       // Scratch number
        int card_num;      // ID of card

        infile.ignore(4);  // "Card"
        infile >> card_num;
        infile.ignore(2);   // ": "

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

        // Add current duplicate count of card to sum
        card_count += card_map[card_num] += 1;  // duplicates + current card

        // Add copies to later cards based on the matching number of scratches
        // on this card and how many duplicates of this card there are
        for (int n = card_num + 1; n <= card_num + matching; ++n) {
            card_map[n] += (card_map[card_num] ? card_map[card_num] : 1);
        }

        infile.ignore();  // '\n' at end of line
    }

    cout << card_count << endl;

    return 0;
}

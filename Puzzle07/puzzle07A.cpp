#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

// Hand class ordering (higher enumeration value == better hand class)
enum hand_type {
    UNKNOWN, HIGH_CARD, ONE_PAIR, TWO_PAIR,
    THREE_KIND, FULL_HOUSE, FOUR_KIND, FIVE_KIND
};

inline int rank(char card) {
    int result;

    switch (card) {
        case 'A': result = 13; break;
        case 'K': result = 12; break;
        case 'Q': result = 11; break;
        case 'J': result = 10; break;
        case 'T': result =  9; break;
        case '9': result =  8; break;
        case '8': result =  7; break;
        case '7': result =  6; break;
        case '6': result =  5; break;
        case '5': result =  4; break;
        case '4': result =  3; break;
        case '3': result =  2; break;
        case '2': result =  1; break;
        default: break;
    }

    return result;
}

inline char get_card(char placehold) {
    char result;

    switch (placehold) {
        case 'A': result = '2'; break;
        case 'B': result = '3'; break;
        case 'C': result = '4'; break;
        case 'D': result = '5'; break;
        case 'E': result = '6'; break;
        case 'F': result = '7'; break;
        case 'G': result = '8'; break;
        case 'H': result = '9'; break;
        case 'I': result = 'T'; break;
        case 'J': result = 'J'; break;
        case 'K': result = 'Q'; break;
        case 'L': result = 'K'; break;
        case 'M': result = 'A'; break;
        default: break;
    }

    return result;
}

unordered_map<string, int> rank_map {

};

struct hand_t {
    char cards[6];   // Collection of 5 cards in the hand (and one null byte)
    hand_type type;  // What type of hand this is

    void classify() {
        unordered_map<char, unsigned> card_count;   // Map a card to its count
        vector<pair<char, unsigned>> sorted_cards;  // Descending card classes

        // count cards
        for (int i = 0; i < 5; ++i) {
            card_count[cards[i]]++;
        }

        // move cards into sortable container
        for (auto& [card, count] : card_count) {
            sorted_cards.push_back({card, count});
        }

        // sort cards into descending order of quantity
        sort(
            sorted_cards.begin(), sorted_cards.end(),
            [](pair<char, unsigned> a, pair<char, unsigned> b) {
                return (
                    (a.second > b.second) ||
                    (a.second == b.second && rank(a.first) > rank(b.first))
                );
            }
        );

        // determine hand type
        if (sorted_cards.front().second == 5) {
            type = FIVE_KIND;
        } else if (sorted_cards.front().second == 4) {
            type = FOUR_KIND;
        } else if (sorted_cards.front().second == 3) {
            if (sorted_cards.at(1).second == 2) {
                type = FULL_HOUSE;
            } else {
                type = THREE_KIND;
            }
        } else if (sorted_cards.front().second == 2) {
            if (sorted_cards.at(1).second == 2) {
                type = TWO_PAIR;
            } else {
                type = ONE_PAIR;
            }
        } else {
            type = HIGH_CARD;
        }

        // Reformat cards for string comparison
        for (int i = 0; i < 5; ++i) {
            char& ch = cards[i];
            ch = (char)rank(ch) + 'A' - 1;
        }
    }
};

int main() {
    ifstream infile("inputA.txt");       // Input file handler
    vector<pair<hand_t, size_t>> hands;  // Collection of player's hands
    string cards;                        // Card content of a hand in the input
    size_t bid;                          // Player's bid for hand in the input
    size_t player_count = 0;             // Number of players in input
    size_t total_winnings = 0;           // Total wins from all player bids

    while (infile >> cards >> bid) {
        hand_t player_hand {"", UNKNOWN};  // Create a hand for the player

        for (size_t i = 0; i < 5; ++i) {
            player_hand.cards[i] = cards.at(i);
        }

        player_hand.classify();
        hands.push_back({player_hand, bid});

        ++player_count;
    }

    // sort hands by class (and by string comparison order within class)
    sort(
        hands.begin(), hands.end(),
        [](pair<hand_t, size_t> a, pair<hand_t, size_t> b) {
            return (
                // better hand classification,
                (a.first.type > b.first.type) ||
                // or better hand in the same classification
                (
                    a.first.type == b.first.type &&
                    string(a.first.cards) > string(b.first.cards)
                )
            );
        }
    );

    // collect winnings of all players
    for (size_t i = 0; i < hands.size(); ++i) {
        for (int j = 0; j < 5; ++j) {
            char& ch = hands.at(i).first.cards[j];  // Each card in the hand
            ch = get_card(ch);
        }

        total_winnings += (player_count - i) * hands.at(i).second;
    }

    cout << total_winnings << endl;

    infile.close();

    return 0;
}

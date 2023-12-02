#include <fstream>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::string;

const int MAX_RED   = 12;
const int MAX_GREEN = 13;
const int MAX_BLUE  = 14;

int main() {
    // Puzzle input content
    ifstream infile("inputA.txt");

    bool valid_game;   // Game validity state
    int count;         // Count of a colored cube
    string name;       // Color label for the cube
    int game_num;      // Game ID number
    int game_sum = 0;  // Sum of possible games

    while (infile.peek() == 'G') {
        infile.ignore(4);  // "Game"
        infile >> game_num;
        infile.ignore();  // ':' before game info
        valid_game = true;

        while (valid_game && infile.peek() != '\n') {
            while (infile.peek() != '\n' && valid_game) {
                infile >> count >> name;

                // Sanitize name string to exclude separator
                if (name.back() == ',' || name.back() == ';') {
                    name.pop_back();
                }

                valid_game = (
                    (name == "red" && count <= MAX_RED) ||
                    (name == "green" && count <= MAX_GREEN) ||
                    (name == "blue" && count <= MAX_BLUE)
                );
            }

            if (!valid_game) {
                string dump;
                getline(infile, dump, '\n');  // Dump rest of game info line
            }
        }

        if (valid_game) {
            game_sum += game_num;
            infile.ignore();  // '\n' at end of game info
        }
    }

    cout << "Valid ID Sum: " << game_sum << endl;

    infile.close();

    return 0;
}

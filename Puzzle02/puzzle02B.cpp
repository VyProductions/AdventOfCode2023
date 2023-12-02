#include <cmath>
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
    ifstream infile("inputB.txt");

    int count;         // Count of a colored cube
    string name;       // Color label for the cube
    int game_num;      // Game ID number
    int game_sum = 0;  // Sum of possible games
    int power;         // Game power value (r * g * b counts)

    while (infile.peek() == 'G') {
        infile.ignore(4);  // "Game"
        infile >> game_num;
        infile.ignore();  // ':' before game info

        int min_nums[3] = {0, 0, 0};  // R, G, B minimum required

        while (infile.peek() != '\n') {
            while (infile.peek() != '\n') {
                infile >> count >> name;

                // Sanitize name string to exclude separator
                if (name.back() == ',' || name.back() == ';') {
                    name.pop_back();
                }

                if (name == "red") {
                    min_nums[0] = min_nums[0] < count ? count : min_nums[0];
                } else if (name == "green") {
                    min_nums[1] = min_nums[1] < count ? count : min_nums[1];
                } else if (name == "blue") {
                    min_nums[2] = min_nums[2] < count ? count : min_nums[2];
                }
            }
        }

        power = min_nums[0] * min_nums[1] * min_nums[2];
        game_sum += power;
        infile.ignore();  // '\n' at end of game info
    }

    cout << "Valid ID Sum: " << game_sum << endl;

    infile.close();

    return 0;
}

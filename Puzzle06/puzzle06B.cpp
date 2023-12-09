#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

struct time_range {
    unsigned long min;  // Lower bound of time range
    unsigned long max;  // Upper bound of time range
};

time_range winning_times(unsigned long race_time, unsigned long record_distance);

int main() {
    ifstream infile("inputB.txt");           // Input file handler

    vector<unsigned long> times;      // Time categories for boat races
    vector<unsigned long> distances;  // Record race distances for each time category
    unsigned long value;              // Input value
    unsigned long win_product = 1;    // Product of possible winning times for races

    // read times
    infile.ignore(5);  // "Time:"
    while (infile.peek() != '\n') {
        infile >> value;
        times.push_back(value);
    }

    // read distances
    infile.ignore(10);  // "\nDistance:"
    while (infile.peek() != '\n') {
        infile >> value;
        distances.push_back(value);
    }

    // compute winning possibilities
    for (size_t race = 0; race < times.size(); ++race) {
        time_range tr = winning_times(times.at(race), distances.at(race));
        win_product *= (tr.max - tr.min + 1);
    }

    cout << win_product << endl;

    infile.close();

    return 0;
}

time_range winning_times(unsigned long race_time, unsigned long record_distance) {
    time_range tr = {1, race_time};  // Possible times to hold the boat's button
    bool win = false;                // Whether held time can beat the record

    // find minimum held time to win
    while (!win && tr.min != tr.max) {
        if (tr.min * (race_time - tr.min) > record_distance) {
            win = true;
        } else {
            ++tr.min;
        }
    }

    // find maximum held time to win
    win = false;

    while (!win && tr.max != tr.min) {
        if (tr.max * (race_time - tr.max) > record_distance) {
            win = true;
        } else {
            --tr.max;
        }
    }

    return tr;
}

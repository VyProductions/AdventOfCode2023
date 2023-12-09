#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::list;
using std::ostream;
using std::vector;

struct range_map_t {
    long dest_start;    // starting value for result range
    long source_start;  // starting value for initial range
    long length;        // length of ranges

    long map(long query) {
        long source_end = source_start + length;
        
        if (query >= source_start && query < source_end) {
            query = dest_start + (query - source_start);
        }

        return query;
    }
};

istream& operator>>(istream& in, range_map_t& rm);
ostream& operator<<(ostream& out, const range_map_t& rm);

int main() {
    ifstream infile("inputA.txt");           // Input file handler
    list<long> seed_list;                    // List of seeds to plant
    long seed_num;                           // Seed ID from input
    vector<range_map_t> seed_soil_ranges;    // Seed-soil map ranges
    vector<range_map_t> soil_fert_ranges;    // Soil-fertilizer map ranges
    vector<range_map_t> fert_water_ranges;   // Fertilizer-water map ranges
    vector<range_map_t> water_light_ranges;  // Water-light map ranges
    vector<range_map_t> light_temp_ranges;   // Light-temperature map ranges
    vector<range_map_t> temp_humid_ranges;   // Temperature-humidity map ranges
    vector<range_map_t> humid_loc_ranges;    // Humidity-location map ranges
    range_map_t input_range;                 // Range description from input
    long min_location;                       // Closest location from almanac

    /// Number of characters to ignore before reading each range map category
    const int map_ignore_lengths[7] {
        20, 25, 26, 21, 27, 30, 27
    };

    // Reference to each set to populate by reading the range map categories
    vector<range_map_t>* range_refs[7] {
        &seed_soil_ranges, &soil_fert_ranges, &fert_water_ranges,
        &water_light_ranges, &light_temp_ranges, &temp_humid_ranges,
        &humid_loc_ranges
    };

    // read seed list
    infile.ignore(6);  // "seeds:"

    while (infile.peek() != '\n') {
        infile >> seed_num;
        seed_list.push_back(seed_num);
    }

    // read range categories
    for (int i = 0; i < 7; ++i) {
        // ignore category header
        infile.ignore(map_ignore_lengths[i]);

        // read ranges of category
        while (infile.peek() != '\n') {
            infile >> input_range;
            range_refs[i]->push_back(input_range);
            infile.ignore();  // '\n'
        }
    }

    // map seeds to location
    for (auto it = seed_list.begin(); it != seed_list.end(); ++it) {
        long& seed = *it;

        for (int i = 0; i < 7; ++i) {
            bool mapped = false;                // Whether the seed changed
            auto beg = range_refs[i]->begin();  // Beginning of current ranges
            auto end = range_refs[i]->end();    // End of current ranges

            while (!mapped && beg != end) {
                long new_seed = beg->map(seed);  // Result of mapping

                if (new_seed != seed) {
                    seed = new_seed;
                    mapped = true;
                }

                ++beg;
            }
        }

        // update min range if necessary
        if (it == seed_list.begin() || seed < min_location) {
            min_location = seed;
        }
    }

    cout << min_location << endl;

    infile.close();

    return 0;
}

istream& operator>>(istream& in, range_map_t& rm) {
    return in >> rm.dest_start >> rm.source_start >> rm.length;
}

ostream& operator<<(ostream& out, const range_map_t& rm) {
    return out << rm.dest_start << ' ' << rm.source_start << ' ' << rm.length;
}
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::ostream;
using std::vector;

inline long labs(long l) {
    return (l < 0 ? -l : l);
}

struct range_map_t {
    long dest_start;    // starting value for result range
    long source_start;  // starting value for initial range
    long length;        // length of ranges
};

struct range_t {
    long start;   // First value of range
    long length;  // Second value of range
    bool proc;    // Whether duple has been processed
};

istream& operator>>(istream& in, range_map_t& rm);
ostream& operator<<(ostream& out, const range_map_t& rm);

int main() {
    ifstream infile("inputB.txt");           // Input file handler
    vector<range_t> seed_ranges;             // Seed ranges to plant
    long seed_range_start;                   // Starting seed ID from input
    long seed_range_length;                  // Length of seed range from input
    vector<range_map_t> seed_soil_ranges;    // Seed-soil map ranges
    vector<range_map_t> soil_fert_ranges;    // Soil-fertilizer map ranges
    vector<range_map_t> fert_water_ranges;   // Fertilizer-water map ranges
    vector<range_map_t> water_light_ranges;  // Water-light map ranges
    vector<range_map_t> light_temp_ranges;   // Light-temperature map ranges
    vector<range_map_t> temp_humid_ranges;   // Temperature-humidity map ranges
    vector<range_map_t> humid_loc_ranges;    // Humidity-location map ranges
    range_map_t input_range;                 // Range description from input
    long min_location ;                      // Closest location from almanac

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
        infile >> seed_range_start;
        infile >> seed_range_length;

        seed_ranges.push_back({seed_range_start, seed_range_length, false});
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

    // partition seed ranges through range maps
    for (int i = 0; i < 7; ++i) {
        size_t seed_beg = 0;

        while (seed_beg != seed_ranges.size()) {
            long s_first  = seed_ranges.at(seed_beg).start;   // First of seed
            long s_length = seed_ranges.at(seed_beg).length;  // Length of range
            long s_last   = s_first + s_length - 1;           // Last of seed

            auto range_it  = range_refs[i]->begin();  // Begin of current range
            auto range_end = range_refs[i]->end();    // End of current range

            bool found_range = false;  // Whether a range has been remapped

            while (
                !found_range &&
                !seed_ranges.at(seed_beg).proc &&
                range_it != range_end
            ) {
                const range_map_t& r = *range_it;       // Current range mapping
                long r_first = r.source_start;          // Start of source range
                long r_last  = r_first + r.length - 1;  // End of source range

                if (
                    (r_first <= s_first && r_last >= s_first) ||
                    (r_first <= s_last && r_last >= s_last)
                ) {
                    // Offset between source and destination of range mapping
                    long part_offs   = r_first - r.dest_start;
                    found_range = true;

                    // range partially intersects seed range
                    if (
                        r_first > s_first ||
                        r_last  < s_last
                    ) {
                        // compute the part of seed range which is intersected

                        // left edge intersection
                        if (r_last < s_last) {
                            // Intersection length of seed range from range map
                            long part_length = (
                                r.length - labs(s_first - r_first)
                            );
                            seed_ranges.at(seed_beg).start -= part_offs;
                            seed_ranges.at(seed_beg).length = part_length;
                            seed_ranges.at(seed_beg).proc = true;

                            // add a new range
                            seed_ranges.push_back(
                                {
                                    s_first + part_length,
                                    s_length - part_length,
                                    false
                                }
                            );
                        // right edge intersection
                        } else {
                            // Intersection length of seed range from range map
                            long part_length = r.length - labs(s_last - r_last);
                            seed_ranges.at(seed_beg).length -= part_length;

                            // duplicate left range to back check it
                            seed_ranges.push_back(seed_ranges.at(seed_beg));

                            // add a new range
                            seed_ranges.push_back(
                                {
                                    s_first + seed_ranges.at(
                                        seed_beg
                                    ).length - part_offs,
                                    part_length, true
                                }
                            );

                            // remove original left range
                            seed_ranges.erase(seed_ranges.begin() + seed_beg);

                            --seed_beg;
                        }
                    // range mapping completely covers seed range
                    } else {
                        // modify existing range by offsetting the entire thing
                        seed_ranges.at(seed_beg).start -= part_offs;
                        seed_ranges.at(seed_beg).proc = true;
                    }
                }

                ++range_it;
            }

            ++seed_beg;
        }

        // reset all seed ranges to unprocessed
        for (range_t& d : seed_ranges) {
            d.proc = false;
        }
    }

    // compute minimum starting value of all seed ranges
    min_location = seed_ranges.at(0).start;

    for (size_t i = 1; i < seed_ranges.size(); ++i) {
        min_location = seed_ranges.at(i).start < min_location ?
            seed_ranges.at(i).start : min_location;
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
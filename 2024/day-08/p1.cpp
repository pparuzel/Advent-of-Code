#include <iostream>
#include <unordered_map>
#include <ranges>
#include <algorithm>
#include <numeric>

struct Location {
    int row;
    int col;

    bool operator==(const Location& rhs) const {
        return row == rhs.row && col == rhs.col;
    }
};

int main() {
    // Read the map
    std::vector<std::string> map;
    while (std::getline(std::cin, map.emplace_back()));
    map.pop_back();  // Remove excess empty line at the end
    // Ugly assumptions
    static const auto height = map.size();
    static const auto width = map[0].size();

    // Build a frequency to location mapping
    std::unordered_map<char, std::vector<Location>> freq2loc;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            const char c = map[row][col];
            if (c != '.') {
                freq2loc[c].emplace_back(row, col);
            }
        }
    }

    std::vector antinodes(height, std::vector<bool>(width, false));
    auto InBounds = [](const auto& loc) {
        return 0 <= loc.row && loc.row < height && 0 <= loc.col && loc.col < width;
    };
    for (const auto& freq_locs : freq2loc) {
        char freq = freq_locs.first;
        const std::vector<Location>& locs = freq_locs.second;
        for (const Location& loc1 : locs) {
            for (const Location& loc2 : locs) {
                if (loc1 == loc2) {
                    continue;
                }
                // Calculate the vectors for two points and add to their antennas
                auto anti_loc1 = Location(2 * loc1.row - loc2.row, 2 * loc1.col - loc2.col);
                auto anti_loc2 = Location(2 * loc2.row - loc1.row, 2 * loc2.col - loc1.col);
                if (InBounds(anti_loc1)) {
                    antinodes[anti_loc1.row][anti_loc1.col] = true;
                }
                if (InBounds(anti_loc2)) {
                    antinodes[anti_loc2.row][anti_loc2.col] = true;
                }
            }
        }
    }

    // for (int row = 0; row < height; ++row) {
    //     for (int col = 0; col < width; ++col) {
    //         const char c = map[row][col];
    //         if (c != '.' || !antinodes[row][col]) {
    //             std::cout << c << ' ';
    //         } else {
    //             std::cout << '#' << ' ';
    //         }
    //     }
    //     std::cout << '\n';
    // }

    int answer = std::accumulate(antinodes.begin(), antinodes.end(), 0, [](int acc, auto&& v) { return acc + std::ranges::count(v, true); });
    std::cout << "answer: " << answer << '\n';
}

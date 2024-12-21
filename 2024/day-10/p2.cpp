#include <iostream>
#include <unordered_set>

using Map = std::vector<std::vector<int>>;

auto ReachTop(const Map& map, int row, int col) -> int {
    if (map[row][col] == 9) {
        return 1;
    }
    int count = 0;
    if (row - 1 >= 0 && map[row][col] + 1 == map[row - 1][col]) {
        count += ReachTop(map, row - 1, col);
    }
    if (row + 1 < map.size() && map[row][col] + 1 == map[row + 1][col]) {
        count += ReachTop(map, row + 1, col);
    }
    if (col - 1 >= 0 && map[row][col] + 1 == map[row][col - 1]) {
        count += ReachTop(map, row, col - 1);
    }
    if (col + 1 < map[0].size() && map[row][col] + 1 == map[row][col + 1]) {
        count += ReachTop(map, row, col + 1);
    }
    return count;
}

int main() {
    Map map;
    std::string line;
    std::vector<std::pair<int, int>> trailheads;
    // Build the map
    int row = 0;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        std::transform(
            line.begin(), line.end(),
            map.emplace_back(line.size(), 0).begin(),
            [&, col = 0](char c) mutable {
                int val = c - '0';
                if (val == 0) {
                    trailheads.emplace_back(row, col);
                }
                ++col;
                return val;
            });
        ++row;
    }
    int total = 0;
    for (auto [r, c] : trailheads) {
        total += ReachTop(map, r, c);
    }
    std::cout << "answer: " << total << '\n';
}

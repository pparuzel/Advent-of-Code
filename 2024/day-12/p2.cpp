#include <iostream>


constexpr int MAXROW = 1000, MAXCOL = 1000, TO_UPPER = 'a' - 'A';

auto Traverse(std::vector<std::string>& map, char plant, int row, int col, int& corners, std::size_t& area) -> void {
    std::size_t key = row * MAXCOL + col;
    // If lowercase then visited
    if ('a' <= map[row][col] && map[row][col] <= 'z') {
        return;
    }
    const char right = map[row][col + 1];
    const char down = map[row + 1][col];
    const char up = map[row - 1][col];
    const char left = map[row][col - 1];
    if ('A' <= map[row][col] && map[row][col] <= 'Z') {
        // Mark as visited
        // TODO: We could use masking to only
        //       compare once instead of twice
        map[row][col] += TO_UPPER;
        ++area;
    }
    const bool can_go_right = right == plant || right == plant + TO_UPPER;
    const bool can_go_down = down == plant || down == plant + TO_UPPER;
    const bool can_go_left = left == plant || left == plant + TO_UPPER;
    const bool can_go_up = up == plant || up == plant + TO_UPPER;

    if (can_go_right) {
        Traverse(map, plant, row, col + 1, corners, area);
    } else {
        // Put fence '>'
        corners += !can_go_up;
        corners += !can_go_down;
    }
    if (can_go_down) {
        Traverse(map, plant, row + 1, col, corners, area);
    } else {
        // Put fence 'v'
        corners += !can_go_left;
        corners += !can_go_right;
    }
    if (can_go_left) {
        Traverse(map, plant, row, col - 1, corners, area);
    } else {
        // Put fence '<'
        corners += !can_go_up;
        corners += !can_go_down;
    }
    if (can_go_up) {
        Traverse(map, plant, row - 1, col, corners, area);
    } else {
        // Put fence '^'
        corners += !can_go_left;
        corners += !can_go_right;
    }
    // Include inner corners
    // TODO: check if can simplify
    corners += (
        can_go_right && can_go_down &&
        map[row + 1][col + 1] != plant &&
        map[row + 1][col + 1] != plant + TO_UPPER) * 2;

    corners += (
        can_go_down && can_go_left &&
        map[row + 1][col - 1] != plant &&
        map[row + 1][col - 1] != plant + TO_UPPER) * 2;

    corners += (
        can_go_left && can_go_up &&
        map[row - 1][col - 1] != plant &&
        map[row - 1][col - 1] != plant + TO_UPPER) * 2;

    corners += (
        can_go_up && can_go_right &&
        map[row - 1][col + 1] != plant &&
        map[row - 1][col + 1] != plant + TO_UPPER) * 2;
}

int main() {
    // Read the input file
    std::string line;
    std::vector<std::string> map;
    // First line determines the width without borders
    std::getline(std::cin, line);
    const auto width = line.size() + 2;
    // Add top border for simplicity
    map.emplace_back(width, '.');
    // ... and only then the first line
    map.emplace_back('.' + std::move(line) + '.');
    while (std::getline(std::cin, line)) {
        map.emplace_back('.' + std::move(line) + '.');
    }
    if (map.size() < 2) {
        throw std::runtime_error("error: Empty map");
    }
    // Fill the bottom border with dots
    map.emplace_back(width, '.');

    const auto height = map.size();
    // Traverse the map to find the perimiters of all plants
    // for (auto&& l : map) { std::cout << l << '\n'; } std::cout << '\n';
    int answer = 0;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if ('A' <= map[row][col] && map[row][col] <= 'Z') {
                int corners = 0;
                std::size_t area = 0;
                Traverse(map, map[row][col], row, col, corners, area);
                int sides = corners / 2;
                // for (auto&& l : map) { std::cout << l << '\n'; }
                // std::cout << "corners: " << corners << '\n';
                // std::cout << "sides: " << sides << '\n';
                // std::cout << "area: " << area << '\n';
                answer += sides * area;
            }
        }
    }
    // for (auto&& l : map) { std::cout << l << '\n'; }
    std::cout << "answer: " << answer << '\n';
}

#include <iostream>


constexpr int MAXROW = 1000, MAXCOL = 1000, TO_UPPER = 'a' - 'A';

auto Traverse(std::vector<std::string>& map, char plant, int row, int col, int& perimeter, std::size_t& area) -> void {
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
    if (right == plant || right == plant + TO_UPPER) {
        Traverse(map, plant, row, col + 1, perimeter, area);
    } else {
        ++perimeter;
        // map[row][col + 1] = '>';
    }
    if (down == plant || down == plant + TO_UPPER) {
        Traverse(map, plant, row + 1, col, perimeter, area);
    } else {
        ++perimeter;
        // map[row + 1][col] = 'v';
    }
    if (left == plant || left == plant + TO_UPPER) {
        Traverse(map, plant, row, col - 1, perimeter, area);
    } else {
        ++perimeter;
        // map[row][col - 1] = '<';
    }
    if (up == plant || up == plant + TO_UPPER) {
        Traverse(map, plant, row - 1, col, perimeter, area);
    } else {
        ++perimeter;
        // map[row - 1][col] = '^';
    }
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
    int answer = 0;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if ('A' <= map[row][col] && map[row][col] <= 'Z') {
                int perimiter = 0;
                std::size_t area = 0;
                Traverse(map, map[row][col], row, col, perimiter, area);
                // for (auto&& l : map) { std::cout << l << '\n'; }
                // std::cout << "perimiter: " << perimiter << '\n';
                // std::cout << "area: " << area << '\n';
                answer += perimiter * area;
            }
        }
    }
    // for (auto&& l : map) { std::cout << l << '\n'; }
    std::cout << "answer: " << answer << '\n';
}

#include <iostream>

int main() {
    // Store the map
    std::vector<std::string> map;
    std::string line;
    while (std::getline(std::cin, line)) {
        map.emplace_back(std::move(line));
    }
    // Ugly assumptions
    int height = map.size();
    int width = map[0].size();
    //
    // Find the guard
    //
    int guard_i = -1;
    int guard_j = -1;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (map[i][j] == '^') {
                guard_i = i;
                guard_j = j;
                break;
            }
        }
    }
    if (guard_i == -1) {
        throw std::runtime_error("The guard was not found!");
    }
    //
    // Move the guard until it hits a wall
    //
    // Guard starts by moving up => (0, -1) vector
    int dx = 0;
    int dy = -1;
    auto ObstacleInFront = [&](int x, int y) {
        return map[y + dy][x + dx] == '#';
    };
    auto HitWall = [&](int x, int y) {
        return x == 0 || y == 0 || x == width - 1 || y == height;
    };
    auto MoveGuard = [&](int& x, int& y) {
        while (true) {
            while (!ObstacleInFront(x, y)) {
                // map[y][x] = 'X';
                if (dx == 0) {
                    if (dy < 0) { map[y][x] = '^'; }
                    else if (dy > 0) { map[y][x] = 'v'; }
                }
                else if (dy == 0) {
                    if (dx > 0) { map[y][x] = '>'; }
                    else if (dx < 0) { map[y][x] = '<'; }
                }

                x += dx;
                y += dy;
                if (HitWall(x, y)) {
                    map[y][x] = 'X';
                    return;
                }
            }
            // Change direction, right by 90 degrees
            // (dx, dy) * R => (-dy, dx)
            int tmpdx = dx;
            dx = -dy;
            dy = tmpdx;
        }
    };
    MoveGuard(guard_j, guard_i);

    for (int i = 0; i < height; ++i) {
        std::cout << map[i] << '\n';
    }

    int total = 0;
    for (int i = 0; i < height; ++i) {
        total += std::count_if(map[i].begin(), map[i].end(), [](char c) { return c != '.' && c != '#'; });
    }

    std::cout << "answer: " << total << '\n';
}

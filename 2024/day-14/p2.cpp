#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <charconv>
#include <algorithm>

struct Robot {
    int posx;
    int posy;
    int velx;
    int vely;

    static Robot Read(std::string_view s) {
        auto before_number = [](char c) { return c == '=' || c == ','; };
        int values[4];
        const char* start;
        const char* ptr = s.data();
        const char* end = s.data() + s.size();
        for (int i = 0; i < 4; ++i) {
            start = std::find_if(ptr, end, before_number) + 1;
            ptr = std::from_chars(start, end, values[i]).ptr;
        }
        return Robot{values[0], values[1], values[2], values[3]};
    }
};

auto absmod(int value, int mod) {
    int result = value % mod;
    if (result < 0) {
        return result + mod;
    }
    return result;
}

int main() {
    // Read input
    std::vector<Robot> robots;
    std::string line;
    while (std::getline(std::cin, line)) {
        robots.push_back(Robot::Read(line));
    }

    static constexpr int width = 101, height = 103;

    auto Print = [](const auto& m) {
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                int val = m[row][col];
                if (val == 0) {
                    std::cout << '.';
                } else {
                    std::cout << char(val + '0');
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    // Segments
    /*
    A | B | C
    - + - + -
    D | E | F
    - + - + -
    G | H | I
    */
    constexpr int segments = 3;
    constexpr int segment_height = height / segments;
    constexpr int segment_width = width / segments;

    auto PrintSegments = [](const auto& seg) {
        std::cout << "Segments\n";
        for (int i = 0; i < 3; ++i) {
            std::cout << seg[i * 3 + 0];
            for (int j = 1; j < 3; ++j) {
                std::cout << " | " << std::setw(3) << seg[i * 3 + j];
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    const int tree_threshold = 0.25 * robots.size();

    for (int sec = 0; true; ++sec) {
        int q[segments * segments] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (const auto& robot : robots) {
            auto posx = absmod(robot.posx + sec * (robot.velx), width);
            auto posy = absmod(robot.posy + sec * (robot.vely), height);
            // Record robot count in its corresponding segment
            auto segment_id = std::min(posx / segment_width, segments - 1) * 3 + std::min(posy / segment_height, segments - 1);
            ++q[segment_id];
        }
        for (int i = 0; i < segments * segments; ++i) {
            // Stop searching once the threshold is satisfied
            if (q[i] > tree_threshold) {
                std::vector map(height, std::vector<int>(width, 0));
                for (const auto& robot : robots) {
                    auto posx = absmod(robot.posx + sec * (robot.velx), width);
                    auto posy = absmod(robot.posy + sec * (robot.vely), height);
                    ++map[posy][posx];
                }
                Print(map);
                PrintSegments(q);
                std::cout << "Answer: " << sec << std::endl;
                return 0;
                // break;
            }
        }
    }
}

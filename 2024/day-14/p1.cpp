#include <iostream>
#include <sstream>
#include <charconv>

struct Robot {
    int posx;
    int posy;
    int velx;
    int vely;

    static Robot Read(std::string_view s) {
        auto before_number = [](char c) { return c == '=' || c == ','; };
        int x, y, vx, vy;
        auto end = s.data() + s.size();
        const char* n1 = std::find_if(s.data(), end, before_number) + 1;
        auto ptr = std::from_chars(n1, end, x).ptr;
        const char* n2 = std::find_if(ptr, end, before_number) + 1;
        ptr = std::from_chars(n2, end, y).ptr;

        n1 = std::find_if(ptr, end, before_number) + 1;
        ptr = std::from_chars(n1, end, vx).ptr;
        n2 = std::find_if(ptr, end, before_number) + 1;
        std::from_chars(n2, end, vy);

        return Robot{x, y, vx, vy};
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
    static constexpr int middle_x = width / 2, middle_y = height / 2;

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

    // Skip forward 100 seconds
    const auto sec = 100;
    // Quadrants
    // Q0 | Q1
    // -- + --
    // Q2 | Q3
    int q[4] = {0, 0, 0, 0};
    // std::vector map(height, std::vector<int>(width, 0));
    for (const auto& robot : robots) {
        auto posx = absmod(robot.posx + sec * (robot.velx), width);
        auto posy = absmod(robot.posy + sec * (robot.vely), height);
        // ++map[posy][posx];
        if (posx > middle_x) {
            // Q1 or Q3
            if (posy > middle_y) {
                ++q[3];
            } else if (posy < middle_y) {
                ++q[1];
            }
        } else if (posx < middle_x) {
            // Q0 or Q2
            if (posy > middle_y) {
                ++q[2];
            } else if (posy < middle_y) {
                ++q[0];
            }
        }
    }
    // std::cout << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << '\n';
    std::cout << "answer: " << q[0] * q[1] * q[2] * q[3] << '\n';
    // Print(map);

}

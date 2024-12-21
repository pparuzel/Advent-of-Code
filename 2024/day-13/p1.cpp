#include <iostream>
#include <charconv>
#include <tuple>

struct Point {
    int x;
    int y;

    Point(int x, int y)
        : x(x), y(y) {
    }

    static Point Read(std::istream& stream, std::string_view s) {
        int x, y;
        auto end = s.data() + s.size();
        const char* n1 = std::find_if(s.data(), end, [](char c) { return std::isdigit(c); });
        auto ptr = std::from_chars(n1, end, x).ptr;
        const char* n2 = std::find_if(ptr, end, [](char c) { return std::isdigit(c); });
        std::from_chars(n2, end, y);
        return Point(x, y);
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        out << p.x << ',' << p.y;
        return out;
    }
};

auto ReadInputFile() -> std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> {
    std::string line;
    std::vector<Point> button_a;
    std::vector<Point> button_b;
    std::vector<Point> prize;
    while (std::getline(std::cin, line)) {
        // Button A line
        auto btn_a = std::string_view(line);
        if (!btn_a.starts_with("Button A:")) {
            throw std::runtime_error("error: No Button A line");
        }
        button_a.emplace_back(Point::Read(std::cin, btn_a));
        // Button B line
        std::getline(std::cin, line);
        auto btn_b = std::string_view(line);
        if (!btn_b.starts_with("Button B:")) {
            throw std::runtime_error("error: No Button B line");
        }
        button_b.emplace_back(Point::Read(std::cin, btn_b));
        // Prize line
        std::getline(std::cin, line);
        auto pr = std::string_view(line);
        if (!pr.starts_with("Prize:")) {
            throw std::runtime_error("error: No Prize line");
        }
        prize.emplace_back(Point::Read(std::cin, pr));
        // Empty space line
        std::getline(std::cin, line);
        if (!line.empty()) {
            throw std::runtime_error("error: No space between configs");
        }
    }
    return std::make_tuple(button_a, button_b, prize);
}

int main() {
    auto [button_a, button_b, prize] = ReadInputFile();
    const auto size = button_a.size();
    int answer = 0;
    for (int case_num = 0; case_num < size; ++case_num) {
        const auto& p = prize[case_num];
        const auto& a = button_a[case_num];
        const auto& b = button_b[case_num];
        int lowest = std::numeric_limits<int>::max();
        for (int a_num = 0; a_num <= 100; ++a_num) {
            // Check if solution exists for a_num
            const auto a_x_final = a.x * a_num;
            const auto a_y_final = a.y * a_num;
            if (a_x_final > p.x || a_y_final > p.y) {
                // We missed the target, and will continue to
                // since A_num is only going to get bigger
                break;
            }
            const auto b_x_needed = p.x - a.x * a_num;
            const auto b_y_needed = p.y - a.y * a_num;
            if (b_x_needed % b.x != 0 ||
                b_y_needed % b.y != 0) {
                // Solution does not exist for a_num
                continue;
            }
            const auto b_num_x = (p.x - a.x * a_num) / b.x;
            const auto b_num_y = (p.y - a.y * a_num) / b.y;
            if (b_num_x != b_num_y) {
                // We require that the presses on the x-axis
                // are equal to the presses on the y-axis
                // because they are bound to themselves
                continue;
            }
            // Calculate the cost
            const auto b_num = b_num_x;
            const auto cost = 3 * a_num + 1 * b_num;
            if (cost < lowest) {
                lowest = cost;
            }
        }
        if (lowest != std::numeric_limits<int>::max()) {
            answer += lowest;
        }
    }
    std::cout << "answer: " << answer << '\n';
}

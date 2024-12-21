#include <iostream>
#include <charconv>
#include <tuple>

struct Point {
    using ValueType = long long;

    ValueType x;
    ValueType y;

    Point(ValueType x, ValueType y)
        : x(x), y(y) {
    }

    static Point Read(std::string_view s) {
        ValueType x, y;
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
        button_a.emplace_back(Point::Read(btn_a));
        // Button B line
        std::getline(std::cin, line);
        auto btn_b = std::string_view(line);
        if (!btn_b.starts_with("Button B:")) {
            throw std::runtime_error("error: No Button B line");
        }
        button_b.emplace_back(Point::Read(btn_b));
        // Prize line
        std::getline(std::cin, line);
        auto pr = std::string_view(line);
        if (!pr.starts_with("Prize:")) {
            throw std::runtime_error("error: No Prize line");
        }
        prize.emplace_back(Point::Read(pr));
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
    Point::ValueType answer = 0;
    for (int case_num = 0; case_num < size; ++case_num) {
        auto p = prize[case_num];
        p.x += 10000000000000;
        p.y += 10000000000000;
        const auto& a = button_a[case_num];
        const auto& b = button_b[case_num];

        auto [a_hat, a_mod] = std::div(p.x * b.y - p.y * b.x, a.x * b.y - a.y * b.x);
        auto [b_hat, b_mod] = std::div(p.x - a.x * a_hat, b.x);
        if (a_mod == 0 and b_mod == 0) {
            answer += 3 * a_hat + b_hat;
        }
    }
    std::cout << "answer: " << answer << '\n';
}

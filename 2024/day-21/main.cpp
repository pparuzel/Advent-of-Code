#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <utility>


// Maximum ID value of a directional keypad robot
constexpr std::size_t MaxDirectionalRobotId = 25 + 2;
// Numeric and Directional keypad type
using Pad = std::vector<std::string_view>;
// Memoization caches
using CostBetweenMemo = std::array<std::array<std::array<
    std::size_t, 128>, 128>, MaxDirectionalRobotId>;
using CostMemo = std::array<std::unordered_map<
    std::string, std::size_t>, MaxDirectionalRobotId>;

// Global caches which we can reuse for Numpad na Keypad since they use
// different buttons except from case of computing cost of path from 'A' to 'A'
// which is the same for both pads. Another approach would be to change the
// button from 'A' to 'B'.
CostBetweenMemo memo_cost_between;
CostMemo memo_cost;

auto ReadInput() -> std::vector<std::string> {
    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.emplace_back(std::move(line));
    }
    return input;
}

auto GetKeyPad() -> Pad {
    return {
        " ^A",
        "<v>",
    };
}

auto GetNumPad() -> Pad {
    return {
        "789",
        "456",
        "123",
        " 0A",
    };
}

class Directions {
public:
    static auto FromChar(char direction) -> std::pair<int, int> {
        auto pos = chars.find(direction);
        return data[pos];
    }

private:
    constexpr static std::string_view chars = "<>^v";
    inline static const std::array<std::pair<int, int>, 4> data = {{
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0},
    }};
};

auto Validate(const Pad& pad, std::string_view path, int row, int col) -> bool
{
    if (pad[row][col] == ' ') {
        return false;
    }
    for (char dir : path) {
        auto [dr, dc] = Directions::FromChar(dir);
        row += dr;
        col += dc;
        if (pad[row][col] == ' ') {
            return false;
        }
    }
    return true;
}

auto ComputePathsBetween(const Pad& pad, char start, char end)
    -> std::vector<std::string>
{
    int start_row = -1, start_col = -1;
    int end_row = -1, end_col = -1;
    for (int row = 0; row < pad.size(); ++row) {
        for (int col = 0; col < pad[row].size(); ++col) {
            if (pad[row][col] == start) {
                start_row = row;
                start_col = col;
            }
            if (pad[row][col] == end) {
                end_row = row;
                end_col = col;
            }
        }
    }
    const auto row_diff = end_row - start_row;
    const auto col_diff = end_col - start_col;
    auto horizontal_part = std::string(std::abs(col_diff), "<>"[col_diff > 0]);
    auto vertical_part = std::string(std::abs(row_diff), "^v"[row_diff > 0]);
    std::vector<std::string> paths;
    std::vector<std::string> paths_to_validate = {
        horizontal_part + vertical_part,
        vertical_part + horizontal_part,
    };
    for (std::string& path : paths_to_validate) {
        if (Validate(pad, path, start_row, start_col)) {
            paths.emplace_back(std::move(path) + 'A');
        }
    }
    return paths;
}

auto ComputePathCost(const Pad& pad, const std::string& keys, int robot_id)
    -> std::size_t;

auto ComputeCostBetween(const Pad& pad, char from, char to, int robot_id)
    -> std::size_t
{
    if (memo_cost_between[robot_id][from][to] != 0) {
        return memo_cost_between[robot_id][from][to];
    }
    if (robot_id == 0) {
        return memo_cost_between[robot_id][from][to] = 1;
    }
    std::vector<std::string> paths = ComputePathsBetween(pad, from, to);
    assert(!paths.empty());
    std::size_t min_cost = ComputePathCost(GetKeyPad(), paths[0], robot_id - 1);
    for (std::size_t i = 1; i < paths.size(); ++i) {
        auto current_cost = ComputePathCost(GetKeyPad(), paths[i], robot_id - 1);
        if (current_cost < min_cost) {
            min_cost = current_cost;
        }
    }
    return memo_cost_between[robot_id][from][to] = min_cost;
}

auto ComputePathCost(const Pad& pad, const std::string& keys, int robot_id)
    -> std::size_t
{
    if (auto found = memo_cost[robot_id].find(keys);
        found != memo_cost[robot_id].end()) {
        return found->second;
    }
    assert(!keys.empty());
    std::size_t total_cost = ComputeCostBetween(pad, 'A', keys[0], robot_id);
    for (std::size_t i = 0; i < keys.size() - 1; ++i) {
        total_cost += ComputeCostBetween(pad, keys[i], keys[i + 1], robot_id);
    }
    return memo_cost[robot_id][keys] = total_cost;
}

template <std::size_t DirectionalRobotsNum>
auto ComputeComplexity(const std::string& code) -> std::size_t {
    std::size_t value;
    std::from_chars(code.data(), code.data() + code.size() - 1, value);
    return ComputePathCost(GetNumPad(), code, DirectionalRobotsNum + 1) * value;
}

template <int DirectionalRobotsNum>
auto SumComplexities(std::size_t acc, const std::string& code) -> std::size_t {
    return acc + ComputeComplexity<DirectionalRobotsNum>(code);
}

int main() {
    std::vector<std::string> codes = ReadInput();

    std::cout << std::accumulate(codes.begin(), codes.end(),
                                 std::size_t{0}, SumComplexities<2>) << '\n';

    std::cout << std::accumulate(codes.begin(), codes.end(),
                                 std::size_t{0}, SumComplexities<25>) << '\n';
}

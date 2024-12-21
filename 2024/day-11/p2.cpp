#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <charconv>


std::unordered_map<std::size_t, std::size_t> memo;
constexpr int max_blinks = 75;

auto CountStones(std::size_t stone, int blink) -> std::size_t {
    if (blink == 0) {
        return 1;
    }

    // Less efficient key
    // std::string key = std::to_string(stone) + "_" + std::to_string(blink);
    const std::size_t key = stone * (max_blinks + 1) + blink;

    auto found = memo.find(key);
    if (found != memo.end()) {
        return found->second;
    }

    if (stone == 0) {
        return memo[key] = CountStones(1, blink - 1);
    }

    std::string digits = std::to_string(stone);
    if (digits.size() % 2 == 0) {
        const auto halfpoint = std::next(digits.data(), digits.size() / 2);
        std::size_t lhs, rhs;
        std::from_chars(digits.data(), halfpoint, lhs);
        std::from_chars(halfpoint, digits.data() + digits.size(), rhs);
        return memo[key] = CountStones(lhs, blink - 1) + CountStones(rhs, blink - 1);
    }

    return memo[key] = CountStones(stone * 2024, blink - 1);
}

int main() {
    // Read the input
    std::vector<std::size_t> stones;
    while (std::cin >> stones.emplace_back());
    stones.pop_back();
    // Count all stones that reach the last blink recursively (blinks == 0)
    std::size_t answer = 0;
    for (auto stone : stones) {
        answer += CountStones(stone, max_blinks);
    }

    std::cout << "Answer: " << answer << '\n';
}

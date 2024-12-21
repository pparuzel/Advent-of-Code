#include <iostream>
#include <charconv>

auto Blink(std::vector<std::uint64_t>& stones) -> void {
    const auto stones_count = stones.size();
    for (int i = 0; i < stones_count; ++i) {
        if (stones[i] == 0) [[unlikely]] {
            stones[i] = 1;
            continue;
        }
        auto digits = std::to_string(stones[i]);
        if (digits.size() % 2 == 0) {
            auto halfpoint = std::next(digits.data(), digits.size() / 2);
            std::from_chars(digits.data(), halfpoint, stones[i]);
            std::from_chars(halfpoint, digits.data() + digits.size(), stones.emplace_back());
        } else {
            stones[i] *= 2024;
        }
    }
}

void Print(const std::vector<std::uint64_t>& vec);

int main() {
    std::vector<std::uint64_t> stones;
    while (std::cin >> stones.emplace_back());
    stones.pop_back();
    for (int i = 0; i < 25; ++i) {
        Blink(stones);
    }
    std::cout << "answer: " << stones.size() << '\n';
}


void Print(const std::vector<std::uint64_t>& vec) {
    std::cout << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << ' ';
    }
    std::cout << "]\n";
}

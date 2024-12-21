#include <iostream>
#include <sstream>
#include <ranges>

using Operands = std::vector<std::uint64_t>;

auto ConcatNumbers(std::uint64_t lhs, std::uint64_t rhs) -> std::uint64_t {
    return std::stoull(std::to_string(lhs) + std::to_string(rhs));
}

auto HasValidCombination(const std::uint64_t target, std::uint64_t result, std::ranges::subrange<Operands::iterator> rest) -> bool {
    if (rest.empty()) {
        return result == target;
    }
    if (result > target) {
        return false;
    }
    return HasValidCombination(target, result + rest.front(), rest.next())
        || HasValidCombination(target, result * rest.front(), rest.next())
        || HasValidCombination(target, ConcatNumbers(result, rest.front()), rest.next());
}

auto HasValidCombination(const std::uint64_t target, std::ranges::subrange<Operands::iterator> rest) -> bool {
    if (rest.empty()) {
        return false;
    }
    return HasValidCombination(target, rest.front(), rest.next());
}

int main() {
    std::vector<std::pair<std::uint64_t, Operands>> values;
    // Read results and operands
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream stream;
        stream << line;
        std::string result;
        stream >> result;
        values.emplace_back(std::stoull(result), Operands{});
        std::uint64_t value;
        while (stream >> value) {
            values.back().second.push_back(value);
        }
    }

    std::uint64_t answer = 0;
    for (auto [result, operands] : values) {
        if (HasValidCombination(result, operands)) {
            answer += result;
        }
    }
    std::cout << "answer: " << answer << '\n';
}

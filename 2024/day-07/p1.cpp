#include <iostream>
#include <sstream>
#include <ranges>

using Operands = std::vector<std::uint64_t>;

auto HasValidCombination(const std::uint64_t target, std::uint64_t result, std::ranges::subrange<Operands::iterator> rest) -> bool {
    if (rest.empty()) {
        return result == target;
    }
    if (HasValidCombination(target, result + rest.front(), rest.next())) {
        return true;
    }
    if (HasValidCombination(target, result * rest.front(), rest.next())) {
        return true;
    }
    return false;
}

auto HasValidCombination(const std::uint64_t target, std::ranges::subrange<Operands::iterator> rest) -> bool {
    if (rest.empty()) {
        throw std::runtime_error("Invalid number of operands");
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

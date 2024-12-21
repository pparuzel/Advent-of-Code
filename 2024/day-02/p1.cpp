#include <cmath>
#include <iostream>
#include <sstream>

auto LoadReports() -> std::vector<std::vector<int>> {
    std::string line;
    std::vector<std::vector<int>> reports;
    while (std::getline(std::cin, line)) {
        std::stringstream ss;
        ss << line;
        int level;
        auto& report = reports.emplace_back();
        while (ss >> level) {
            report.push_back(level);
        }
    }
    return reports;
}

auto CheckLevels(const std::vector<int>& levels) -> bool {
    if (levels.size() < 2) {
        return true;
    }
    const bool last_negative = (levels[0] - levels[1]) < 0;
    for (int i = 1; i < levels.size(); ++i) {
        const int diff = levels[i - 1] - levels[i];
        const int absdiff = std::abs(diff);
        const bool negative = diff < 0;
        if (absdiff < 1 || absdiff > 3 || last_negative != negative) {
            return false;
        }
    }
    return true;
}

int main() {
    const auto reports = LoadReports();
    int safe = 0;
    for (const std::vector<int>& report : reports) {
        safe += CheckLevels(report);
    }
    std::cout << "Answer: " << safe << '\n';
}
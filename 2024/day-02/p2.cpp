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
    const int last_negative = levels[0] < levels[1];
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

auto CheckLevelsTolerate1(const std::vector<int>& report) -> bool {
    auto begin = report.cbegin();
    auto end = report.cend();
    for (auto it = begin; it != end; ++it) {
        std::vector<int> fixed_report(begin, it);
        fixed_report.insert(fixed_report.end(), std::next(it), end);
        if (CheckLevels(fixed_report)) {
            return true;
        }
    }
    return false;
}

int main() {
    const auto reports = LoadReports();
    int safe = 0;
    for (const std::vector<int>& report : reports) {
        safe += CheckLevelsTolerate1(report);
    }
    std::cout << "Answer: " << safe << '\n';
}
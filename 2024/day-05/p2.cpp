#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>

using DisallowedMap = std::unordered_map<int, std::unordered_set<int>>;

template <typename Iter>
bool IsCorrectSequence(Iter begin, Iter end, const DisallowedMap& disallowed_for) {
    std::unordered_set<int> disallowed;
    return std::find_if(begin, end, [&](int value) {
        if (disallowed.find(value) != disallowed.end()) {
            // Reject value!
            return true;
        }
        auto found = disallowed_for.find(value);
        if (found != disallowed_for.end()) {
            disallowed.insert(found->second.begin(), found->second.end());
        }
        return false;
    }) == end;
}

int FixedMiddleTotal(const std::vector<int>& values, const DisallowedMap& disallowed_next, const DisallowedMap& disallowed_prev) {
    if (IsCorrectSequence(values.begin(), values.end(), disallowed_next) &&
        IsCorrectSequence(values.rbegin(), values.rend(), disallowed_prev)) {
        return 0;
    }
    if (values.size() % 2 == 0) {
        throw std::runtime_error("did not expect even-size `values` array!");
    }
    // 35,73,15,79,81,85,11
    // {85,81,11,79},35,{15,73}
    // {81,11,79,35,85},73,{15}
    // {85,81,11},79,{35,73,15}
    // The left side has three elements which is around...
    // ...the halfpoint we are looking for, hence, "79" is the answer
    for (int i = 0; i < values.size(); ++i) {
        auto found = disallowed_prev.find(values[i]);
        if (found != disallowed_prev.end()) {
            auto count = std::count_if(values.begin(), values.end(), [&](int i) {
                return found->second.find(i) != found->second.end();
            });
            if (count == values.size() / 2) {
                return values[i];
            }
        }
    }
    // There must have not been enough rules.
    // In that case simply return the middle element.
    return values[values.size() / 2];
}

int main() {
    int operand1, operand2;
    char c = '|';
    std::vector<int> updates;
    DisallowedMap disallowed_prev;
    DisallowedMap disallowed_next;
    // Read rules
    while (c == '|') {
        std::cin >> operand1 >> c >> operand2;
        disallowed_prev[operand1].insert(operand2);
        disallowed_next[operand2].insert(operand1);
    }
    {
        char comma;
        std::cin >> comma;
        if (c != ',' || comma != ',') {
            throw std::runtime_error("Unexpected input");
        }
    }
    updates.push_back(operand1);
    updates.push_back(operand2);

    int total = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        // Read updates
        std::stringstream str;
        char comma;
        str << line;
        int val;
        while (str >> val) {
            str >> comma;
            updates.push_back(val);
        }
        total += FixedMiddleTotal(updates, disallowed_next, disallowed_prev);
        updates.clear();
    }
    std::cout << "answer: " << total << '\n';
}

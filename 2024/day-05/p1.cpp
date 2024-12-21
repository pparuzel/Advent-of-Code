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

        // Check line forwards for disallowed_next and backwards for disallowed_prev
        bool ok = IsCorrectSequence(updates.begin(), updates.end(), disallowed_next) &&
                  IsCorrectSequence(updates.rbegin(), updates.rend(), disallowed_prev);

        if (ok) {
            if (updates.size() % 2 == 0) {
                throw std::runtime_error("did not expect even-size `updates` array!");
            }
            total += updates[updates.size() / 2];
        }
        updates.clear();
    }
    std::cout << "answer: " << total << '\n';
}

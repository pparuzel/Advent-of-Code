#include <iostream>
#include <string_view>

int CountWordRightSide(std::vector<std::string>& arr, std::string_view word) {
    if (word.empty()) {
        return 0;
    }
    // Only check 4 cases on the RIGHT SIDE:
    // - Two diagonals left-to-right (up-diagonal, down-diagonal)
    // - Two sideways (down-vertical, right-horizontal)
    //       S
    //     A
    //   M
    // X M A S
    // M M
    // A   A
    // S     S
    //
    // The rest of the cases (right-to-left, etc.) can be counted by supplying
    // the same word in reverse, e.g.:
    //
    //       X
    //     M
    //   A
    // S A M X
    // A A
    // M   M
    // X     X
    //
    // Since all left-to-right cases can represent right-to-left in reverse:
    // - down-diagonal SAMX can represent right-to-left up-diagonal XMAS
    // - up-diagonal SAMX can represent right-to-left down-diagonal XMAS
    // - horizontal SAMX can represent right-to-left horizontal XMAS
    // - vertical SAMX can represent right-to-left vertical XMAS
    // Thus, all cases are covered.

    // Four categories for debugging purposes
    // could just be one `count` variable
    int count[4] = {0, 0, 0, 0};
    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < arr[i].size(); ++j) {
            if (arr[i][j] != word.front()) {
                continue;
            }
            // Horizontal check
            bool horizontal_safe = j <= arr[i].size() - word.size();
            bool vertical_safe = i <= arr.size() - word.size();
            if (horizontal_safe) {
                ++count[0];  // Assume it's correct
                for (int k = 0; k < word.size(); ++k) {
                    if (arr[i][j + k] != word[k]) {
                        --count[0];  // Revert when it's not correct
                        break;
                    }
                }
            }
            // Vertical check
            if (vertical_safe) {
                ++count[1];
                for (int k = 0; k < word.size(); ++k) {
                    if (arr[i + k][j] != word[k]) {
                        --count[1];
                        break;
                    }
                }
            }
            // Down-diagonal check
            if (horizontal_safe && vertical_safe) {
                ++count[2];
                for (int k = 0; k < word.size(); ++k) {
                    if (arr[i + k][j + k] != word[k]) {
                        --count[2];
                        break;
                    }
                }
            }
            // Up-diagonal check
            if (horizontal_safe && i >= word.size() - 1) {
                ++count[3];
                for (int k = 0; k < word.size(); ++k) {
                    if (arr[i - k][j + k] != word[k]) {
                        --count[3];
                        break;
                    }
                }
            }
        }
    }
    std::cout << "Word: \"" << word << "\"\n";
    std::cout << "\tHorizontal   : " << count[0] << '\n';
    std::cout << "\tVertical     : " << count[1] << '\n';
    std::cout << "\tDown-Diagonal: " << count[2] << '\n';
    std::cout << "\tUp-Diagonal  : " << count[3] << '\n';
    return count[0] + count[1] + count[2] + count[3];
}

int main() {
    std::vector<std::string> arr;
    std::string line;
    while (std::getline(std::cin, line)) {
        arr.emplace_back(std::move(line));
    }
    int total = CountWordRightSide(arr, "XMAS") + CountWordRightSide(arr, "SAMX");
    std::cout << "answer: " << total << '\n';
}

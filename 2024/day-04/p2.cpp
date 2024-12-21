#include <iostream>
#include <string_view>

int CountXMAS(std::vector<std::string>& arr) {
    int count = 0;
    for (int i = 1; i < arr.size() - 1; ++i) {
        for (int j = 1; j < arr[i].size() - 1; ++j) {
            if (arr[i][j] != 'A') {
                continue;
            }
            bool down_diagonal = false, up_diagonal = false;
            if ((arr[i - 1][j - 1] == 'M' && arr[i + 1][j + 1] == 'S') ||
                (arr[i - 1][j - 1] == 'S' && arr[i + 1][j + 1] == 'M')) {
                down_diagonal = true;
            }
            if ((arr[i + 1][j - 1] == 'S' && arr[i - 1][j + 1] == 'M') ||
                (arr[i + 1][j - 1] == 'M' && arr[i - 1][j + 1] == 'S')) {
                up_diagonal = true;
            }
            if (down_diagonal && up_diagonal) {
                ++count;
            }
        }
    }
    return count;
}

int main() {
    std::vector<std::string> arr;
    std::string line;
    while (std::getline(std::cin, line)) {
        arr.emplace_back(std::move(line));
    }
    int total = CountXMAS(arr);
    std::cout << "answer: " << total << '\n';
}

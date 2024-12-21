#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <unordered_map>


int main() {
    std::vector<int> l1, l2;
    std::unordered_map<int, unsigned> m1;
    while (std::cin >> l1.emplace_back() >> l2.emplace_back());
    for (int v : l2) {
        ++m1[v];
    }
    int answer = 0;
    for (int v : l1) {
        answer += m1[v] * v;
    }
    std::cout << "answer: " << answer << '\n';
}

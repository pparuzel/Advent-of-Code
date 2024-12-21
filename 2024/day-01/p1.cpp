#include <iostream>
#include <vector>
#include <functional>
#include <cmath>

int absdiff(int a, int b) {
    return std::abs(a - b);
}

int main() {
    std::vector<int> l1, l2;
    while (std::cin >> l1.emplace_back() >> l2.emplace_back());
    std::sort(l1.begin(), l1.end());
    std::sort(l2.begin(), l2.end());
    const int answer = std::transform_reduce(l1.begin(), l1.end(), l2.begin(), 0, std::plus<int>(), absdiff);
    std::cout << "answer: " << answer << '\n';
}

#include <iostream>
#include <string_view>


int main() {
    // regex: mul\(\d+,\d+\)
    int answer = 0;
    std::string_view mul_prefix = "mul(";
    std::string_view do_view = "do()";
    std::string_view dont_view = "don't()";
    std::string buffer;
    bool mul_enabled = true;
    while (std::getline(std::cin, buffer)) {
        buffer += "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        for (int i = 0; i < buffer.size(); ++i) {
            if (buffer[i] == 'd') {
                auto do_dont = std::string_view(buffer.data() + i, dont_view.size());
                if (do_dont == dont_view) {
                    mul_enabled = false;
                    i += dont_view.size();
                    continue;
                }
                do_dont.remove_suffix(dont_view.size() - do_view.size());
                if (do_dont == do_view) {
                    mul_enabled = true;
                    i += do_view.size();
                }
            }
            if (!mul_enabled || buffer[i] != 'm') {
                continue;
            }
            if (std::string_view(buffer.data() + i, mul_prefix.size()) != mul_prefix) {
                continue;
            }
            i += mul_prefix.size();
            // prefix passed, expect at least one digit
            if (!isdigit(buffer[i])) {
                continue;
            }
            int number1 = 0;
            while (isdigit(buffer[i])) {
                number1 = number1 * 10 + buffer[i] - '0';
                ++i;
            }

            if (buffer[i] != ',' || !isdigit(buffer[i + 1])) {
                continue;
            }
            ++i;
            int number2 = 0;
            while (isdigit(buffer[i])) {
                number2 = number2 * 10 + buffer[i] - '0';
                ++i;
            }
            if (buffer[i] == ')') {
                answer += number1 * number2;
            }
        }
    }
    std::cout << "answer: " << answer << '\n';
}
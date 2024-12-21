#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Connections = std::unordered_map<std::string, std::vector<std::string>>;
using StrRef = const std::string&;
using StringVector = std::vector<std::string>;
using VisitSet = std::unordered_set<std::size_t>;

auto Encode6Chars(char chr0, char chr1, char chr2,
                  char chr3, char chr4, char chr5) -> std::size_t {
    static_assert(sizeof(std::size_t) >= sizeof(char) * 6);
    return (static_cast<std::size_t>(chr0) << 40 |
            static_cast<std::size_t>(chr1) << 32 |
            static_cast<std::size_t>(chr2) << 24 |
            static_cast<std::size_t>(chr3) << 16 |
            static_cast<std::size_t>(chr4) << 8  |
            static_cast<std::size_t>(chr5));
}

auto EncodePaths(const std::string& first,
                 const std::string& second,
                 const std::string& third) -> std::size_t {
    // Find the lexicographical order of the three strings
    const std::string* a = &first;
    const std::string* b = &second;
    const std::string* c = &third;

    if (*b < *a) { std::swap(a, b); }
    if (*c < *b) { std::swap(b, c); }
    if (*b < *a) { std::swap(a, b); }

    return Encode6Chars((*a)[0], (*a)[1],
                        (*b)[0], (*b)[1],
                        (*c)[0], (*c)[1]);
}

class InterconnectedThree {
public:
    explicit InterconnectedThree(const Connections& conns)
        : conns_(conns) {
    }

    auto Count(char starting_letter) -> std::size_t {
        std::size_t count = 0;
        VisitSet visited;
        for (const auto& node : conns_) {
            if (node.first[0] != starting_letter) {
                continue;
            }
            Visit(node.first, count, visited);
        }
        return count;
    }

private:
    void Visit2(StrRef first, StrRef second,
                std::size_t& count, VisitSet& visited) {
        for (StrRef third : conns_.find(second)->second) {
            const StringVector& conns_third = conns_.find(third)->second;
            if (std::find(conns_third.begin(), conns_third.end(), first) ==
                conns_third.end()) {
                continue;
            }
            std::size_t key = EncodePaths(first, second, third);
            if (visited.find(key) == visited.end()) {
                visited.insert(key);
                ++count;
            }
        }
    }

    void Visit(StrRef first, std::size_t& count, VisitSet& visited) {
        for (StrRef second : conns_.find(first)->second) {
            Visit2(first, second, count, visited);
        }
    }

private:
    const Connections& conns_;
};


int main() {
    std::string line;
    Connections conns;
    while (std::getline(std::cin, line)) {
        auto dash = line.find('-');
        if (dash == std::string::npos) {
            throw std::runtime_error("error: Could not split by '-'");
        }
        line[dash] = '\0';
        std::string a = line.data();
        std::string b = line.data() + dash + 1;
        conns[a].push_back(b);
        conns[std::move(b)].push_back(std::move(a));
    }
    auto solver = InterconnectedThree(conns);
    auto ans = solver.Count('t');
    std::cout << "Answer: " << ans << '\n';
}

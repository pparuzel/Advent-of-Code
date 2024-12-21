#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>


// To distinguish 90 degree rotations we set up values so that:
// abs(Left) == abs(Right) && abs(Up) == abs(Down)
enum Facing : int {
    Left = -2,
    Up = -1,
    None = 0,
    Down = 1,
    Right = 2,
};

auto GetRotationCost(Facing origin_dir, Facing desired_dir) {
    assert(origin_dir != None && desired_dir != None);
    if (desired_dir == origin_dir) {
        // 0-degree rotation
        return 0;
    }
    if (std::abs(desired_dir) == std::abs(origin_dir)) {
        // 180-degree rotation
        return 2000;
    }
    // 90-degree rotation
    return 1000;
}

template <typename Map>
auto GetOrSetDefault(Map& m, const typename Map::key_type& key, typename Map::mapped_type default_value) -> typename Map::mapped_type& {
    return m.try_emplace(key, default_value).first->second;
}

template <typename Map>
auto GetOrDefault(const Map& m, const typename Map::key_type& key, typename Map::mapped_type default_value) -> typename Map::mapped_type {
    auto found = m.find(key);
    if (found == m.end()) {
        return default_value;
    }
    return found->second;
}

struct MinCostKeyHash {
    std::size_t operator()(const std::tuple<int, int, Facing>& t) const {
        // Pretty solid assumptions.
        // Worst case we get more hash collisions
        // and slightly degraded performance.
        constexpr int MAX_DIM = 10'000;
        constexpr int MAX_DIR = 5;

        const auto [row, col, facing] = t;
        return (row * MAX_DIM + col) * MAX_DIR + (facing + 2);
    }
};

class Dijkstra {
public:
    explicit Dijkstra(std::vector<std::string>& input, bool update_input = false)
        : input_(input)
        , update_input_(update_input) {
    }

    struct Node;

    using Offset = std::pair<int, int>;
    using Cost = std::size_t;
    using PriorityQueue = std::priority_queue<Node, std::vector<Node>, std::greater<>>;  // Min-heap
    using MinCost = std::unordered_map<std::tuple<int, int, Facing>, Cost, MinCostKeyHash>;  // (Row, Col, Facing) -> Cost

    inline static const std::vector<std::pair<Offset, Facing>> directions = {
        {{-1, 0}, Facing::Up},
        {{1, 0}, Facing::Down},
        {{0, -1}, Facing::Left},
        {{0, 1}, Facing::Right},
    };
    inline static auto inf = std::numeric_limits<Cost>::max();

    struct Node {
        Cost cost;
        int row;
        int col;
        Facing dir;

        bool operator>(const Node& other) const {
            return cost > other.cost;
        }
    };

    auto Run(Offset start, Offset finish, Facing start_facing) -> std::optional<Cost> {
        // Initialize starting parameters
        MinCost min_cost;
        PriorityQueue pq;
        pq.emplace(0, start.first, start.second, start_facing);

        while (!pq.empty()) {
            auto [current_cost, row, col, facing] = pq.top();
            pq.pop();

            if (input_[row][col] == 'E') {
                // Exit early, we have reached the destination
                if (update_input_) {
                    UpdateReconstructPath(min_cost, {row, col});
                }
                return current_cost;
            }

            if (current_cost > GetOrDefault(min_cost, {row, col, facing}, inf)) {
                // This cost is already worse
                continue;
            }

            // Explore neighbors
            for (const auto& [delta, new_facing] : directions) {
                int new_row = row + delta.first;
                int new_col = col + delta.second;

                const auto rotation_cost = GetRotationCost(facing, new_facing);
                if (rotation_cost > 1000) {
                    // Ignore 180-degree rotations
                    continue;
                }
                if (input_[new_row][new_col] == '#') {
                    // That's a wall... Ouch
                    continue;
                }
                Cost new_cost = current_cost + rotation_cost + 1;
                auto& neighbor_cost = GetOrSetDefault(min_cost, {new_row, new_col, new_facing}, inf);
                if (new_cost < neighbor_cost) {
                    neighbor_cost = new_cost;
                    pq.emplace(new_cost, new_row, new_col, new_facing);
                }
            }
        }
        // No path was found
        return std::nullopt;
    }

private:
    auto UpdateReconstructPath(const MinCost& min_cost, Offset pos) -> void {
        auto [best_cost, trail, offset] = BestNeighbor(min_cost, pos.first, pos.second);
        // Do not update the finish node
        pos.first -= offset.first;
        pos.second -= offset.second;
        // Reconstruct the path backwards
        while (input_[pos.first][pos.second] != 'S') {
            std::tie(best_cost, trail, offset) = BestNeighbor(min_cost, pos.first, pos.second);
            input_[pos.first][pos.second] = trail;

            pos.first -= offset.first;
            pos.second -= offset.second;
        }
    }

    auto BestNeighbor(const MinCost& min_cost, int r, int c) -> std::tuple<Cost, char, Offset> {
        using Neighbor = std::tuple<Cost, char, Offset>;
        // Find the lowest-cost neighbor by direction
        auto neighbors = {
            Neighbor(GetOrDefault(min_cost, {r, c, Facing::Right}, inf), '>', {0, 1}),
            Neighbor(GetOrDefault(min_cost, {r, c, Facing::Up}, inf), '^', {-1, 0}),
            Neighbor(GetOrDefault(min_cost, {r, c, Facing::Down}, inf), 'v', {1, 0}),
            Neighbor(GetOrDefault(min_cost, {r, c, Facing::Left}, inf), '<', {0, -1}),
        };
        return *std::min_element(neighbors.begin(), neighbors.end());
    }

private:
    std::vector<std::string>& input_;
    bool update_input_;
};

using InputData = std::vector<std::string>;

auto ReadInput(std::istream& input) -> std::tuple<InputData, Dijkstra::Offset, Dijkstra::Offset> {
    InputData input_data;
    std::string line;
    Dijkstra::Offset start{-1, -1}, finish{-1, -1};
    int lineno = 0;
    for (; std::getline(input, line); ++lineno) {
        auto found = input_data.emplace_back(std::move(line)).find_first_of("SE");
        if (found == std::string::npos) {
            continue;
        }
        (input_data.back()[found] == 'S' ? start : finish) = {lineno, found};
    }
    assert(input_data.at(start.first).at(start.second) == 'S');
    assert(input_data.at(finish.first).at(finish.second) == 'E');
    return std::make_tuple(input_data, start, finish);
}

int main(int argc, char* argv[]) {
    // Read the input data. Default to standard input
    std::istream* source = &std::cin;
    std::fstream file;

    if (argc > 1) {
        // Use file as input
        file.open(argv[1]);
        if (!file.is_open()) {
            std::cerr << "error: Could not open file " << argv[1] << std::endl;
            return 255;
        }
        source = &file;
    }

    auto [input, start, finish] = ReadInput(*source);
    // The input data will contain an optimal
    // reconstructed path after the solution is found
    bool update_when_solved = true;
    // Run Dijkstra's algorithm facing East
    auto answer = Dijkstra(input, update_when_solved).Run(start, finish, Facing::Right);
    if (!answer.has_value()) {
        std::cerr << "error: No path from S to E was found" << std::endl;
        return 1;
    }
    for (const auto& line : input) {
        std::cout << line << '\n';
    }
    std::cout << "Answer: " << answer.value() << '\n';
}

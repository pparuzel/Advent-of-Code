#include <iostream>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>


constexpr auto MAX_INPUT_DIM = 256;

using Vec2D = std::pair<int, int>;
using Maze = std::vector<std::string>;

struct Node {
    std::size_t cost;
    struct Key {
        int row;
        int col;
        Vec2D facing;

        bool operator<(const Key& other) const {
            return std::tie(row, col, facing) <
                   std::tie(other.row, other.col, other.facing);
        }

        bool operator==(const Key& other) const {
            return std::tie(row, col, facing) ==
                   std::tie(other.row, other.col, other.facing);
        }
    } key;

    bool operator>(const Node& other) const {
        return std::tie(cost, key) > std::tie(other.cost, other.key);
    }
};

template <>
struct std::hash<Node::Key> {
    std::uint64_t operator()(const Node::Key& key) const {
        return (key.row * MAX_INPUT_DIM) + key.col;
    }
};

using PQ = std::priority_queue<Node, std::vector<Node>, std::greater<>>;
using PrevMap = std::unordered_map<Node::Key, std::vector<Node::Key>>;

struct MazeInfo {
    Maze maze;
    Vec2D start;
    Vec2D finish;

    void AssertCorrect() {
        assert(maze[start.first][start.second] == 'S');
        assert(maze[finish.first][finish.second] == 'E');
    }
};

auto Rotate90(Vec2D direction, bool clockwise=true) -> Vec2D
{
    if (!clockwise) {
        return {direction.second, -direction.first};
    }
    return {-direction.second, direction.first};
}

auto BuildNeighbors(const Maze& maze, int row, int col, const Vec2D& facing)
    -> std::vector<Node>
{
    auto new_row = row + facing.first;
    auto new_col = col + facing.second;
    if (maze[new_row][new_col] == '#') {
        return std::vector<Node>{
            {1000, row, col, Rotate90(facing, true)},
            {1000, row, col, Rotate90(facing, false)},
        };
    }
    return std::vector<Node>{
        {1, row + facing.first, col + facing.second, facing},
        {1000, row, col, Rotate90(facing, true)},
        {1000, row, col, Rotate90(facing, false)},
    };
}

void BacktrackVisitGrid(const Maze& maze,
                        const PrevMap& prev,
                        const Node::Key& key,
                        std::unordered_set<std::size_t>& visited) {
    visited.insert(key.row * MAX_INPUT_DIM + key.col);
    auto found = prev.find(key);
    if (found == prev.end()) {
        return;
    }
    for (const auto& prev_node : found->second) {
        BacktrackVisitGrid(maze, prev, prev_node, visited);
    }
};

auto Dijkstra(const Maze& maze, Vec2D start, Vec2D finish,
              const Vec2D& start_facing) -> int {

    constexpr auto MAXCOST = std::numeric_limits<std::size_t>::max();
    auto min_cost = std::unordered_map<Node::Key, std::size_t>();
    auto prev = PrevMap();
    auto GetMinCost = [&](const auto& key) -> std::size_t {
        auto found = min_cost.find(key);
        if (found == min_cost.end()) {
            return MAXCOST;
        }
        return found->second;
    };
    auto init_nodes = BuildNeighbors(maze, start.first, start.second,
                                     start_facing);
    PQ pq(init_nodes.begin(), init_nodes.end());
    for (const Node& node : init_nodes) {
        min_cost[node.key] = node.cost;
    }
    while (!pq.empty()) {
        Node node = std::move(pq.top());
        pq.pop();
        if (node.cost > GetMinCost(node.key)) {
            continue;
        }
        auto [row, col, facing] = node.key;
        auto neighbors = BuildNeighbors(maze, row, col, facing);
        for (Node& new_node : neighbors) {
            new_node.cost += node.cost;
            const auto curr_min_cost = GetMinCost(new_node.key);
            if (new_node.cost < curr_min_cost) {
                min_cost[new_node.key] = new_node.cost;
                pq.push(new_node);
                prev[new_node.key] = {node.key};
            } else if (new_node.cost == curr_min_cost) {
                prev[new_node.key].push_back(node.key);
            }
        }
    }
    const auto dirs = std::vector<Node::Key>{
        {finish.first, finish.second, {-1, 0}},
        {finish.first, finish.second, {0, 1}},
        {finish.first, finish.second, {1, 0}},
        {finish.first, finish.second, {0, -1}},
    };
    auto best_key = *std::min_element(dirs.begin(), dirs.end(),
        [&](const auto& lhs, const auto& rhs) {
            return GetMinCost(lhs) < GetMinCost(rhs);
        });

    std::unordered_set<std::size_t> visited;
    BacktrackVisitGrid(maze, prev, best_key, visited);
    return visited.size();
}

auto ReadInput() -> MazeInfo {
    std::string line;
    MazeInfo info;
    int lineno = 0;
    while (std::getline(std::cin, line)) {
        auto found_start = line.find('S');
        auto found_end = line.find('E');
        if (found_start != std::string::npos) {
            info.start.first = lineno;
            info.start.second = found_start;
        }
        if (found_end != std::string::npos) {
            info.finish.first = lineno;
            info.finish.second = found_end;
        }
        info.maze.emplace_back(std::move(line));
        ++lineno;
    }
    info.AssertCorrect();
    return info;
}

int main() {
    auto info = ReadInput();
    auto answer = Dijkstra(info.maze, info.start, info.finish, {0, 1});
    std::cout << "Answer: " << answer << '\n';
}

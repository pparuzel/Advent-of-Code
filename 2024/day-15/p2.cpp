#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cassert>
#include <iomanip>

using WarehouseMap = std::vector<std::string>;

auto TryPushObstacleH(WarehouseMap& map, int origin_row, int origin_col, int cdir) -> bool {
    auto next_col = origin_col + cdir;
    const char next_entity = map[origin_row][next_col];
    switch (next_entity) {
        case '.':
            // Empty space available, move the entity
            std::swap(map[origin_row][next_col], map[origin_row][origin_col]);
            return true;
        case ']':  [[fallthrough]];
        case '[':
            if (TryPushObstacleH(map, origin_row, next_col, cdir)) {
                std::swap(map[origin_row][next_col], map[origin_row][origin_col]);
                return true;
            }
            [[fallthrough]];
        case '#':
            return false;
        default:  [[unlikely]]
            throw std::runtime_error("Unexpected map entity \"" + std::to_string(next_entity) + '"');
    }
}

auto TryPushObstacleV(WarehouseMap& map, int origin_row, int origin_col, int rdir) -> bool {
    struct State {
        int row;
        int col;

        bool operator==(const State& other) const {
            return row == other.row && col == other.col;
        }
    };
    const auto HashState = [&](const State& state) -> std::size_t {
        return state.col * (map.size() + 1) + state.row;
    };
    auto next_row = origin_row + rdir;
    auto dir = map[next_row][origin_col] == ']' ? -1 : 1;
    std::vector<State> swap_next({State{next_row, origin_col}, State{next_row, origin_col + dir}});
    std::unordered_set<State, decltype(HashState)> swap_set(swap_next.begin(), swap_next.end(), 5, HashState);
    auto front = 0;
    for (; front < swap_next.size(); ++front) {
        auto this_row = swap_next[front].row;
        auto this_col = swap_next[front].col;
        auto next_row = this_row + rdir;
        if (map[next_row][this_col] == '#') {
            // A box is blocked. We cannot move.
            return false;
        }
        if (map[next_row][this_col] == '[' || map[next_row][this_col] == ']') {
            int dir = map[next_row][this_col] == ']' ? -1 : 1;
            auto child_1 = State{next_row, this_col};
            auto child_2 = State{next_row, this_col + dir};
            bool duplicate_1 = !swap_set.insert(child_1).second;
            bool duplicate_2 = !swap_set.insert(child_2).second;
            if (duplicate_1) {
                assert(duplicate_2 && "Unexplored state ignored!");
                // Consider these boxes pushed downwards.
                // The middle box could be added twice by its parents.
                // . . [ ] [ ] . .
                // . [ ] [ ] [ ] .
                // . . . . . . . .
                // We cannot allow duplicates but also have to maintain
                // order, therefore, we leverage a vector + set.
                continue;
            }
            assert(!duplicate_2 && "Unexpected duplicate child_2");
            assert(map[child_2.row][child_2.col] == '[' || map[child_2.row][child_2.col] == ']');
            swap_next.push_back(child_1);
            swap_next.push_back(child_2);
        }
    }
    // No box is blocked, move the obstacles in reverse order.
    for (auto it = swap_next.rbegin(); it != swap_next.rend(); ++it) {
        std::swap(map[it->row][it->col], map[it->row + rdir][it->col]);
    }
    return true;
}

auto BuildWideWarehouse(const WarehouseMap& input_map) -> WarehouseMap {
    WarehouseMap map(input_map.size(), std::string(input_map[0].size() * 2, '?'));
    for (int row = 0; row < input_map.size(); ++row) {
        for (int col = 0; col < map[0].size(); col += 2) {
            const char entity = input_map[row][col / 2];
            char& lhs = map[row][col];
            char& rhs = map[row][col + 1];
            switch (entity) {
                case '#':
                    lhs = '#';
                    rhs = '#';
                    break;
                case 'O':
                    lhs = '[';
                    rhs = ']';
                    break;
                case '.':
                    lhs = '.';
                    rhs = '.';
                    break;
                case '@':
                    lhs = '@';
                    rhs = '.';
                    break;
                default:  [[unlikely]]
                    throw std::runtime_error(std::string("Unexpected map entity: '") + entity + '\'');
            }
        }
    }
    return map;
}

int main() {
    WarehouseMap input_map;
    std::string line;
    std::string moves;
    // Read the input file for the map, move list and the robot
    int origin_row = -1, origin_col = -1;
    int row_read = 0;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        if (line.starts_with('#')) {
            auto found = line.find('@');
            if (found != std::string::npos) {
                // Robot was found at column `origin_col`
                origin_row = row_read;
                origin_col = found;
            }
            input_map.emplace_back(std::move(line));
            ++row_read;
        } else {
            // Add moves
            for (char c : line) {
                if (c == '>' || c == '<' || c == '^' || c == 'v') {
                    moves += c;
                }
            }
        }
    }
    // Check for the robot
    if (origin_row == -1) {
        throw std::runtime_error("Robot was not found");
    }
    assert(input_map[origin_row][origin_col] == '@');
    // Adjust the map to satisfy new requirements
    WarehouseMap map = BuildWideWarehouse(input_map);
    assert(!map.empty());
    auto Print = [](const auto& map) {
        std::cout << "  ";
        for (int i = 0; i < map[0].size(); ++i) {
            std::cout << i % 10;
        }
        std::cout << '\n';
        int r = 0;
        for (auto&& l : map) {
            std::cout << std::setw(2) << r++ << l << '\n';
        }
    };
    const auto GetGpsValue = [W=map[0].size()](std::size_t row, std::size_t col) {
        return 100 * row + col;//std::min(col, W - (col + 1));
    };
    // Print(map);
    // Check again for the robot on the widened map
    origin_col *= 2;
    assert(map[origin_row][origin_col] == '@');
    // Move the robot
    for (int move = 0; move < moves.size(); ++move) {
        // std::cout << "Move: " << moves[move] << '\n';
        int rdir = 0, cdir = 0;  // row-velocity, col-velocity
        switch (moves[move]) {
            case '^':
                rdir = -1;
                break;
            case '>':
                cdir = 1;
                break;
            case 'v':
                rdir = 1;
                break;
            case '<':
                cdir = -1;
                break;
            default: [[unlikely]]
                throw std::runtime_error(std::string("Unexpected move: '") + moves[move] + '\'');
        }
        const char next_entity = map[origin_row + rdir][origin_col + cdir];
        int bracket_dir = next_entity == ']' ? -1 : 1;
        switch (next_entity) {
            case '#':
                break;
            case '[':  [[fallthrough]];
            case ']':
                if (rdir != 0) {
                    if (!TryPushObstacleV(map, origin_row, origin_col, rdir)) {
                        break;
                    }
                } else {
                    if (!TryPushObstacleH(map, origin_row, origin_col, cdir)) {
                        break;
                    }
                }
                [[fallthrough]];
            case '.':
                // Update the movement of the robot
                map[origin_row][origin_col] = '.';
                origin_row += rdir;
                origin_col += cdir;
                map[origin_row][origin_col] = '@';
                break;
            default: [[unlikely]]
                throw std::runtime_error(std::string("Unexpected map entity: '") + map[origin_row + rdir][origin_col + cdir] + '\'');
        }
        // Make sure the robot's position was updated
        assert(map[origin_row][origin_col] == '@');
        // Print(map);
    }
    // Calculate the sum of the GPS values
    int answer = 0;
    for (int row = 0; row < map.size(); ++row) {
        for (int col = 0; col < map[0].size(); ++col) {
            if (map[row][col] != '[') {
                continue;
            }
            answer += GetGpsValue(row, col);
        }
        // std::cout << map[row] << '\n';
    }
    std::cout << '\n';
    std::cout << "Answer: " << answer << '\n';
}

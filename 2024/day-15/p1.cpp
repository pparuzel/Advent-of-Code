#include <iostream>
#include <cassert>

using WarehouseMap = std::vector<std::string>;

auto GetGpsValue(int row, int col) {
    return 100 * row + col;
}

auto AttemptToPush(WarehouseMap& map, int row, int col, int rdir, int cdir) -> bool {
    auto next_row = row + rdir;
    auto next_col = col + cdir;
    switch (map[next_row][next_col]) {
        case '.':
            // Empty space available => may push
            return true;
        case 'O':
            if (AttemptToPush(map, next_row, next_col, rdir, cdir)) {
                map[next_row + rdir][next_col + cdir] = map[next_row][next_col];
                return true;
            }
            [[fallthrough]];
        case '#':
            // Logic when we encounter a wall
            return false;
        default:
            throw std::runtime_error("Unexpected map entity");
    }
}

int main() {
    WarehouseMap map;
    std::string line;
    std::string moves;
    // Read the input file for the map, move list and the robot
    int robot_row = -1, robot_col = -1;
    int row_read;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        if (line.starts_with('#')) {
            auto found = line.find('@');
            if (found != std::string::npos) {
                // Robot was found at column `robot_col`
                robot_row = row_read;
                robot_col = found;
            }
            map.emplace_back(std::move(line));
            ++row_read;
        } else {
            moves += std::move(line);
            if (moves.ends_with('\n')) {
                moves.pop_back();
            }
        }
    }
    if (robot_row == -1) {
        throw std::runtime_error("Robot was not found");
    }
    assert(map[robot_row][robot_col] == '@');
    // Move the robot
    for (int move = 0; move < moves.size(); ++move) {
        // std::cout << "Move: " << moves[move] << '\n';
        int rvel = 0, cvel = 0;  // row-velocity, col-velocity
        switch (moves[move]) {
            case '^':
                rvel = -1;
                break;
            case '>':
                cvel = 1;
                break;
            case 'v':
                rvel = 1;
                break;
            case '<':
                cvel = -1;
                break;
            default: [[unlikely]]
                throw std::runtime_error(std::string("Unexpected move: '") + moves[move] + '\'');
        }
        switch (map[robot_row + rvel][robot_col + cvel]) {
            case '#':
                break;
            case 'O':
                if (!AttemptToPush(map, robot_row, robot_col, rvel, cvel)) {
                    break;
                }
                [[fallthrough]];
            case '.':
                // Logic for moving the robot by its velocity
                map[robot_row][robot_col] = '.';
                robot_row += rvel;
                robot_col += cvel;
                map[robot_row][robot_col] = '@';
                break;
            default: [[unlikely]]
                throw std::runtime_error(std::string("Unexpected map entity: '") + map[robot_row + rvel][robot_col + cvel] + '\'');
        }
    }
    // Print the map and calculate the sum of the GPS values
    int answer = 0;
    for (int row = 0; row < map.size(); ++row) {
        for (int col = 0; col < map[0].size(); ++col) {
            if (map[row][col] != 'O') {
                continue;
            }
            answer += GetGpsValue(row, col);
        }
        // std::cout << map[row] << '\n';
    }
    std::cout << '\n';
    std::cout << "Answer: " << answer << '\n';
}

#include <iostream>

enum MapEntity : unsigned {
    Floor = 0,
    Obstacle = 1 << 0,
    UpTrail = 1 << 1,
    RightTrail = 1 << 2,
    DownTrail = 1 << 3,
    LeftTrail = 1 << 4,
    HasTrail = UpTrail | RightTrail | DownTrail | LeftTrail,
};


MapEntity TrailFromDirection(int dx, int dy) {
    // (0, -1) => up
    // (1, 0) => right
    // (0, 1) => down
    // (-1, 0) => left
    if (dx == 0) {
        if (dy < 0) {
            return UpTrail;
        }
        if (dy > 0) {
            return DownTrail;
        }
    } else if (dy == 0) {
        if (dx > 0) {
            return RightTrail;
        }
        if (dx < 0) {
            return LeftTrail;
        }
    }
    // (dx, dy) should never be (0, 0) or a different direction
    // than sideways (up, down, left, right)
    return Floor;
}

using EntityMap = std::vector<std::vector<unsigned>>;

enum StepFeedback {
    Forward,
    Rotation,
    Loop,
    Wall,
};

auto Step(EntityMap& map, int& x, int& y, int& dx, int& dy) -> StepFeedback {
    MapEntity trail = TrailFromDirection(dx, dy);
    if (map[y][x] & trail) {
        // Already left this trail means
        // we will continue in a loop
        return Loop;
    }
    map[y][x] |= trail;
    const int x_new = x + dx;
    const int y_new = y + dy;
    if (x_new == -1 || x_new == map[0].size() || y_new == -1 || y_new == map.size()) {
        return Wall;
    }
    if (map[y_new][x_new] == Obstacle) {
        int tmpdx = dx;
        dx = -dy;
        dy = tmpdx;
        return Rotation;
    }
    x = x_new;
    y = y_new;
    return Forward;
}

auto PrintMapWithTrails(const EntityMap& map) -> void;

auto CheckNewObstacle(EntityMap new_map, int x, int y, int dx, int dy) -> bool {
    while (true) {
        StepFeedback last_move = Step(new_map, x, y, dx, dy);
        if (last_move == Wall) {
            break;
        }
        if (last_move == Loop) {
            // PrintMapWithTrails(new_map);
            return true;
        }
    }
    return false;
}

int main() {
    // Store the board
    std::vector<std::string> board;
    std::string line;
    while (std::getline(std::cin, line)) {
        board.emplace_back(std::move(line));
    }
    // Ugly assumptions
    static const int height = board.size();
    static const int width = board[0].size();
    // Find the guard
    int start_row = -1;
    int start_col = -1;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (board[i][j] == '^') {
                start_row = i;
                start_col = j;
                break;
            }
        }
    }
    if (start_row == -1) {
        throw std::runtime_error("The guard was not found!");
    }
    // Create an entity map
    std::vector entity_map(height, std::vector<unsigned>(width, Floor));
    // Encode obstacles into the entity map
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (board[i][j] == '#') {
                entity_map[i][j] = Obstacle;
            }
        }
    }
    // Guard starts by moving up => (0, -1) vector
    int x = start_col;
    int y = start_row;
    int dx = 0;
    int dy = -1;

    int answer = 0;
    // Move the guard until it hits a wall
    while (true) {
        StepFeedback last_move = Step(entity_map, x, y, dx, dy);
        if (last_move == Wall) {
            break;
        }
        if (last_move == Loop) {
            ++answer;
            break;
        }
        // After a move forward, if we encounter an unoccupied/unvisited
        // space, put an obstacle and check for loops
        if (last_move == Forward && entity_map[y][x] == Floor) {
            entity_map[y][x] = Obstacle;
            answer += CheckNewObstacle(entity_map, x - dx, y - dy, -dy, dx);
            entity_map[y][x] = Floor;
        }
    }

    PrintMapWithTrails(entity_map);

    std::cout << "answer: " << answer << '\n';
}







auto PrintMapWithTrails(const EntityMap& map) -> void {
    std::cout << "  ";
    for (int j = 0; j < map[0].size(); ++j) {
        std::cout << (j % 10) << ' ';
    }
    std::cout << '\n';
    for (int i = 0; i < map.size(); ++i) {
        std::cout << (i % 10) << ' ';
        for (int j = 0; j < map[0].size(); ++j) {
            unsigned entity = map[i][j];
            if (entity & HasTrail) {
                if (entity & UpTrail) { std::cout << '^'; }
                else if (entity & RightTrail) { std::cout << '>'; }
                else if (entity & DownTrail) { std::cout << 'v'; }
                else if (entity & LeftTrail) { std::cout << '<'; }
            } else if (entity == Obstacle) {
                std::cout << '#';
            } else if (entity == Floor) {
                std::cout << '.';
            } else {
                throw std::runtime_error("unexpected entity map value");
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

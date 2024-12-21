'''
Although the Dijkstra's algorithm is running on a grid,
due to the cost of rotations in place the nodes have to
point to themselves. This makes it so that each grid cell
of position (row, column) will be represented by 4 distinct
nodes (row, column, facing_direction). Each of that nodes
will have 3 edges, two for 90-degree rotations with cost 1000
and one for traversing to the next cell with cost 1.

        +------+
        |  ^^  | <--*
        |(0, 0)|    |
        +------+    | cost=1000
                    |
                    o
+------+        +------+    cost=1   +------+
|  <<  |        |  >>  | o---------> |  >>  |
|(0, 0)|        |(0, 0)|             |(0, 1)|
+------+        +------+             +------+
                    o
                    |
                    | cost=1000
        +------+    |
        |  vv  | <--*
        |(0, 0)|
        +------+

To reconstruct the paths we also need a predecessor map (`prev`)
which stores all the predecessors of the same cost.
When a new best cost is found we overwrite the list with only
one element (i.e., the current predecessor). On the other hand,
when another cost has the same value as the best cost, we append.
'''
import sys
import heapq
import math

from collections import defaultdict


DIRS = UP, RIGHT, DOWN, LEFT = ((-1, 0), (0, 1), (1, 0), (0, -1))

def rotate90(direction, clockwise=True):
    if not clockwise:
        return (direction[1], -direction[0])
    return (-direction[1], direction[0])


def build_neighbors(maze, row, col, facing):  # Returns a list of (cost, row, col, facing)
    return ([(1, row + facing[0], col + facing[1], facing)] if maze[row + facing[0]][col + facing[1]] != '#' else []) + [
        (1000, row, col, rotate90(facing, True)),
        (1000, row, col, rotate90(facing, False)),
    ]


def get_directions(min_cost, row, col):
    return [(min_cost[(row, col, facing)], rep, facing) for facing, rep in zip(DIRS, list('^>v<'))]


# def best_directions(min_cost, row, col):
#     neighbors = get_directions(min_cost, row, col)
#     neighbors.sort(key=lambda cost: cost[0])
#     return [(facing, rep) for cost, rep, facing in neighbors if cost == neighbors[0][0]]


def dijkstra(maze, start, finish, start_facing):
    min_cost = defaultdict(lambda: math.inf)
    pq = build_neighbors(maze, *start, start_facing)
    for cost, *key in pq:
        min_cost[tuple(key)] = cost
    heapq.heapify(pq)
    prev = defaultdict(list)

    while pq:
        state = cost, row, col, facing = heapq.heappop(pq)
        key = state[1:]
        if cost > min_cost[key]:
            # A better route exists already
            continue
        for action_cost, new_row, new_col, new_facing in build_neighbors(maze, row, col, facing):
            new_state = new_cost, *new_key = (cost + action_cost, new_row, new_col, new_facing)
            new_key = tuple(new_key)
            if new_cost < min_cost[new_key]:
                min_cost[new_key] = new_cost
                heapq.heappush(pq, new_state)
                prev[new_key] = [key]
            elif new_cost == min_cost[new_key]:
                # Add to existing
                prev[new_key].append(key)

    def count_visited(start_node):
        visited = set()
        def backtrack(row, col, facing):
            visited.add((row, col))
            for p in prev[(row, col, facing)]:
                backtrack(*p)
        backtrack(*start_node)
        return len(visited)

    final_direction = min(DIRS, key=lambda facing: min_cost[(*finish, facing)])
    return count_visited((*finish, final_direction))


def read_input():
    maze = []
    start = [-1, -1]
    finish = [-1, -1]
    for lineno, line in enumerate(sys.stdin):
        if 'S' in line:
            start[0] = lineno
            start[1] = line.index('S')
        if 'E' in line:
            finish[0] = lineno
            finish[1] = line.index('E')
        maze += [list(line)]
    assert maze[start[0]][start[1]] == 'S'
    assert maze[finish[0]][finish[1]] == 'E'
    return maze, tuple(start), tuple(finish)


def main():
    answer = dijkstra(*read_input(), (0, 1))
    print('Answer:', answer)


if __name__ == '__main__':
    main()

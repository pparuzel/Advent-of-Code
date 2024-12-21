import sys
import heapq
from collections import defaultdict


def directions(board, row, col):
    for dr, dc in ((0, 1), (-1, 0), (1, 0), (0, -1)):
        if board[row + dr][col + dc] == '#':
            continue
        yield row + dr, col + dc


def cheat_reachable(board, row, col):
    ROWS, COLS = len(board), len(board[0])
    for dr, dc in ((0, 1), (-1, 0), (1, 0), (0, -1)):
        if board[row + dr][col + dc] != '#':
            continue
        for step in range(2, 4):
            test_row = row + step * dr
            test_col = col + step * dc
            if (test_row <= -1 or test_row >= ROWS or
                test_col <= -1 or test_col >= COLS or
                board[test_row][test_col] == '#'):
                continue
            # row, col, steps_required
            yield test_row, test_col, step
            break


def dijkstra_all(board, start):
    pq = [(0, *start)]
    heapq.heapify(pq)
    min_cost = defaultdict(lambda: float('inf'))
    min_cost[start] = 0
    while pq:
        cost, row, col = heapq.heappop(pq)
        if cost > min_cost[row, col]:
            continue
        for new_row, new_col in directions(board, row, col):
            new_cost = cost + 1
            if new_cost < min_cost[new_row, new_col]:
                heapq.heappush(pq, (new_cost, new_row, new_col))
                min_cost[new_row, new_col] = new_cost
    return min_cost


def count_cheats(board, end_min_cost):
    d = defaultdict(int)
    for row, col in end_min_cost:
        for new_row, new_col, steps in cheat_reachable(board, row, col):
            d[end_min_cost[row, col] - end_min_cost[new_row, new_col] - steps] += 1
    return d


def main():
    board = [list(line.strip()) for line in sys.stdin]
    start, end = (-1, -1), (-1, -1)
    for row, line in enumerate(board):
        for col, char in enumerate(line):
            if char == 'S':
                start = (row, col)
            elif char == 'E':
                end = (row, col)
    assert board[start[0]][start[1]] == 'S'
    assert board[end[0]][end[1]] == 'E'
    cost_reverse = dijkstra_all(board, end)  # Dijkstra from end to start

    # cost_board = [b[:] for b in board]
    # for (row, col), cost in cost_reverse.items():
    #     cost_board[row][col] = cost
    # for row in cost_board:
    #     print(''.join(f'{c:^3}' for c in row))

    best = cost_reverse[start]
    d = count_cheats(board, cost_reverse)
    better_routes = sorted(filter(lambda k: best - k < best, d))
    for k in better_routes:
        print(f'There is {d[k]} routes that save {k} picoseconds')
    answer = sum(d[k] for k in better_routes if k >= 100)
    print(f'Answer: {answer}')



if __name__ == '__main__':
    main()

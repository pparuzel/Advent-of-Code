from collections import defaultdict
import sys
import heapq

N = 71
START = 0, 0
END = N - 1, N - 1
BYTES_FALLEN = 1024


def adjacent(grid, row, col):
    for dr, dc in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
        if (row + dr < 0 or row + dr >= N or
            col + dc < 0 or col + dc >= N or
            grid[row + dr][col + dc] == '#'):
            continue
        yield row + dr, col + dc


def dijkstra(grid):
    pq = [(0, *START)]
    heapq.heapify(pq)
    min_dist = defaultdict(lambda: float('inf'))

    while pq:
        dist, row, col = heapq.heappop(pq)
        if (row, col) == END:
            return min_dist[(row, col)]
        if dist > min_dist[(row, col)]:
            continue
        for new_row, new_col in adjacent(grid, row, col):
            new_dist = dist + 1
            if new_dist < min_dist[(new_row, new_col)]:
                heapq.heappush(pq, (new_dist, new_row, new_col))
                min_dist[(new_row, new_col)] = new_dist
    return None


def main():
    coords = []
    for line in sys.stdin:
        coords.append(tuple(map(int, line.split(','))))
    grid = [['.' for _ in range(N)] for _ in range(N)]

    for i, coord in enumerate(coords):
        print('Testing', i, 'of', len(coords))
        grid[coord[1]][coord[0]] = '#'
        if not dijkstra(grid):
            print('Answer:', coord)
            break
    else:
        print('No solution')


if __name__ == '__main__':
    main()

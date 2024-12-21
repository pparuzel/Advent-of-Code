import sys
from collections import deque, defaultdict

BOARD = [list(line.strip()) for line in sys.stdin]
CHEAT_LEN = 20
MINIMUM_SAVED = 100
ROWS, COLS = len(BOARD), len(BOARD[0])
SAVED_GROUPED = defaultdict(int)  # for debugging
INF = float('inf')

## Utilities

def print_board(board):
    print('  ', ''.join(f'{i:^3}' for i in range(len(board[0]))))
    for i, row in enumerate(board):
        print(f'{i:>2}', ''.join(f'{c:^3}' for c in row))


## Solution

for r in range(ROWS):
    for c in range(COLS):
        if BOARD[r][c] == 'S':
            START = (r, c)
        elif BOARD[r][c] == 'E':
            END = (r, c)
assert START and END


def manh(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


def in_bounds(r, c):
    return 0 <= r < ROWS and 0 <= c < COLS


def shortcuts(cell, cache={}):
    if cell in cache:
        return cache[cell]
    cache[cell] = {}
    for offr in range(-CHEAT_LEN, CHEAT_LEN + 1):
        for offc in range(-CHEAT_LEN, CHEAT_LEN + 1):
            '''
            Manhattan Distance of 2 from the middle
            #######
            ###o###
            ##ooo##
            #ooooo#
            ##ooo##
            ###o###
            #######
            '''
            if abs(offr) + abs(offc) > CHEAT_LEN:
                continue
            # Destination coordinates
            r, c = cell[0] + offr, cell[1] + offc
            if not in_bounds(r, c) or BOARD[r][c] == '#':
                continue
            cache[cell][r, c] = manh(cell, (r, c))
    return cache[cell]
    # return {(0, 0):
    #     {
    #         (1, 2): manh((0, 0), (1, 2)),
    #         (2, 2): manh((0, 0), (2, 2)),
    #     },
    #     ...
    # }


def bfs_dist(start):
    dist = defaultdict(lambda: INF)
    q = deque([(*start, 0)])
    while q:
        r, c, steps = q.popleft()
        if steps >= dist[r, c]:
            continue
        dist[r, c] = steps
        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nr, nc = r + dr, c + dc
            if BOARD[nr][nc] != '#':
                # in_bounds(nr, nc) check not needed thanks to wall borders
                q.append((nr, nc, steps + 1))
    return dist


def count_good_shortcuts(start, remaining):
    dist = defaultdict(lambda: INF)
    q = deque([(*start, 0)])
    vis = set()
    answer = 0
    while q:
        r, c, steps = q.popleft()
        if (r, c) in vis:
            continue
        vis.add((r, c))
        for dest, steps in shortcuts((r, c)).items():
            cheat_steps = steps + remaining[dest]
            saved = remaining[r, c] - cheat_steps
            if saved >= MINIMUM_SAVED:
                SAVED_GROUPED[saved] += 1
                answer += 1
        dist[r, c] = steps
        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nr, nc = r + dr, c + dc
            if BOARD[nr][nc] != '#':
                q.append((nr, nc, steps + 1))
    return answer


DIST_END = bfs_dist(start=END)
ANSWER = count_good_shortcuts(start=START, remaining=DIST_END)


print_board(BOARD)
for pico_saved, count in sorted(SAVED_GROUPED.items()):
    print(f'There are {count} cheats that save {pico_saved} picoseconds.')

print('Answer:', ANSWER)

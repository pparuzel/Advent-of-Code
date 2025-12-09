import sys


def area(p1: tuple[int, int], p2: tuple[int, int]) -> int:
    return (abs(p2[0] - p1[0]) + 1) * (abs(p2[1] - p1[1]) + 1)


def proximity(point: tuple[int, int], to: tuple[int, int]) -> int:
    return abs(to[0] - point[0]) + abs(to[1] - point[1])


# (x, y) => (col, row)
points = [tuple(map(int, line.rstrip().split(","))) for line in sys.stdin]
min_row, max_row, min_col, max_col = float("inf"), 0, float("inf"), 0
for p in points:  # find boundaries
    min_row = min(min_row, p[1])
    max_row = max(max_row, p[1])
    min_col = min(min_col, p[0])
    max_col = max(max_col, p[0])

TOP_LEFT = (min_col, min_row)
TOP_RIGHT = (max_col, min_row)
BOT_LEFT = (min_col, max_row)
BOT_RIGHT = (max_col, max_row)
FARTHEST = proximity(TOP_LEFT, BOT_RIGHT)

best_tl, best_tr, best_bl, best_br = [(FARTHEST, None) for _ in range(4)]
for p in points:
    best_tl = min(best_tl, (proximity(p, to=TOP_LEFT), p), key=lambda k: k[0])
    best_tr = min(best_tr, (proximity(p, to=TOP_RIGHT), p), key=lambda k: k[0])
    best_bl = min(best_bl, (proximity(p, to=BOT_LEFT), p), key=lambda k: k[0])
    best_br = min(best_br, (proximity(p, to=BOT_RIGHT), p), key=lambda k: k[0])

rect_tlbr = best_tl[1], best_br[1]
rect_trbl = best_tr[1], best_bl[1]
ans = max(area(*rect_tlbr), area(*rect_trbl))

print(ans)

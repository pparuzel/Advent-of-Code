import sys
from functools import cache

diagram = list(line.rstrip() for line in sys.stdin)
start = diagram[0].index("S")
diagram = diagram[2::2]  # every other line contains splitters
rows = len(diagram)


@cache
def beam_dfs(row, col):
    if row >= rows:
        return 1
    if diagram[row][col] == "^":
        return beam_dfs(row + 1, col - 1) + beam_dfs(row + 1, col + 1)
    return beam_dfs(row + 1, col)


ans = beam_dfs(0, start)
print(ans)

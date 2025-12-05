import contextlib
import sys
from bisect import bisect_left

ranges = []

for line in sys.stdin:
    if line[0] == "\n":
        break
    lower, upper = line.split("-")
    ranges.append((int(lower), int(upper)))

ranges.sort()  # For fast logarithmic access using bisect

ans = 0
for line in sys.stdin:
    food = int(line)
    cutoff = bisect_left(ranges, food, key=lambda k: k[0])
    with contextlib.suppress(StopIteration):
        ans += next(1 for (_lo, hi) in reversed(ranges[:cutoff]) if food <= hi)

print(ans)

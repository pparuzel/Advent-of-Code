import sys

ranges = []

for line in sys.stdin:
    if line[0] == "\n":
        break
    lower, upper = line.split("-")
    ranges.append((int(lower), int(upper)))

ranges.sort()

ans = 0
start, end = ranges[0]
for lo, hi in ranges:
    if end >= lo:
        end = max(end, hi)  # expand if possible
        continue
    ans += end - start + 1  # end of the range, recalculate
    start, end = lo, hi
ans += end - start + 1

print(ans)

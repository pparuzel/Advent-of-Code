import sys

start = 50
ans = 0

for line in sys.stdin:
    rot = (2 * line.startswith("R") - 1) * int(line[1:])
    if (start := (start + rot) % 100) == 0:
        ans += 1

print(ans)

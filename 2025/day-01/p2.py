import sys

start = 50
ans = 0

for line in sys.stdin:
    rot = (2 * line.startswith("R") - 1) * int(line[1:])
    ans += (abs(rot) // 100)  # count in rotations by 100
    new_start = start + (rot % 100 if rot > 0 else (rot % 100) - 100)
    if new_start == 0 or start != 0 and (new_start < 0 or new_start > 99):
        ans += 1
    start = new_start % 100

print(ans)

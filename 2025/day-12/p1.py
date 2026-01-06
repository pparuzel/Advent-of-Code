import sys

ans = 0
for line in sys.stdin:
    if "x" not in line:
        continue
    lhs, rhs = line.split(":")
    width, height = list(map(int, lhs.split("x")))
    counts = list(map(int, rhs.split()))
    ans += sum(counts) * 9 <= width * height

print(ans)


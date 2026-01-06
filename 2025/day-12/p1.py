import sys

can_fit = [0, 0]  # no, yes
for line in sys.stdin:
    if "x" not in line:
        continue
    lhs, rhs = line.split(":")
    width, height = list(map(int, lhs.split("x")))
    counts = list(map(int, rhs.split()))
    can_fit[sum(counts) * 9 <= width * height] += 1

no_fit, ans = can_fit
print(ans)


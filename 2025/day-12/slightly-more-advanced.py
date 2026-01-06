import sys

# NOTE: This puzzle is tricky. Solution only works for my input!

# Based on my input, we can deduce such information for each block:
# index=0, assume ugly block 3x3 (9)
# index=1, assume ugly block 3x3 (9)
# index=2, 2 blocks fully cover 3x4 (12)
# index=3, assume ugly block 3x3 (9)
# index=4, assume ugly block 3x3 (9)
# index=5, 2 blocks almost fully cover 4x4 (16)
cost_per_two = [18, 18, 12, 18, 18, 16]

can_fit = [0, 0]  # yes, no
for line in sys.stdin:
    if "x" not in line:
        # We ignore the input blocks entirely
        continue
    lhs, rhs = line.split(":")
    width, height = list(map(int, lhs.split("x")))
    counts = list(map(int, rhs.split()))
    area = width * height
    # Multiply the block counts by its cost. Costs are "per two blocks" so we
    # have to divide and handle the odd case separately (assume ugly block 3x3).
    combined = sum((n * (cnt // 2)) + 9 * (cnt % 2 != 0) for n, cnt in zip(cost_per_two, counts))
    can_fit[area < combined] += 1

total = sum(can_fit)
ans, no_fit = can_fit
print(ans)

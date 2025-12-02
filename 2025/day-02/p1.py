import sys

data = sys.stdin.read().rstrip()
assert "\n" not in data, "Invalid input"


def count_invalid_in_range(low: str, high: str) -> int:
    count = 0
    for num in range(int(low), int(high) + 1):
        s = str(num)
        halfpoint, remainder = divmod(len(s), 2)
        if remainder != 0:
            continue  # could skip faster, optimize using a while loop
        if s[halfpoint:] == s[:halfpoint]:
            count += num
    return count


ans = 0
for low_high in data.split(","):
    lo, hi = low_high.split("-")
    ans += count_invalid_in_range(lo, hi)

print(ans)

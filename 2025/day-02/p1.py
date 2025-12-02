import sys

data = sys.stdin.read().rstrip()
assert "\n" not in data, "Invalid input"


def count_invalid_in_range(low: str, high: str):
    count = 0
    for num in range(int(low), int(high) + 1):
        s = str(num)
        s_len_div2, s_len_mod2 = divmod(len(s), 2)
        if s_len_mod2 != 0:
            continue  # could skip faster, optimize using a while loop
        if s[s_len_div2:] == s[:s_len_div2]:
            count += num
    return count


ans = 0
for low_high in data.split(","):
    lo, hi = low_high.split("-")
    ans += count_invalid_in_range(lo, hi)

print(ans)

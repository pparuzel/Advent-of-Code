import sys

data = sys.stdin.read().rstrip()
assert "\n" not in data, "Invalid input"

# For an n-letter number, how long the sequence has to repeat in order to
# construct and invalid ID.
#
# Example: An 8-digit word requires only counting repeating sequences
# of 4 (abcd). For range 0 - 99'999'999:
# Sequences of 4 digits: [0-9][0-9][0-9][0-9] which is:
# 10 * 10 * 10 * 10 = 10^4 = 10'000 invalid cases.
# Obviously, this only happens for ranges starting from 0 to 999... etc.,
# so we will need to account for this.
ndigit_seqs = {
    1: [],
    2: [1],
    3: [1],
    4: [2],
    5: [1],
    6: [2, 3],
    7: [1],
    8: [4],
    9: [3],
    10: [2, 5],
}


def count_invalid_in_range(low: str, high: str):
    invalid = set()
    lo, hi = int(low), int(high)
    for curr_len in range(len(low), len(high) + 1):
        for seq_len in ndigit_seqs[curr_len]:
            # Construct the following pattern: [1-9][0-9]{curr_len - 1}
            # Generate all possible sequences of size `seq_len`
            for part in range(10 ** (seq_len - 1), 10 ** (seq_len)):
                if lo <= (num := int(str(part) * (curr_len // seq_len))) <= hi:
                    invalid.add(num)
    return sum(invalid)


ans = 0
for low_high in data.split(","):
    lo, hi = low_high.split("-")
    ans += count_invalid_in_range(lo, hi)

print(ans)

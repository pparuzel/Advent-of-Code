import sys

data = sys.stdin.read().rstrip()
assert "\n" not in data, "Invalid input"

# Assuming max 10 digits, for an n-letter number, we can hardcode the length
# of the sequence which needs to be repeated to construct an invalid ID.
#
# Optimization: We do not need to check all divisors! Consider only those
# divisors that do not share a common divisor with each other. The only problem
# is the divisor of 1 which is always shared. Therefore, we will need to remove
# some duplicates with sequence length of size 1 for 6-digit and 10-digit.
#
# Example: An 8-digit word requires only counting repeating sequences
# of 4 (abcd). Why only 4 and not divisors 2 and 1 as well? That's because
# sequences abababab (len=2) and aaaaaaaa (len=1) are already covered by the
# larger divisor of 4: abcdabcd.
#
# For range 0 - 99'999'999:
# Sequences of 4 digits: [0-9][0-9][0-9][0-9] which is:
# 10 * 10 * 10 * 10 = 10^4 = 10'000 invalid cases.
# Obviously, this only happens for ranges starting from 0 to 999... etc.,
# so we will need to account for this.
ndigit_seqs = {
    1: [],
    2: [1],      # aa
    3: [1],      # aaa
    4: [2],      # abab
    5: [1],      # aaaaa
    6: [2, 3],   # ababab, abcabc
    7: [1],      # aaaaaaa
    8: [4],      # abcdabcd
    9: [3],      # abcabcabc
    10: [2, 5],  # ababababab, abcdeabcde
}


def count_invalid_in_range(low: str, high: str) -> int:
    invalid = set()
    lo, hi = int(low), int(high)
    for curr_len in range(len(low), len(high) + 1):
        for seq_len in ndigit_seqs[curr_len]:
            # Construct the following pattern: [1-9][0-9]{curr_len - 1}
            # Generate all possible sequences of size `seq_len` and concatenate
            # them to fit the digit requirement.
            for part in range(10 ** (seq_len - 1), 10 ** seq_len):
                if lo <= (num := int(str(part) * (curr_len // seq_len))) <= hi:
                    invalid.add(num)
    return sum(invalid)


ans = 0
for low_high in data.split(","):
    lo, hi = low_high.split("-")
    ans += count_invalid_in_range(lo, hi)

print(ans)

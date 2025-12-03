import sys

ans = 0
for bank in sys.stdin:
    # Search for the first max digit char in range [0, n-1)
    left_digit = max(bank[:-2])  # also skips \n char
    idx = bank.index(left_digit)  # second digit in [idx+1, n)
    right_digit = max(bank[idx + 1:-1])
    ans += int(left_digit + right_digit)

print(ans)

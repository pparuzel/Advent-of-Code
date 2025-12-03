import sys

ans = 0
for bank in sys.stdin:
    digits = ''
    start = 0
    for remaining in range(12, 0, -1):
        digit = max(bank[start:-remaining])
        start = bank.index(digit, start) + 1
        digits += digit
    ans += int(digits)

print(ans)

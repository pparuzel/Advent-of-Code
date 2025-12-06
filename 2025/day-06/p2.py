import math
import sys

reduction = {
    "+": sum,
    "*": math.prod,
}
# Replace the newline with a space to also trigger the final reduction
# instead of handling it manually after.
*lines, ops = list(line[:-1] + " " for line in sys.stdin)
oper = iter(ops.split())

ans = 0
values = []
for digits in zip(*lines):  # zip trick to get the digits
    num = "".join(digits).strip()
    if not num:
        ans += reduction[next(oper)](values)
        values = []
        continue
    values.append(int(num))

print(ans)

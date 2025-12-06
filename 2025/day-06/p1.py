import math
import sys

values = []
ops = []
for line in sys.stdin:
    ops = line.split()
    if not ops[0].isdigit():
        break
    values += [int(val) for val in ops]

stride = len(ops)
reduction = {
    "+": sum,
    "*": math.prod,
}

ans = 0
for col, op in enumerate(ops):
    ans += reduction[op](values[col::stride])

print(ans)

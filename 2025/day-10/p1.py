import sys
from itertools import product
from math import inf

ans = 0

for line in sys.stdin:
    parts = line.split()
    lights, button_specs = parts[0], parts[1:-1]

    buttons = [
        tuple(map(int, b.strip("()").split(",")))
        for b in button_specs
    ]

    expected = [c == "#" for c in lights[1:-1]]
    n = len(expected)

    best = inf
    for combo in product((0, 1), repeat=len(buttons)):
        state = [
            sum(combo[i] for i, btn in enumerate(buttons) if idx in btn) % 2
            for idx in range(n)
        ]
        if state == expected:
            best = min(best, sum(combo))

    ans += best

print(ans)

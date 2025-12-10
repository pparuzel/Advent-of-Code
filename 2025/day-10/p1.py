import sys
from itertools import product

ans = 0
for lights, *buttons in [line.split()[:-1] for line in sys.stdin]:
    buttons = [tuple(map(int, b.strip("()").split(","))) for b in buttons]
    expected_lights = [int(lit == "#") for lit in lights[1:-1]]
    width = len(expected_lights)
    min_pressed = float("inf")
    for enablement in product(range(2), repeat=len(buttons)):
        state = [0] * len(expected_lights)
        for button_id, on in enumerate(enablement):
            for idx in buttons[button_id]:
                state[idx] += on
        if [k % 2 for k in state] == expected_lights:
            min_pressed = min(min_pressed, enablement.count(1))
    ans += min_pressed

print(ans)

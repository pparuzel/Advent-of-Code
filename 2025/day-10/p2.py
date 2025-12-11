import sys

import numpy as np
from scipy.optimize import milp  # Mixed-Integer Linear Programming Solver


ans = 0
diagrams = [line.split()[1:] for line in sys.stdin]
for *buttons, joltages in diagrams:
    buttons = [list(map(int, b.strip("()").split(","))) for b in buttons]
    joltages = list(map(int, joltages.strip("}{").split(",")))

    # Define these as a linear problem and solve
    n_joltages, n_buttons = len(joltages), len(buttons)
    eq_lhs = [[int(j in buttons[i]) for i in range(n_buttons)] for j in range(n_joltages)]
    eq_rhs = np.array(joltages)
    constraints = (eq_lhs, eq_rhs, eq_rhs)  # equality
    bounds = (0, np.inf)  # non-negative integers
    integrality = np.ones(n_buttons)  # integers only
    result = milp(
        c=integrality,  # coincidental shortcut, they happen to be the same matrix
        constraints=constraints,
        bounds=bounds,
        integrality=integrality,
    )
    ans += int(result.fun)

print(ans)

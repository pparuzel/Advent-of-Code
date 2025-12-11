import sys

import numpy as np
from scipy.optimize import milp  # Mixed-Integer Linear Programming Solver


ans = 0
for line in sys.stdin:
    buttons = [list(map(int, b.strip("()").split(","))) for b in line[:-1]]
    joltages = list(map(int, line[-1].strip("}{").split(",")))
    n_joltages, n_buttons = len(joltages), len(buttons)

    # Define these as a linear problem and solve
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

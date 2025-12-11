import numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds

"""
Example: (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}

x[n] is # of button presses of button[n]
a,b,c,d are joltage levels respectively

We must minimize S = x1+x2+x3+x4+x5+x6

This is the equation:
x1*d+x2*(b+d)+x3*(c)+x4*(c+d)+x5*(a+c)+x6*(a+b) == 3a+5b+4c+7d

Let's reorder in terms of a,b,c,d

(x5+x6)a+(x2+x6)b+(x3+x4+x5)c+(x1+x2+x4)d = 3a+5b+4c+7d

We get:

x5+x6=3     => [0,0,0,0,1,1]
x2+x6=5     => [0,1,0,0,0,1]
x3+x4+x5=4  => [0,0,1,1,1,0]
x1+x2+x4=7  => [1,1,0,1,0,0]

These are the linear constraints that make a constraint matrix.
"""

# Minimize S = x1 + x2 + x3 + x4 + x5 + x6
problem = np.array([1, 1, 1, 1, 1, 1])

# Constraints
lhs = np.array([
    [0, 0, 0, 0, 1, 1],  # x5 + x6 = 3
    [0, 1, 0, 0, 0, 1],  # x2 + x6 = 5
    [0, 0, 1, 1, 1, 0],  # x3 + x4 + x5 = 4
    [1, 1, 0, 1, 0, 0],  # x1 + x2 + x4 = 7
])
rhs = np.array([3, 5, 4, 7])

# Since we have equality constraints (lhs * x = rhs), the lower and upper bounds are both rhs
constraints = LinearConstraint(lhs, rhs, rhs)

# Solution bounds for all 6 variables
bounds = Bounds(0, np.inf)

# Integrality requirement (All 6 variables must be integers)
integrality = np.ones(6)

# Finally, let the solver do the work...
result = milp(
    c=problem,
    constraints=constraints,
    bounds=bounds,
    integrality=integrality,
)

print(int(result.fun))

import math
import sys
from heapq import nlargest


def dist_sq(a: tuple[int, int, int], b: tuple[int, int, int]) -> int:
    ax, ay, az = a
    bx, by, bz = b
    return (bx - ax) ** 2 + (by - ay) ** 2 + (bz - az) ** 2


iters = 1000
# Junction box coordinates
eboxes = [tuple(int(x) for x in line.rstrip().split(",")) for line in sys.stdin]
total = len(eboxes)
groups = [{i} for i in range(total)]  # affiliation groups per ebox
dists = [  # all possible connections
    (dist_sq(eboxes[i], eboxes[j]), i, j)
    for i in range(total)
    for j in range(i + 1, total)
]
dists.sort(key=lambda k: k[0])

for it in range(iters):
    _dist, box1, box2 = dists[it]
    if groups[box1] is groups[box2]:
        continue
    groups[box1] |= groups[box2]
    for each in groups[box2]:
        groups[each] = groups[box1]


circuits = {frozenset(group) for group in groups}  # get rid of duplicate groups
ans = math.prod(nlargest(3, (len(circuit) for circuit in circuits)))
print(ans)

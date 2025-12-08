import sys


def dist_sq(a: tuple[int, int, int], b: tuple[int, int, int]) -> int:
    ax, ay, az = a
    bx, by, bz = b
    return (bx - ax) ** 2 + (by - ay) ** 2 + (bz - az) ** 2


eboxes = [tuple(int(x) for x in line.rstrip().split(",")) for line in sys.stdin]
total = len(eboxes)
groups = [{i} for i in range(total)]  # affiliation groups per node
dists = [  # all possible connections
    (dist_sq(eboxes[i], eboxes[j]), i, j)
    for i in range(total)
    for j in range(i + 1, total)
]
dists.sort(key=lambda k: k[0])

# Once any group size reaches # of total eboxes - we found the solution
for _dist, box1, box2 in dists:
    if groups[box1] is groups[box2]:
        continue
    groups[box1] |= groups[box2]
    if len(groups[box1]) == total:
        break  # full circuit
    for each in groups[box2]:
        groups[each] = groups[box1]

ans = eboxes[box1][0] * eboxes[box2][0]
print(ans)

import sys
from itertools import combinations

from shapely.geometry import Polygon

points = [tuple(map(int, line.rstrip().split(","))) for line in sys.stdin]
shape = Polygon(points)


def area(x1: int, y1: int, x2: int, y2: int) -> int:
    return (abs(x2 - x1) + 1) * (abs(y2 - y1) + 1)


def contained_area(x1: int, y1: int, x2: int, y2: int) -> int:
    rect = Polygon([(x1, y1), (x1, y2), (x2, y2), (x2, y1), (x1, y1)])
    if not shape.contains(rect):
        return 0  # reject candidate
    return area(x1, y1, x2, y2)


ans = max(contained_area(*p1, *p2) for p1, p2 in combinations(points, 2))
print(ans)

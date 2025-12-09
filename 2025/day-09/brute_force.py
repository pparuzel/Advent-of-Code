import sys
import time


def area(p1, p2):
    return (abs(p2[0] - p1[0]) + 1) * (abs(p2[1] - p1[1]) + 1)


start = time.time()
points = [tuple(map(int, line.rstrip().split(","))) for line in sys.stdin]
max_area = 0
max_pair = (None, None)
for i, p1 in enumerate(points):
    for p2 in points[i + 1 :]:
        a = area(p1, p2)
        if a > max_area:
            max_area = a
            max_pair = (p1, p2)

print(f"{max_area=} with p1={max_pair[0]}, p2={max_pair[1]}")
print(f"Finished brute force after {time.time() - start:.6f} seconds")

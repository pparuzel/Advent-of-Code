import sys

diagram = list(line.rstrip() for line in sys.stdin)
beams = set([diagram[0].index("S")])  # longitudes where the beams travel

ans = 0
for split_line in diagram[2::2]:  # every other line contains splitters
    for longitude, space in enumerate(split_line):
        if space == "^" and longitude in beams:
            ans += 1
            beams.remove(longitude)
            beams.add(longitude - 1)
            beams.add(longitude + 1)

print(ans)

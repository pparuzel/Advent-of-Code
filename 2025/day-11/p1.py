import sys

conns = {}
for line in sys.stdin:
    node, outs = line.split(":")
    conns[node] = outs.strip().split()


def count_paths(node: str, target: str) -> int:
    if node == target:
        return 1
    return sum(count_paths(p, target) for p in conns[node])


ans = count_paths("you", "out")
print(ans)

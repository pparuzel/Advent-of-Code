import sys
from functools import cache

conns = {"out": []}
for line in sys.stdin:
    node, outs = line.split(":")
    conns[node] = outs.strip().split()


def count_paths_via(node: str, via: tuple[str], target: str) -> int:
    @cache
    def _count_dfs(node, target, visited):
        for to_visit in via:
            if node == to_visit and to_visit not in visited:
                visited += (to_visit,)
        if node == target:
            return set(visited) == set(via)
        return sum(_count_dfs(p, target, visited) for p in conns[node])

    return _count_dfs(node, target, tuple())


ans = count_paths_via("svr", ("fft", "dac"), "out")
print(ans)

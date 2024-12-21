import sys
from collections import defaultdict


def main(lines):
    # That's a maximum clique problem that we need to solve here
    # Basically let's cycle through all possible clique sizes starting from the
    # largest and eventually we should encounter the solution
    all_edges = defaultdict(set)
    for line in lines:
        a, b = line.rstrip().split('-')
        all_edges[a].add(b)
        all_edges[b].add(a)
    most_edges = max(map(len, all_edges.values()))
    print(f'At most there is {most_edges} edges which means that the largest',
          f'fully-connected cycle can be of size {most_edges + 1}')

    def find_clique(graph, size):
        # if size == 1:
        #     return {next(iter(graph))}
        # or
        # if size == 1:
        #     for k in graph:
        #         return {k}
        # but checking for 0 is simpler
        if size == 0:
            return set()
        for node in graph:
            neighbors = graph[node]
            if len(neighbors) < size - 1:
                continue
            subgraph = {neigh: graph[neigh] & neighbors for neigh in neighbors}
            clique = find_clique(subgraph, size - 1)
            if clique is not None:
                return clique | {node}
        return None

    max_clique_size = most_edges + 1
    for clique_size in range(max_clique_size, -1, -1):
        clique = find_clique(all_edges, clique_size)
        if clique:
            clique = sorted(clique)
            print(f'Maximum clique of size {clique_size}: {clique}')
            print('The password is')
            print('\t', ','.join(clique))
            break
        print(f'No clique of size {clique_size} found')


if __name__ == '__main__':
    main(sys.stdin.readlines())

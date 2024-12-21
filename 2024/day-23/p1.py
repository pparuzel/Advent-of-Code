import sys
from collections import defaultdict


def count_interconn3(conns, letter):
    count = 0
    visited = set()
    def visit2(first, second):
        nonlocal count
        for third in conns[second]:
            if first in conns[third]:
                k = tuple(sorted([first, second, third]))
                if k not in visited:
                    visited.add(k)
                    count += 1
    def visit(first):
        for second in conns[first]:
            visit2(first, second)
    for node in conns:
        if node.startswith(letter):
            visit(node)
    return count


def main(lines):
    conns = defaultdict(list)
    for line in lines:
        a, b = line.rstrip().split('-')
        conns[a].append(b)
        conns[b].append(a)
    answer = count_interconn3(conns, 't')
    print(f'Answer: {answer}')


if __name__ == '__main__':
    main(sys.stdin.readlines())

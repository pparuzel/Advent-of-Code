import sys
from functools import cache


def check(sequence, towels) -> bool:
    @cache
    def check_aux(start, end):
        nonlocal sequence
        if start == end:
            return True
        seq = sequence[start:end]
        for towel in towels:
            if towel not in seq:
                continue
            i = sequence.index(towel, start, end)
            if check_aux(start, i) and check_aux(i + len(towel), end):
                return True
        return not seq
    return check_aux(0, len(sequence))


def count_combinations(sequence, towels) -> int:
    end = len(sequence)
    @cache
    def count_aux(i):
        if i == end:
            return True
        total_count = 0
        for towel in towels:
            if sequence[i:].startswith(towel):
                total_count += count_aux(i + len(towel))
        return total_count
    return count_aux(0)


def main() -> None:
    # Read input data
    towels = []
    designs = []
    answer = 0
    for line in sys.stdin:
        if not line.strip():
            continue
        if ',' in line:
            assert not towels
            towels.extend(line.rstrip().split(', '))
        else:
            designs.append(line.rstrip())
    # Fast checking for occurrence
    towels = set(towels)
    # Solve for all designs
    for i, design in enumerate(designs, 1):
        print(i, 'of', len(designs))
        answer += count_combinations(design, towels)
    print(f'Answer: {answer}')


if __name__ == '__main__':
    main()

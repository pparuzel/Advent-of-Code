import sys
from functools import cache

def check(sequence, towels) -> bool:
    @cache
    def check_aux(start, end):
        nonlocal sequence
        seq = sequence[start:end]
        if not seq:
            return True
        for towel in towels:
            if towel not in seq:
                continue
            i = sequence.index(towel, start, end)
            if check_aux(start, i) and check_aux(i + len(towel), end):
                return True
        return not seq
    return check_aux(0, len(sequence))


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
    # Towels must be sorted by length in descending order
    # to ensure that we greedily remove the biggest chunks
    # Otherwise, we could remove parts that overlap with
    # bigger towel patterns.
    towels.sort(key=len, reverse=True)
    # Solve for all designs
    for design in designs:
        answer += check(design, towels)
    print(f'Answer: {answer}')


if __name__ == '__main__':
    main()

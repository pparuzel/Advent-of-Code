import sys
from collections import deque


MASK = (1 << 24) - 1


def generate_secret(seed):
    seed = ((seed << 6) ^ seed) & MASK
    seed = ((seed >> 5) ^ seed) & MASK
    seed = ((seed << 11) ^ seed) & MASK
    return seed


def handle_seed(seed):
    # Initialize last four changes
    last_four = deque([])
    for _ in range(4):
        new_seed = generate_secret(seed)
        change = (new_seed % 10) - (seed % 10)
        last_four.append(change)
        seed = new_seed
    banana_count = new_seed % 10
    changes2bananas = {tuple(last_four): banana_count}
    # Cycle through all the rest of the changes
    for _ in range(2000 - 4):
        new_seed = generate_secret(seed)
        banana_count = new_seed % 10
        change = (banana_count) - (seed % 10)
        last_four[0] = change
        last_four.rotate(-1)
        if tuple(last_four) not in changes2bananas:
            changes2bananas[tuple(last_four)] = banana_count
        seed = new_seed
    return changes2bananas


def main(data):
    sums = {}
    for seed in data:
        for (change, bananas) in handle_seed(seed).items():
            sums[change] = sums.get(change, 0) + bananas
    change_sequence, max_bananas = max(sums.items(), key=lambda k: k[1])
    print(f'Change sequence: {change_sequence} | Max Bananas: {max_bananas}')
    # for change, acc in sorted(sums.items(), key=lambda k: k[1], reverse=True):
    #     print(f'Changes = {change}, bananas = {acc}')
    #     break


if __name__ == '__main__':
    main(list(map(int, sys.stdin.readlines())))

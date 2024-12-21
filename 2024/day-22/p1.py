MASK = (1 << 24) - 1

def generate_secret(seed):
    seed = ((seed << 6) ^ seed) & MASK
    seed = ((seed >> 5) ^ seed) & MASK
    seed = ((seed << 11) ^ seed) & MASK
    return seed


def main(data):
    for _ in range(2000):
        data[:] = map(generate_secret, data)
    answer = sum(data)
    print(f'Answer: {answer}')


def test():
    expected_values = [
        15887950,
        16495136,
        527345,
        704524,
        1553684,
        12683156,
        11100544,
        12249484,
        7753432,
        5908254,
    ]
    seed = 123
    for expected in expected_values:
        seed = generate_secret(seed)
        assert expected == seed, f'got {seed}, expected {expected}'


if __name__ == '__main__':
    test()
    import sys
    main(list(map(int, sys.stdin.readlines())))

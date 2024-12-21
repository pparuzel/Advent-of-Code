import sys


WIDTH = 5
HEIGHT = 7

def main():
    schemas = [sch.splitlines() for sch in sys.stdin.read().split('\n\n')]
    keys = []
    locks = []
    for schema in schemas:
        histogram = []
        for col in range(WIDTH):
            count = sum(schema[row][col] == '#' for row in range(HEIGHT)) - 1
            histogram.append(count)
        if schema[0][0] == '#':
            # It's a lock
            locks.append(histogram)
        else:
            keys.append(histogram)

    ans = 0
    for key in keys:
        for lock in locks:
            ans += all(k + l < 6 for k, l in zip(key, lock))
    print(f'Answer: {ans}')


if __name__ == '__main__':
    main()

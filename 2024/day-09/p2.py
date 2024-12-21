import sys


class Node:
    def __init__(self, value, size):
        self.value = value
        self.size = size
        self.next = None
        self.prev = None

    def __repr__(self):
        return f'Node({self.size} x {repr(self.value)})'


def show_memory(root) -> None:
    while root is not None:
        print(root.value * root.size, end='')
        root = root.next
    print()


def build_memory(sizes) -> tuple[Node, Node]:
    memory_root = Node('0', int(sizes[0]))
    curr = memory_root
    for i in range(1, len(sizes), 2):
        curr.next = Node('.', int(sizes[i]))
        curr.next.prev = curr
        curr = curr.next
        curr.next = Node(f'{i // 2 + 1}', int(sizes[i + 1]))
        curr.next.prev = curr
        curr = curr.next
    # First node, Last node
    return memory_root, curr


def checksum(root) -> int:
    result = 0
    pos = 0
    while root is not None:
        if root.value != '.':
            for i in range(root.size):
                result += (pos + i) * int(root.value)
        pos += root.size
        root = root.next
    return result


def main() -> None:
    memory_root, block = build_memory(sys.stdin.read().rstrip())
    # show_memory(memory_root)

    # Traverse through all blocks in reverse while fitting the free slots
    while block.value != '0':
        free = memory_root.next
        while block.next != free:
            if block.size > free.size:
                free = free.next.next
                continue
            # Free slot before the moved block now has more space
            block.prev.size += block.size
            new_block = Node(block.value, block.size)
            block.size = 0
            # Copy the new block after the fitting free slot
            right_cut = free.next
            free.next = new_block
            new_block.prev = free
            # After the new block, put a free slot node of updated size and
            # also reset size of the left-most free slot
            new_block.next = Node('.', free.size - new_block.size)
            free.size = 0
            new_block.next.next = right_cut
            right_cut.prev = new_block.next
            new_block.next.prev = new_block
            # show_memory(memory_root)
            break
        block = block.prev.prev
    ans = checksum(memory_root)
    print('Answer:', ans, file=sys.stderr)


if __name__ == '__main__':
    main()

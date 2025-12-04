import sys

papers = [f".{line.rstrip()}." for line in sys.stdin]
padding = "." * len(papers[0])
papers = [padding] + papers + [padding]


def count_papers_around(papers: list[str], row: int, col: int) -> int:
    around = [
        (-1, -1), (-1, 0), (-1, 1),  # top
        (0, -1), (0, 1),             # middle
        (1, -1), (1, 0), (1, 1),     # bottom
    ]
    return sum(papers[row + dr][col + dc] == "@" for dr, dc in around)


ans = 0
for row in range(1, len(papers)):
    for col in range(1, len(padding)):
        if papers[row][col] != "@":
            continue  # skip empty
        if count_papers_around(papers, row, col) < 4:
            ans += 1

print(ans)

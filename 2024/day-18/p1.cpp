#include <array>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

constexpr auto N = 71;
constexpr auto BYTES_FALLEN = 1024;
constexpr auto INF = std::numeric_limits<int>::max();
constexpr auto DIRECTIONS = std::array<std::pair<int, int>, 4>({
	{0, 1}, {1, 0}, {0, -1}, {-1, 0},
});

struct Node {
	int cost;
	long pos;

	bool operator>(const Node& other) const {
		return cost > other.cost;
	}
};

using Grid = std::array<char, N * N>;
using PQ = std::priority_queue<Node, std::vector<Node>, std::greater<>>;

auto Dijkstra(const Grid& grid, long start, long end)
	-> std::optional<long> {

	PQ pq;
	pq.push(Node{0, 0});
	std::array<int, N * N> min_cost;
	min_cost.fill(INF);
	min_cost[0] = 0;

	while (!pq.empty()) {
		auto [cost, pos] = pq.top();
		pq.pop();
		if (pos == end) {
			return min_cost[pos];
		}
		if (cost > min_cost[pos]) {
			continue;
		}
		for (auto [dr, dc] : DIRECTIONS) {
			auto rowcol = std::ldiv(pos, N);
			int new_row = rowcol.quot + dr;
			int new_col = rowcol.rem + dc;
			long new_pos = new_row * N + new_col;
			if (new_row < 0 || new_row >= N ||
				new_col < 0 || new_col >= N ||
				grid[new_row * N + new_col] == '#') {

				continue;
			}
			auto new_cost = cost + 1;
			if (new_cost < min_cost[new_pos]) {
				pq.push(Node{new_cost, new_pos});
				min_cost[new_pos] = new_cost;
			}
		}
	}
	return std::nullopt;
}

auto ReadInput() -> std::vector<std::pair<int, int>> {
	int p1, p2;
	std::vector<std::pair<int, int>> data;
	while (!std::cin.eof()) {
		std::cin >> p1;
		std::cin.ignore(1, ',');
		std::cin >> p2;
		data.emplace_back(p2, p1);
	}
	return data;
}


int main() {
	auto input_data = ReadInput();
	Grid grid;
	grid.fill('.');
	for (int i = 0; i < BYTES_FALLEN; ++i) {
		grid[input_data[i].first * N + input_data[i].second] = '#';
	}
	auto maybe_best = Dijkstra(grid, 0, grid.size() - 1);
	if (!maybe_best) {
		std::cerr << "error: Did not find a solution\n";
		return 1;
	}
	// for (int i = 0; i < N; ++i) {
	// 	for (int j = 0; j < N; ++j) {
	// 		std::cout << grid[i * N + j];
	// 	}
	// 	std::cout << '\n';
	// }
	std::cout << "Answer: " << *maybe_best << '\n';
}

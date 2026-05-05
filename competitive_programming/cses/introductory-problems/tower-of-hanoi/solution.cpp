#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

void solve(int disks, int from, int auxiliary, int to, std::vector<std::pair<int, int>>& moves) {
	if (disks == 0) {
		return;
	}

	solve(disks - 1, from, to, auxiliary, moves);
	moves.push_back({from, to});
	solve(disks - 1, auxiliary, from, to, moves);
}

int main() {
	int n;
	std::cin >> n;

	std::vector<std::pair<int, int>> moves;
	moves.reserve(static_cast<std::size_t>((1LL << n) - 1));
	solve(n, 1, 2, 3, moves);

	std::cout << moves.size() << '\n';
	for (const auto& move : moves) {
		std::cout << move.first << ' ' << move.second << '\n';
	}

	return 0;
}

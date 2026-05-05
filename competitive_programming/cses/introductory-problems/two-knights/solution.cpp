#include <cstdint>
#include <iostream>

int main() {
	std::int64_t n;
	std::cin >> n;

	for (std::int64_t k = 1; k <= n; ++k) {
		const std::int64_t squares = k * k;
		const std::int64_t total_pairs = squares * (squares - 1) / 2;
		const std::int64_t attacking_pairs = 4 * (k - 1) * (k - 2);
		std::cout << total_pairs - attacking_pairs << '\n';
	}

	return 0;
}

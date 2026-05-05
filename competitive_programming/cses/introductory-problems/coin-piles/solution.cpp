#include <algorithm>
#include <cstdint>
#include <iostream>

int main() {
	int t;
	std::cin >> t;

	while (t--) {
		std::int64_t a, b;
		std::cin >> a >> b;

		const bool divisible_by_three = (a + b) % 3 == 0;
		const bool balanced_enough = 2 * std::min(a, b) >= std::max(a, b);
		std::cout << (divisible_by_three && balanced_enough ? "YES" : "NO") << '\n';
	}

	return 0;
}

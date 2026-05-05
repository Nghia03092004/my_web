#include <cstdint>
#include <iostream>

int main() {
	std::int64_t n;
	std::cin >> n;

	std::int64_t actual_sum = 0;
	for (std::int64_t i = 0; i < n - 1; ++i) {
		std::int64_t value;
		std::cin >> value;
		actual_sum += value;
	}

	const std::int64_t expected_sum = n * (n + 1) / 2;
	std::cout << expected_sum - actual_sum << '\n';
	return 0;
}

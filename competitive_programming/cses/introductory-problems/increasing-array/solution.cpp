#include <cstdint>
#include <iostream>

int main() {
	int n;
	std::cin >> n;

	std::int64_t answer = 0;
	std::int64_t current_max = 0;

	for (int i = 0; i < n; ++i) {
		std::int64_t value;
		std::cin >> value;

		if (i == 0) {
			current_max = value;
			continue;
		}

		if (value < current_max) {
			answer += current_max - value;
		} else {
			current_max = value;
		}
	}

	std::cout << answer << '\n';
	return 0;
}

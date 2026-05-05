#include <cstdint>
#include <iostream>

int main() {
	int t;
	std::cin >> t;

	while (t--) {
		std::int64_t y, x;
		std::cin >> y >> x;

		std::int64_t answer;
		if (y > x) {
			if (y % 2 == 1) {
				answer = (y - 1) * (y - 1) + x;
			} else {
				answer = y * y - x + 1;
			}
		} else {
			if (x % 2 == 0) {
				answer = (x - 1) * (x - 1) + y;
			} else {
				answer = x * x - y + 1;
			}
		}

		std::cout << answer << '\n';
	}

	return 0;
}

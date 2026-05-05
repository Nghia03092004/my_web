#include <cstdint>
#include <iostream>

int main() {
	std::int64_t n;
	std::cin >> n;

	std::int64_t answer = 0;
	for (std::int64_t divisor = 5; divisor <= n; divisor *= 5) {
		answer += n / divisor;
	}

	std::cout << answer << '\n';
	return 0;
}

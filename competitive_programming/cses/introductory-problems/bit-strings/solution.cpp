#include <cstdint>
#include <iostream>

int main() {
	const std::int64_t mod = 1000000007LL;

	std::int64_t n;
	std::cin >> n;

	std::int64_t result = 1;
	std::int64_t base = 2;

	while (n > 0) {
		if (n & 1LL) {
			result = (result * base) % mod;
		}
		base = (base * base) % mod;
		n >>= 1LL;
	}

	std::cout << result << '\n';
	return 0;
}

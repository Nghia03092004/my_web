#include <iostream>

int main() {
	int n;
	std::cin >> n;

	const int limit = 1 << n;
	for (int i = 0; i < limit; ++i) {
		const int gray = i ^ (i >> 1);
		for (int bit = n - 1; bit >= 0; --bit) {
			std::cout << ((gray >> bit) & 1);
		}
		std::cout << '\n';
	}

	return 0;
}

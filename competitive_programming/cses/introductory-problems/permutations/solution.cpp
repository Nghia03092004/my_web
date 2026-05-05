#include <iostream>

int main() {
	int n;
	std::cin >> n;

	if (n == 1) {
		std::cout << 1 << '\n';
		return 0;
	}

	if (n == 2 || n == 3) {
		std::cout << "NO SOLUTION\n";
		return 0;
	}

	bool first = true;
	for (int value = 2; value <= n; value += 2) {
		if (!first) {
			std::cout << ' ';
		}
		std::cout << value;
		first = false;
	}
	for (int value = 1; value <= n; value += 2) {
		if (!first) {
			std::cout << ' ';
		}
		std::cout << value;
		first = false;
	}
	std::cout << '\n';
	return 0;
}

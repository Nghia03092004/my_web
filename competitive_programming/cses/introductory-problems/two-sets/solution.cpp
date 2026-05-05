#include <cstdint>
#include <iostream>
#include <vector>

int main() {
	std::int64_t n;
	std::cin >> n;

	const std::int64_t total = n * (n + 1) / 2;
	if (total % 2 == 1) {
		std::cout << "NO\n";
		return 0;
	}

	std::int64_t target = total / 2;
	std::vector<int> first_set;
	std::vector<int> second_set;

	for (int value = static_cast<int>(n); value >= 1; --value) {
		if (value <= target) {
			first_set.push_back(value);
			target -= value;
		} else {
			second_set.push_back(value);
		}
	}

	std::cout << "YES\n";
	std::cout << first_set.size() << '\n';
	for (std::size_t i = 0; i < first_set.size(); ++i) {
		std::cout << first_set[i] << (i + 1 == first_set.size() ? '\n' : ' ');
	}
	std::cout << second_set.size() << '\n';
	for (std::size_t i = 0; i < second_set.size(); ++i) {
		std::cout << second_set[i] << (i + 1 == second_set.size() ? '\n' : ' ');
	}

	return 0;
}

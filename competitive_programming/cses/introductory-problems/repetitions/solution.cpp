#include <algorithm>
#include <iostream>
#include <string>

int main() {
	std::string s;
	std::cin >> s;

	int best = 1;
	int current = 1;

	for (int i = 1; i < static_cast<int>(s.size()); ++i) {
		if (s[i] == s[i - 1]) {
			++current;
		} else {
			current = 1;
		}
		best = std::max(best, current);
	}

	std::cout << best << '\n';
	return 0;
}

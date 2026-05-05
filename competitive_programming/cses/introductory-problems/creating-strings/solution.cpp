#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
	std::string s;
	std::cin >> s;

	std::sort(s.begin(), s.end());

	std::vector<std::string> permutations;
	do {
		permutations.push_back(s);
	} while (std::next_permutation(s.begin(), s.end()));

	std::cout << permutations.size() << '\n';
	for (const std::string& permutation : permutations) {
		std::cout << permutation << '\n';
	}

	return 0;
}

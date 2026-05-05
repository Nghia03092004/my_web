#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
	std::string s;
	std::cin >> s;

	std::vector<int> count(26, 0);
	for (char c : s) {
		++count[c - 'A'];
	}

	int odd_index = -1;
	for (int i = 0; i < 26; ++i) {
		if (count[i] % 2 == 1) {
			if (odd_index != -1) {
				std::cout << "NO SOLUTION\n";
				return 0;
			}
			odd_index = i;
		}
	}

	std::string left_half;
	for (int i = 0; i < 26; ++i) {
		left_half.append(count[i] / 2, static_cast<char>('A' + i));
	}

	std::string right_half = left_half;
	std::reverse(right_half.begin(), right_half.end());

	std::cout << left_half;
	if (odd_index != -1) {
		std::cout << std::string(count[odd_index] % 2, static_cast<char>('A' + odd_index));
	}
	std::cout << right_half << '\n';
	return 0;
}

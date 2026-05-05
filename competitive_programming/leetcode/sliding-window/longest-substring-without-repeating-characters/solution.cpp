#include <algorithm>
#include <array>
#include <string>

class Solution {
public:
	int lengthOfLongestSubstring(std::string s) {
		std::array<int, 256> lastSeen;
		lastSeen.fill(-1);

		int bestLength = 0;
		int left = 0;

		for (int right = 0; right < static_cast<int>(s.size()); ++right) {
			const unsigned char current = static_cast<unsigned char>(s[right]);
			left = std::max(left, lastSeen[current] + 1);
			bestLength = std::max(bestLength, right - left + 1);
			lastSeen[current] = right;
		}

		return bestLength;
	}
};

#include <algorithm>
#include <vector>

class Solution {
public:
	int minEatingSpeed(std::vector<int>& piles, int h) {
		int left = 1;
		int right = *std::max_element(piles.begin(), piles.end());

		while (left < right) {
			const int middle = left + (right - left) / 2;

			if (canFinish(piles, h, middle)) {
				right = middle;
			} else {
				left = middle + 1;
			}
		}

		return left;
	}

private:
	bool canFinish(const std::vector<int>& piles, int h, int speed) {
		long long hoursNeeded = 0;

		for (int pile : piles) {
			hoursNeeded += (pile + speed - 1LL) / speed;
			if (hoursNeeded > h) {
				return false;
			}
		}

		return true;
	}
};

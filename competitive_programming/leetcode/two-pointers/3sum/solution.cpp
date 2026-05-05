#include <algorithm>
#include <vector>

class Solution {
public:
	std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
		std::sort(nums.begin(), nums.end());
		std::vector<std::vector<int>> triplets;

		for (int index = 0; index < static_cast<int>(nums.size()); ++index) {
			if (nums[index] > 0) {
				break;
			}

			if (index > 0 && nums[index] == nums[index - 1]) {
				continue;
			}

			int left = index + 1;
			int right = static_cast<int>(nums.size()) - 1;

			while (left < right) {
				const int sum = nums[index] + nums[left] + nums[right];

				if (sum < 0) {
					++left;
				} else if (sum > 0) {
					--right;
				} else {
					triplets.push_back({nums[index], nums[left], nums[right]});
					++left;
					--right;

					while (left < right && nums[left] == nums[left - 1]) {
						++left;
					}

					while (left < right && nums[right] == nums[right + 1]) {
						--right;
					}
				}
			}
		}

		return triplets;
	}
};

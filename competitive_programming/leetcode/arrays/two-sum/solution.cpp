#include <unordered_map>
#include <vector>

class Solution {
public:
	std::vector<int> twoSum(std::vector<int>& nums, int target) {
		std::unordered_map<int, int> indexByValue;

		for (int index = 0; index < static_cast<int>(nums.size()); ++index) {
			const int value = nums[index];
			const int complement = target - value;

			auto it = indexByValue.find(complement);
			if (it != indexByValue.end()) {
				return {it->second, index};
			}

			indexByValue[value] = index;
		}

		return {};
	}
};

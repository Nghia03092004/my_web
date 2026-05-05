#include <stack>
#include <vector>

class Solution {
public:
	std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
		std::vector<int> answer(temperatures.size(), 0);
		std::stack<int> pendingIndices;

		for (int index = 0; index < static_cast<int>(temperatures.size()); ++index) {
			while (!pendingIndices.empty() && temperatures[index] > temperatures[pendingIndices.top()]) {
				const int previousIndex = pendingIndices.top();
				pendingIndices.pop();
				answer[previousIndex] = index - previousIndex;
			}

			pendingIndices.push(index);
		}

		return answer;
	}
};

#include <algorithm>
#include <vector>

class Solution {
public:
	int coinChange(std::vector<int>& coins, int amount) {
		const int unreachable = amount + 1;
		std::vector<int> minimumCoins(amount + 1, unreachable);
		minimumCoins[0] = 0;

		for (int currentAmount = 1; currentAmount <= amount; ++currentAmount) {
			for (int coin : coins) {
				if (coin > currentAmount || minimumCoins[currentAmount - coin] == unreachable) {
					continue;
				}

				minimumCoins[currentAmount] = std::min(
					minimumCoins[currentAmount],
					minimumCoins[currentAmount - coin] + 1
				);
			}
		}

		return minimumCoins[amount] == unreachable ? -1 : minimumCoins[amount];
	}
};

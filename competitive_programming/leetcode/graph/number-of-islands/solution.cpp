#include <vector>

class Solution {
public:
	int numIslands(std::vector<std::vector<char>>& grid) {
		const int rows = static_cast<int>(grid.size());
		const int columns = static_cast<int>(grid[0].size());
		int islandCount = 0;

		for (int row = 0; row < rows; ++row) {
			for (int column = 0; column < columns; ++column) {
				if (grid[row][column] != '1') {
					continue;
				}

				++islandCount;
				eraseIsland(grid, row, column);
			}
		}

		return islandCount;
	}

private:
	void eraseIsland(std::vector<std::vector<char>>& grid, int row, int column) {
		const int rows = static_cast<int>(grid.size());
		const int columns = static_cast<int>(grid[0].size());

		if (row < 0 || row >= rows || column < 0 || column >= columns || grid[row][column] != '1') {
			return;
		}

		grid[row][column] = '0';

		eraseIsland(grid, row + 1, column);
		eraseIsland(grid, row - 1, column);
		eraseIsland(grid, row, column + 1);
		eraseIsland(grid, row, column - 1);
	}
};

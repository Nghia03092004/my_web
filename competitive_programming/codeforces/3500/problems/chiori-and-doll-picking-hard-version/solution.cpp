#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MOD = 998244353;

int mod_pow(int base, int exponent) {
	int result = 1;
	while (exponent > 0) {
		if (exponent & 1) {
			result = int(1LL * result * base % MOD);
		}
		base = int(1LL * base * base % MOD);
		exponent >>= 1;
	}
	return result;
}

vector<int> enumerate_weight_distribution(const vector<unsigned long long>& basis, int m) {
	const int dimension = int(basis.size());
	const uint32_t total = 1u << dimension;
	vector<int> counts(m + 1, 0);
	unsigned long long current = 0;
	uint32_t previous_gray = 0;

	for (uint32_t mask = 0; mask < total; ++mask) {
		const uint32_t gray = mask ^ (mask >> 1);
		if (mask != 0) {
			const uint32_t diff = gray ^ previous_gray;
			const int changed_bit = __builtin_ctz(diff);
			current ^= basis[changed_bit];
		}
		++counts[__builtin_popcountll(current)];
		previous_gray = gray;
	}

	return counts;
}

}  // namespace

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;

	vector<unsigned long long> pivot_basis(m, 0);
	for (int i = 0; i < n; ++i) {
		unsigned long long value;
		cin >> value;
		for (int bit = m - 1; bit >= 0; --bit) {
			if (((value >> bit) & 1ULL) == 0) {
				continue;
			}
			if (pivot_basis[bit] == 0) {
				pivot_basis[bit] = value;
				break;
			}
			value ^= pivot_basis[bit];
		}
	}

	for (int bit = 0; bit < m; ++bit) {
		if (pivot_basis[bit] == 0) {
			continue;
		}
		for (int lower = 0; lower < bit; ++lower) {
			if ((pivot_basis[bit] >> lower) & 1ULL) {
				pivot_basis[bit] ^= pivot_basis[lower];
			}
		}
	}
	for (int bit = 0; bit < m; ++bit) {
		if (pivot_basis[bit] == 0) {
			continue;
		}
		for (int higher = bit + 1; higher < m; ++higher) {
			if ((pivot_basis[higher] >> bit) & 1ULL) {
				pivot_basis[higher] ^= pivot_basis[bit];
			}
		}
	}

	vector<int> pivot_columns;
	vector<unsigned long long> basis_rows;
	vector<char> is_pivot(m, false);
	for (int bit = 0; bit < m; ++bit) {
		if (pivot_basis[bit] == 0) {
			continue;
		}
		is_pivot[bit] = true;
		pivot_columns.push_back(bit);
		basis_rows.push_back(pivot_basis[bit]);
	}

	const int rank = int(basis_rows.size());
	const int subset_multiplier = mod_pow(2, n - rank);
	vector<int> answer(m + 1, 0);

	if (rank <= 26) {
		const vector<int> codeword_counts = enumerate_weight_distribution(basis_rows, m);
		for (int weight = 0; weight <= m; ++weight) {
			answer[weight] = int(1LL * codeword_counts[weight] * subset_multiplier % MOD);
		}
	} else {
		vector<int> free_columns;
		for (int bit = 0; bit < m; ++bit) {
			if (!is_pivot[bit]) {
				free_columns.push_back(bit);
			}
		}

		vector<unsigned long long> dual_basis;
		dual_basis.reserve(free_columns.size());
		for (int free_column : free_columns) {
			unsigned long long vector_mask = 1ULL << free_column;
			for (int row = 0; row < rank; ++row) {
				if ((basis_rows[row] >> free_column) & 1ULL) {
					vector_mask |= 1ULL << pivot_columns[row];
				}
			}
			dual_basis.push_back(vector_mask);
		}

		const vector<int> dual_counts = enumerate_weight_distribution(dual_basis, m);
		vector<vector<int>> combinations(m + 1, vector<int>(m + 1, 0));
		for (int i = 0; i <= m; ++i) {
			combinations[i][0] = 1;
			for (int j = 1; j <= i; ++j) {
				combinations[i][j] = combinations[i - 1][j - 1];
				if (j < i) {
					combinations[i][j] += combinations[i - 1][j];
					if (combinations[i][j] >= MOD) {
						combinations[i][j] -= MOD;
					}
				}
			}
		}

		const int inverse_dual_size = mod_pow(mod_pow(2, m - rank), MOD - 2);
		for (int target_weight = 0; target_weight <= m; ++target_weight) {
			long long total = 0;
			for (int dual_weight = 0; dual_weight <= m; ++dual_weight) {
				if (dual_counts[dual_weight] == 0) {
					continue;
				}
				long long coefficient = 0;
				const int low = max(0, target_weight - (m - dual_weight));
				const int high = min(target_weight, dual_weight);
				for (int take = low; take <= high; ++take) {
					long long ways = 1LL * combinations[dual_weight][take] *
						combinations[m - dual_weight][target_weight - take] % MOD;
					if (take & 1) {
						coefficient -= ways;
					} else {
						coefficient += ways;
					}
				}
				coefficient %= MOD;
				if (coefficient < 0) {
					coefficient += MOD;
				}
				total += coefficient * dual_counts[dual_weight];
				total %= MOD;
			}
			answer[target_weight] = int(total * inverse_dual_size % MOD);
			answer[target_weight] = int(1LL * answer[target_weight] * subset_multiplier % MOD);
		}
	}

	for (int weight = 0; weight <= m; ++weight) {
		cout << answer[weight] << (weight == m ? '\n' : ' ');
	}
	return 0;
}

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MAX_LOG = 34;

struct Interval {
	int left;
	int right;
};

Interval merge_intervals(const Interval& first, const Interval& second) {
	return {min(first.left, second.left), max(first.right, second.right)};
}

bool operator==(const Interval& first, const Interval& second) {
	return first.left == second.left && first.right == second.right;
}

struct SparseTable {
	vector<int> logs;
	vector<vector<Interval>> table;

	void build(const vector<Interval>& values, int size) {
		logs.assign(size + 1, 0);
		for (int i = 2; i <= size; ++i) {
			logs[i] = logs[i / 2] + 1;
		}
		const int levels = logs[size] + 1;
		table.assign(levels, vector<Interval>(size + 1));
		for (int i = 1; i <= size; ++i) {
			table[0][i] = values[i];
		}
		for (int level = 1; level < levels; ++level) {
			const int length = 1 << level;
			const int half = length >> 1;
			for (int i = 1; i + length - 1 <= size; ++i) {
				table[level][i] = merge_intervals(table[level - 1][i], table[level - 1][i + half]);
			}
		}
	}

	Interval query(int left, int right) const {
		const int level = logs[right - left + 1];
		return merge_intervals(table[level][left], table[level][right - (1 << level) + 1]);
	}
};

Interval advance_interval(
	const Interval& current,
	int level,
	const SparseTable& sparse,
	const vector<vector<int>>& single_jumps
) {
	if (current.left == current.right) {
		const int next_index = single_jumps[level][current.left];
		return {next_index, next_index};
	}

	return sparse.query(current.left, current.right - 1);
}

}  // namespace

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;

	if (n == 1) {
		while (q--) {
			int left, right;
			cin >> left >> right;
			cout << 0 << '\n';
		}
		return 0;
	}

	vector<int> a(n + 1);
	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
	}

	vector<pair<int, int>> queries(q);
	for (int i = 0; i < q; ++i) {
		cin >> queries[i].first >> queries[i].second;
	}

	vector<vector<int>> single_jumps(MAX_LOG, vector<int>(n + 1));
	for (int i = 1; i <= n; ++i) {
		single_jumps[0][i] = a[i];
	}
	for (int level = 0; level + 1 < MAX_LOG; ++level) {
		for (int i = 1; i <= n; ++i) {
			single_jumps[level + 1][i] = single_jumps[level][single_jumps[level][i]];
		}
	}

	vector<vector<Interval>> pair_jumps(MAX_LOG, vector<Interval>(n));
	for (int i = 1; i < n; ++i) {
		pair_jumps[0][i] = {min(a[i], a[i + 1]), max(a[i], a[i + 1])};
	}

	SparseTable sparse;
	for (int level = 0; level + 1 < MAX_LOG; ++level) {
		sparse.build(pair_jumps[level], n - 1);
		for (int i = 1; i < n; ++i) {
			const Interval current = pair_jumps[level][i];
			if (current.left == current.right) {
				const int next_index = single_jumps[level][current.left];
				pair_jumps[level + 1][i] = {next_index, next_index};
			} else {
				pair_jumps[level + 1][i] = sparse.query(current.left, current.right - 1);
			}
		}
	}

	vector<long long> answers(q, -1);
	vector<Interval> current(q);
	vector<char> active(q, false);
	const Interval target{1, n};

	for (int i = 0; i < q; ++i) {
		const int left = queries[i].first;
		const int right = queries[i].second;
		if (left == 1 && right == n) {
			answers[i] = 0;
		} else if (left == right) {
			answers[i] = -1;
		} else {
			current[i] = Interval{left, right};
			answers[i] = 0;
			active[i] = true;
		}
	}

	for (int level = MAX_LOG - 1; level >= 0; --level) {
		sparse.build(pair_jumps[level], n - 1);
		for (int i = 0; i < q; ++i) {
			if (!active[i]) {
				continue;
			}
			const Interval next = advance_interval(current[i], level, sparse, single_jumps);
			if (!(next == target)) {
				answers[i] += 1LL << level;
				current[i] = next;
			}
		}
	}

	sparse.build(pair_jumps[0], n - 1);
	for (int i = 0; i < q; ++i) {
		if (!active[i]) {
			continue;
		}
		const Interval next = advance_interval(current[i], 0, sparse, single_jumps);
		answers[i] = (next == target ? answers[i] + 1 : -1);
	}

	for (long long answer : answers) {
		cout << answer << '\n';
	}
	return 0;
}

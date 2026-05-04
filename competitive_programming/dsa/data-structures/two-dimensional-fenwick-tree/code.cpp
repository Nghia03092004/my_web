struct Fenwick2D {
    int n;
    int m;
    vector<vector<long long>> bit;

    Fenwick2D(int n, int m) : n(n), m(m), bit(n + 1, vector<long long>(m + 1, 0)) {}

    void add(int x, int y, long long delta) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                bit[i][j] += delta;
            }
        }
    }

    long long prefix_sum(int x, int y) const {
        long long result = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                result += bit[i][j];
            }
        }
        return result;
    }

    long long rectangle_sum(int x1, int y1, int x2, int y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        return prefix_sum(x2, y2)
            - prefix_sum(x1 - 1, y2)
            - prefix_sum(x2, y1 - 1)
            + prefix_sum(x1 - 1, y1 - 1);
    }
};

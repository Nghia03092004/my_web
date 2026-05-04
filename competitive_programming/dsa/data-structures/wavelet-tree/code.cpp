struct WaveletTree {
    int lo;
    int hi;
    WaveletTree* left;
    WaveletTree* right;
    vector<int> pref;

    WaveletTree(vector<int>::iterator from, vector<int>::iterator to, int x, int y)
        : lo(x), hi(y), left(nullptr), right(nullptr) {
        if (from >= to || lo == hi) return;
        int mid = (lo + hi) / 2;
        auto goes_left = [mid](int value) { return value <= mid; };
        pref.reserve((to - from) + 1);
        pref.push_back(0);
        for (auto it = from; it != to; ++it) {
            pref.push_back(pref.back() + goes_left(*it));
        }
        auto pivot = stable_partition(from, to, goes_left);
        if (from < pivot) left = new WaveletTree(from, pivot, lo, mid);
        if (pivot < to) right = new WaveletTree(pivot, to, mid + 1, hi);
    }

    int kth(int l, int r, int k) const {
        if (l > r) return -1;
        if (lo == hi) return lo;
        int in_left = pref[r] - pref[l - 1];
        if (k <= in_left) {
            return left->kth(pref[l - 1] + 1, pref[r], k);
        }
        return right->kth(l - pref[l - 1], r - pref[r], k - in_left);
    }

    int lte(int l, int r, int x) const {
        if (l > r || x < lo) return 0;
        if (hi <= x) return r - l + 1;
        int left_count = left ? left->lte(pref[l - 1] + 1, pref[r], x) : 0;
        int right_count = right ? right->lte(l - pref[l - 1], r - pref[r], x) : 0;
        return left_count + right_count;
    }
};

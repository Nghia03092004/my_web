template <int MAX_SUM>
struct SubsetSumBitset {
    bitset<MAX_SUM + 1> reachable;

    SubsetSumBitset() {
        reachable[0] = 1;
    }

    void add_value(int x) {
        reachable |= (reachable << x);
    }

    bool can_make(int sum) const {
        return 0 <= sum && sum <= MAX_SUM && reachable[sum];
    }

    vector<int> all_reachable_sums() const {
        vector<int> sums;
        for (int sum = 0; sum <= MAX_SUM; ++sum) {
            if (reachable[sum]) sums.push_back(sum);
        }
        return sums;
    }
};

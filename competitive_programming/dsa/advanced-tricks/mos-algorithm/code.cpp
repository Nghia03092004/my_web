#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l;
    int r;
    int idx;
};

// Example application: count distinct values on many static subarrays.
vector<int> mos_distinct(const vector<int>& values, vector<Query> queries) {
    int n = (int)values.size();
    if (n == 0) return vector<int>(queries.size(), 0);

    int block = max(1, (int)sqrt(n));
    sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
        int block_a = a.l / block;
        int block_b = b.l / block;
        if (block_a != block_b) return block_a < block_b;
        if (block_a & 1) return a.r > b.r;
        return a.r < b.r;
    });

    int max_value = 0;
    for (int x : values) max_value = max(max_value, x);

    vector<int> freq(max_value + 1, 0);
    vector<int> answers(queries.size(), 0);
    int current_left = 0, current_right = -1;
    int distinct = 0;

    auto add = [&](int index) {
        if (++freq[values[index]] == 1) ++distinct;
    };

    auto remove = [&](int index) {
        if (--freq[values[index]] == 0) --distinct;
    };

    for (const Query& query : queries) {
        while (current_left > query.l) add(--current_left);
        while (current_right < query.r) add(++current_right);
        while (current_left < query.l) remove(current_left++);
        while (current_right > query.r) remove(current_right--);
        answers[query.idx] = distinct;
    }
    return answers;
}

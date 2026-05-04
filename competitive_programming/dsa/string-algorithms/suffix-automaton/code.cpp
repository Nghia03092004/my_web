#include <bits/stdc++.h>

using namespace std;

struct SuffixAutomaton {
    struct State {
        int link = -1;
        int len = 0;
        int occ = 0;
        array<int, 26> next{};

        State() {
            next.fill(-1);
        }
    };

    vector<State> st;
    int last = 0;

    SuffixAutomaton() {
        st.push_back(State{});
    }

    void extend(char ch) {
        int c = ch - 'a';
        int cur = (int)st.size();
        st.push_back(State{});
        st[cur].len = st[last].len + 1;
        st[cur].occ = 1;

        int p = last;
        while (p != -1 && st[p].next[c] == -1) {
            st[p].next[c] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = (int)st.size();
                st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].occ = 0;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = clone;
                st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string& s) {
        for (char ch : s) extend(ch);
    }

    bool contains(const string& s) const {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (st[v].next[c] == -1) return false;
            v = st[v].next[c];
        }
        return true;
    }

    long long count_distinct_substrings() const {
        long long answer = 0;
        for (int v = 1; v < (int)st.size(); ++v) {
            answer += st[v].len - st[st[v].link].len;
        }
        return answer;
    }

    void propagate_occurrences() {
        int max_len = 0;
        for (const State& state : st) max_len = max(max_len, state.len);

        vector<int> cnt(max_len + 1, 0), order(st.size());
        for (const State& state : st) ++cnt[state.len];
        for (int i = 1; i <= max_len; ++i) cnt[i] += cnt[i - 1];
        for (int i = (int)st.size() - 1; i >= 0; --i) {
            order[--cnt[st[i].len]] = i;
        }

        for (int i = (int)order.size() - 1; i > 0; --i) {
            int v = order[i];
            int p = st[v].link;
            if (p != -1) st[p].occ += st[v].occ;
        }
    }

    int occurrences_of(const string& pattern) const {
        int v = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (st[v].next[c] == -1) return 0;
            v = st[v].next[c];
        }
        return st[v].occ;
    }
};

// Example application: count occurrences of many patterns in one fixed text.
vector<int> count_occurrences_of_patterns(const string& text, const vector<string>& patterns) {
    SuffixAutomaton sam;
    sam.build(text);
    sam.propagate_occurrences();

    vector<int> answer;
    answer.reserve(patterns.size());
    for (const string& pattern : patterns) {
        answer.push_back(sam.occurrences_of(pattern));
    }
    return answer;
}

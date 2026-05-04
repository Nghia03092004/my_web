vector<int> next_strictly_smaller_right(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> answer(n, -1);
    vector<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[i] < a[st.back()]) {
            answer[st.back()] = i;
            st.pop_back();
        }
        st.push_back(i);
    }
    return answer;
}

vector<int> sliding_window_minimum(const vector<int>& a, int k) {
    deque<int> dq;
    vector<int> answer;
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
        dq.push_back(i);
        if (i + 1 >= k) answer.push_back(a[dq.front()]);
    }
    return answer;
}

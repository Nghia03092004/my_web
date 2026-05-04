struct CartesianTree {
    int root;
    vector<int> parent;
    vector<int> left_child;
    vector<int> right_child;
};

CartesianTree build_min_cartesian_tree(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> parent(n, -1), left_child(n, -1), right_child(n, -1);
    vector<int> st;

    for (int i = 0; i < n; ++i) {
        int last = -1;
        while (!st.empty() && a[i] < a[st.back()]) {
            last = st.back();
            st.pop_back();
        }
        if (!st.empty()) {
            parent[i] = st.back();
            right_child[st.back()] = i;
        }
        if (last != -1) {
            parent[last] = i;
            left_child[i] = last;
        }
        st.push_back(i);
    }

    int root = st.front();
    while (parent[root] != -1) root = parent[root];
    return {root, parent, left_child, right_child};
}

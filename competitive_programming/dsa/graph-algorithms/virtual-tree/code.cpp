struct VirtualTree {
    vector<int> nodes;
    vector<vector<int>> children;
};

VirtualTree build_virtual_tree(
    vector<int> marked,
    const vector<int>& tin,
    const function<int(int, int)>& lca,
    const function<bool(int, int)>& is_ancestor
) {
    auto by_tin = [&](int a, int b) { return tin[a] < tin[b]; };
    sort(marked.begin(), marked.end(), by_tin);

    vector<int> nodes = marked;
    for (int i = 0; i + 1 < (int)marked.size(); ++i) {
        nodes.push_back(lca(marked[i], marked[i + 1]));
    }
    sort(nodes.begin(), nodes.end(), by_tin);
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    vector<int> st;
    vector<vector<int>> children(nodes.size());
    unordered_map<int, int> id;
    for (int i = 0; i < (int)nodes.size(); ++i) id[nodes[i]] = i;

    for (int v : nodes) {
        while (!st.empty() && !is_ancestor(st.back(), v)) st.pop_back();
        if (!st.empty()) children[id[st.back()]].push_back(id[v]);
        st.push_back(v);
    }
    return {nodes, children};
}

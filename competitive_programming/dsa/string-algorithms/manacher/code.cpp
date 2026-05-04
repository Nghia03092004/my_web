pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = (int)s.size();
    vector<int> odd(n), even(n);

    for (int left = 0, right = -1, i = 0; i < n; ++i) {
        int radius = (i > right) ? 1 : min(odd[left + right - i], right - i + 1);
        while (i - radius >= 0 && i + radius < n && s[i - radius] == s[i + radius]) {
            ++radius;
        }
        odd[i] = radius;
        if (i + radius - 1 > right) {
            left = i - radius + 1;
            right = i + radius - 1;
        }
    }

    for (int left = 0, right = -1, i = 0; i < n; ++i) {
        int radius = (i > right) ? 0 : min(even[left + right - i + 1], right - i + 1);
        while (i - radius - 1 >= 0 && i + radius < n && s[i - radius - 1] == s[i + radius]) {
            ++radius;
        }
        even[i] = radius;
        if (i + radius - 1 > right) {
            left = i - radius;
            right = i + radius - 1;
        }
    }

    return {odd, even};
}

struct Event {
    long long x;
    int type;  // 0 = add, 1 = query, 2 = remove
    int id;
};

vector<int> count_covering_intervals(
    const vector<pair<long long, long long>>& intervals,
    const vector<long long>& queries
) {
    vector<Event> events;
    events.reserve(intervals.size() * 2 + queries.size());

    for (auto [l, r] : intervals) {
        events.push_back({l, 0, -1});
        events.push_back({r, 2, -1});
    }
    for (int i = 0; i < (int)queries.size(); ++i) {
        events.push_back({queries[i], 1, i});
    }

    sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.type < b.type;
    });

    int active = 0;
    vector<int> answer(queries.size());
    for (const Event& event : events) {
        if (event.type == 0) {
            ++active;
        } else if (event.type == 1) {
            answer[event.id] = active;
        } else {
            --active;
        }
    }
    return answer;
}

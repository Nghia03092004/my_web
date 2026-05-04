struct Interval {
    long long l;
    long long r;
};

vector<Interval> maximum_non_overlapping_intervals(vector<Interval> intervals) {
    sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        if (a.r != b.r) return a.r < b.r;
        return a.l < b.l;
    });

    vector<Interval> chosen;
    long long current_end = numeric_limits<long long>::lowest();
    for (const Interval& interval : intervals) {
        if (interval.l >= current_end) {
            chosen.push_back(interval);
            current_end = interval.r;
        }
    }
    return chosen;
}

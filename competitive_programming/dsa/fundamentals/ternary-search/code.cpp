template <class F>
double ternary_search_real(double lo, double hi, F f, int iterations = 200) {
    for (int it = 0; it < iterations; ++it) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if (f(m1) <= f(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    return (lo + hi) * 0.5;
}

double minimum_meeting_time(const vector<double>& position, const vector<double>& speed) {
    double lo = *min_element(position.begin(), position.end());
    double hi = *max_element(position.begin(), position.end());
    auto cost = [&](double x) {
        double worst = 0.0;
        for (int i = 0; i < (int)position.size(); ++i) {
            worst = max(worst, abs(position[i] - x) / speed[i]);
        }
        return worst;
    };
    double best_x = ternary_search_real(lo, hi, cost);
    return cost(best_x);
}

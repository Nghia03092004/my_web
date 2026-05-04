struct Point {
    long long x;
    long long y;
};

long long dist2(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

long long closest_pair_rec(vector<Point>& points, vector<Point>& buffer, int left, int right) {
    if (right - left <= 3) {
        long long best = numeric_limits<long long>::max();
        for (int i = left; i < right; ++i) {
            for (int j = i + 1; j < right; ++j) {
                best = min(best, dist2(points[i], points[j]));
            }
        }
        sort(points.begin() + left, points.begin() + right, [](const Point& a, const Point& b) {
            return a.y < b.y;
        });
        return best;
    }

    int mid = (left + right) / 2;
    long long mid_x = points[mid].x;
    long long best = min(
        closest_pair_rec(points, buffer, left, mid),
        closest_pair_rec(points, buffer, mid, right)
    );

    merge(
        points.begin() + left, points.begin() + mid,
        points.begin() + mid, points.begin() + right,
        buffer.begin(),
        [](const Point& a, const Point& b) { return a.y < b.y; }
    );
    copy(buffer.begin(), buffer.begin() + (right - left), points.begin() + left);

    vector<Point> strip;
    for (int i = left; i < right; ++i) {
        long long dx = points[i].x - mid_x;
        if (dx * dx < best) {
            for (int j = (int)strip.size() - 1; j >= 0; --j) {
                long long dy = points[i].y - strip[j].y;
                if (dy * dy >= best) break;
                best = min(best, dist2(points[i], strip[j]));
            }
            strip.push_back(points[i]);
        }
    }
    return best;
}

long long closest_pair_squared(vector<Point> points) {
    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    vector<Point> buffer(points.size());
    return closest_pair_rec(points, buffer, 0, (int)points.size());
}

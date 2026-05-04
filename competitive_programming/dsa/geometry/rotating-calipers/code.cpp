struct Point {
    long long x;
    long long y;
};

long long cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

long long dist2(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

long long convex_polygon_diameter_sq(const vector<Point>& hull) {
    int n = (int)hull.size();
    if (n <= 1) return 0;
    if (n == 2) return dist2(hull[0], hull[1]);

    long long answer = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (true) {
            int nj = (j + 1) % n;
            if (abs(cross(hull[i], hull[ni], hull[nj])) > abs(cross(hull[i], hull[ni], hull[j]))) {
                j = nj;
            } else {
                break;
            }
        }
        answer = max(answer, dist2(hull[i], hull[j]));
        answer = max(answer, dist2(hull[ni], hull[j]));
    }
    return answer;
}

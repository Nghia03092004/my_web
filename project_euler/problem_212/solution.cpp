#include <bits/stdc++.h>
using namespace std;

namespace {

struct Cuboid {
    int x1, y1, z1;
    int x2, y2, z2;
};

struct Event {
    int x;
    int delta;
    int y1;
    int y2;

    bool operator<(const Event& other) const {
        return x < other.x;
    }
};

class SegmentTree {
public:
    explicit SegmentTree(int size) : size_(size), cover_(4 * size, 0), length_(4 * size, 0) {}

    void reset() {
        fill(cover_.begin(), cover_.end(), 0);
        fill(length_.begin(), length_.end(), 0);
    }

    void addInterval(int left, int right, int delta) {
        update(1, 0, size_, left, right, delta);
    }

    int coveredLength() const {
        return length_[1];
    }

private:
    int size_;
    vector<int> cover_;
    vector<int> length_;

    void update(int node, int left, int right, int queryLeft, int queryRight, int delta) {
        if (queryRight <= left || right <= queryLeft) {
            return;
        }

        if (queryLeft <= left && right <= queryRight) {
            cover_[node] += delta;
        } else {
            int mid = (left + right) / 2;
            update(node * 2, left, mid, queryLeft, queryRight, delta);
            update(node * 2 + 1, mid, right, queryLeft, queryRight, delta);
        }

        if (cover_[node] > 0) {
            length_[node] = right - left;
        } else if (right - left == 1) {
            length_[node] = 0;
        } else {
            length_[node] = length_[node * 2] + length_[node * 2 + 1];
        }
    }
};

long long computeUnionArea(const vector<int>& active, const vector<Cuboid>& cuboids, SegmentTree& tree) {
    if (active.empty()) {
        return 0;
    }

    vector<Event> events;
    events.reserve(active.size() * 2);
    for (int idx : active) {
        const Cuboid& cuboid = cuboids[idx];
        events.push_back({cuboid.x1, 1, cuboid.y1, cuboid.y2});
        events.push_back({cuboid.x2, -1, cuboid.y1, cuboid.y2});
    }

    sort(events.begin(), events.end());
    tree.reset();

    long long area = 0;
    int prevX = events.front().x;
    int i = 0;

    while (i < static_cast<int>(events.size())) {
        int x = events[i].x;
        area += 1LL * tree.coveredLength() * (x - prevX);

        while (i < static_cast<int>(events.size()) && events[i].x == x) {
            tree.addInterval(events[i].y1, events[i].y2, events[i].delta);
            i++;
        }

        prevX = x;
    }

    return area;
}

}  // namespace

int main() {
    const int cuboidCount = 50'000;
    const int sequenceLength = 6 * cuboidCount;
    const int maxCoord = 10'400;

    vector<int> sequence(sequenceLength + 1, 0);
    for (int k = 1; k <= 55; k++) {
        long long k3 = 1LL * k * k * k;
        sequence[k] = static_cast<int>((100003LL - 200003LL * k + 300007LL * k3) % 1'000'000LL);
        if (sequence[k] < 0) {
            sequence[k] += 1'000'000;
        }
    }
    for (int k = 56; k <= sequenceLength; k++) {
        sequence[k] = (sequence[k - 24] + sequence[k - 55]) % 1'000'000;
    }

    vector<Cuboid> cuboids(cuboidCount);
    int maxZ = 0;
    for (int n = 1; n <= cuboidCount; n++) {
        int x1 = sequence[6 * n - 5] % 10'000;
        int y1 = sequence[6 * n - 4] % 10'000;
        int z1 = sequence[6 * n - 3] % 10'000;
        int x2 = x1 + 1 + sequence[6 * n - 2] % 399;
        int y2 = y1 + 1 + sequence[6 * n - 1] % 399;
        int z2 = z1 + 1 + sequence[6 * n] % 399;
        cuboids[n - 1] = {x1, y1, z1, x2, y2, z2};
        maxZ = max(maxZ, z2);
    }

    vector<vector<int>> starts(maxZ + 1), ends(maxZ + 1);
    for (int i = 0; i < cuboidCount; i++) {
        starts[cuboids[i].z1].push_back(i);
        ends[cuboids[i].z2].push_back(i);
    }

    vector<int> active;
    active.reserve(cuboidCount);
    vector<int> position(cuboidCount, -1);
    SegmentTree tree(maxCoord);

    long long totalVolume = 0;

    for (int z = 0; z < maxZ; z++) {
        for (int idx : ends[z]) {
            int pos = position[idx];
            if (pos == -1) {
                continue;
            }
            int last = active.back();
            active[pos] = last;
            position[last] = pos;
            active.pop_back();
            position[idx] = -1;
        }

        for (int idx : starts[z]) {
            position[idx] = static_cast<int>(active.size());
            active.push_back(idx);
        }

        if (!active.empty()) {
            totalVolume += computeUnionArea(active, cuboids, tree);
        }
    }

    cout << totalVolume << '\n';
    return 0;
}

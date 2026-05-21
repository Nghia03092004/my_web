def solve():
    cuboid_count = 50_000
    sequence_length = 6 * cuboid_count

    sequence = [0] * (sequence_length + 1)
    for k in range(1, 56):
        sequence[k] = (100003 - 200003 * k + 300007 * k * k * k) % 1_000_000
    for k in range(56, sequence_length + 1):
        sequence[k] = (sequence[k - 24] + sequence[k - 55]) % 1_000_000

    cuboids = []
    max_z = 0
    for n in range(1, cuboid_count + 1):
        x1 = sequence[6 * n - 5] % 10_000
        y1 = sequence[6 * n - 4] % 10_000
        z1 = sequence[6 * n - 3] % 10_000
        x2 = x1 + 1 + sequence[6 * n - 2] % 399
        y2 = y1 + 1 + sequence[6 * n - 1] % 399
        z2 = z1 + 1 + sequence[6 * n] % 399
        cuboids.append((x1, y1, z1, x2, y2, z2))
        if z2 > max_z:
            max_z = z2

    starts = [[] for _ in range(max_z + 1)]
    ends = [[] for _ in range(max_z + 1)]
    for idx, (_, _, z1, _, _, z2) in enumerate(cuboids):
        starts[z1].append(idx)
        ends[z2].append(idx)

    max_coord = 10_400
    tree_size = 4 * max_coord

    def compute_union_area(active):
        if not active:
            return 0

        cover = [0] * tree_size
        covered_length = [0] * tree_size
        events = []

        for idx in active:
            x1, y1, _, x2, y2, _ = cuboids[idx]
            events.append((x1, 1, y1, y2))
            events.append((x2, -1, y1, y2))

        events.sort()

        def update(node, left, right, query_left, query_right, delta):
            if query_right <= left or right <= query_left:
                return

            if query_left <= left and right <= query_right:
                cover[node] += delta
            else:
                mid = (left + right) // 2
                update(node * 2, left, mid, query_left, query_right, delta)
                update(node * 2 + 1, mid, right, query_left, query_right, delta)

            if cover[node] > 0:
                covered_length[node] = right - left
            elif right - left == 1:
                covered_length[node] = 0
            else:
                covered_length[node] = covered_length[node * 2] + covered_length[node * 2 + 1]

        area = 0
        prev_x = events[0][0]
        index = 0

        while index < len(events):
            x = events[index][0]
            area += covered_length[1] * (x - prev_x)

            while index < len(events) and events[index][0] == x:
                _, delta, y1, y2 = events[index]
                update(1, 0, max_coord, y1, y2, delta)
                index += 1

            prev_x = x

        return area

    active = []
    position = [-1] * cuboid_count
    total_volume = 0

    for z in range(max_z):
        for idx in ends[z]:
            pos = position[idx]
            if pos != -1:
                last = active[-1]
                active[pos] = last
                position[last] = pos
                active.pop()
                position[idx] = -1

        for idx in starts[z]:
            position[idx] = len(active)
            active.append(idx)

        if active:
            total_volume += compute_union_area(active)

    print(total_volume)


if __name__ == "__main__":
    solve()

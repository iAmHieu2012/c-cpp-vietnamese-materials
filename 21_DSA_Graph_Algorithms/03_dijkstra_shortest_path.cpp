// ============================================================================
// File: 03_dijkstra_shortest_path.cpp
// Bài 4.6: Thuật toán Dijkstra (Đường đi ngắn nhất từ 1 nguồn với cạnh không âm)
// ============================================================================

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

const long long INF = std::numeric_limits<long long>::max() / 4;

struct Edge {
    int to;
    int weight;
};

void dijkstraShortestPath(int V, const std::vector<std::vector<Edge>> &adj,
                          int src, int target) {
    std::vector<long long> dist(V, INF);
    std::vector<int> parent(V, -1);

    // Min-Heap lưu cặp {khoảng cách hiện tại, đỉnh u}
    using P = std::pair<long long, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Lazy Deletion: Bỏ qua bản ghi cũ nếu đã tìm được đường ngắn hơn tới u
        if (d > dist[u]) continue;

        for (const auto &e : adj[u]) {
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                parent[e.to] = u;
                pq.push({dist[e.to], e.to});
            }
        }
    }

    std::cout << "=== DIJKSTRA'S ALGORITHM TU DINH " << src
              << " (O((V + E) log V)) ===\n";
    for (int i = 0; i < V; ++i) {
        std::cout << "  -> Khoang cach ngan nhat toi dinh " << i << ": ";
        if (dist[i] == INF) std::cout << "INF\n";
        else std::cout << dist[i] << "\n";
    }

    if (dist[target] != INF) {
        std::vector<int> path;
        for (int curr = target; curr != -1; curr = parent[curr]) {
            path.push_back(curr);
        }
        std::reverse(path.begin(), path.end());
        std::cout << "\n  Lo trinh toi uu (" << src << " -> " << target
                  << ", tong chi phi = " << dist[target] << "): ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
        }
    }
}

int main() {
    int V = 5;
    std::vector<std::vector<Edge>> adj(V);
    auto addDirected = [&](int u, int v, int w) {
        adj[u].push_back({v, w});
    };

    addDirected(0, 1, 10);
    addDirected(0, 2, 3);
    addDirected(1, 2, 1);
    addDirected(1, 3, 2);
    addDirected(2, 1, 4);
    addDirected(2, 3, 8);
    addDirected(2, 4, 2);
    addDirected(3, 4, 7);
    addDirected(4, 3, 9);

    dijkstraShortestPath(V, adj, 0, 3);
    return 0;
}

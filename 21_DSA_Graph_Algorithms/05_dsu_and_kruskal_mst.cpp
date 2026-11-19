#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

// =============================================================================
// 1. DISJOINT SET UNION (PATH COMPRESSION + UNION BY RANK) - O(alpha(V)) ~ O(1)
// =============================================================================
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank_arr;

public:
    explicit DSU(int n) : parent(n), rank_arr(n, 0) {
        std::iota(parent.begin(), parent.end(), 0); // parent[i] = i
    }

    // Path Compression: Nén toàn bộ node trên đường đi trỏ thẳng về gốc
    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Union by Rank: Trả về true nếu gộp thành công (chưa cùng tập hợp)
    bool unite(int u, int v) {
        int ru = find(u);
        int rv = find(v);
        if (ru == rv) return false; // Đã cùng tập -> Thêm cạnh sẽ tạo chu trình

        if (rank_arr[ru] < rank_arr[rv]) {
            parent[ru] = rv;
        } else if (rank_arr[ru] > rank_arr[rv]) {
            parent[rv] = ru;
        } else {
            parent[rv] = ru;
            ++rank_arr[ru];
        }
        return true;
    }
};

struct WeightedEdge {
    int u, v, w;
    bool operator<(const WeightedEdge &other) const {
        return w < other.w;
    }
};

// =============================================================================
// 2. PRIM'S ALGORITHM (DỰA TRÊN ĐỈNH + MIN-HEAP) - O((V + E) log V)
// =============================================================================
long long prim_mst(int V, const std::vector<WeightedEdge> &edges) {
    std::vector<std::vector<std::pair<int, int>>> adj(V);
    for (const auto &e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }

    std::vector<bool> in_mst(V, false);
    // Min-Heap lưu {trọng số cạnh, đỉnh đích}
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    pq.push({0, 0}); // Bắt đầu từ đỉnh 0
    long long total_weight = 0;
    int vertices_taken = 0;

    while (!pq.empty() && vertices_taken < V) {
        auto [w, u] = pq.top();
        pq.pop();

        if (in_mst[u]) continue;
        in_mst[u] = true;
        total_weight += w;
        ++vertices_taken;

        for (const auto &[v, weight] : adj[u]) {
            if (!in_mst[v]) {
                pq.push({weight, v});
            }
        }
    }
    return total_weight;
}

int main() {
    int V = 5;
    std::vector<WeightedEdge> edges = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3},
        {1, 3, 8}, {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    };

    std::sort(edges.begin(), edges.end());

    DSU dsu(V);
    long long mst_weight = 0;
    std::vector<WeightedEdge> mst_edges;

    for (const auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst_weight += e.w;
            mst_edges.push_back(e);
            if (static_cast<int>(mst_edges.size()) == V - 1) break;
        }
    }

    std::cout << "=== 1. KRUSKAL'S MINIMUM SPANNING TREE (O(E log E)) ===\n";
    for (const auto &e : mst_edges) {
        std::cout << "  Canh (" << e.u << " - " << e.v << ") | Trong so: " << e.w << "\n";
    }
    std::cout << "-> Tong trong so MST (Kruskal) = " << mst_weight << "\n\n";

    std::cout << "=== 2. PRIM'S MINIMUM SPANNING TREE (O((V + E) log V)) ===\n";
    std::cout << "-> Tong trong so MST (Prim)    = " << prim_mst(V, edges) << "\n";

    return 0;
}

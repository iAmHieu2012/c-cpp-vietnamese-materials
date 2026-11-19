// ============================================================================
// File: 02_dfs_cycle_and_topological_sort.cpp
// Bài 4.6: Duyệt chiều sâu DFS, Phát hiện Chu trình 3 màu & Sắp xếp Tô-pô (Topological Sort)
// ============================================================================

#include <iostream>
#include <queue>
#include <vector>

// ============================================================================
// 1. DUYỆT CHIỀU SÂU DFS & ĐẾM SỐ THÀNH PHẦN LIÊN THÔNG
// ============================================================================
void dfsRec(int u, const std::vector<std::vector<int>> &adj,
            std::vector<bool> &visited) {
    visited[u] = true;
    std::cout << u << " ";
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfsRec(v, adj, visited);
        }
    }
}

// ============================================================================
// 2. PHÁT HIỆN CHU TRÌNH TRÊN ĐỒ THỊ CÓ HƯỚNG (3-COLOR DFS: 0=White, 1=Gray, 2=Black)
// ============================================================================
bool dfsCycleDirected(int u, const std::vector<std::vector<int>> &adj,
                      std::vector<int> &color) {
    color[u] = 1; // Đang trên nhánh đệ quy hiện tại (Gray)
    for (int v : adj[u]) {
        if (color[v] == 1) return true; // Gặp Back-Edge -> Có chu trình!
        if (color[v] == 0 && dfsCycleDirected(v, adj, color)) return true;
    }
    color[u] = 2; // Đã duyệt xong toàn bộ nhánh con (Black)
    return false;
}

bool hasDirectedCycle(int V, const std::vector<std::vector<int>> &adj) {
    std::vector<int> color(V, 0);
    for (int i = 0; i < V; ++i) {
        if (color[i] == 0 && dfsCycleDirected(i, adj, color)) return true;
    }
    return false;
}

// ============================================================================
// 3. SẮP XẾP TÔ-PÔ TRÊN ĐỒ THỊ DAG (KAHN'S ALGORITHM: BFS + IN-DEGREE)
// ============================================================================
std::vector<int> topologicalSortKahn(int V, const std::vector<std::vector<int>> &adj) {
    std::vector<int> inDegree(V, 0);
    for (int u = 0; u < V; ++u) {
        for (int v : adj[u]) ++inDegree[v];
    }

    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }

    std::vector<int> topoOrder;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topoOrder.push_back(u);

        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    return topoOrder;
}

int main() {
    std::cout << "=== 1. DUYET DFS & THANH PHAN LIEN THONG ===\n";
    int V = 6;
    std::vector<std::vector<int>> undirected(V);
    auto addUndirected = [&](int u, int v) {
        undirected[u].push_back(v);
        undirected[v].push_back(u);
    };
    addUndirected(0, 1);
    addUndirected(1, 2);
    addUndirected(3, 4); // {0,1,2}, {3,4}, {5} -> 3 thành phần liên thông

    std::vector<bool> visited(V, false);
    int components = 0;
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            ++components;
            std::cout << "  Thanh phan #" << components << ": ";
            dfsRec(i, undirected, visited);
            std::cout << "\n";
        }
    }

    std::cout << "\n=== 2. PHAT HIEN CHU TRINH DO THI CO HUONG (3-COLOR DFS) ===\n";
    std::vector<std::vector<int>> cyclicGraph = {{1}, {2}, {0}};
    std::vector<std::vector<int>> dagGraph = {
        {},       // 0
        {},       // 1
        {3},      // 2 -> 3
        {1},      // 3 -> 1
        {0, 1},   // 4 -> 0, 1
        {2, 0}    // 5 -> 2, 0
    };
    std::cout << "  Do thi (0->1->2->0) co chu trinh? "
              << (hasDirectedCycle(3, cyclicGraph) ? "Co" : "Khong") << "\n";
    std::cout << "  Do thi DAG 6 dinh co chu trinh?   "
              << (hasDirectedCycle(6, dagGraph) ? "Co" : "Khong") << "\n";

    std::cout << "\n=== 3. SAP XEP TO-PO (KAHN'S ALGORITHM) ===\n  Thu tu Topo: ";
    for (int u : topologicalSortKahn(6, dagGraph)) {
        std::cout << u << " ";
    }
    std::cout << "\n";

    return 0;
}

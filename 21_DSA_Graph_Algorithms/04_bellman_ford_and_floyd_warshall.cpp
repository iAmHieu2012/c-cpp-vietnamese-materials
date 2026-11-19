// ============================================================================
// File: 04_bellman_ford_and_floyd_warshall.cpp
// Bài 4.6: Bellman-Ford (Cạnh âm & Chu trình âm) và Floyd-Warshall (Mọi cặp đỉnh)
// ============================================================================

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

const long long INF = std::numeric_limits<long long>::max() / 4;

struct FullEdge {
    int u, v, weight;
};

// ============================================================================
// 1. BELLMAN-FORD ALGORITHM (XỬ LÝ CẠNH ÂM & PHÁT HIỆN CHU TRÌNH ÂM) - O(V * E)
// ============================================================================
void bellmanFord(int V, const std::vector<FullEdge> &edges, int src) {
    std::vector<long long> dist(V, INF);
    dist[src] = 0;

    // Thư giãn toàn bộ E cạnh trong V - 1 lượt
    for (int step = 1; step <= V - 1; ++step) {
        bool changed = false;
        for (const auto &e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
                changed = true;
            }
        }
        if (!changed) break; // Dừng sớm nếu không còn cạnh nào tối ưu hơn
    }

    // Lượt thứ V: Kiểm tra chu trình âm (Negative Weight Cycle)
    for (const auto &e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
            std::cout << "  [Canh bao] Do thi chua CHU TRINH AM!\n";
            return;
        }
    }

    std::cout << "=== 1. BELLMAN-FORD TU DINH " << src
              << " (CO CANH AM - O(V * E)) ===\n";
    for (int i = 0; i < V; ++i) {
        std::cout << "  -> Dinh " << i << ": " << dist[i] << "\n";
    }
}

// ============================================================================
// 2. FLOYD-WARSHALL ALGORITHM (ĐƯỜNG ĐI NGẮN NHẤT MỌI CẶP ĐỈNH) - O(V^3)
// ============================================================================
void floydWarshall(int V, std::vector<std::vector<long long>> dist) {
    for (int k = 0; k < V; ++k) {         // Đỉnh trung gian k
        for (int i = 0; i < V; ++i) {     // Đỉnh xuất phát i
            for (int j = 0; j < V; ++j) { // Đỉnh đích j
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    std::cout << "\n=== 2. FLOYD-WARSHALL (MA TRAN KHOANG CACH MOI CAP DINH - O(V^3)) ===\n";
    for (int i = 0; i < V; ++i) {
        std::cout << "  ";
        for (int j = 0; j < V; ++j) {
            if (dist[i][j] == INF) std::cout << std::setw(6) << "INF";
            else std::cout << std::setw(6) << dist[i][j];
        }
        std::cout << "\n";
    }
}

int main() {
    std::vector<FullEdge> edges = {
        {0, 1, 4},
        {0, 2, 5},
        {1, 2, -3}, // Cạnh có trọng số âm: đi 0 -> 1 -> 2 chỉ tốn 4 + (-3) = 1 < 5
        {2, 3, 4}
    };

    bellmanFord(4, edges, 0);

    std::vector<std::vector<long long>> fwMat(4, std::vector<long long>(4, INF));
    for (int i = 0; i < 4; ++i) fwMat[i][i] = 0;
    for (const auto &e : edges) fwMat[e.u][e.v] = e.weight;

    floydWarshall(4, fwMat);
    return 0;
}

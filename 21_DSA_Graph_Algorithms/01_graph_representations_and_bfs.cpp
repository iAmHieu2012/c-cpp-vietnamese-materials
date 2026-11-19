// ============================================================================
// File: 01_graph_representations_and_bfs.cpp
// Bài 4.6: Biểu diễn Đồ thị (Adjacency Matrix vs List) & Duyệt chiều rộng BFS
// ============================================================================

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

class UndirectedGraph {
    int V_;
    std::vector<std::vector<int>> adjMat_;
    std::vector<std::vector<int>> adjList_;

public:
    explicit UndirectedGraph(int vertices)
        : V_(vertices),
          adjMat_(vertices, std::vector<int>(vertices, 0)),
          adjList_(vertices) {}

    void addEdge(int u, int v) {
        adjMat_[u][v] = adjMat_[v][u] = 1;
        adjList_[u].push_back(v);
        adjList_[v].push_back(u);
    }

    void printRepresentations() const {
        std::cout << "--- 1. Ma tran ke (Adjacency Matrix - O(V^2) RAM) ---\n";
        for (int i = 0; i < V_; ++i) {
            std::cout << "  ";
            for (int j = 0; j < V_; ++j) {
                std::cout << adjMat_[i][j] << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\n--- 2. Danh sach ke (Adjacency List - O(V + E) RAM) ---\n";
        for (int i = 0; i < V_; ++i) {
            std::cout << "  Dinh " << i << ": ";
            for (int neighbor : adjList_[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << "\n";
        }
    }

    // BFS: Duyệt theo chiều rộng + Truy vết đường đi ngắn nhất (ít cạnh nhất)
    void bfsShortestPath(int start, int target) const {
        std::vector<int> dist(V_, -1);
        std::vector<int> parent(V_, -1);
        std::queue<int> q;

        dist[start] = 0;
        q.push(start);

        std::cout << "\n--- 3. Duyet BFS tu dinh " << start << " ---\n  Thu tu tham: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            std::cout << u << " ";

            for (int v : adjList_[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        std::cout << "\n";

        if (dist[target] != -1) {
            std::vector<int> path;
            for (int cur = target; cur != -1; cur = parent[cur]) {
                path.push_back(cur);
            }
            std::reverse(path.begin(), path.end());
            std::cout << "  Duong di ngan nhat (" << start << " -> " << target
                      << ") qua " << dist[target] << " canh: ";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
            }
        }
    }
};

int main() {
    std::cout << "=== BIEU DIEN DO THI & DUYET CHIEU RONG (BFS) ===\n\n";

    UndirectedGraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    g.printRepresentations();
    g.bfsShortestPath(0, 5);

    return 0;
}

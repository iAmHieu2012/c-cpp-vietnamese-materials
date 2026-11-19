#include <iostream>
#include <vector>
#include <queue>
#include <string>

// =============================================================================
// BÀI 1: ĐẾM SỐ HÒN ĐẢO TRÊN LƯỚI 2D (NUMBER OF ISLANDS - DFS O(R * C))
// =============================================================================
void dfs_island(std::vector<std::string> &grid, int r, int c) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] != '1') return;

    grid[r][c] = '0'; // Đánh dấu đã thăm bằng cách nhấn chìm ô đất
    dfs_island(grid, r - 1, c);
    dfs_island(grid, r + 1, c);
    dfs_island(grid, r, c - 1);
    dfs_island(grid, r, c + 1);
}

int count_islands(std::vector<std::string> grid) {
    int islands = 0;
    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[0].size()); ++c) {
            if (grid[r][c] == '1') {
                ++islands;
                dfs_island(grid, r, c);
            }
        }
    }
    return islands;
}


// =============================================================================
// BÀI 2: PHÁT HIỆN CHU TRÌNH ĐỒ THỊ CÓ HƯỚNG (3-COLOR DFS - O(V + E))
// state: 0 = chưa thăm, 1 = đang trên nhánh đệ quy, 2 = đã thăm xong
// =============================================================================
bool dfs_cycle_directed(int u, const std::vector<std::vector<int>> &adj, std::vector<int> &state) {
    state[u] = 1; // Đang duyệt
    for (int v : adj[u]) {
        if (state[v] == 1) return true; // Gặp đỉnh đang nằm trên nhánh hiện tại -> Chu trình!
        if (state[v] == 0 && dfs_cycle_directed(v, adj, state)) return true;
    }
    state[u] = 2; // Duyệt xong
    return false;
}

bool has_directed_cycle(int V, const std::vector<std::vector<int>> &adj) {
    std::vector<int> state(V, 0);
    for (int i = 0; i < V; ++i) {
        if (state[i] == 0 && dfs_cycle_directed(i, adj, state)) return true;
    }
    return false;
}


// =============================================================================
// BÀI 3: SẮP XẾP TOPO (KAHN'S ALGORITHM - BFS & IN-DEGREE - O(V + E))
// =============================================================================
std::vector<int> topological_sort_kahn(int V, const std::vector<std::vector<int>> &adj) {
    std::vector<int> in_degree(V, 0);
    for (int u = 0; u < V; ++u) {
        for (int v : adj[u]) ++in_degree[v];
    }

    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (in_degree[i] == 0) q.push(i);
    }

    std::vector<int> order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : adj[u]) {
            if (--in_degree[v] == 0) q.push(v);
        }
    }

    // Nếu order.size() < V nghĩa là đồ thị có chu trình -> Không tồn tại thứ tự Topo
    if (static_cast<int>(order.size()) != V) return {};
    return order;
}


// =============================================================================
// BÀI 4: KIỂM TRA ĐỒ THỊ 2 PHÍA (BIPARTITE GRAPH - TÔ 2 MÀU BFS - O(V + E))
// =============================================================================
bool is_bipartite(int V, const std::vector<std::vector<int>> &adj) {
    std::vector<int> color(V, -1); // -1: chưa tô, 0 và 1: 2 màu đối lập
    for (int start = 0; start < V; ++start) {
        if (color[start] != -1) continue;

        std::queue<int> q;
        color[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false; // 2 đỉnh kề nhau cùng màu -> Không phải đồ thị 2 phía
                }
            }
        }
    }
    return true;
}


// =============================================================================
// BÀI 5: ĐƯỜNG ĐI NGẮN NHẤT TRONG MÊ CUNG (SHORTEST PATH IN GRID - BFS O(R*C))
// 0 = đường đi, 1 = tường. Tìm số bước ít nhất từ (0, 0) -> (R-1, C-1)
// =============================================================================
int shortest_path_grid(const std::vector<std::vector<int>> &grid) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) return -1;

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int, int>> q;

    dist[0][0] = 0;
    q.push({0, 0});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == rows - 1 && c == cols - 1) return dist[r][c];

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] == 0 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return -1;
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.6 (GRAPH ALGORITHMS) ==========\n";
        std::cout << "1. Dem so hon dao tren luoi 2D (Number of Islands - DFS)\n";
        std::cout << "2. Phat hien chu trinh do thi co huong (3-Color DFS)\n";
        std::cout << "3. Sap xep Topo (Kahn's Algorithm - Course Schedule)\n";
        std::cout << "4. Kiem tra do thi 2 phia (Bipartite Check - BFS)\n";
        std::cout << "5. Duong di ngan nhat trong me cung (Grid BFS)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::vector<std::string> grid = {
                    "11000",
                    "11000",
                    "00100",
                    "00011"
                };
                std::cout << "\n[Bai 1] So hon dao dem duoc = " << count_islands(grid) << "\n";
                break;
            }

            case 2: {
                // 0 -> 1 -> 2 -> 0 (có chu trình)
                std::vector<std::vector<int>> adj1 = {{1}, {2}, {0}};
                // 0 -> 1 -> 2 (DAG, không chu trình)
                std::vector<std::vector<int>> adj2 = {{1}, {2}, {}};
                std::cout << "\n[Bai 2]\n";
                std::cout << "  Do thi 1 (0->1->2->0): " << (has_directed_cycle(3, adj1) ? "Co chu trinh" : "Khong") << "\n";
                std::cout << "  Do thi 2 (0->1->2)   : " << (has_directed_cycle(3, adj2) ? "Co chu trinh" : "Khong") << "\n";
                break;
            }

            case 3: {
                // 6 môn học: 5->2, 5->0, 4->0, 4->1, 2->3, 3->1
                int V = 6;
                std::vector<std::vector<int>> adj(V);
                adj[5] = {2, 0};
                adj[4] = {0, 1};
                adj[2] = {3};
                adj[3] = {1};

                auto order = topological_sort_kahn(V, adj);
                std::cout << "\n[Bai 3] Thu tu hoc mon (Topological Order): ";
                for (int u : order) std::cout << u << " ";
                std::cout << "\n";
                break;
            }

            case 4: {
                // Hình vuông 0-1-2-3-0 (chu trình chẵn -> 2 phía)
                std::vector<std::vector<int>> g1 = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
                // Hình tam giác 0-1-2-0 (chu trình lẻ -> KHÔNG phải 2 phía)
                std::vector<std::vector<int>> g2 = {{1, 2}, {0, 2}, {0, 1}};
                std::cout << "\n[Bai 4]\n";
                std::cout << "  Do thi hinh vuong (4 dinh): " << (is_bipartite(4, g1) ? "La do thi 2 phia" : "Khong") << "\n";
                std::cout << "  Do thi tam giac   (3 dinh): " << (is_bipartite(3, g2) ? "La do thi 2 phia" : "Khong") << "\n";
                break;
            }

            case 5: {
                std::vector<std::vector<int>> maze = {
                    {0, 0, 1, 0},
                    {1, 0, 1, 0},
                    {0, 0, 0, 0},
                    {0, 1, 1, 0}
                };
                std::cout << "\n[Bai 5] So buoc ngan nhat tu (0,0) -> (3,3) = "
                          << shortest_path_grid(maze) << "\n";
                break;
            }

            case 0:
                std::cout << "-> Tam biet!\n";
                break;

            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

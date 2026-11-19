#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iomanip>

const long long INF = std::numeric_limits<long long>::max() / 4;

// =============================================================================
// 1. DISJOINT SET UNION (PATH COMPRESSION + UNION BY RANK) PHỤC VỤ KRUSKAL MST
// =============================================================================
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank_arr;

public:
    explicit DSU(int n) : parent(n), rank_arr(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unite(int u, int v) {
        int ru = find(u), rv = find(v);
        if (ru == rv) return false;
        if (rank_arr[ru] < rank_arr[rv])      parent[ru] = rv;
        else if (rank_arr[ru] > rank_arr[rv]) parent[rv] = ru;
        else { parent[rv] = ru; ++rank_arr[ru]; }
        return true;
    }
};


// =============================================================================
// 2. HỆ THỐNG MẠNG LƯỚI GIAO THÔNG (CITYNETWORK)
// =============================================================================
class CityNetwork {
private:
    struct Edge {
        int to;
        int dist_km;
    };

    struct FullEdge {
        int u, v, dist_km;
        bool operator<(const FullEdge &other) const {
            return dist_km < other.dist_km;
        }
    };

    std::unordered_map<std::string, int> city_to_id;
    std::vector<std::string> id_to_city;
    std::vector<std::vector<Edge>> adj;
    std::vector<FullEdge> all_edges;

    int get_or_create_id(const std::string &name) {
        auto it = city_to_id.find(name);
        if (it != city_to_id.end()) return it->second;
        int new_id = static_cast<int>(id_to_city.size());
        city_to_id[name] = new_id;
        id_to_city.push_back(name);
        adj.emplace_back();
        return new_id;
    }

    void dfs_component(int u, std::vector<bool> &visited, std::vector<int> &comp) const {
        visited[u] = true;
        comp.push_back(u);
        for (const auto &e : adj[u]) {
            if (!visited[e.to]) dfs_component(e.to, visited, comp);
        }
    }

public:
    void add_road(const std::string &city_a, const std::string &city_b, int dist_km) {
        if (dist_km <= 0 || city_a == city_b) return;
        int u = get_or_create_id(city_a);
        int v = get_or_create_id(city_b);
        adj[u].push_back({v, dist_km});
        adj[v].push_back({u, dist_km});
        all_edges.push_back({u, v, dist_km});
    }

    void print_network() const {
        std::cout << "\n=== BAN DO MANG LUOI GIAO THONG (" << id_to_city.size()
                  << " thanh pho, " << all_edges.size() << " tuyen duong) ===\n";
        for (size_t u = 0; u < id_to_city.size(); ++u) {
            std::cout << "  " << std::left << std::setw(12) << id_to_city[u] << " -> ";
            for (size_t i = 0; i < adj[u].size(); ++i) {
                std::cout << id_to_city[adj[u][i].to] << "(" << adj[u][i].dist_km << "km)"
                          << (i + 1 < adj[u].size() ? ", " : "");
            }
            std::cout << "\n";
        }
    }

    // DIJKSTRA: Tìm đường đi ngắn nhất theo tổng km - O((V + E) log V)
    void shortest_path_dijkstra(const std::string &src_name, const std::string &dst_name) const {
        auto it_s = city_to_id.find(src_name);
        auto it_d = city_to_id.find(dst_name);
        if (it_s == city_to_id.end() || it_d == city_to_id.end()) {
            std::cout << "-> Khong tim thay ten thanh pho tren ban do!\n";
            return;
        }

        int V = static_cast<int>(id_to_city.size());
        int src = it_s->second, dst = it_d->second;
        std::vector<long long> dist(V, INF);
        std::vector<int> parent(V, -1);

        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            if (u == dst) break; // Đích đã đạt tối ưu

            for (const auto &e : adj[u]) {
                if (dist[u] + e.dist_km < dist[e.to]) {
                    dist[e.to] = dist[u] + e.dist_km;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        if (dist[dst] == INF) {
            std::cout << "-> Khong co duong di tu " << src_name << " den " << dst_name << "!\n";
            return;
        }

        std::vector<int> path;
        for (int curr = dst; curr != -1; curr = parent[curr]) path.push_back(curr);
        std::reverse(path.begin(), path.end());

        std::cout << "\n[Dijkstra] Lo trinh ngan nhat (" << dist[dst] << " km):\n  ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << id_to_city[path[i]] << (i + 1 < path.size() ? " -> " : "\n");
        }
    }

    // BFS: Tìm đường đi qua ít trạm dừng (thành phố trung gian) nhất - O(V + E)
    void fewest_stops_bfs(const std::string &src_name, const std::string &dst_name) const {
        auto it_s = city_to_id.find(src_name);
        auto it_d = city_to_id.find(dst_name);
        if (it_s == city_to_id.end() || it_d == city_to_id.end()) {
            std::cout << "-> Khong tim thay ten thanh pho tren ban do!\n";
            return;
        }

        int V = static_cast<int>(id_to_city.size());
        int src = it_s->second, dst = it_d->second;
        std::vector<int> hops(V, -1);
        std::vector<int> parent(V, -1);
        std::queue<int> q;

        hops[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == dst) break;

            for (const auto &e : adj[u]) {
                if (hops[e.to] == -1) {
                    hops[e.to] = hops[u] + 1;
                    parent[e.to] = u;
                    q.push(e.to);
                }
            }
        }

        if (hops[dst] == -1) {
            std::cout << "-> Khong co duong di tu " << src_name << " den " << dst_name << "!\n";
            return;
        }

        std::vector<int> path;
        for (int curr = dst; curr != -1; curr = parent[curr]) path.push_back(curr);
        std::reverse(path.begin(), path.end());

        std::cout << "\n[BFS] Lo trinh it chang nhat (" << hops[dst] << " chang):\n  ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << id_to_city[path[i]] << (i + 1 < path.size() ? " -> " : "\n");
        }
    }

    // KRUSKAL'S MST: Quy hoạch mạng lưới kết nối toàn bộ thành phố với tổng km nhỏ nhất - O(E log E)
    void build_optimal_mst() const {
        int V = static_cast<int>(id_to_city.size());
        if (V <= 1) return;

        std::vector<FullEdge> sorted_edges = all_edges;
        std::sort(sorted_edges.begin(), sorted_edges.end());

        DSU dsu(V);
        long long total_km = 0;
        std::vector<FullEdge> mst;

        for (const auto &e : sorted_edges) {
            if (dsu.unite(e.u, e.v)) {
                total_km += e.dist_km;
                mst.push_back(e);
                if (static_cast<int>(mst.size()) == V - 1) break;
            }
        }

        std::cout << "\n=== QUY HOACH MANG LUOI TOI UU (KRUSKAL MST) ===\n";
        for (const auto &e : mst) {
            std::cout << "  " << std::left << std::setw(11) << id_to_city[e.u]
                      << " <---> " << std::setw(11) << id_to_city[e.v]
                      << " | " << e.dist_km << " km\n";
        }
        std::cout << "-> Tong chieu dai tuyen duong toi thieu: " << total_km << " km\n";
        if (static_cast<int>(mst.size()) < V - 1) {
            std::cout << "  [Canh bao] Ban do chua lien thong hoan toan (ket qua la Spanning Forest).\n";
        }
    }

    // DFS: Phân tích tính liên thông của toàn bộ bản đồ - O(V + E)
    void analyze_connectivity() const {
        int V = static_cast<int>(id_to_city.size());
        std::vector<bool> visited(V, false);
        int comp_count = 0;

        std::cout << "\n=== PHAN TICH TINH LIEN THONG (DFS) ===\n";
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                std::vector<int> comp;
                dfs_component(i, visited, comp);
                std::cout << "  Cum #" << ++comp_count << " (" << comp.size() << " thanh pho): ";
                for (size_t k = 0; k < comp.size(); ++k) {
                    std::cout << id_to_city[comp[k]] << (k + 1 < comp.size() ? ", " : "\n");
                }
            }
        }
        if (comp_count == 1) std::cout << "-> Toan bo ban do LIEN THONG hoan toan.\n";
        else                 std::cout << "-> Ban do bi chia cat thanh " << comp_count << " cum roi rac!\n";
    }
};


// =============================================================================
// 3. HÀM MAIN: KHỞI TẠO BẢN ĐỒ MẪU & MENU TƯƠNG TÁC
// =============================================================================
int main() {
    CityNetwork net;

    // Nạp sẵn bản đồ mẫu miền Nam & Nam Trung Bộ
    net.add_road("HCM",       "BienHoa",   30);
    net.add_road("HCM",       "CanTho",    170);
    net.add_road("HCM",       "VungTau",   95);
    net.add_road("BienHoa",   "VungTau",   75);
    net.add_road("BienHoa",   "PhanThiet", 160);
    net.add_road("BienHoa",   "DaLat",     275);
    net.add_road("PhanThiet", "DaLat",     160);
    net.add_road("PhanThiet", "NhaTrang",  240);
    net.add_road("DaLat",     "NhaTrang",  135);

    int choice;
    do {
        std::cout << "\n========= HE THONG TIM DUONG & QUY HOACH (STAGE 4 - DSA) =========\n";
        std::cout << "1. In ban do mang luoi giao thong (Adjacency List)\n";
        std::cout << "2. Them tuyen duong moi (VD: CanTho PhuQuoc 120)\n";
        std::cout << "3. Tim duong di ngan nhat theo km (Dijkstra + Min-Heap)\n";
        std::cout << "4. Tim duong di it tram dung nhat (BFS)\n";
        std::cout << "5. Quy hoach mang luoi duong bo toi uu chi phi (Kruskal MST + DSU)\n";
        std::cout << "6. Kiem tra tinh lien thong ban do (DFS)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==================================================================\n";
        std::cout << "Nhap lua chon (0-6): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                net.print_network();
                break;

            case 2: {
                std::string a, b;
                int km;
                std::cout << "\nNhap <ThanhPhoA> <ThanhPhoB> <KhoangCach_km> (khong dau cach): ";
                std::cin >> a >> b >> km;
                net.add_road(a, b, km);
                std::cout << "-> Da cap nhat tuyen duong " << a << " <-> " << b << " (" << km << " km).\n";
                break;
            }

            case 3: {
                std::string s, d;
                std::cout << "\nNhap <DiemDi> <DiemDen> (VD: HCM NhaTrang): ";
                std::cin >> s >> d;
                net.shortest_path_dijkstra(s, d);
                break;
            }

            case 4: {
                std::string s, d;
                std::cout << "\nNhap <DiemDi> <DiemDen> (VD: HCM NhaTrang): ";
                std::cin >> s >> d;
                net.fewest_stops_bfs(s, d);
                break;
            }

            case 5:
                net.build_optimal_mst();
                break;

            case 6:
                net.analyze_connectivity();
                break;

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
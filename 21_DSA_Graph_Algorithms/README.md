# Bài 4.6: Lý Thuyết Đồ Thị & Giải Thuật Đồ Thị (Graph Algorithms Toàn Tập)

Tài liệu hệ thống hóa 3 phương pháp biểu diễn đồ thị trong bộ nhớ (**Adjacency Matrix**, **Adjacency List**, **Edge List**) và các nhóm giải thuật đồ thị trọng tâm: **Duyệt đồ thị (BFS & DFS)**, **Đường đi ngắn nhất (Dijkstra, Bellman-Ford, Floyd-Warshall)**, **Cây khung nhỏ nhất (Kruskal + DSU & Prim)**, và **Sắp xếp Tô-pô (Topological Sort)**.

---

## 1. Ba Phương Pháp Biểu Diễn Đồ Thị Trong Bộ Nhớ ($V$ Đỉnh, $E$ Cạnh)

| Tiêu chí | 1. Ma trận kề *(Adjacency Matrix)* | 2. Danh sách kề *(Adjacency List)* | 3. Danh sách cạnh *(Edge List)* |
| :--- | :--- | :--- | :--- |
| **Cấu trúc C++** | `vector<vector<int>> mat(V, vector<int>(V, 0))` | `vector<vector<Edge>> adj(V)` | `vector<FullEdge> edges` *(lưu `{u, v, w}`)* |
| **Bộ nhớ (Space)** | $O(V^2)$ *(Tốn RAM nếu đồ thị thưa)* | **$O(V + E)$** *(Tiết kiệm nhất)* | **$O(E)$** |
| **Kiểm tra có cạnh `(u, v)`?** | **$O(1)$** (`mat[u][v] != 0`) | $O(\deg(u))$ | $O(E)$ |
| **Duyệt các đỉnh kề của `u`** | $O(V)$ | **$O(\deg(u))$** *(Cực nhanh)* | $O(E)$ |
| **Thuật toán phù hợp nhất** | Đồ thị dày ($E \approx V^2$), thuật toán **Floyd-Warshall** ($V \le 500$). | **Chuẩn mặc định cho 90% bài toán:** BFS, DFS, Dijkstra, Prim, Topo Sort. | Thuật toán xoay quanh cạnh: **Kruskal's MST**, **Bellman-Ford**. |

---

## 2. Duyệt Đồ Thị: BFS vs DFS (`01_graph_representation_bfs_dfs.cpp`)

```text
  BFS (Loang theo từng lớp sóng đồng tâm — Queue FIFO):       DFS (Đâm sâu tới tận cùng nhánh — Stack / Đệ quy):
       [0] (Lớp 0)                                                 [0] (Bước 1)
      ╱   ╲                                                       ╱   ╲
    [1]   [2] (Lớp 1: cách gốc 1 cạnh)                          [1]   [4] (Bước 5)
    ╱       ╲                                                   ╱
  [3]       [4] (Lớp 2: cách gốc 2 cạnh)                      [2] -> [3] (Bước 2 -> 3 -> 4 rồi quay lui)
```

| Tiêu chí | BFS *(Breadth-First Search — Chiều rộng)* | DFS *(Depth-First Search — Chiều sâu)* |
| :--- | :--- | :--- |
| **Cấu trúc điều khiển** | Hàng đợi **`std::queue` (FIFO)** | Ngăn xếp **Call Stack (Đệ quy)** hoặc `std::stack` |
| **Time / Space Complexity** | Time: **$O(V + E)$** \| Space: **$O(V)$** | Time: **$O(V + E)$** \| Space: **$O(V)$** |
| **Tìm đường ngắn nhất?** | **CÓ** — Đảm bảo đường đi qua **ít cạnh nhất** trên đồ thị không trọng số (hoặc trọng số 0/1 với 0-1 BFS). | **KHÔNG** — Đường tìm thấy đầu tiên của DFS thường không phải là đường ngắn nhất. |
| **Ứng dụng đặc trưng** | Đường ngắn nhất trên lưới mê cung 2D, kiểm tra đồ thị 2 phía (Bipartite), Kahn's Topo Sort. | Đếm thành phần liên thông, phát hiện chu trình (3-Color DFS), tìm Cầu/Khớp (Tarjan), SCC. |

### Phát hiện chu trình trên Đồ thị Có hướng bằng 3-Color DFS

Trên đồ thị có hướng, chỉ dùng mảng `visited[u] = true/false` là **không đủ** để kết luận có chu trình. Ta phải dùng mảng trạng thái **3 màu**:

* **`0` (Trắng - Unvisited):** Đỉnh chưa từng được thăm.
* **`1` (Xám - Visiting):** Đỉnh đang nằm trên nhánh đệ quy hiện tại (chưa hoàn tất). **Nếu từ `u` đi tới đỉnh `v` đang mang màu `1` (Cạnh ngược - Back Edge) $\implies$ Phát hiện Chu trình!**
* **`2` (Đen - Visited):** Đỉnh và toàn bộ nhánh con của nó đã duyệt xong an toàn.

---

## 3. Bài Toán Đường Đi Ngắn Nhất: Dijkstra, Bellman-Ford & Floyd-Warshall

### 3.1. Thuật toán Dijkstra (`02_dijkstra_shortest_path.cpp`) — $O((V + E) \log V)$

* **Mục tiêu:** Tìm đường đi ngắn nhất từ 1 đỉnh nguồn `src` tới mọi đỉnh còn lại.
* **Điều kiện sống còn:** Trọng số tất cả các cạnh phải **không âm ($w \ge 0$)**.
* **Cơ chế Greedy + Min-Heap (`std::priority_queue`):**
  1. Gán `dist[src] = 0`, các đỉnh khác `= INF`, đẩy `{0, src}` vào Min-Heap.
  2. Lấy `{d, u}` ở đỉnh Heap ra. **Kỹ thuật Lazy Deletion:** Kiểm tra `if (d > dist[u]) continue;` để vứt bỏ ngay các bản ghi cũ đã lỗi thời mà không cần hàm `decrease_key`.
  3. **Phép Thư giãn cạnh (Edge Relaxation):** Với mỗi cạnh `(u -> v, w)`, nếu `dist[u] + w < dist[v]` thì cập nhật `dist[v] = dist[u] + w`, lưu vết `parent[v] = u`, và đẩy `{dist[v], v}` vào Min-Heap.

### 3.2. Khi nào dùng Bellman-Ford và Floyd-Warshall?

| Thuật toán | Bài toán giải quyết | Độ phức tạp | Xử lý cạnh âm ($w < 0$)? | Cơ chế cốt lõi |
| :--- | :--- | :---: | :---: | :--- |
| **Dijkstra** *(Min-Heap)* | Từ **1 nguồn** (`src`) tới mọi đỉnh | **$O((V + E) \log V)$** | **KHÔNG** *(Sai nếu có cạnh âm)* | Tham lam rút đỉnh có `dist` nhỏ nhất từ Min-Heap và thư giãn cạnh kề. |
| **Bellman-Ford** | Từ **1 nguồn** (`src`) tới mọi đỉnh | $O(V \times E)$ | **CÓ** + Phát hiện **Chu trình âm** | Lặp $V - 1$ lần qua toàn bộ danh sách $E$ cạnh để thư giãn. Nếu ở lần thứ $V$ vẫn giảm được `dist` $\rightarrow$ có Chu trình âm! |
| **Floyd-Warshall** | Giữa **mọi cặp đỉnh** `(i, j)` | $O(V^3)$ Time $O(V^2)$ Space | **CÓ** *(Không có chu trình âm)* | Quy hoạch động 3 vòng lặp: thử lần lượt từng đỉnh trung gian `k` (`0..V-1`): `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);` |

---

## 4. Cây Khung Nhỏ Nhất (Minimum Spanning Tree — MST): Kruskal & Prim (`03_dsu_and_kruskal_mst.cpp`)

Cho đồ thị vô hướng, liên thông, có trọng số. **Cây khung nhỏ nhất (MST)** là tập hợp gồm đúng **$V - 1$ cạnh** nối thông toàn bộ $V$ đỉnh sao cho không tạo chu trình và có **tổng trọng số nhỏ nhất**.

### 4.1. Cấu trúc Disjoint Set Union (DSU / Union-Find) — $O(\alpha(V)) \approx O(1)$

DSU quản lý các tập hợp rời nhau cực nhanh nhờ kết hợp 2 kỹ thuật tối ưu:

```text
  Path Compression (Nén đường đi trong hàm find):     Union by Rank (Gộp cây thấp vào dưới cây cao):
       [0]                     [0]                           [0] (Rank 1)    [3] (Rank 0)
        │      find(2)       ╱  │  ╲                          │      +             ===>   Gắn [3] làm con
       [1]   ==========►   [1] [2] [3]                       [1]                          trực tiếp của [0]
        │     (Tất cả trỏ thẳng về gốc [0]!)
       [2]
```

### 4.2. So sánh Kruskal's Algorithm vs Prim's Algorithm

| Tiêu chí | Thuật toán Kruskal *(Dựa trên Cạnh + DSU)* | Thuật toán Prim *(Dựa trên Đỉnh + Min-Heap)* |
| :--- | :--- | :--- |
| **Ý tưởng chính** | Sắp xếp toàn bộ $E$ cạnh tăng dần theo trọng số. Duyệt từ cạnh nhỏ nhất, nếu 2 đầu mút `u, v` chưa cùng tập hợp (`dsu.unite(u, v) == true`) thì đưa vào MST. | Bắt đầu từ 1 đỉnh bất kỳ, dùng Min-Heap luôn chọn cạnh ngắn nhất nối từ tập đỉnh đã thuộc MST sang một đỉnh mới bên ngoài (tương tự Dijkstra). |
| **Độ phức tạp** | **$O(E \log E)$** (hoặc $O(E \log V)$) | **$O((V + E) \log V)$** (hoặc $O(V^2)$ nếu không dùng Heap) |
| **Khi nào tối ưu?** | Đồ thị thưa ($E \approx V$), cài đặt cực gọn khi đã có sẵn danh sách cạnh và DSU. | Đồ thị dày ($E \approx V^2$), hoặc khi đồ thị chỉ cho dưới dạng Danh sách kề / Ma trận kề. |

---

## 5. Sắp Xếp Tô-Pô (Topological Sort trên Đồ Thị DAG)

* **Điều kiện bắt buộc:** Đồ thị phải là **DAG (Directed Acyclic Graph — Đồ thị có hướng không có chu trình)**. Ứng dụng: Lập lịch thứ tự học các môn tiên quyết, thứ tự biên dịch các thư viện (Makefile/CMake).
* **Thuật toán Kahn (BFS + Bậc vào `in_degree`) — $O(V + E)$:**
  1. Tính `in_degree[v]` (số cung đi vào đỉnh `v`) cho mọi đỉnh. Đẩy tất cả các đỉnh có `in_degree == 0` (không phụ thuộc ai) vào `std::queue`.
  2. Khi rút đỉnh `u` khỏi Queue đưa vào kết quả Topo, duyệt mọi đỉnh kề `v` của `u`, giảm `--in_degree[v]`. Nếu `in_degree[v] == 0` thì đẩy `v` vào Queue.
  3. **Kiểm tra chu trình tự động:** Nếu sau khi Queue rỗng mà số đỉnh lấy ra **`< V`**, chứng tỏ đồ thị **có chu trình** (không tồn tại thứ tự Topo)!

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_graph_representations_and_bfs.cpp`: So sánh **Adjacency Matrix vs Adjacency List**, duyệt chiều rộng **BFS** và truy vết đường đi ngắn nhất không trọng số.
* `02_dfs_cycle_and_topological_sort.cpp`: Duyệt chiều sâu **DFS**, đếm thành phần liên thông, **Phát hiện chu trình 3 màu** và **Sắp xếp Tô-pô (Kahn's Algorithm)**.
* `03_dijkstra_shortest_path.cpp`: Thuật toán **Dijkstra** với Min-Heap (`std::priority_queue`), Lazy Deletion & truy vết đường đi tối ưu.
* `04_bellman_ford_and_floyd_warshall.cpp`: Thuật toán **Bellman-Ford** (xử lý cạnh trọng số âm & phát hiện chu trình âm) và **Floyd-Warshall** (đường đi ngắn nhất mọi cặp đỉnh $O(V^3)$).
* `05_dsu_and_kruskal_mst.cpp`: **Disjoint Set Union (DSU)** (Path Compression + Union by Rank) & Cây khung nhỏ nhất (**Kruskal & Prim**).
* `06_practice_exercises.cpp`: 5 bài tập kinh điển (Number of Islands trên lưới 2D, 3-Color DFS Cycle Detection, Kahn's Topological Sort, Bipartite Graph Check 2-Coloring, Shortest Path in Binary Grid).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_graph_representations_and_bfs.cpp -o 01_graph_representations_and_bfs
./01_graph_representations_and_bfs

g++ -Wall -Wextra -std=c++17 02_dfs_cycle_and_topological_sort.cpp -o 02_dfs_cycle_and_topological_sort
./02_dfs_cycle_and_topological_sort

g++ -Wall -Wextra -std=c++17 03_dijkstra_shortest_path.cpp -o 03_dijkstra_shortest_path
./03_dijkstra_shortest_path

g++ -Wall -Wextra -std=c++17 04_bellman_ford_and_floyd_warshall.cpp -o 04_bellman_ford_and_floyd_warshall
./04_bellman_ford_and_floyd_warshall

g++ -Wall -Wextra -std=c++17 05_dsu_and_kruskal_mst.cpp -o 05_dsu_and_kruskal_mst
./05_dsu_and_kruskal_mst

g++ -Wall -Wextra -std=c++17 06_practice_exercises.cpp -o 06_practice_exercises
./06_practice_exercises
```

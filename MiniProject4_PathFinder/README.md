# Mini-Project (Stage 4): Hệ Thống Tìm Đường & Quy Hoạch Mạng Lưới Giao Thông (DSA)

Dự án tổng kết **Giai đoạn 4 (Cấu trúc dữ liệu & Giải thuật — DSA)**. Hệ thống mô phỏng bản đồ kết nối giao thông liên tỉnh dưới dạng **Đồ thị vô hướng có trọng số (Weighted Undirected Graph)**, kết hợp **Bảng băm (`std::unordered_map`)**, **Danh sách kề (Adjacency List)**, **Hàng đợi ưu tiên (Min-Heap)** và **Disjoint Set Union (DSU)** để giải quyết 4 bài toán thực tế: Tìm lộ trình ngắn nhất theo km (**Dijkstra**), Tìm lộ trình qua ít trạm trung chuyển nhất (**BFS**), Quy hoạch mạng lưới đường bộ tối thiểu hóa chi phí xây dựng (**Kruskal's MST**) và Phân tích vùng liên thông (**DFS**).

---

## 1. Cấu Trúc Thư Mục & Lệnh Biên Dịch

```text
MiniProject4_PathFinder/
├── README.md         <-- Tài liệu kiến trúc dữ liệu, sơ đồ bản đồ mẫu & phân tích Big-O
└── path_finder.cpp   <-- Mã nguồn chính (C++17) tích hợp sẵn bản đồ mẫu 6 thành phố, 9 tuyến đường
```

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 path_finder.cpp -o path_finder
./path_finder
```

---

## 2. Sơ Đồ Bản Đồ Giao Thông Mẫu Tích Hợp Trong Hệ Thống

Khi khởi động, hệ thống nạp sẵn mạng lưới giao thông khu vực Miền Nam & Nam Trung Bộ gồm **6 thành phố** và **9 tuyến đường hai chiều**:

```text
                                     [DaLat]
                                    ╱   │   ╲
                          275 km  ╱     │     ╲ 135 km
                                ╱       │       ╲
         170 km               ╱  160 km │         ▼
  [CanTho] ─── [HCM] ─── [BienHoa]      │     [NhaTrang]
                 ╲          │    ╲      │         ▲
            95 km  ╲  75 km │      ╲    │         │ 240 km
                     ╲      │ 160 km ╲  │         │
                     [VungTau]       [PhanThiet] ─┘

  • Ví dụ so sánh Dijkstra vs BFS khi tìm đường từ [HCM] đi [NhaTrang]:
    - BFS (Ít trạm dừng nhất - 2 chặng)  : HCM -> BienHoa -> PhanThiet -> NhaTrang (30 + 160 + 240 = 430 km)
                                           hoặc HCM -> BienHoa -> DaLat -> NhaTrang (30 + 275 + 135 = 440 km)
    - Dijkstra (Tổng km ngắn nhất)       : HCM -> BienHoa -> PhanThiet -> NhaTrang (430 km)
    - Kruskal MST (Quy hoạch 5 tuyến tối ưu nối thông cả 6 thành phố):
      (HCM-BienHoa: 30) + (BienHoa-VungTau: 75) + (DaLat-NhaTrang: 135)
      + (BienHoa-PhanThiet: 160) + (PhanThiet-DaLat: 160) + (HCM-CanTho: 170) ===> Tổng: 730 km!
```

---

## 3. Các Tính Năng Chính Của Hệ Thống

| Phím | Chức năng | Mô tả chi tiết nghiệp vụ & Giải thuật vận hành bên dưới |
| :-: | :--- | :--- |
| **`1`** | **In bản đồ mạng lưới** | Duyệt Danh sách kề (`adj`), hiển thị toàn bộ thành phố và các tuyến đường kết nối trực tiếp kèm cự ly (km). |
| **`2`** | **Thêm tuyến đường mới** | Nhập `<ThànhPhốA> <ThànhPhốB> <Số_km>`. Tự động cấp ID mới trong $O(1)$ qua Hash Map nếu là thành phố mới (ví dụ thêm đảo `PhuQuoc`) và cập nhật đồ thị 2 chiều. |
| **`3`** | **Tìm đường ngắn nhất (km)** | Chạy thuật toán **Dijkstra + Min-Heap**, in ra tổng quãng đường km tối ưu và truy vết ngược mảng `parent[]` để hiển thị chi tiết từng chặng từ điểm đi đến điểm đến. |
| **`4`** | **Tìm đường ít trạm nhất** | Chạy thuật toán **BFS (`std::queue`)**, tìm lộ trình đi qua ít thành phố trung gian nhất (bỏ qua khoảng cách km). |
| **`5`** | **Quy hoạch hạ tầng tối ưu** | Chạy thuật toán **Kruskal's MST + DSU**, chọn ra bộ $V - 1$ tuyến đường kết nối toàn bộ các thành phố với tổng số km phải trải nhựa là nhỏ nhất (cảnh báo nếu có vùng đảo bị cô lập). |
| **`6`** | **Phân tích tính liên thông** | Chạy **DFS**, liệt kê từng cụm thành phố liên thông và kiểm tra toàn bộ bản đồ đã thông suốt hay bị chia cắt thành nhiều cụm rời rạc. |
| **`0`** | **Thoát chương trình** | Kết thúc vòng lặp điều khiển và tự động giải phóng bộ nhớ. |

---

## 4. Kiến Trúc Cấu Trúc Dữ Liệu & Độ Phức Tạp Thuật Toán (Tổng Hợp Giai Đoạn 4)

### 4.1. Kỹ thuật Ánh xạ Song ánh `Tên Thành Phố (String) <-> Chỉ số Đỉnh (int 0..V-1)`

Nếu dùng trực tiếp `std::string` làm khóa của Danh sách kề (`unordered_map<string, vector<...>>`), mọi thao tác trong vòng lặp của Dijkstra, BFS, DFS và DSU đều phải băm chuỗi nhiều lần rất tốn kém. Hệ thống áp dụng kỹ thuật **Chuẩn hóa đỉnh về số nguyên liên tiếp `0..V-1`**:

```text
  city_to_id (unordered_map<string, int>)         id_to_city (vector<string>)       adj (vector<vector<Edge>>)
  ┌───────────────────┐                           ┌───────────────────┐             ┌──────────────────────────────┐
  │ "HCM"       -> 0  │   Tra cứu xuôi O(1) avg   │ [0] = "HCM"       │   Chỉ số    │ [0] -> {(1, 30), (2, 170)...}│
  │ "BienHoa"   -> 1  │ ◄───────────────────────► │ [1] = "BienHoa"   │ ──────────► │ [1] -> {(0, 30), (3, 75)...} │
  │ "CanTho"    -> 2  │   Tra cứu ngược O(1)      │ [2] = "CanTho"    │   mảng O(1) │ [2] -> {(0, 170)}            │
  └───────────────────┘                           └───────────────────┘             └──────────────────────────────┘
```

### 4.2. Bảng phân tích Big-O của từng Module trong `path_finder.cpp`

| Module nghiệp vụ | Cấu trúc dữ liệu cốt lõi | Time Complexity | Space Complexity | Điểm kỹ thuật nổi bật trong code |
| :--- | :--- | :---: | :---: | :--- |
| **`get_or_create_id()`** | `unordered_map` + `vector` | **$O(1)$** *(avg)* | $O(V)$ | Tự động mở rộng đồ thị động khi người dùng thêm thành phố mới lúc chạy. |
| **`shortest_path_dijkstra()`** | Adjacency List + Min-Heap (`priority_queue`) | **$O((V + E) \log V)$** | $O(V)$ | Dùng **Lazy Deletion** (`if (d > dist[u]) continue;`), dừng sớm ngay khi rút trúng đích `u == dst`, truy vết qua `parent[]`. |
| **`fewest_stops_bfs()`** | Adjacency List + `std::queue` | **$O(V + E)$** | $O(V)$ | Mảng `hops[]` đóng đồng thời 2 vai trò: đánh dấu `visited` (`!= -1`) và lưu số chặng ngắn nhất. |
| **`build_optimal_mst()`** | Edge List + `std::sort` + Class `DSU` | **$O(E \log E)$** | $O(V + E)$ | `DSU` cài đặt cả **Path Compression** lẫn **Union by Rank** đạt $O(\alpha(V)) \approx O(1)$, dừng sớm ngay khi đủ $V - 1$ cạnh. |
| **`analyze_connectivity()`** | Adjacency List + Đệ quy DFS | **$O(V + E)$** | $O(V)$ | Phân tách chính xác từng thành phần liên thông (Connected Components) khi bản đồ có các cụm thành phố/đảo tách rời. |

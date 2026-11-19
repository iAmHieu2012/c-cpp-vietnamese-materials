# Bài 5.3: Associative Containers, Unordered Containers & Container Adapters

Tài liệu hệ thống hóa kiến trúc bên dưới, độ phức tạp, cách thiết kế **Functors (Custom Comparators & Custom Hash)** và các cạm bẫy thực chiến của 3 nhóm cấu trúc STL: **Ordered Associative** (`set`, `multiset`, `map`, `multimap`), **Unordered Hash Containers** (`unordered_set`, `unordered_map`) và **Container Adapters** (`stack`, `queue`, `priority_queue`).

---

## 1. Bảng Đối Chiếu Toàn Diện: Ordered (Red-Black Tree) vs Unordered (Hash Table)

```text
  Ordered (std::set / std::map — Red-Black Tree)         Unordered (std::unordered_set / map — Hash Buckets)
              [Key: 20] (Đen)                            Buckets:
              ╱             ╲                            [0] ──► nullptr
      [Key: 10] (Đỏ)    [Key: 30] (Đỏ)                   [1] ──► [Key: 25] ──► [Key: 10] ──► nullptr
  • Luôn sắp xếp tăng dần theo Key (In-order)            [2] ──► [Key: 30] ──► nullptr
  • Hỗ trợ lower_bound / upper_bound trong O(log N)      • Không có thứ tự, tra cứu trung bình O(1)!
```

| Tiêu chí | `std::set` / `std::map` | `std::multiset` / `std::multimap` | `std::unordered_set` / `std::unordered_map` |
| :--- | :---: | :---: | :---: |
| **Cấu trúc lõi bên dưới** | **Red-Black Tree** *(Cây tự cân bằng)* | **Red-Black Tree** | **Hash Table** *(Separate Chaining)* |
| **Cho phép trùng Key?** | **Không** *(Key là duy nhất)* | **Có** *(Lưu nhiều bản sao cùng Key)* | **Không** *(Dùng `unordered_multiset/map` nếu cần)* |
| **Thứ tự các phần tử** | Tự động **sắp xếp tăng dần** (`std::less`) | Tự động **sắp xếp tăng dần** | **Hoàn toàn lộn xộn** (phụ thuộc hàm băm) |
| **Insert / Erase / Find** | **$O(\log N)$** *(Cam kết mọi trường hợp)* | **$O(\log N)$** | **$O(1)$** trung bình \| **$O(N)$** Worst-case |
| **Truy vấn đoạn `[L, R]`** | **Có** (`lower_bound`, `upper_bound`) | **Có** (`lower_bound`, `equal_range`) | **Không hỗ trợ** |
| **Hỗ trợ `operator[]`?** | Chỉ có ở **`std::map`** | **Không** *(vì 1 Key có nhiều Value)* | Chỉ có ở **`std::unordered_map`** |
| **Độ ổn định Iterator** | Không bao giờ hỏng khi `insert` thêm | Không bao giờ hỏng khi `insert` thêm | **Hỏng toàn bộ Iterator** nếu xảy ra **Rehash**! |

---

## 2. Ba "Cạm Bẫy" Thực Chiến Kinh Điển Với Associative & Unordered Containers

### 2.1. Cạm bẫy tự động tạo Key rác của `map::operator[]` (`01_ordered_set_and_map.cpp`)

* Khi bạn viết `if (scores["Dung"] > 5)` để kiểm tra xem `"Dung"` có điểm lớn hơn 5 không: nếu `"Dung"` **chưa hề tồn tại**, toán tử `[]` sẽ **âm thầm chèn một cặp mới `{"Dung", 0}` vào map**, làm tăng `.size()` và làm bẩn dữ liệu! Ngoài ra `operator[]` không thể gọi trên một `const std::map`.
* **Quy tắc chuẩn công nghiệp:**
  * Chỉ dùng `mp[key] = val` hoặc `++mp[key]` khi bạn **chủ đích muốn ghi/chèn** dữ liệu.
  * Khi **chỉ muốn kiểm tra tồn tại hoặc đọc dữ liệu**, bắt buộc dùng **`mp.find(key)`**:

    ```cpp
    if (auto it = scores.find("Dung"); it != scores.end()) {
        std::cout << "Diem: " << it->second << "\n"; // Truy cập an toàn tuyệt đối, không tạo Key rác!
    }
    ```

### 2.2. Cạm bẫy xóa nhầm toàn bộ bản sao của `multiset::erase(val)` & Cách duyệt `multimap`

* Trong `std::multiset<int> ms = {10, 20, 20, 20, 30};`:
  * Gọi `ms.erase(20);` (truyền **giá trị**) sẽ **xóa sạch cả 3 số `20`**!
  * Muốn xóa **đúng 1 số `20`**, phải truyền **Iterator**: `if (auto it = ms.find(20); it != ms.end()) ms.erase(it);`.
* Trong `std::multimap<Key, Val> mmp`: Vì không có `mmp[key]`, để lấy tất cả các giá trị có cùng một `key`, ta dùng **`auto [first, last] = mmp.equal_range(key);`** rồi lặp từ `first` đến `last` trong $O(\log N + K)$.

### 2.3. Cạm bẫy gọi hàm toàn cục `std::lower_bound(s.begin(), s.end(), x)` trên `set` / `map`

* Iterator của `std::set` và `std::map` là **Bidirectional Iterator** (không phải Random Access).
* Nếu gọi hàm thuật toán bên ngoài `std::lower_bound(s.begin(), s.end(), x)`, nó phải đi bộ từng bước tốn **$O(N)$**!
* **Bắt buộc phải gọi phương thức thành viên** **`s.lower_bound(x)`** hoặc **`mp.lower_bound(x)`** để tận dụng cấu trúc cây Red-Black chạy trong **$O(\log N)$**!

---

## 3. Container Adapters (`std::stack`, `std::queue`, `std::priority_queue`) (`03_container_adapters.cpp`)

**Container Adapters** không phải là cấu trúc lưu trữ mới, mà là lớp vỏ bọc (**Wrapper**) khóa bớt giao diện của một Sequence Container nền bên dưới để ép tuân thủ nghiêm ngặt quy tắc LIFO, FIFO hoặc Heap. **(Tất cả Adapters đều KHÔNG có Iterator — không thể duyệt bằng vòng lặp `for`!)**.

| Adapter | Container nền mặc định | Có thể đổi sang nền nào? | Bộ hàm giao diện duy nhất ($O(1)$ / $O(\log N)$) |
| :--- | :---: | :---: | :--- |
| **`std::stack<T>`** | `std::deque<T>` | `vector`, `list` | `push(x)`, `emplace(args)`, `pop()`, `top()`, `size()`, `empty()` — Đều **$O(1)$** |
| **`std::queue<T>`** | `std::deque<T>` | `list` *(Không dùng được `vector`!)* | `push(x)`, `pop()`, `front()`, `back()`, `size()`, `empty()` — Đều **$O(1)$** |
| **`std::priority_queue<T>`** | `std::vector<T>` | `deque` *(Cần Random Access)* | `top()` — **$O(1)$**; `push(x)`, `pop()` — **$O(\log N)$** |

### Khai báo Max-Heap (Mặc định) vs Min-Heap trong `std::priority_queue`

* Mặc định `std::priority_queue<int>` là **Max-Heap** (phần tử lớn nhất ở đỉnh `top()`).
* Muốn tạo **Min-Heap** (phần tử nhỏ nhất ở đỉnh `top()`):
  ```cpp
  std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
  ```
  *(Cách viết bộ so sánh tùy chỉnh nâng cao cho `priority_queue` và `set`/`sort` sẽ được phân tích chuyên sâu ở **Bài 5.5 — Functors**).*

---

## 4. Dùng Custom Struct Làm Key Cho `std::unordered_map` / `std::unordered_set` (`02_unordered_hash_containers.cpp`)

Để đưa một `struct Point { int x, y; }` vào bảng băm STL, bạn bắt buộc phải cung cấp đủ **2 thứ**: (1) Toán tử so sánh bằng `operator==` (để xử lý khi đụng độ mã băm) và (2) Một hàm băm tùy chỉnh:

```cpp
struct Point {
    int x, y;
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
};

struct PointHash {
    size_t operator()(const Point &p) const noexcept {
        size_t h1 = std::hash<int>{}(p.x);
        size_t h2 = std::hash<int>{}(p.y);
        // Công thức trộn bit chuẩn Boost (hash_combine) chống đụng độ khi (x, y) đảo vị trí như (1, 2) và (2, 1):
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

// Khai báo sử dụng:
std::unordered_map<Point, std::string, PointHash> grid_map;
```

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_set_and_multiset.cpp`: **`std::set` & `std::multiset`** (Red-Black Tree, `lower_bound`/`upper_bound`, xóa đúng 1 bản sao bằng Iterator, Custom Comparator).
* `02_map_and_multimap.cpp`: **`std::map` & `std::multimap`** (So sánh `operator[]` vs `find()` vs `try_emplace`, truy vấn `equal_range` trên `multimap`).
* `03_unordered_hash_containers.cpp`: **`std::unordered_map` & `std::unordered_set`** (`reserve()`, Load Factor và Custom Hash Functor `hash_combine` cho `struct`).
* `04_container_adapters.cpp`: **Container Adapters** (`std::stack`, `std::queue`, `std::priority_queue` Max-Heap, Min-Heap và Custom Functor Comparator).
* `05_practice_exercises.cpp`: 5 bài tập thực chiến (Group Anagrams, Top K Frequent Words bằng Min-Heap, LRU Cache $O(1)$ kết hợp `list::splice` + `unordered_map`, Median from Data Stream bằng 2 Heaps, Range Count & Closest Element trên `set`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_set_and_multiset.cpp -o 01_set_and_multiset
./01_set_and_multiset

g++ -Wall -Wextra -std=c++17 02_map_and_multimap.cpp -o 02_map_and_multimap
./02_map_and_multimap

g++ -Wall -Wextra -std=c++17 03_unordered_hash_containers.cpp -o 03_unordered_hash_containers
./03_unordered_hash_containers

g++ -Wall -Wextra -std=c++17 04_container_adapters.cpp -o 04_container_adapters
./04_container_adapters

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

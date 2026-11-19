# Bài 5.2: Sequence Containers (`std::array`, `std::vector`, `std::deque`, `std::list`, `std::forward_list`) & Iterators

Tài liệu hệ thống hóa kiến trúc bộ nhớ dưới thanh RAM của 5 **Sequence Containers** trong C++ STL, phân cấp 5 loại **Iterators**, và quy tắc phòng chống lỗi **Iterator Invalidation**.

---

## 1. Kiến Trúc Bộ Nhớ & Bảng So Sánh 5 Sequence Containers

```text
  1. std::vector<T> (1 khối liền kề trên Heap — Cache Locality tốt nhất, mở rộng bằng cách nhân đôi):
     [ 10 | 20 | 30 | 40 | (Trống) | (Trống) ]  (|<-- size = 4 -->|  |<-- capacity = 6 -->|)

  2. std::deque<T> (Mảng con trỏ Map quản lý nhiều khối Chunks cố định — Chèn 2 đầu O(1) + Random Access O(1)):
     Map: [ * ]───► Chunk 0: [ .. | .. | 10 | 20 ]  (Mở rộng thêm đầu trước mà KHÔNG cần copy dữ liệu cũ!)
          [ * ]───► Chunk 1: [ 30 | 40 | 50 | .. ]  (Mở rộng thêm đầu sau)

  3. std::list<T> (Doubly Linked List — Các Node nằm rải rác trên Heap, nối 2 chiều prev <-> next):
     nullptr ◄── [prev | 10 | next] ◄──► [prev | 20 | next] ◄──► [prev | 30 | next] ──► nullptr
```

| Tiêu chí | `std::array<T, N>` | `std::vector<T>` | `std::deque<T>` | `std::list<T>` | `std::forward_list<T>` |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Bản chất cấu trúc** | Mảng tĩnh cố định trên **Stack** | Mảng động liền kề trên **Heap** | Nhiều khối Chunks trên **Heap** | Doubly Linked List (**Heap**) | Singly Linked List (**Heap**) |
| **Truy cập `c[i]` / `.at(i)`** | **$O(1)$** | **$O(1)$** | **$O(1)$** | Không hỗ trợ ($O(N)$) | Không hỗ trợ ($O(N)$) |
| **Chèn / Xóa ở Đầu** | Không hỗ trợ | $O(N)$ | **$O(1)$** | **$O(1)$** | **$O(1)$** |
| **Chèn / Xóa ở Cuối** | Không hỗ trợ | **$O(1)$** *(Amortized)* | **$O(1)$** | **$O(1)$** | $O(N)$ *(Không có `push_back`)* |
| **Chèn / Xóa ở Giữa** *(khi đã có Iterator)* | Không hỗ trợ | $O(N)$ | $O(N)$ | **$O(1)$** | **$O(1)$** (`insert_after`) |
| **Chi phí bộ nhớ phụ** | `0 bytes` | `0 bytes`/phần tử | Rất nhỏ (bảng Map) | **+16 bytes/Node** | **+8 bytes/Node** *(Không có `.size()`)* |
| **Cache Locality** | **Xuất sắc** | **Xuất sắc** | Tốt | Kém (Cache Miss) | Kém (Cache Miss) |

---

## 2. Kỹ Thuật Tối Ưu Hóa Từng Container

### 2.1. `std::array` vs `std::vector` (`01_vector_and_array.cpp`)

* **`std::array<T, N>`:** Là lớp vỏ bọc $0$-overhead quanh mảng tĩnh `T a[N]`. Khắc phục hoàn toàn nhược điểm **Array Decay** (không bị thoái hóa thành con trỏ khi truyền vào hàm), biết rõ `.size()`, hỗ trợ `.at(i)` có kiểm tra tràn mảng (ném `std::out_of_range`).
* **3 Quy tắc tối ưu hiệu năng cho `std::vector<T>`:**
  1. **`v.reserve(N)`:** Nếu ước lượng được số phần tử khoảng $N$, luôn gọi `reserve(N)` trước vòng lặp để cấp phát Heap đúng 1 lần duy nhất, loại bỏ hoàn toàn chi phí cấp phát lại và copy dữ liệu nhiều lần.
  2. **`v.emplace_back(args...)` vs `v.push_back(obj)`:** `emplace_back` chuyển tiếp thẳng đối số xuống khởi tạo đối tượng **ngay tại ô nhớ cuối của vector** (0 Copy, 0 Move đối tượng tạm).
  3. **Thu hồi bộ nhớ thừa:** `v.clear()` chỉ đưa `size = 0` nhưng **`capacity` trên Heap vẫn giữ nguyên**. Muốn trả vùng nhớ thừa về cho Hệ điều hành, phải gọi thêm `v.shrink_to_fit();`.

### 2.2. `std::deque`, `std::list` & `std::forward_list` (`02_deque_and_list.cpp`)

* **`std::deque<T>`:** Tuyệt vời khi cần vừa đẩy/rút ở cả 2 đầu (`push_front`, `push_back`) trong $O(1)$ vừa cần truy cập chỉ số `dq[i]` trong $O(1)$ (như thuật toán Sliding Window Maximum). Không có `.reserve()` hay `.capacity()` vì bộ nhớ chia theo từng chunk.
* **`std::list<T>`:** Điểm mạnh độc quyền là phương thức **`lst1.splice(pos, lst2, it)`** — bứt trực tiếp 1 node (hoặc cả đoạn node) từ danh sách này gắn sang danh sách khác trong **$O(1)$** chỉ bằng cách nối lại con trỏ mà không copy dữ liệu (cực kỳ quan trọng khi cài đặt **LRU Cache $O(1)$**).
* **`std::forward_list<T>` (C++11):** Danh sách liên kết đơn siêu tiết kiệm RAM (chỉ tốn 1 con trỏ `next` mỗi node, thậm chí không lưu biến `size` nên không có hàm `.size()`), mọi thao tác chèn/xóa đều thực hiện phía sau node hiện tại (`insert_after`, `erase_after`).

---

## 3. Phân Cấp Iterators & Quy Tắc Chống Iterator Invalidation (`03_iterators_and_invalidation.cpp`)

### 3.1. Phân loại Iterators và các cặp hàm lấy Iterator

```text
  Mảng / Vector:     [  10  |  20  |  30  |  40  ]
                 ▲      ▲                    ▲      ▲
              rend()  begin()             rbegin() end()  (Lưu ý: end() và rend() trỏ RA NGOÀI phần tử cuối/đầu!)
```

* **3 nhóm Iterator phổ biến nhất trong STL:**
  1. **Random Access Iterator** (`vector`, `array`, `deque`, `string`): Mạnh nhất — hỗ trợ nhảy cóc `it + k`, `it - k`, lấy khoảng cách `it2 - it1` trong $O(1)$, so sánh `<`, `>` $\implies$ **Dùng được với `std::sort` và `std::binary_search`**.
  2. **Bidirectional Iterator** (`list`, `set`, `map`, `multiset`, `multimap`): Chỉ tiến `++it` hoặc lùi `--it` từng bước 1 (muốn nhảy $k$ bước phải dùng `std::advance(it, k)` trong $O(k)$). Không dùng được `std::sort(begin, end)` (riêng `std::list` có hàm thành viên `lst.sort()` riêng).
  3. **Forward Iterator** (`forward_list`, `unordered_set`, `unordered_map`): Chỉ đi tiến 1 chiều `++it`, không có `--it`.

### 3.2. Bảng quy tắc Iterator Invalidation (Trọng tâm tránh lỗi Crash / Undefined Behavior)

| Container | Khi Thêm phần tử (`push` / `insert`) | Khi Xóa phần tử (`pop` / `erase`) |
| :--- | :--- | :--- |
| **`std::vector`** | Nếu `size == capacity` (bị Reallocate): **Hỏng TOÀN BỘ** iterator, con trỏ, tham chiếu!, Nếu chưa vượt `capacity`: Chỉ hỏng các iterator từ vị trí `insert` về cuối. | Làm hỏng mọi iterator, con trỏ, tham chiếu **từ vị trí bị xóa cho tới `end()`**. |
| **`std::deque`** | `push_front` / `push_back` làm **hỏng mọi iterator** (nhưng con trỏ/tham chiếu tới phần tử cũ vẫn sống!). `insert` ở giữa làm hỏng tất cả. | Xóa ở 2 đầu chỉ làm hỏng iterator tại đầu đó; xóa ở giữa làm **hỏng toàn bộ**. |
| **`std::list` / `forward_list`** | **An toàn 100%** — Không có bất kỳ iterator hay tham chiếu nào bị hỏng! | **Chỉ làm hỏng duy nhất iterator trỏ vào node bị xóa**, mọi node khác an toàn 100%. |

### 3.3. Hai mẫu code xóa phần tử theo điều kiện an toàn tuyệt đối

1. **Erase-Remove Idiom (Chuẩn cho `std::vector`, `std::deque`, `std::string` — $O(N)$):**
   * *Tại sao không gọi `v.erase(it)` trong vòng lặp?* Vì mỗi lần `erase` phải dịch mảng phía sau tốn $O(N)$, làm cả vòng lặp bị chậm thành $O(N^2)$!
   * *Giải pháp $O(N)$:* `std::remove_if` dồn toàn bộ phần tử cần giữ lại lên đầu mảng trong 1 lượt quét $O(N)$ và trả về iterator điểm cắt mới, sau đó `v.erase` cắt đuôi đúng 1 lần:

   ```cpp
   v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
   ```

2. **Hứng lại Iterator trả về từ `erase(it)` (Chuẩn cho `std::list`, `std::set`, `std::map`):**

   ```cpp
   for (auto it = lst.begin(); it != lst.end(); ) {
       if (should_delete(*it)) it = lst.erase(it); // erase() trả về iterator hợp lệ đứng ngay sau node vừa xóa!
       else                    ++it;
   }
   ```

---

## 4. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_vector_and_array.cpp`: `std::array`, `std::vector`, `reserve()`, đối chiếu `push_back` vs `emplace_back`.
* `02_deque_double_ended_queue.cpp`: **`std::deque`** (Kiến trúc Map of Chunks, thao tác 2 đầu `push_front`/`push_back` $O(1)$ + Random Access `dq[i]` $O(1)$).
* `03_list_and_forward_list.cpp`: **`std::list`** (Doubly Linked List, `splice` $O(1)$ zero-copy, `sort`, `remove_if`) & **`std::forward_list`** (Singly Linked List, `insert_after`).
* `04_iterators_and_invalidation.cpp`: Erase-Remove Idiom, xóa an toàn trong vòng lặp, `std::advance` & `std::distance`.
* `05_practice_exercises.cpp`: 5 bài tập thực chiến (Sliding Window Maximum bằng Monotonic `deque`, Deduplicate bằng Erase-Remove, Text Editor Cursor bằng `list`, Browser History, `list::merge()`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_vector_and_array.cpp -o 01_vector_and_array
./01_vector_and_array

g++ -Wall -Wextra -std=c++17 02_deque_double_ended_queue.cpp -o 02_deque_double_ended_queue
./02_deque_double_ended_queue

g++ -Wall -Wextra -std=c++17 03_list_and_forward_list.cpp -o 03_list_and_forward_list
./03_list_and_forward_list

g++ -Wall -Wextra -std=c++17 04_iterators_and_invalidation.cpp -o 04_iterators_and_invalidation
./04_iterators_and_invalidation

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

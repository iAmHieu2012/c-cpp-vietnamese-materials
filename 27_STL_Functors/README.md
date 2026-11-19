# Bài 5.5: Functors (Function Objects) & Bộ So Sánh Tùy Chỉnh (Custom Comparators)

Tài liệu hệ thống hóa thành phần cốt lõi thứ 4 của thư viện chuẩn STL: bản chất của **Functor (Function Object)** dưới góc nhìn trình biên dịch, kỹ thuật **Stateful Functor** (Functor lưu giữ trạng thái), bộ Functor có sẵn trong `<functional>`, và quy tắc thiết kế **Custom Comparator** cho `std::sort`, `std::set` / `std::map` và `std::priority_queue`.

---

## 1. Bản Chất Của Functor (Function Object) & Ưu Thế So Với Con Trỏ Hàm

### 1.1. Functor là gì?

**Functor (Đối tượng hàm)** là một `struct` hoặc `class` có nạp chồng toán tử gọi hàm **`operator()`**. Nhờ đó, một đối tượng của lớp có thể được gọi bằng cú pháp `obj(arg1, arg2)` giống hệt như một hàm thông thường!

```cpp
struct Multiplier {
    int factor; // Trạng thái nội bộ (State) mà hàm thường không có được!

    explicit Multiplier(int f) : factor(f) {}

    // Nạp chồng toán tử gọi hàm operator()
    int operator()(int x) const noexcept {
        return x * factor;
    }
};

Multiplier times3(3);
int res = times3(10); // Gọi times3.operator()(10) ===> Kết quả: 30
```

### 1.2. Tại sao STL luôn ưu tiên Functor hơn Con trỏ hàm (Function Pointer)?

| Tiêu chí | Con trỏ hàm (`bool (*cmp)(int, int)`) | Functor (`struct Cmp { bool operator()(...) }`) |
| :--- | :--- | :--- |
| **Tốc độ thực thi** | **Chậm hơn:** Mỗi lần gọi phải nhảy gián tiếp qua địa chỉ lưu trong con trỏ (**Indirect Call**), cản trở trình biên dịch tối ưu hóa. | **Cực nhanh (Zero-overhead):** Trình biên dịch biết chính xác kiểu của Functor lúc biên dịch nên **Inline (chép thẳng mã lệnh)** vào bên trong vòng lặp của `std::sort`! |
| **Lưu trữ trạng thái** | **Không thể:** Hàm thường không lưu được dữ liệu riêng giữa các lần gọi (trừ khi dùng biến `global`/`static` nguy hiểm). | **Có (Stateful Functor):** Mỗi đối tượng Functor có các thuộc tính riêng (`min_val`, `max_val`, `call_count`) độc lập hoàn toàn. |
| **Dùng làm tham số Template** | Rất rườm rà (phải truyền cả kiểu con trỏ hàm vào Template và truyền địa chỉ hàm vào Constructor). | **Trực tiếp:** Truyền thẳng tên `struct` vào `std::set<T, MyCmp>` hoặc `std::priority_queue<T, vector<T>, MyCmp>`. |

---

## 2. Bộ Functor Dựng Sẵn Trong Thư Viện `<functional>`

STL cung cấp sẵn các khuôn mẫu Functor chuẩn trong `<functional>` để bạn dùng ngay mà không cần tự viết:

| Nhóm | Tên Functor trong `<functional>` | Phép toán tương ứng | Ứng dụng phổ biến nhất trong STL |
| :--- | :--- | :---: | :--- |
| **So sánh** | `std::less<T>` *(Mặc định)* và `std::greater<T>` | `a < b` và `a > b` | `std::sort(v.begin(), v.end(), std::greater<int>{});` (Sắp giảm dần) và `std::set<int, std::greater<int>>` (Set giảm dần) và `std::priority_queue<int, vector<int>, std::greater<int>>` (**Min-Heap**) |
| **Số học** | `std::plus<T>`, `std::minus<T>`, `std::multiplies<T>`, `std::divides<T>` | `a + b`, `a - b`, `a * b`, `a / b` | Tính tích cả mảng: `std::accumulate(v.begin(), v.end(), 1LL, std::multiplies<long long>{});` |
| **Bitwise** | `std::bit_and<T>`, `std::bit_or<T>`, `std::bit_xor<T>` | `a & b`, `a \| b`, `a ^ b` | XOR toàn mảng: `std::accumulate(v.begin(), v.end(), 0, std::bit_xor<int>{});` |

---

## 3. Quy Tắc Thiết Kế Custom Comparator Cho `sort`, `set`/`map` & `priority_queue`

### 3.1. Phân biệt truyền "Kiểu Functor" vs truyền "Đối tượng Functor"

Đây là điểm dễ nhầm lẫn nhất khi mới dùng STL:

1. **Với Hàm thuật toán (`std::sort`, `std::stable_sort`, `std::lower_bound`):**
   * Nhận một **Đối tượng (Instance)** tại thời điểm gọi hàm $\implies$ Phải có cặp ngoặc **`MyComparator{}`** (hoặc `MyComparator()`):
   * `std::sort(v.begin(), v.end(), MyComparator{});`
2. **Với Khuôn mẫu Container (`std::set`, `std::map`, `std::priority_queue`):**
   * Nhận một **Kiểu dữ liệu (Type)** bên trong cặp ngoặc nhọn `<...>` lúc khai báo $\implies$ **KHÔNG có cặp ngoặc `{}`**:
   * `std::set<Student, StudentComparator> s;`
   * `std::priority_queue<Task, std::vector<Task>, TaskComparator> pq;`

### 3.2. Sự ngược chiều kinh điển giữa `std::sort` / `std::set` và `std::priority_queue`

```text
  Cùng một biểu thức so sánh: return a < b; (std::less)
  ┌───────────────────────────────────────────────────┬───────────────────────────────────────────────────┐
  │ 1. Trong std::sort / std::set / std::map          │ 2. Trong std::priority_queue (Binary Heap)        │
  ├───────────────────────────────────────────────────┼───────────────────────────────────────────────────┤
  │ • cmp(a, b) == true  ===> `a` ĐỨNG TRƯỚC `b`      │ • cmp(a, b) == true  ===> `a` ƯU TIÊN THẤP HƠN `b`│
  │ • Dùng `a < b` (std::less)   -> Tăng dần (Min đầu)│ • Dùng `a < b` (std::less)   -> MAX-HEAP (Max đỉnh)│
  │ • Dùng `a > b` (std::greater)-> Giảm dần (Max đầu)│ • Dùng `a > b` (std::greater)-> MIN-HEAP (Min đỉnh)│
  └───────────────────────────────────────────────────┴───────────────────────────────────────────────────┘
```

* **Mẹo nhớ bất bại cho `std::priority_queue`:**
  Trong `bool operator()(const T &a, const T &b) const` của `priority_queue`, hãy đọc câu hỏi là: ***"Có phải `a` thua `b` (nên đẩy `b` lên đỉnh thay cho `a`) hay không?"***
  * Muốn phần tử có `score` **lớn nhất** lên đỉnh `top()` $\rightarrow$ `a` thua `b` khi `a.score < b.score`.
  * Muốn phần tử có `cost` **nhỏ nhất** lên đỉnh `top()` (Min-Heap cho Dijkstra) $\rightarrow$ `a` thua `b` khi `a.cost > b.cost`.

---

## 4. Hai Quy Tắc Sống Còn Khi Viết Custom Comparator

1. **Bắt buộc tuân thủ *Strict Weak Ordering* (Không bao giờ dùng `<=` hoặc `>=`):**
   * Khi `a` và `b` bằng nhau, `cmp(a, b)` và `cmp(b, a)` **bắt buộc phải cùng trả về `false`**.
   * Trong `std::set` và `std::map`, cây Đỏ-Đen kiểm tra 2 phần tử `a` và `b` có trùng khóa hay không bằng công thức: **`!cmp(a, b) && !cmp(b, a)`** (chứ không hề gọi `operator==`!). Nếu bạn viết `return a.id <= b.id;`, cả hai chiều đều trả về `true` làm hỏng toàn bộ cấu trúc cây!
2. **Bắt buộc đánh dấu `const` ở cuối `operator()`:**
   * Luôn viết: `bool operator()(const T &a, const T &b) const { ... }`
   * Từ C++17 trở đi, nếu quên từ khóa `const` ở cuối `operator()`, `std::set` và `std::map` sẽ **báo lỗi biên dịch ngay lập tức** vì cây Đỏ-Đen gọi bộ so sánh thông qua tham chiếu hằng (`const Comparator&`).

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_functor_basics_and_stateful.cpp`: Cú pháp `operator()`, Stateful Functor (lưu trạng thái đếm & lọc theo ngưỡng động với `std::for_each`, `std::remove_if`) và các Functor dựng sẵn trong `<functional>`.
* `02_custom_comparators_sort_and_set.cpp`: Thiết kế Functor đa tiêu chí cho `std::sort`, `std::set` và `std::map` (kèm Transparent Comparator `is_transparent`).
* `03_priority_queue_comparators.cpp`: Giải mã quy tắc ngược chiều của `std::priority_queue` (Max-Heap, Min-Heap và lập lịch tiến trình đa tiêu chí).
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Bộ lọc & giảm giá sản phẩm bằng Stateful Functor, Xếp hạng ứng viên đa tiêu chí, Hàng đợi cấp cứu Bệnh viện `priority_queue`, Sổ lệnh chứng khoán Bid/Ask bằng `std::map` + Functor, Custom Hash & Equality Functor cho `unordered_map`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_functor_basics_and_stateful.cpp -o fn_basics
./fn_basics

g++ -Wall -Wextra -std=c++17 02_custom_comparators_sort_and_set.cpp -o fn_sort_set
./fn_sort_set

g++ -Wall -Wextra -std=c++17 03_priority_queue_comparators.cpp -o fn_pq
./fn_pq

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o fn_exercises
./fn_exercises
```

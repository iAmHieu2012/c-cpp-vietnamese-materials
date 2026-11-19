# Bài 6.5: Các Tính Năng Đột Phá Của C++20 (Concepts, Ranges `<ranges>`, Coroutines, Modules, `<=>` & `std::span`)

Tài liệu hệ thống hóa 4 trụ cột lớn nhất của chuẩn **C++20** (Big Four: **Concepts**, **Ranges**, **Coroutines**, **Modules**) cùng hai công cụ thực chiến **`operator<=>` (Spaceship)** và **`std::span<T>`**.

---

## 1. Concepts (`<concepts>`) & Toán Tử Ba Chiều Spaceship (`<=>`) (`01_concepts_and_spaceship.cpp`)

### 1.1. Tại sao cần Concepts? (Chấm dứt cơn ác mộng lỗi Template)

Trước C++20, nếu bạn truyền một kiểu dữ liệu không có toán tử `+` hoặc `<` vào hàm Template, trình biên dịch sẽ bung ra hàng trăm dòng lỗi khó hiểu sâu bên trong thư viện chuẩn. **Concepts** cho phép đặt **hợp đồng ràng buộc kiểu dữ liệu ngay trên chữ ký hàm**:

```cpp
#include <concepts>

// 1. Tự định nghĩa một Concept chỉ chấp nhận kiểu số nguyên hoặc số thực:
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// 2. Bốn cú pháp sử dụng Concept trong C++20 (tương đương nhau):
template <Numeric T>        T add_v1(T a, T b) { return a + b; }          // Cách 1: Thay trực tiếp từ khóa typename
template <typename T> requires Numeric<T> T add_v2(T a, T b) { return a + b; } // Cách 2: Mệnh đề requires trước hàm
Numeric auto add_v3(Numeric auto a, Numeric auto b) { return a + b; }     // Cách 3: Cú pháp rút gọn (Abbreviated)
```

### 1.2. Toán tử so sánh 3 chiều Spaceship (`<=>` — `<compare>`)

Thay vì phải viết thủ công cả 6 hàm nạp chồng toán tử (`==`, `!=`, `<`, `<=`, `>`, `>=`), trong C++20 bạn chỉ cần viết **đúng 1 dòng duy nhất**:

```cpp
struct Version {
    int major, minor, patch;
    // Tự động sinh đầy đủ cả 6 phép so sánh ==, !=, <, <=, >, >= theo thứ tự từ trên xuống!
    auto operator<=>(const Version &other) const = default;
};
```

* Trả về kết quả tương tự hàm `strcmp`: `< 0` (nhỏ hơn), `== 0` (bằng nhau), hoặc `> 0` (lớn hơn).

---

## 2. Thư Viện Ranges (`<ranges>`) & `std::span<T>` (`<span>`) (`02_ranges_views_and_span.cpp`)

### 2.1. Tư duy Đường ống (Pipeline `|`) & Tính toán lười (Lazy Evaluation)

```text
  Cách cũ (STL C++11/17):   Vector gốc ──(copy lọc)──► Vector tạm 1 ──(copy nhân)──► Vector tạm 2 (Tốn 2 lần cấp phát Heap!)
  C++20 Views Pipeline:     v | views::filter(chan) | views::transform(binh_phuong) | views::take(3)
                            ===> 0 cấp phát Heap! Chỉ khi vòng lặp `for` đọc tới phần tử nào thì mới tính phần tử đó!
```

| Tính năng trong `<ranges>` | Cú pháp C++20 hiện đại | Ưu thế vượt trội so với STL truyền thống |
| :--- | :--- | :--- |
| **Truyền thẳng Container** | `std::ranges::sort(v);` | Không cần gõ lặp lại `.begin(), .end()`. |
| **Projections (Phép chiếu)** | `std::ranges::sort(emps, {}, &Emp::salary);` | Sắp xếp/tìm kiếm trực tiếp theo một thuộc tính `&Class::member` **mà không cần viết Lambda so sánh!** |
| **`views::filter(pred)`** | `v \| std::views::filter([](int x){...})` | Lọc phần tử thỏa điều kiện theo cơ chế **Lazy (0 Copy)**. |
| **`views::transform(fn)`** | `v \| std::views::transform([](int x){...})` | Biến đổi giá trị từng phần tử trên đường ống. |
| **`views::take(K)` / `drop(K)`** | `v \| std::views::take(5)` | Chỉ lấy $K$ phần tử đầu tiên (hoặc bỏ qua $K$ phần tử đầu) và dừng tính toán ngay khi đủ! |
| **`views::iota(L, R)`** | `std::views::iota(1, 1000000)` | Sinh dãy số ảo từ $L$ đến $R - 1$ trong **$O(1)$ RAM** (không tốn bộ nhớ lưu mảng!). |

### 2.2. `std::span<T>` — Cửa sổ nhìn mảng liên tiếp Zero-Copy

Nếu `std::string_view` (C++17) là cửa sổ chỉ đọc cho chuỗi ký tự, thì **`std::span<T>` (C++20)** là cửa sổ nhìn chung (đọc và **sửa được**!) cho **mọi mảng liên tiếp** (Mảng C `int a[5]`, `std::array`, `std::vector`). Hàm nhận `std::span<int>` chỉ tốn 16 bytes (1 con trỏ + 1 biến `size`) và chấp nhận tất cả các loại mảng trên mà không bị Array Decay!

---

## 3. Coroutines (`<coroutine>`: `co_yield`, `co_return`, `co_await`) (`03_coroutines_generator.cpp`)

```text
  1. Hàm thường (Subroutine):  Gọi hàm ──► Chạy từ đầu đến cuối ──► return (Hủy sạch Stack Frame!)
  2. C++20 Coroutine:          Gọi hàm ──► Chạy tới `co_yield val` (TẠM DỪNG / SUSPEND, đóng băng toàn bộ biến cục bộ!)
                                       ◄── Gọi `.next()` (TIẾP TỤC / RESUME chạy tiếp ngay dòng dưới `co_yield`!)
```

* Một hàm trong C++20 tự động trở thành **Coroutine** khi chứa ít nhất 1 trong 3 từ khóa:
  * **`co_yield expr`:** Tạm dừng hàm, trả một giá trị ra ngoài cho người gọi, và giữ nguyên trạng thái để lần sau chạy tiếp (ứng dụng viết **Lazy Generator** sinh luồng dữ liệu vô hạn với $O(1)$ RAM).
  * **`co_await expr`:** Tạm dừng hàm chờ một tác vụ bất đồng bộ (I/O mạng, đọc đĩa) hoàn tất mà không làm treo luồng (Thread).
  * **`co_return expr`:** Kết thúc hoàn toàn Coroutine.

---

## 4. C++20 Modules (`export module`, `import`) — Thay Thế `#include` Truyền Thống

```text
  Cơ chế #include cũ (Từ thập niên 1970):                  Cơ chế C++20 Modules mới (export module / import):
  • Preprocessor copy-paste toàn bộ 40,000 dòng của       • Biên dịch module `math.cppm` ĐÚNG 1 LẦN DUY NHẤT ra
    <iostream>, <vector> vào MỌI file .cpp!                 file nhị phân BMI (Binary Module Interface).
  • Rò rỉ Macro (#define) giữa các file header.           • Không rò rỉ Macro, không cần #pragma once / Header Guards.
  • Thời gian build dự án lớn cực chậm.                   • Tốc độ biên dịch nhanh gấp 3 – 10 lần!
```

* **Ví dụ cú pháp C++20 Module (Định nghĩa & Sử dụng):**

  ```cpp
  // File 1: math_utils.cppm (Module Interface Unit)
  export module math_utils; // Khai báo tên module

  export int fast_add(int a, int b) { // Chỉ những gì gắn 'export' mới lộ ra ngoài
      return a + b;
  }

  // File 2: main.cpp
  import math_utils; // Nạp trực tiếp BMI trong tích tắc, không copy-paste văn bản!
  ```

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_concepts_and_spaceship.cpp`: Tự định nghĩa `concept Numeric`, `Printable` (`requires`), và nạp chồng toán tử 3 chiều `<=>` (`<compare>`).
* `02_ranges_views_and_span.cpp`: Đường ống `std::views` (`iota | filter | transform | take`), `std::ranges::sort` với **Projections**, và `std::span<int>`.
* `03_coroutines_generator.cpp`: Cài đặt `Generator<T>` chuẩn `<coroutine>` với `co_yield` để sinh luồng số Fibonacci lười ($O(1)$ Space).
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến C++20 (Concepts cho Container & Số học, Sắp xếp đa năng bằng Ranges Projections, Pipeline phân tích dữ liệu Lazy Views, Xử lý lát cắt mảng tại chỗ bằng `std::span`, Sinh luồng số nguyên tố bằng Coroutine `co_yield`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++20 01_concepts_and_spaceship.cpp -o cpp20_concepts
./cpp20_concepts

g++ -Wall -Wextra -std=c++20 02_ranges_views_and_span.cpp -o cpp20_ranges
./cpp20_ranges

g++ -Wall -Wextra -std=c++20 03_coroutines_generator.cpp -o cpp20_coro
./cpp20_coro

g++ -Wall -Wextra -std=c++20 04_practice_exercises.cpp -o cpp20_exercises
./cpp20_exercises
```

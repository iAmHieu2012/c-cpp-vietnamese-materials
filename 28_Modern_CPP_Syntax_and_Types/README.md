# Bài 6.1: Tiện Ích Cú Pháp Hiện Đại & Suy Luận Kiểu (`auto`, `constexpr`, Structured Bindings, `optional`, `variant`, `string_view`)

Tài liệu hệ thống hóa các tiện ích cú pháp và kiểu dữ liệu an toàn quan trọng nhất từ C++11 đến C++17: suy luận kiểu lúc biên dịch (**`auto`**, **`decltype`**), vòng lặp **Range-based `for`**, khởi tạo đồng nhất (**Uniform Initialization `{}`**), tính toán lúc biên dịch (**`constexpr`**), **Structured Bindings**, và bộ tứ kiểu dữ liệu hiện đại **`std::string_view`**, **`std::optional`**, **`std::variant`**, **`std::any`**.

---

## 1. Suy Luận Kiểu (`auto`, `decltype`) & Vòng Lặp Range-Based `for` (`01_auto_decltype_constexpr.cpp`)

### 1.1. Quy tắc suy luận kiểu của `auto` (Compile-Time Type Deduction)

`auto` **không phải** là kiểu động lúc chạy như Python/JavaScript. Trình biên dịch C++ suy luận chính xác kiểu tĩnh của biến ngay lúc biên dịch (0 chi phí Runtime). Tuy nhiên, **`auto` mặc định sẽ làm rơi mất từ khóa `const` và dấu tham chiếu `&`!**

| Cách viết trong vòng lặp Range-based `for` | Hành vi dưới thanh RAM | Khi nào bắt buộc dùng? |
| :--- | :--- | :--- |
| `for (auto x : container)` | **Tạo bản sao (Copy)** từng phần tử ra biến `x`. | Chỉ dùng cho kiểu nguyên thủy nhỏ (`int`, `char`, `double`, `std::string_view`). |
| `for (auto &x : container)` | Tham chiếu trực tiếp tới phần tử gốc (0 Copy). | Khi cần **sửa đổi trực tiếp** giá trị phần tử bên trong container. |
| **`for (const auto &x : container)`** | Tham chiếu hằng chỉ đọc (**0 Copy + An toàn**). | **Chuẩn vàng mặc định** khi duyệt `std::string`, `std::vector`, `struct`, `class`. |

* **`decltype(expr)`:** Trích xuất kiểu dữ liệu chính xác của một biểu thức lúc biên dịch (giữ nguyên cả `const` và `&`), rất hữu ích trong lập trình Template.

### 1.2. Khởi tạo đồng nhất (`Uniform Initialization {}`), `nullptr` & `constexpr`

1. **Khởi tạo bằng ngoặc nhọn `{}` (Brace Initialization):**
   * Dùng thống nhất cho mọi nơi (biến đơn, mảng, `struct`, `std::vector`).
   * **Ưu thế cốt tử:** Tự động **cấm ép kiểu thu hẹp gây mất dữ liệu (Narrowing Conversion)** lúc biên dịch:
     * `int a = 3.14;` $\rightarrow$ Biên dịch trót lọt nhưng mất phần thập phân `.14`.
     * `int a{3.14};` $\rightarrow$ **Trình biên dịch báo lỗi chặn ngay lập tức!**
2. **`nullptr` (thay thế `NULL` / `0`):**
   * Trong C cũ, `NULL` thực chất chỉ là macro số nguyên `0`, dẫn tới gọi nhầm hàm khi nạp chồng `func(int)` và `func(int*)`. Từ khóa `nullptr` có kiểu riêng là `std::nullptr_t`, chỉ khớp với kiểu con trỏ.
3. **`constexpr` (Constant Expression — Tính toán ngay lúc biên dịch):**
   * `const` chỉ cam kết "biến không được sửa sau khi khởi tạo" (giá trị có thể tính lúc chạy Runtime).
   * **`constexpr`** ép trình biên dịch **tính ra kết quả ngay lúc biên dịch (Compile-time)** và nhúng thẳng con số kết quả vào mã máy (ví dụ: hàm `constexpr long long factorial(int n)` tính sẵn $10! = 3628800$ ngay khi build!).

---

## 2. Structured Bindings & `std::string_view` (C++17) (`02_structured_bindings_string_view.cpp`)

### 2.1. Structured Bindings (`auto [a, b, c] = ...`)

Cho phép "rã" trực tiếp các thành phần của một `std::pair`, `std::tuple`, `struct`, hoặc mảng cố định thành các biến có tên rõ ràng chỉ trong 1 dòng:

```cpp
// Thay vì viết it->first và it->second khó đọc:
for (const auto &[username, score] : leaderboard_map) {
    std::cout << username << ": " << score << "\n";
}
```

### 2.2. `std::string_view` — Cửa sổ nhìn chuỗi Zero-Copy ($O(1)$ Substring)

Khi truyền một chuỗi `"Hello World"` hoặc cắt chuỗi con `s.substr(pos, len)` bằng `const std::string&`, C++ thường phải cấp phát bộ nhớ trên **Heap** và copy ký tự tốn $O(N)$. **`std::string_view`** giải quyết triệt để vấn đề này:

```text
  Bộ nhớ gốc (std::string hoặc .rodata):     [ 'H' | 'e' | 'l' | 'l' | 'o' | ' ' | 'W' | 'o' | 'r' | 'l' | 'd' ]
                                                                             ▲                       ▲
  std::string_view sv (Nặng đúng 16 bytes trên Stack):                       │                       │
  ┌──────────────────────────┬──────────────┐                                │                       │
  │ const char *data         │ size = 5     ├────────────────────────────────┴───────────────────────┘
  └──────────────────────────┴──────────────┘
  * Cắt chuỗi con sv.substr(6, 5) chỉ là dịch con trỏ `data + 6` và gán `size = 5` ---> O(1) Time, 0 Heap Allocation!
```

* **Cạm bẫy sống còn của `std::string_view`:** Vì `string_view` **không sở hữu dữ liệu** (chỉ giữ con trỏ nhìn vào chuỗi gốc), **TUYỆT ĐỐI KHÔNG** trả về `std::string_view` trỏ vào một `std::string` cục bộ trong hàm, hoặc giữ `string_view` sau khi `std::string` gốc đã bị hủy (**Dangling View**)!

---

## 3. Bộ Ba Kiểu Dữ Liệu An Toàn C++17: `std::optional`, `std::variant` & `std::any` (`03_optional_variant_any.cpp`)

| Kiểu C++17 | Thư viện | Vấn đề cũ nó thay thế | Bản chất & Cách sử dụng chuẩn |
| :--- | :---: | :--- | :--- |
| **`std::optional<T>`** | `<optional>` | Trả về số ma thuật (`-1`, `NULL`) hoặc truyền con trỏ out-param khi hàm có thể không tìm thấy kết quả. | Biểu diễn một giá trị kiểu `T` **có thể tồn tại hoặc rỗng (`std::nullopt`)**. Kiểm tra bằng `if (opt.has_value())`, lấy giá trị bằng `*opt` hoặc `opt.value_or(default_val)`. |
| **`std::variant<T1, T2...>`** | `<variant>` | `union` kiểu C không an toàn kiểu (không biết thành viên nào đang hoạt động). | **Type-safe Union** nằm trên Stack: tại một thời điểm giữ đúng 1 kiểu trong danh sách `<T1, T2...>`. Truy xuất an toàn bằng `std::get_if<T>(&v)` hoặc `std::visit`. |
| **`std::any`** | `<any>` | Con trỏ vô kiểu `void*` (dễ ép kiểu sai gây Crash). | Hộp chứa có thể lưu **bất kỳ kiểu dữ liệu nào** (có kiểm tra kiểu lúc chạy qua `std::any_cast<T>`). *(Chỉ dùng khi tập kiểu không biết trước; nếu biết trước hãy ưu tiên `std::variant`)*. |

---

## 4. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_auto_decltype_and_range_for.cpp`: Suy luận kiểu `auto`, `decltype` và vòng lặp Range-based `for (const auto& x : container)`.
* `02_uniform_init_nullptr_constexpr.cpp`: Khởi tạo đồng nhất `Uniform Initialization {}`, từ khóa `nullptr` và hàm `constexpr` tính lúc biên dịch.
* `03_structured_bindings_string_view.cpp`: Structured Bindings trên `pair`/`tuple`/`struct`/`map` và cắt chuỗi $O(1)$ Zero-Copy bằng `std::string_view`.
* `04_optional_variant_any.cpp`: Thực hành `std::optional` (`nullopt`, `value_or`), `std::variant` (`std::visit`) và `std::any` (`std::any_cast`).
* `05_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Bảng tra cứu Lũy thừa/Giai thừa `constexpr`, Bộ tách URL/CSV Zero-Copy bằng `std::string_view`, Tìm kiếm cấu hình an toàn bằng `std::optional`, Máy trạng thái Kết quả API bằng `std::variant`, Giải phương trình trả về đa kết quả bằng Structured Bindings).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_auto_decltype_and_range_for.cpp -o 01_auto_decltype_and_range_for
./01_auto_decltype_and_range_for

g++ -Wall -Wextra -std=c++17 02_uniform_init_nullptr_constexpr.cpp -o 02_uniform_init_nullptr_constexpr
./02_uniform_init_nullptr_constexpr

g++ -Wall -Wextra -std=c++17 03_structured_bindings_string_view.cpp -o 03_structured_bindings_string_view
./03_structured_bindings_string_view

g++ -Wall -Wextra -std=c++17 04_optional_variant_any.cpp -o 04_optional_variant_any
./04_optional_variant_any

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

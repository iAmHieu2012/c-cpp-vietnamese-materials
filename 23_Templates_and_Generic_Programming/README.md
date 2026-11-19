# Bài 5.1: Lập Trình Tổng Quát — Templates (Function/Class Templates, Specialization & Variadic Templates)

Tài liệu hệ thống hóa cơ chế sinh mã lúc biên dịch (**Compile-Time Instantiation**) của **Templates** trong C++, cách thiết kế **Function Template**, **Class Template**, **Non-Type Template Parameters**, kỹ thuật **Chuyên biệt hóa (Full & Partial Specialization)**, **Variadic Templates + Fold Expressions (C++17)** và nguyên nhân tại sao code Template bắt buộc phải đặt trong file Header (`.h` / `.hpp`).

---

## 1. Bản Chất Của Templates Dưới Góc Nhìn Trình Biên Dịch

### 1.1. Cơ chế Sinh mã lúc biên dịch (Compile-Time Instantiation)

Khác với Đa hình Runtime (`virtual` tốn `vptr` 8 bytes và tra bảng `vtable` lúc chạy), **Template là Đa hình lúc biên dịch (Compile-time Polymorphism — Zero Runtime Overhead)**. Bản thân một Template chưa phải là mã máy, mà là một **khuôn đúc mã nguồn**:

```text
  Khuôn mẫu trong Code:                    Trình biên dịch tự động đúc ra các hàm độc lập (Text Segment):
  template <typename T>                    • Khi gọi add(10, 20)     ===> int add<int>(int a, int b)
  T add(T a, T b) { return a + b; }  ───►  • Khi gọi add(1.5, 2.5)   ===> double add<double>(double a, double b)
                                           • Khi gọi add(s1, s2)     ===> string add<string>(string a, string b)
```

### 1.2. Cạm bẫy Linker kinh điển: Tại sao KHÔNG ĐƯỢC tách Template ra file `.cpp`?

* Với hàm bình thường, ta khai báo Prototype trong `math.h` và viết thân hàm trong `math.cpp`.
* Nhưng với **Template**, nếu bạn để phần thân hàm trong `math.cpp`, khi trình biên dịch dịch riêng file `math.cpp`, nó chưa biết `T` là kiểu gì nên **không sinh ra bất kỳ dòng mã máy nào** trong `math.o`! Đến khi `main.cpp` gọi `add<int>(1, 2)`, Linker sẽ báo lỗi **`undefined reference to add<int>`**.
* **Quy tắc sống còn:** Toàn bộ phần định nghĩa (thân hàm/lớp) của Template **bắt buộc phải nằm trực tiếp trong file Header (`.h` / `.hpp`)**!

---

## 2. Function Templates & Class Templates (`01_function_and_class_templates.cpp`)

### 2.1. Khuôn mẫu hàm (Function Templates) & Suy luận kiểu (`typename` vs `class`)

* Trong khai báo `template <typename T>` và `template <class T>`, hai từ khóa **`typename` và `class` hoàn toàn tương đương 100%** (chuẩn hiện đại ưu tiên dùng `typename` để tránh nhầm với lớp OOP).
* **Xử lý khi truyền 2 đối số khác kiểu (`my_max(10, 3.14)`):**
  * Nếu khai báo `template <typename T> T my_max(T a, T b)`, gọi `my_max(10, 3.14)` sẽ **lỗi biên dịch** vì trình biên dịch bối rối giữa `T = int` và `T = double`.
  * **Giải pháp 1:** Chỉ định kiểu tường minh lúc gọi: `my_max<double>(10, 3.14);`.
  * **Giải pháp 2:** Dùng 2 tham số kiểu `template <typename T1, typename T2> auto my_max(T1 a, T2 b)` (C++14/17 tự suy luận kiểu trả về chung).

### 2.2. Khuôn mẫu lớp (Class Templates) & Tham số hằng số (Non-Type Template Parameters)

Bên cạnh tham số kiểu (`typename T`), Template còn cho phép truyền **một hằng số nguyên biết trước lúc biên dịch** (như `size_t N`), giúp cấp phát mảng tĩnh trực tiếp trên **Stack** mà không tốn chi phí Heap (đây chính là kiến trúc lõi của `std::array<T, N>`):

```cpp
template <typename T, size_t N>
class StaticBuffer {
private:
    T data[N]; // Mảng tĩnh kích thước N được ấn định ngay lúc biên dịch trên Stack!
public:
    constexpr size_t size() const noexcept { return N; }
    T& operator[](size_t i) { return data[i]; }
};
```

---

## 3. Chuyên Biệt Hóa Khuôn Mẫu (Template Specialization) (`02_template_specialization.cpp`)

Đôi khi khuôn mẫu tổng quát `T` hoạt động đúng với 99% kiểu dữ liệu nhưng lại **chạy sai bản chất** với một kiểu đặc thù (ví dụ: con trỏ chuỗi `const char*`).

### 3.1. Cạm bẫy so sánh `const char*` trong Template tổng quát & Giải pháp Full Specialization

* Giả sử có `template <typename T> bool is_equal(T a, T b) { return a == b; }`.
* Nếu truyền hai chuỗi C-style `const char*`: phép `a == b` chỉ so sánh **2 địa chỉ bộ nhớ RAM** chứ không so sánh nội dung chữ bên trong!
* **Giải pháp — Chuyên biệt hóa toàn phần (`template <>` — Full Specialization):**

```cpp
// 1. Khuôn mẫu tổng quát cho mọi kiểu T
template <typename T>
bool is_equal(const T &a, const T &b) { return a == b; }

// 2. Chuyên biệt hóa toàn phần (Full Specialization) dành riêng cho T = const char*
template <>
bool is_equal<const char*>(const char* const &a, const char* const &b) {
    return std::strcmp(a, b) == 0; // So sánh từng ký tự chuẩn xác!
}
```

### 3.2. Phân biệt Full Specialization vs Partial Specialization

| Tiêu chí | Full Specialization *(Chuyên biệt hóa toàn phần)* | Partial Specialization *(Chuyên biệt hóa một phần)* |
| :--- | :--- | :--- |
| **Cú pháp** | `template <> class Box<bool> { ... };` | `template <typename T> class Box<T*> { ... };` |
| **Bản chất** | Cố định **tất cả** các tham số kiểu thành kiểu cụ thể (`int`, `bool`, `const char*`). | Chỉ cố định **một phần** hoặc ràng buộc một nhóm kiểu (như nhóm mọi con trỏ `T*`, nhóm cặp cùng kiểu `Pair<T, T>`). |
| **Áp dụng cho** | Cả **Class Template** lẫn **Function Template**. | **Chỉ áp dụng cho Class Template** *(Function Template không có Partial Specialization mà dùng Function Overloading)*. |

---

## 4. Variadic Templates, Fold Expressions & `if constexpr` (`03_variadic_templates_fold.cpp`)

### 4.1. Variadic Templates (`typename... Args`) & C++17 Fold Expressions

**Variadic Templates** cho phép hàm/lớp nhận **số lượng tham số tùy ý với kiểu dữ liệu hoàn toàn khác nhau** (thay thế cơ chế `va_list` không an toàn kiểu của ngôn ngữ C, ứng dụng trong `emplace_back`, `std::make_unique`, `std::tuple`):

```cpp
// Trước C++17: Phải viết hàm cơ sở + hàm đệ quy tách từng tham số (head, tail...)
// Từ C++17: Dùng Fold Expression `(... op args)` gói gọn trong đúng 1 dòng duy nhất!

template <typename... Args>
auto sum_all(Args... args) {
    return (args + ... + 0); // Binary Right Fold: cộng dồn mọi tham số (trả về 0 nếu không truyền gì)
}

template <typename... Args>
void print_all(const Args&... args) {
    ((std::cout << args << " "), ...); // Comma Fold: in lần lượt từng tham số cách nhau dấu cách
    std::cout << "\n";
}
```

### 4.2. Rẽ nhánh lúc biên dịch với `if constexpr` & `<type_traits>` (C++17)

Trong hàm Template, câu lệnh `if` thông thường yêu cầu **cả nhánh `true` lẫn nhánh `false` đều phải hợp lệ về cú pháp cho mọi kiểu `T`**. Ngược lại, **`if constexpr (điều_kiện_compile_time)`** chỉ biên dịch nhánh đúng và **loại bỏ hoàn toàn nhánh sai** ngay lúc biên dịch:

```cpp
#include <type_traits>

template <typename T>
std::string describe_type(const T &val) {
    if constexpr (std::is_integral_v<T>) {
        return "So nguyen: " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "So thuc: " + std::to_string(val);
    } else {
        return "Chuoi/Doi tuong: " + std::string(val); // Hợp lệ cho string mà không làm lỗi khi T = int!
    }
}
```

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_function_and_class_templates.cpp`: Function Template (`my_swap`, `my_max` đa kiểu), Class Template `StaticBuffer<T, N>` và `GenericStack<T>`.
* `02_template_specialization.cpp`: Full Specialization cho `const char*` (`std::strcmp`) và Partial Specialization cho con trỏ `T*`.
* `03_variadic_templates_fold.cpp`: Variadic Templates, C++17 Fold Expressions (`sum_all`, `print_all`, `all_true`) và `if constexpr` với `<type_traits>`.
* `04_practice_exercises.cpp`: Giải 5 bài tập tổng hợp (`GenericVector<T>` RAII trên Heap, `KeyValuePair<K, V>` Specialization, Variadic Logger & Clamp, Generic `filter`/`map_reduce` với Lambda, Bộ tuần tự hóa `serialize_to_json<T>` bằng `if constexpr`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_function_and_class_templates.cpp -o tpl_basic
./tpl_basic

g++ -Wall -Wextra -std=c++17 02_template_specialization.cpp -o tpl_spec
./tpl_spec

g++ -Wall -Wextra -std=c++17 03_variadic_templates_fold.cpp -o tpl_variadic
./tpl_variadic

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o tpl_exercises
./tpl_exercises
```

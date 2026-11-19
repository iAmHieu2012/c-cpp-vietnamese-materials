# Bài 6.2: Biểu Thức Lambda (Lambda Expressions, Capture Modes, Generic Lambdas & Closures)

Tài liệu hệ thống hóa cơ chế vận hành của **Biểu thức Lambda (Lambda Expressions)** trong C++11/14/17: giải phẫu cú pháp `[capture](parameters) mutable noexcept -> return_type { body }`, cách trình biên dịch biến đổi Lambda thành một **Anonymous Functor (Closure Class)** dưới thanh RAM, bảng phân loại các chế độ bắt biến (**Capture by Value, by Reference, Init-Capture**), **Generic Lambda (`auto`)**, và ứng dụng truyền hàm ẩn danh vào STL Algorithms.

---

## 1. Giải Phẫu Một Biểu Thức Lambda & Bản Chất Bên Dưới Trình Biên Dịch

### 1.1. Sơ đồ cú pháp đầy đủ của Lambda

```text
  [capture_clause] (parameter_list) mutable noexcept -> ReturnType { function_body }
  │                │                │       │           │            │
  │                │                │       │           │            └─ 6. Thân hàm thực thi
  │                │                │       │           └─ 5. Kiểu trả về (Tự động suy luận nếu bỏ qua)
  │                │                │       └─ 4. Cam kết không ném ngoại lệ (Tùy chọn)
  │                │                └─ 3. Cho phép sửa bản sao của biến bắt theo giá trị (Tùy chọn)
  │                └─ 2. Danh sách tham số truyền vào như hàm thường
  └─ 1. Mệnh đề bắt biến (Capture): Quyết định những biến cục bộ nào ở bên ngoài được đưa vào trong Lambda
```

### 1.2. Bản chất dưới RAM: Lambda chính là một Functor ẩn danh (Closure Object)

Khi bạn viết một biểu thức Lambda, trình biên dịch C++ **tự động sinh ra một `class` ẩn danh có nạp chồng `operator() const`** (giống hệt Functor ở Bài 5.5):

```text
  Code Lambda bạn viết:                              Class Functor trình biên dịch tự động đúc ra:
  int threshold = 50;                                class __AnonymousLambda_1 {
  auto is_pass = [threshold](int score) {     ===>   private:
      return score >= threshold;                         int threshold; // Biến được capture trở thành thuộc tính!
  };                                                 public:
                                                         __AnonymousLambda_1(int t) : threshold(t) {}
                                                         bool operator()(int score) const { // Mặc định là CONST!
                                                             return score >= threshold;
                                                         }
                                                     };
```

* **Hệ quả quan trọng 1:** Vì `operator()` của Lambda mặc định là **`const` member function**, bạn **không được phép sửa đổi biến bắt theo giá trị (`[x]`)** bên trong Lambda trừ khi thêm từ khóa **`mutable`** ngay sau ngoặc tròn tham số!
* **Hệ quả quan trọng 2:** Mỗi biểu thức Lambda có một kiểu ẩn danh độc nhất vô nhị do trình biên dịch tự đặt tên $\implies$ **Luôn dùng từ khóa `auto`** khi lưu một Lambda vào biến (`auto fn = [](int x) { ... };`).

---

## 2. Bảng Tra Cứu Các Chế Độ Bắt Biến (Capture Modes) (`01_lambda_syntax_and_captures.cpp`)

| Cú pháp Capture | Cơ chế hoạt động dưới bộ nhớ | Sửa được biến gốc bên ngoài? | Khi nào nên sử dụng? |
| :--- | :--- | :---: | :--- |
| **`[]`** *(Empty)* | Không bắt biến ngoài nào. `sizeof(lambda) == 1 byte`. | Không | Lambda thuần túy chỉ xử lý dựa trên tham số truyền vào. *(Có thể ép kiểu trực tiếp về Con trỏ hàm C!)*. |
| **`[x, y]`** *(By Value)* | **Sao chép giá trị** của `x, y` tại thời điểm **tạo** Lambda vào làm thuộc tính của Closure. | **Không** *(Muốn sửa bản sao nội bộ phải thêm `mutable`)* | Khi cần giữ lại giá trị chụp nhanh (Snapshot) của biến nhỏ hoặc khi Lambda sống lâu hơn biến gốc. |
| **`[&x, &y]`** *(By Reference)* | Lưu **tham chiếu (địa chỉ)** trỏ thẳng tới biến `x, y` ở hàm bên ngoài. | **Có** *(Thay đổi trực tiếp biến gốc!)* | Khi muốn tích lũy kết quả ra biến ngoài (`sum`, `count`), hoặc tránh copy đối tượng lớn (`vector`, `string`). |
| **`[=]`** *(Default Value)* | Tự động bắt **theo giá trị** mọi biến bên ngoài được nhắc tới trong thân Lambda. | Không | Tiện cho Lambda ngắn, nhưng cẩn thận tránh vô tình copy các mảng/đối tượng lớn. |
| **`[&]`** *(Default Ref)* | Tự động bắt **theo tham chiếu** mọi biến bên ngoài được nhắc tới trong thân Lambda. | **Có** | Rất phổ biến khi truyền Lambda chạy ngay lập tức trong `std::sort`, `std::for_each`. |
| **`[=, &cnt]`** / **`[&, x]`** | Bắt mặc định theo giá trị nhưng riêng `cnt` theo tham chiếu (hoặc ngược lại). | Chỉ sửa được biến có `&` | Kiểm soát chính xác biến nào chỉ đọc, biến nào dùng để xuất kết quả. |
| **`[this]`** / **`[*this]`** *(C++17)* | `[this]` bắt con trỏ đối tượng hiện tại; `[*this]` tạo bản sao toàn bộ đối tượng hiện tại. | `[this]` sửa được thuộc tính Class | Khi viết Lambda bên trong một phương thức của `class`. |
| **`[p = std::move(ptr)]`** *(C++14 Init-Capture)* | Tạo biến thành viên mới `p` ngay trong mệnh đề Capture (hỗ trợ **Move-Only** như `unique_ptr`). | Sửa được `p` (với `mutable`) | Khi cần chuyển giao quyền sở hữu `std::unique_ptr` hoặc biểu thức tính sẵn vào trong Lambda. |

---

## 3. Generic Lambdas (`auto`), `std::function` & Cạm Bẫy Dangling Reference

### 3.1. Generic Lambda (C++14) & `constexpr` Lambda (C++17) (`02_generic_and_higher_order_lambdas.cpp`)

* **Generic Lambda:** Dùng từ khóa `auto` trong danh sách tham số `[](const auto &a, const auto &b) { return a < b; }`. Trình biên dịch tự động biến `operator()` của Lambda thành một **Function Template**, dùng chung được cho cả `int`, `double`, `std::string`, `std::pair`!
* **`std::function<Return(Args...)>` (trong `<functional>`):**
  * Dùng khi cần lưu nhiều Lambda khác nhau vào cùng một `std::vector`, làm thuộc tính của `class`, hoặc viết **Lambda đệ quy**.
  * *Lưu ý hiệu năng:* `auto` nhanh hơn `std::function` vì `std::function` dùng cơ chế xóa kiểu (Type Erasure) tốn một bước gọi ảo và có thể cấp phát Heap nếu Closure quá lớn.

### 3.2. Cạm bẫy chết người: Dangling Reference với `[&]`

* Nếu một hàm trả về một Lambda ra bên ngoài (hoặc lưu Lambda vào hàng đợi chạy sau) mà dùng **`[&]`** để bắt biến cục bộ trên Stack của hàm đó: ngay khi hàm kết thúc, biến cục bộ bị hủy $\implies$ Lambda sẽ tham chiếu vào vùng nhớ rác gây **Undefined Behavior / Crash**!
* **Quy tắc vàng:** Nếu Lambda sống lâu hơn phạm vi hàm tạo ra nó, **BẮT BUỘC phải bắt theo giá trị (`[x]` hoặc `[=]`) hoặc Move (`[x = std::move(x)]`)**, tuyệt đối không dùng `[&]`!

---

## 4. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_lambda_syntax_and_captures.cpp`: Thực nghiệm các chế độ Capture (`[]`, `[x]`, `[&x]`, `[x] mutable`, C++14 Init-Capture `[v = std::move(vec)]`) và kiểm chứng thời điểm chụp giá trị.
* `02_generic_and_higher_order_lambdas.cpp`: Generic Lambda (`const auto&`), Higher-Order Functions (hàm trả về Lambda đóng gói trạng thái - Closure Factory) và `std::function`.
* `03_lambdas_with_stl_algorithms.cpp`: Kết hợp Lambda với bộ thuật toán STL (`std::sort`, `std::remove_if`, `std::transform`, `std::count_if`, `std::priority_queue`).
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Hệ thống Event Dispatcher / Callback bằng `std::function` + Lambda, Sắp xếp tùy chỉnh động bằng Lambda, Closure Factory tạo hàm tính thuế/chiết khấu, Pipeline lọc-biến đổi-tích lũy dữ liệu nhân sự, Đệ quy Lambda với `auto&& self`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_lambda_syntax_and_captures.cpp -o lambda_captures
./lambda_captures

g++ -Wall -Wextra -std=c++17 02_generic_and_higher_order_lambdas.cpp -o lambda_generic
./lambda_generic

g++ -Wall -Wextra -std=c++17 03_lambdas_with_stl_algorithms.cpp -o lambda_stl
./lambda_stl

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o lambda_exercises
./lambda_exercises
```

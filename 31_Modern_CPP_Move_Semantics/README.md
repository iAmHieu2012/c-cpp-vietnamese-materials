# Bài 6.4: Move Semantics, Rvalue References (`&&`) & Perfect Forwarding (`std::move`, `std::forward`)

Tài liệu hệ thống hóa một trong những bước ngoặt hiệu năng lớn nhất của C++ hiện đại: phân biệt **Lvalue vs Rvalue**, tham chiếu Rvalue (**`T&&`**), bản chất thật sự của **`std::move()`**, cơ chế đánh cắp tài nguyên trong **$O(1)$** của **Move Constructor & Move Assignment Operator**, và kỹ thuật chuyển tiếp hoàn hảo (**Perfect Forwarding với `std::forward<T>()`**).

---

## 1. Phân Biệt Lvalue và Rvalue Dưới Góc Nhìn Bộ Nhớ (`01_lvalue_vs_rvalue_references.cpp`)

### 1.1. Hai tiêu chí nhận diện Lvalue vs Rvalue trong 1 giây

| Tiêu chí | Lvalue *(Locator Value)* | Rvalue *(Temporary / Read Value)* |
| :--- | :--- | :--- |
| **Định danh & Vòng đời** | Có **tên biến cụ thể**, tồn tại lâu dài qua nhiều câu lệnh trong scope. | Là **giá trị tạm thời không có tên**, sẽ bị **hủy ngay ở cuối dòng lệnh** (dấu `;`). |
| **Lấy địa chỉ bằng `&`?** | **ĐƯỢC** (`&x` hợp lệ 100%). | **CẤM** (Không thể viết `&42` hay `&(a + b)`). |
| **Ví dụ điển hình** | Biến `int x = 10;`, phần tử `arr[0]`, `*ptr`, chuỗi hằng `"Hello"`. | Hằng số `42`, biểu thức `x + y`, kết quả trả về theo giá trị của hàm `create_vector()`. |

### 1.2. Bảng quy tắc gắn kết của 3 loại Tham chiếu (`T&`, `const T&`, `T&&`)

| Loại tham chiếu | Nhận được Lvalue (`x`)? | Nhận được Rvalue (`10`, `x + y`, `std::move(x)`)? | Mục đích thiết kế trong C++ |
| :--- | :---: | :---: | :--- |
| **1. Lvalue Ref (`T&`)** | **CÓ** | Không | Sửa đổi trực tiếp một biến có tên truyền vào hàm. |
| **2. Const Lvalue Ref (`const T&`)** | **CÓ** | **CÓ** *(Kéo dài vòng đời biến tạm)* | Chỉ đọc dữ liệu (Fallback an toàn cho hàm Copy Constructor khi không có hàm Move). |
| **3. Rvalue Ref (`T&&`)** *(C++11)* | Không | **CÓ** *(Ưu tiên số 1 cho Rvalue!)* | **Nhận diện đối tượng tạm sắp chết** để "cướp" thẳng con trỏ Heap của nó trong **$O(1)$** mà không cần Copy! |

---

## 2. Sự Thật Về `std::move()` & Cơ Chế Move Constructor / Move Assignment (`02_move_constructor_and_assignment.cpp`)

### 2.1. Câu hỏi phỏng vấn kinh điển: *`std::move(x)` thực sự làm gì?*

> **Trả lời:** Bản thân hàm **`std::move(x)` KHÔNG HỀ di chuyển bất kỳ một byte dữ liệu nào** và không sinh ra lệnh CPU nào lúc chạy!  
> `std::move(x)` thực chất chỉ là một phép ép kiểu tĩnh **`static_cast<T&&>(x)`** nhằm nói với trình biên dịch: *"Biến `x` này tôi không cần dùng giá trị cũ của nó nữa, hãy coi nó như một **Rvalue tạm thời** để gọi **Move Constructor / Move Assignment** thay vì Copy!"*

```text
  Quá trình Move Vector / Buffer:   Buffer b2 = std::move(b1);   (Độ phức tạp: O(1) Time, 0 Heap Allocation!)

  TRƯỚC KHI MOVE:                                    SAU KHI MOVE (b2 cướp con trỏ, b1 về nullptr):
  b1 (Stack)              Mảng 1,000,000 ints (Heap) b1 (Stack - Rỗng an toàn)    b2 (Stack - Chủ sở hữu mới)
  ┌──────────────┐        ┌─────────────────────┐    ┌──────────────┐             ┌──────────────┐
  │ data: 0x9000 ├───────►│ [ 10, 20, 30, ... ] │    │ data: nullptr│             │ data: 0x9000 ├──────┐
  │ size: 10^6   │        └─────────────────────┘    │ size: 0      │             │ size: 10^6   │      ▼
  └──────────────┘                                   └──────────────┘             └──────────────┘  [Heap 0x9000]
```

### 2.2. Hai quy tắc bắt buộc khi cài đặt Move Semantics

1. **Đưa đối tượng nguồn (`other`) về trạng thái rỗng hợp lệ (`nullptr`):**
   * Ngay sau khi gán `this->data = other.data;`, bắt buộc phải đặt **`other.data = nullptr; other.size = 0;`**. Nếu quên, khi `other` bị hủy ở cuối hàm, Destructor `~other()` sẽ gọi `delete[]` làm sập luôn vùng nhớ mà đối tượng mới vừa nhận!
2. **Luôn đánh dấu từ khóa `noexcept`:**
   * `MyBuffer(MyBuffer &&other) noexcept` và `MyBuffer& operator=(MyBuffer &&other) noexcept`.
   * Các container như `std::vector` chỉ dám dùng Move Constructor khi mở rộng (`reallocate`) nếu hàm Move được cam kết `noexcept`. Thiếu `noexcept`, `std::vector` sẽ lùi về dùng Copy Constructor tốn $O(N)$!

---

## 3. Universal Reference (`T&&`) & Perfect Forwarding (`std::forward<T>`) (`03_perfect_forwarding.cpp`)

### 3.1. Cạm bẫy lớn nhất của C++11: *"Một biến Rvalue Reference có tên lại là một Lvalue!"*

Khi bạn viết hàm `void wrapper(int &&r)`, bên trong thân hàm `wrapper`, bản thân biến `r` **đã có tên và lấy được địa chỉ `&r`**, nên trình biên dịch coi biểu thức `r` là một **Lvalue**! Nếu từ trong `wrapper` bạn gọi tiếp `process(r)`, nó sẽ gọi bản `process(int&)` (Copy) chứ không gọi bản `process(int&&)` (Move)!

### 3.2. Phân biệt Rvalue Reference vs Universal (Forwarding) Reference & `std::forward`

```text
  1. void f(Widget&& w);                           --> Kiểu cố định (Widget) -> CHỈ nhận Rvalue (Rvalue Reference).
  2. template <typename T> void f(T&& arg);        --> Có suy luận kiểu T    -> NHẬN CẢ LVALUE LẪN RVALUE!
                                                       (Gọi là Universal Reference / Forwarding Reference).
```

* **Quy tắc Co cụm Tham chiếu (Reference Collapsing Rules):**
  * Nếu truyền **Lvalue** kiểu `U` vào `f(T&& arg)` $\implies$ `T` suy luận thành `U&`, và `U& &&` co lại thành **`U&` (Lvalue Ref)**.
  * Nếu truyền **Rvalue** kiểu `U` vào `f(T&& arg)` $\implies$ `T` suy luận thành `U`, và `arg` trở thành **`U&&` (Rvalue Ref)**.
* **`std::forward<T>(arg)` (Chuyển tiếp hoàn hảo — Perfect Forwarding):**
  * Khác với `std::move` (luôn ép vô điều kiện thành Rvalue), **`std::forward<T>(arg)`** bảo toàn 100% bản chất gốc của đối số:
    * Nếu ban đầu người gọi truyền **Lvalue** $\rightarrow$ chuyển tiếp đi dưới dạng **Lvalue (Copy/Ref)**.
    * Nếu ban đầu người gọi truyền **Rvalue** $\rightarrow$ chuyển tiếp đi dưới dạng **Rvalue (Move)**!
  * Đây chính là cơ chế lõi giúp C++ cài đặt `std::make_unique`, `std::make_shared` và `vector::emplace_back`!

---

## 4. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_lvalue_vs_rvalue_references.cpp`: Phân biệt Lvalue vs Rvalue, nạp chồng hàm phân biệt `int&`, `const int&`, `int&&` và bản chất ép kiểu của `std::move`.
* `02_move_constructor_and_assignment.cpp`: Cài đặt Move Semantics (`noexcept`) và đo hiệu năng thực tế khi `std::vector` mở rộng bộ nhớ.
* `03_perfect_forwarding.cpp`: Chứng minh biến `T&&` có tên là Lvalue, quy tắc Universal Reference và chuyển tiếp hoàn hảo bằng `std::forward<T>`.
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (`fast_swap<T>` bằng `std::move`, Benchmark Copy $O(N)$ vs Move $O(1)$, Tự viết `my_make_unique<T>` bằng Perfect Forwarding, Hàm đo thời gian `benchmark_call(F&&, Args&&...)`, Hàng đợi `MoveOnlyQueue` cho `unique_ptr`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_lvalue_vs_rvalue_references.cpp -o move_lval_rval
./move_lval_rval

g++ -Wall -Wextra -std=c++17 02_move_constructor_and_assignment.cpp -o move_ctor_assign
./move_ctor_assign

g++ -Wall -Wextra -std=c++17 03_perfect_forwarding.cpp -o move_forwarding
./move_forwarding

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o move_exercises
./move_exercises
```

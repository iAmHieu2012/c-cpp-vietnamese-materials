# Bài 3.2: 4 Tính Chất Cốt Lõi Của OOP (Đóng Gói, Kế Thừa, Đa Hình, Trừu Tượng)

Tài liệu hệ thống hóa 4 trụ cột của Lập trình Hướng đối tượng trong C++: **Đóng gói (Encapsulation)**, **Kế thừa (Inheritance)** & giải pháp **Diamond Problem**, **Đa hình (Polymorphism)** lúc biên dịch (Operator Overloading) & lúc chạy (`virtual`, sơ đồ bộ nhớ `vtable`/`vptr`, `Virtual Destructor`, cạm bẫy Object Slicing), và **Trừu tượng (Abstraction)** với Abstract Class / Interface.

---

## 1. Bảng Tổng Quan 4 Tính Chất Cốt Lõi Của OOP

| Tính chất | Bản chất kỹ thuật trong C++ | Công cụ & Cú pháp thực thi |
| :--- | :--- | :--- |
| **1. Đóng gói** *(Encapsulation)* | Gom dữ liệu và phương thức xử lý vào chung một lớp; che giấu trạng thái nội bộ để ngăn chặn truy cập/sửa đổi trái phép từ bên ngoài. | `private`, `protected`, `public`, Getter/Setter có kiểm tra ràng buộc, `const` method. |
| **2. Kế thừa** *(Inheritance)* | Lớp dẫn xuất (Derived) tái sử dụng và mở rộng thuộc tính/phương thức của lớp cơ sở (Base) theo mối quan hệ **"IS-A"** (Con là một Cha). | `class Derived : public Base`, `virtual public` (giải quyết Diamond Problem). |
| **3. Đa hình** *(Polymorphism)* | Cùng một lời gọi hàm hoặc toán tử nhưng thực thi hành vi khác nhau tùy theo kiểu dữ liệu hoặc đối tượng thực tế lúc chạy. | **Compile-time:** Function & Operator Overloading. **Runtime:** `virtual`, `override`, `final`, gọi qua con trỏ/tham chiếu lớp cha. |
| **4. Trừu tượng** *(Abstraction)* | Ẩn đi chi tiết cài đặt phức tạp bên dưới, chỉ công khai bộ giao diện (hợp đồng hành vi) bắt buộc các lớp con phải định nghĩa. | Phương thức thuần ảo `virtual void f() = 0;`, Abstract Class, Interface. |

---

## 2. Kế Thừa (Inheritance) & Bài Toán Hình Thoi (Diamond Problem)

### 2.1. Quy tắc chuyển đổi phạm vi khi Kế thừa & Thứ tự gọi Constructor/Destructor

| Thành viên ở lớp cha (`Base`) | Kế thừa `public Base` *(Chuẩn 99%)* | Kế thừa `protected Base` | Kế thừa `private Base` |
| :--- | :---: | :---: | :---: |
| **`public`** | Giữ nguyên **`public`** | Hạ xuống `protected` | Hạ xuống `private` |
| **`protected`** | Giữ nguyên **`protected`** | Giữ nguyên `protected` | Hạ xuống `private` |
| **`private`** | Ẩn (truy cập qua Getter/Setter của cha) | Ẩn | Ẩn |

* **Thứ tự khởi tạo & hủy (Chuẩn phỏng vấn):**
  * **Khi Tạo đối tượng lớp con:** Constructor chạy **từ Cha xuống Con** (`Base()` $\rightarrow$ `Derived()`).
  * **Khi Hủy đối tượng lớp con:** Destructor chạy **ngược từ Con lên Cha** (`~Derived()` $\rightarrow$ `~Base()`).

### 2.2. Đa kế thừa & Giải pháp `virtual public` cho Diamond Problem

Khi lớp `D` kế thừa từ cả `B` và `C`, trong khi `B` và `C` lại cùng kế thừa từ lớp gốc `A`, đối tượng `D` sẽ chứa **2 bản sao vùng nhớ của `A`** gây lỗi nhập nhằng (`request for member is ambiguous`):

```text
       Kế thừa thường (LỖI NHẬP NHẰNG)            Kế thừa ảo (virtual public - CHUẨN)
               A           A                                     A (1 bản duy nhất)
               │           │                                    ╱ ╲
               B           C                         (virtual) B   C (virtual)
                ╲         ╱                                     ╲ ╱
                     D                                           D
       [ B::A | B | C::A | C | D ]                    [ vptr_B | B | vptr_C | C | D | A ]
```

* **Giải pháp:** Khai báo `class B : virtual public A` và `class C : virtual public A`. Khi đó lớp con dưới cùng (`D`) chịu trách nhiệm gọi trực tiếp Constructor của lớp gốc `A(args)` trong Member Initializer List.

---

## 3. Đa Hình Compile-Time: Nạp Chồng Toán Tử (Operator Overloading)

Nạp chồng toán tử cho phép các kiểu dữ liệu tự định nghĩa (`Complex`, `Fraction`, `Matrix`, `MyString`) sử dụng trực tiếp các ký hiệu `+`, `-`, `*`, `==`, `[]`, `<<` tự nhiên như kiểu nguyên thủy:

| Nhóm toán tử | Nên viết làm Method hay `friend`? | Mẫu khai báo chuẩn công nghiệp |
| :--- | :--- | :--- |
| **Toán tử 2 ngôi đối xứng** (`+`, `-`, `*`, `/`, `==`, `!=`, `<`) | Hàm `friend` hoặc Method `const` | `Complex operator+(const Complex &rhs) const;` |
| **Toán tử tự gán / sửa đổi** (`=`, `+=`, `-=`, `[]`, `()`, `->`) | **Bắt buộc là Method của Class** | `double& operator[](size_t i);` và `const double& operator[](size_t i) const;` *(Cho đối tượng `const`)* |
| **Tăng / Giảm** (`++a` vs `a++`) | Method của Class | **Tiền tố (`++a`):** `T& operator++();` và **Hậu tố (`a++`):** `T operator++(int);` *(Tham số `int` giả để phân biệt)* |
| **Luồng Nhập / Xuất** (`<<`, `>>`) | **Bắt buộc là hàm `friend`** *(vì vế trái là `std::ostream`)* | `friend std::ostream& operator<<(std::ostream &os, const T &x);` và `friend std::istream& operator>>(std::istream &is, T &x);` |

> **4 Toán tử KHÔNG THỂ nạp chồng trong C++:** Toán tử phân giải phạm vi **`::`**, toán tử dấu chấm **`.`**, toán tử con trỏ thành viên **`.*`**, và toán tử 3 ngôi **`?:`** (cùng `sizeof`).

---

## 4. Đa Hình Runtime: Cơ Chế Bộ Nhớ `vtable` / `vptr` & Virtual Destructor

### 4.1. Sơ đồ giải mã `vptr` và `vtable` dưới thanh RAM

Ngay khi một lớp có ít nhất một phương thức `virtual`, trình biên dịch tự động thiết lập cơ chế **Dynamic Dispatch** (Liên kết động lúc chạy):

```text
  Bộ nhớ Object trên Stack/Heap              Bảng hàm ảo (vtable ở vùng .rodata)        Mã máy (Text Segment)
  ┌─────────────────────────────┐            ┌──────────────────────────────────┐
  │ Circle c(5.0):              │            │ vtable của lớp Circle:           │
  │  • vptr (8 bytes ẩn ở đầu) ─┼───────────►│  [0] &Circle::area()             ├─────► Tính PI * r * r
  │  • radius = 5.0 (8 bytes)   │            │  [1] &Circle::draw()             ├─────► Vẽ hình tròn
  └─────────────────────────────┘            │  [2] &Circle::~Circle()          │
                                             └──────────────────────────────────┘
  Khi gọi qua con trỏ lớp cha: Shape *s = &c; s->area();
  CPU thực hiện:  (*(s->vptr)[0])(s)  --> Tra bảng vtable tại ô [0] và gọi đúng Circle::area()!
```

* **Chi phí của `virtual`:** Mỗi đối tượng tốn thêm **8 bytes** (trên hệ 64-bit) cho con trỏ ẩn `vptr`, và mỗi lần gọi hàm ảo tốn thêm 1 bước tra bảng `vtable` (Indirect Call).
* **Từ khóa `override` & `final` (C++11):**
  * **`override`:** Bắt buộc trình biên dịch kiểm tra xem ở lớp cha có đúng hàm `virtual` cùng chữ ký (tên, tham số, `const`) hay không. Nếu gõ nhầm tên hàm hoặc thiếu `const`, trình biên dịch báo lỗi ngay thay vì tạo hàm mới ngầm.
  * **`final`:** Cấm các lớp cháu ghi đè tiếp phương thức đó (hoặc đặt sau tên class `class Circle final` để cấm kế thừa tiếp).

### 4.2. Hai cạm bẫy Đa hình chết người (Phỏng vấn bắt buộc hỏi)

1. **Quên `virtual` ở Destructor của lớp cha (`virtual ~Base()`):**
   * Khi viết `Shape *s = new Circle(5.0); delete s;`:
   * Nếu `~Shape()` **không phải `virtual`**, trình biên dịch nhìn kiểu tĩnh `Shape*` và **chỉ gọi mỗi `~Shape()`**, bỏ qua hoàn toàn `~Circle()` $\rightarrow$ Toàn bộ bộ nhớ Heap của lớp con bị **Memory Leak**!
   * **Quy tắc bất di bất dịch:** Bất kỳ lớp nào được thiết kế làm lớp cơ sở (có hàm `virtual`) đều **bắt buộc phải có `virtual ~Base() = default;`**.
2. **Hiện tượng Cắt lát đối tượng (Object Slicing):**
   * Nếu bạn truyền hoặc gán đối tượng lớp con vào biến lớp cha **theo giá trị (Pass by Value)**: `Shape s = circle;` hoặc `void render(Shape s)`.
   * Phần dữ liệu riêng của `Circle` (`radius`) và `vptr` của `Circle` sẽ **bị cắt bỏ hoàn toàn**, chỉ còn lại phần xác của `Shape`!
   * **Quy tắc:** Đa hình Runtime trong C++ **CHỈ hoạt động thông qua Con trỏ (`Shape*`) hoặc Tham chiếu (`Shape&` / `const Shape&`)**.

---

## 5. Trừu Tượng (Abstraction): Abstract Class vs Interface

| Tiêu chí | Abstract Class (Lớp trừu tượng) | Interface (Giao diện thuần túy) |
| :--- | :--- | :--- |
| **Đặc điểm cấu trúc** | Có ít nhất **1 hàm thuần ảo (`virtual void f() = 0;`)**, có thể chứa thuộc tính (`id`, `name`) và các phương thức đã có sẵn thân hàm. | **Không có bất kỳ thuộc tính dữ liệu nào** (ngoài hằng `static constexpr`); **100% phương thức đều là thuần ảo (`= 0`)** kèm `virtual ~I() = default;`. |
| **Khởi tạo trực tiếp?** | **CẤM** (`Shape s;` báo lỗi biên dịch). Chỉ dùng làm con trỏ/tham chiếu lớp cha. | **CẤM**. Lớp con kế thừa bắt buộc phải cài đặt (implement) toàn bộ các hàm `= 0`. |
| **Ứng dụng thiết kế** | Gom chung dữ liệu và logic cốt lõi của một họ đối tượng (`Employee`, `Shape`, `Vehicle`). | Định nghĩa "Hợp đồng khả năng" (`IPayment`, `ISerializable`, `ILogger`), cho phép đa kế thừa nhiều Interface mà không sợ Diamond Problem. |

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_inheritance_and_diamond.cpp`: Đa kế thừa và giải quyết Diamond Problem bằng `virtual public`.
* `02_operator_overloading.cpp`: Nạp chồng `+`, `-`, `==`, `[]`, `<<`, `>>` cho lớp Số phức `Complex`.
* `03_polymorphism_and_vtable.cpp`: Đa hình Runtime (`virtual`, `override`, `final`), chứng minh kích thước `vptr` (+8 bytes), `dynamic_cast`, Object Slicing và `Virtual Destructor`.
* `04_abstraction_and_interfaces.cpp`: Phân biệt **Abstract Base Class** (`PaymentProcessor`) và **Pure Interface** (`ISerializable`, `IAuditable`) kết hợp đa kế thừa giao diện an toàn.
* `05_practice_exercises.cpp`: Giải 5 bài tập thực chiến (`Fraction` nạp chồng toán tử, `Animal` Zoo đa hình, `IPayment` Interface, `Employee` Payroll, `WorkingStudent` Diamond).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_inheritance_and_diamond.cpp -o 01_inheritance_and_diamond
./01_inheritance_and_diamond

g++ -Wall -Wextra -std=c++17 02_operator_overloading.cpp -o 02_operator_overloading
./02_operator_overloading

g++ -Wall -Wextra -std=c++17 03_polymorphism_and_vtable.cpp -o 03_polymorphism_and_vtable
./03_polymorphism_and_vtable

g++ -Wall -Wextra -std=c++17 04_abstraction_and_interfaces.cpp -o 04_abstraction_and_interfaces
./04_abstraction_and_interfaces

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

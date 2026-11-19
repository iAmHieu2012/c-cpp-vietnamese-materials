# Bài 3.1: Class & Object (Lớp, Đối Tượng, Constructor, Destructor, `this`, `static`, `friend`)

Tài liệu tổng hợp nền tảng Lập trình Hướng đối tượng (OOP) trong C++: cách tổ chức bộ nhớ của `class` và `object` trên RAM, kiểm soát phạm vi truy cập, vòng đời đối tượng qua Constructor/Destructor, từ khóa `explicit`, con trỏ `this`, thành viên tĩnh `static` và đặc quyền truy cập `friend`.

---

## 1. Class vs Object & Bản Chất Bộ Nhớ Dưới Thanh RAM

### 1.1. Phân biệt Class, Object và `struct`

* **Class (Lớp):** Là bản thiết kế (Blueprint) định nghĩa cấu trúc dữ liệu (**Attributes/Properties**) và hành vi (**Methods/Behaviors**). Bản thân định nghĩa `class` chỉ tồn tại lúc biên dịch, **không chiếm bộ nhớ RAM**.
* **Object (Đối tượng / Instance):** Là thực thể cụ thể được cấp phát vùng nhớ thực tế trên **Stack** hoặc **Heap** khi chương trình chạy.
* **Khác biệt duy nhất giữa `struct` và `class` trong C++:**
  * Thành viên và kế thừa của `struct` mặc định là **`public`**.
  * Thành viên và kế thừa của `class` mặc định là **`private`**.

### 1.2. Những gì nằm trong `sizeof(Object)`? (Trọng tâm phỏng vấn)

Nhiều người lầm tưởng một Object chứa cả biến lẫn hàm bên trong nó. Thực tế, C++ tối ưu bộ nhớ rất chặt chẽ:

```text
  Giả sử class Account có: int id (4B), double balance (8B), static int total_acc (4B), và 3 hàm.

  TRÊN STACK / HEAP (Mỗi Object độc lập)       TRÊN DATA/BSS SEGMENT       TRÊN TEXT/CODE SEGMENT
  ┌────────────────────────────────────┐       ┌───────────────────────┐   ┌───────────────────────────┐
  │ Object acc1 (sizeof = 16B do Pad)  │       │ Account::total_acc    │   │ Mã máy các hàm thành viên:│
  │  • id = 101      (4B + 4B Padding) │       │ (Chỉ có 1 bản sao duy │   │  • Account::deposit()     │
  │  • balance = 500 (8B)              │       │  nhất cho cả Class)   │   │  • Account::get_balance() │
  ├────────────────────────────────────┤       └───────────────────────┘   └───────────────────────────┘
  │ Object acc2 (sizeof = 16B do Pad)  │                                                ▲
  │  • id = 102      (4B + 4B Padding) │ ──(Truyền ngầm con trỏ `this = &acc2` vào)─────┘
  │  • balance = 900 (8B)              │
  └────────────────────────────────────┘
```

* **Nằm trong `sizeof(Object)`:** Chỉ có các **biến thành viên không phải `static`** (cộng thêm byte **Struct Padding** căn chỉnh bộ nhớ, và con trỏ `vptr` 8 bytes nếu lớp có hàm `virtual`).
* **KHÔNG nằm trong `sizeof(Object)`:** Biến `static` (nằm ở Data/BSS Segment) và tất cả các phương thức/hàm thành viên (nằm ở Text Segment, dùng chung cho mọi Object thông qua con trỏ ẩn `this`).
* *Lưu ý:* Một `class Empty {};` không có thuộc tính nào vẫn có `sizeof(Empty) == 1 byte` để đảm bảo hai đối tượng khác nhau luôn có hai địa chỉ RAM phân biệt.

---

## 2. Phạm Vi Truy Cập (Access Modifiers) & `const` Member Function

| Access Modifier | Bên trong chính Class | Lớp con kế thừa (Derived) | Bên ngoài Class (`main`) | Mục đích thiết kế |
| :--- | :---: | :---: | :---: | :--- |
| **`private`** | **Được** | Cấm | Cấm | **Đóng gói (Encapsulation):** Giấu trạng thái nội bộ, ngăn bên ngoài gán giá trị bất hợp lệ. |
| **`protected`** | **Được** | **Được** | Cấm | Mở quyền cho các lớp con truy cập trực tiếp khi Kế thừa (Bài 3.2). |
| **`public`** | **Được** | **Được** | **Được** | Cung cấp giao diện tương tác an toàn (Constructors, Getters, Setters, Business Methods). |

### Quy tắc vàng với `const` Member Function

* Cú pháp: Đặt từ khóa `const` ở **cuối khai báo hàm**: `double get_balance() const;`
* **Bản chất:** Biến con trỏ ẩn `this` từ `ClassName * const this` thành **`const ClassName * const this`**, cam kết với trình biên dịch rằng hàm này **chỉ đọc dữ liệu**, tuyệt đối không sửa đổi bất kỳ thuộc tính nào của đối tượng.
* **Cạm bẫy thực tế:** Khi truyền đối tượng vào hàm theo chuẩn `void print(const BankAccount &acc)`, bạn **chỉ có thể gọi được các phương thức có đánh dấu `const`**. Nếu quên gắn `const` cho hàm Getter, trình biên dịch sẽ báo lỗi ngay lập tức!

---

## 3. Constructor, Member Initializer List & Destructor

### 3.1. Bảng phân loại Constructor & Destructor

| Thành phần | Cú pháp tiêu biểu | Thời điểm tự động kích hoạt | Vai trò cốt lõi |
| :--- | :--- | :--- | :--- |
| **Default Constructor** | `MyClass() = default;` | Khi tạo đối tượng không truyền tham số: `MyClass a;` | Khởi tạo trạng thái mặc định ban đầu. |
| **Parameterized Ctor** | `MyClass(int x) : val(x) {}` | Khi tạo đối tượng kèm đối số: `MyClass a(10);` | Khởi tạo trực tiếp thuộc tính với dữ liệu đầu vào. |
| **Copy Constructor** | `MyClass(const MyClass &o)` | Khi `MyClass b = a;` hoặc truyền/trả đối tượng theo tham trị. | Tạo bản sao từ đối tượng có sẵn (**Deep Copy** nếu quản lý Heap). |
| **Destructor** | `~MyClass() { ... }` | Khi đối tượng trên Stack ra khỏi khối `{ }` hoặc khi gọi `delete`. | Thu hồi vùng nhớ Heap, đóng File/Socket (**Triết lý RAII**). |

### 3.2. Tại sao luôn ưu tiên Member Initializer List (`: a(x), b(y)`)?

1. **Hiệu năng cao hơn:** Khởi tạo trực tiếp các biến thành viên (đặc biệt là `std::string`, `std::vector`, đối tượng lớp khác) ngay lúc cấp phát bộ nhớ. Nếu gán trong thân `{ a = x; }`, C++ phải gọi Default Constructor tạo đối tượng rỗng trước rồi mới gọi `operator=` ghi đè lên (tốn 2 bước).
2. **4 Trường hợp BẮT BUỘC phải dùng Initializer List:**
   * Thuộc tính là hằng số (`const int id;`).
   * Thuộc tính là tham chiếu (`int &ref;`).
   * Thuộc tính là đối tượng của một lớp khác không có Default Constructor.
   * Gọi Constructor của lớp cha (`Base(args)`) hoặc **Delegating Constructor** (C++11: một Constructor ủy quyền gọi Constructor khác trong cùng class: `Box() : Box(1.0, 1.0) {}`).
3. **Cạm bẫy thứ tự khởi tạo:** Các thuộc tính luôn được khởi tạo **theo đúng thứ tự khai báo trong `class`**, hoàn toàn KHÔNG phụ thuộc vào thứ tự bạn viết trên dòng Initializer List!

### 3.3. Từ khóa `explicit`, `= default` và `= delete` (C++11)

* **`explicit`:** Đặt trước Constructor 1 tham số (`explicit IntArray(int size);`) để **cấm trình biên dịch tự động ép kiểu ngầm** (ví dụ cấm viết `IntArray arr = 10;` mà bắt buộc viết rõ `IntArray arr(10);`).
* **`= default`:** Yêu cầu trình biên dịch tự sinh Default Constructor ngay cả khi ta đã viết Parameterized Constructor.
* **`= delete`:** Cấm hoàn toàn một hàm (ví dụ khóa Copy Constructor `MyClass(const MyClass&) = delete;` để chống sao chép nông).

---

## 4. Con Trỏ `this`, Thành Viên `static` & Đặc Quyền `friend`

### 4.1. Con trỏ `this`

* Là con trỏ hằng ẩn (`ClassName * const this`) được trình biên dịch tự động truyền vào mọi phương thức không phải `static`, lưu địa chỉ của chính đối tượng đang gọi hàm.
* **2 ứng dụng thực chiến:**
  1. Phân biệt biến thành viên trùng tên với tham số: `this->width = width;`.
  2. **Method Chaining (Chuỗi lời gọi hàm):** Khi phương thức trả về `return *this;` với kiểu trả về là tham chiếu `ClassName&`, ta có thể gọi nối tiếp trên cùng 1 đối tượng: `box.set_width(5).set_height(4).set_depth(3);`.

### 4.2. Thành viên `static` (Thuộc về cả Class, không thuộc riêng Object nào)

* **Biến `static`:** Nằm trên Data/BSS Segment, tồn tại từ lúc chương trình khởi chạy cho đến khi kết thúc. Mọi đối tượng của lớp đều chia sẻ chung 1 biến này (ứng dụng: đếm số lượng đối tượng đang sống, bộ sinh mã ID tự tăng `++next_id`).
  * *Mẹo C++17:* Có thể khởi tạo trực tiếp ngay trong class bằng `inline static int count = 0;` mà không cần định nghĩa lại ở bên ngoài class.
* **Phương thức `static`:** Gọi trực tiếp qua tên lớp (`Box::get_total_boxes()`). Vì không gắn với đối tượng cụ thể nào nên hàm `static` **không có con trỏ `this`** và chỉ truy cập được các thành viên `static` khác.

### 4.3. Hàm bạn (`friend function`) & Lớp bạn (`friend class`)

* Từ khóa `friend` cho phép một hàm tự do bên ngoài (như hàm nạp chồng toán tử `operator<<`) hoặc toàn bộ một `class` khác (như `Auditor`, `Node` với `LinkedList`) truy cập trực tiếp vào các thành viên `private` và `protected`.
* **3 Quy tắc của quan hệ `friend`:**
  1. **Không đối xứng (1 chiều):** `A` khai báo `friend class B;` thì `B` truy cập được `private` của `A`, nhưng `A` không truy cập được `private` của `B`.
  2. **Không bắc cầu:** `A` là bạn của `B`, `B` là bạn của `C` $\nRightarrow$ `A` không phải bạn của `C`.
  3. **Không kế thừa:** Tình bạn của lớp cha không tự động truyền xuống lớp con.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_class_object_access.cpp`: Đóng gói lớp `BankAccount`, kiểm soát `private`/`public`, Getter/Setter và `const` method.
* `02_constructors_destructors.cpp`: Thực nghiệm 3 loại Constructor, Member Initializer List, Deep Copy và thứ tự hủy LIFO của Destructor trên Stack/Heap.
* `03_this_static_friend.cpp`: Method Chaining bằng `this`, đếm số đối tượng bằng `static`, truy cập `private` qua `friend function` và `friend class`.
* `04_practice_exercises.cpp`: Giải 5 bài tập tổng hợp (`Rectangle`, `Student` tự cấp ID bằng `static`, `Vector2D` Method Chaining, `IntArray` RAII, `BankVault` & `Auditor`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_class_object_access.cpp -o oop_access
./oop_access

g++ -Wall -Wextra -std=c++17 02_constructors_destructors.cpp -o oop_ctor_dtor
./oop_ctor_dtor

g++ -Wall -Wextra -std=c++17 03_this_static_friend.cpp -o oop_this_static
./oop_this_static

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o oop_exercises
./oop_exercises
```

# Bài 3.3: Quản Lý Tài Nguyên Trong Class (Shallow vs Deep Copy, Rule of Three, Rule of Five & Rule of Zero)

Tài liệu tổng hợp cách quản lý tài nguyên cấp phát động (Heap, File, Socket) an toàn tuyệt đối bên trong `class` C++: trực quan hóa bộ nhớ của **Shallow Copy vs Deep Copy vs Move Semantics**, nguyên lý **Rule of Three**, **Rule of Five**, **Rule of Zero**, kỹ thuật **Copy-and-Swap Idiom**, và thiết kế lớp sở hữu độc quyền (Move-Only Class).

---

## 1. Trực Quan Hóa Bộ Nhớ: Shallow Copy vs Deep Copy vs Move Semantics

Khi một `class` sở hữu con trỏ quản lý vùng nhớ trên **Heap** (`int *data`), trình biên dịch mặc định chỉ sao chép giá trị địa chỉ của con trỏ (**Shallow Copy**). Hãy xem sự khác biệt dưới thanh RAM giữa 3 cơ chế:

```text
  1. SHALLOW COPY (Sao chép nông mặc định của Compiler — NGUY HIỂM!)
     Stack (Object a)         Heap                    Stack (Object b = a)
     ┌──────────────┐     ┌──────────────┐            ┌──────────────┐
     │ data: 0x5000 ├────►│ [10, 20, 30] │◄───────────┤ data: 0x5000 │
     └──────────────┘     └──────────────┘            └──────────────┘
     * Hậu quả: Sửa b làm hỏng a. Khi ra khỏi hàm, ~b() delete 0x5000, rồi ~a() delete 0x5000
       lần nữa ===> LỖI DOUBLE FREE (Sập chương trình ngay lập tức)!

  2. DEEP COPY (Sao chép sâu — Copy Constructor / Copy Assignment — O(N) Time)
     Stack (Object a)         Heap (Vùng 1)           Stack (Object b = a)        Heap (Vùng 2 mới)
     ┌──────────────┐     ┌──────────────┐            ┌──────────────┐        ┌──────────────┐
     │ data: 0x5000 ├────►│ [10, 20, 30] │            │ data: 0x8000 ├───────►│ [10, 20, 30] │
     └──────────────┘     └──────────────┘            └──────────────┘        └──────────────┘
     * Kết quả: Hai vùng nhớ Heap độc lập 100%, hủy an toàn nhưng tốn O(N) thời gian cấp phát & copy.

  3. MOVE SEMANTICS (Chuyển giao quyền sở hữu — Move Ctor / Move Assignment — O(1) Time)
     Stack (Tạm / other)      Heap (Giữ nguyên)       Stack (Object b = std::move(other))
     ┌──────────────┐     ┌──────────────┐            ┌──────────────┐
     │ data: nullptr│     │ [10, 20, 30] │◄───────────┤ data: 0x5000 │ (Cướp thẳng địa chỉ 0x5000!)
     └──────────────┘     └──────────────┘            └──────────────┘
     * Kết quả: Không cấp phát mới, không copy phần tử (O(1)). Khi ~other() chạy, delete nullptr là vô hại!
```

| Tiêu chí | Shallow Copy (Mặc định) | Deep Copy (Rule of 3) | Move Semantics (Rule of 5) |
| :--- | :--- | :--- | :--- |
| **Độ phức tạp** | $O(1)$ | **$O(N)$** Time & Space | **$O(1)$** Time & $O(1)$ Space |
| **Vùng nhớ Heap** | Dùng chung 1 địa chỉ (Lỗi!) | Tạo vùng nhớ mới độc lập | Chuyển quyền sở hữu vùng nhớ cũ |
| **Trạng thái đối tượng nguồn** | Bị dính chung với bản sao | Giữ nguyên vẹn 100% | Bị rút rỗng (`data = nullptr, size = 0`) |
| **An toàn khi hủy (`~dtor`)** | **Crash (Double Free)** | An toàn tuyệt đối | An toàn tuyệt đối |

---

## 2. Nguyên Lý Rule of Three, Rule of Five & Rule of Zero

### 2.1. Rule of Three (Chuẩn C++98/03)

Nếu một lớp trực tiếp quản lý tài nguyên thô (con trỏ Heap, File handle) và cần tự định nghĩa **1 trong 3** hàm đặc biệt sau, thì **bắt buộc phải định nghĩa đủ cả 3**:

1. **Destructor (`~MyClass()`):** Thu hồi tài nguyên (`delete[] data;`).
2. **Copy Constructor (`MyClass(const MyClass &other)`):** Cấp phát vùng nhớ mới và Deep Copy từ `other`.
3. **Copy Assignment Operator (`MyClass& operator=(const MyClass &other)`):**
   * **Bước 1 (Bắt buộc):** Kiểm tra tự gán **`if (this == &other) return *this;`** (Nếu thiếu bước này, lệnh `a = a;` sẽ tự `delete[]` mất dữ liệu của chính nó trước khi kịp copy!).
   * **Bước 2:** Cấp phát vùng nhớ mới, copy dữ liệu từ `other`, sau đó `delete[]` vùng nhớ cũ của `this`.
   * **Bước 3:** Trả về `return *this;`.

### 2.2. Rule of Five (Chuẩn C++11 trở lên)

Khi đối tượng nguồn là một giá trị tạm thời sắp bị hủy (**Rvalue**, hoặc được bọc trong `std::move()`), việc cấp phát mới rồi copy $N$ phần tử trong Deep Copy là sự lãng phí lớn. C++11 bổ sung thêm **2 hàm Move** nhận tham chiếu Rvalue (`&&`):

1. **Destructor (`~MyClass()`):** Thu hồi tài nguyên (`delete[] data;`).
2. **Copy Constructor (`MyClass(const MyClass &other)`):** Cấp phát vùng nhớ mới và Deep Copy từ `other`.
3. **Copy Assignment Operator (`MyClass& operator=(const MyClass &other)`):**
   * **Bước 1 (Bắt buộc):** Kiểm tra tự gán **`if (this == &other) return *this;`** (Nếu thiếu bước này, lệnh `a = a;` sẽ tự `delete[]` mất dữ liệu của chính nó trước khi kịp copy!).
   * **Bước 2:** Cấp phát vùng nhớ mới, copy dữ liệu từ `other`, sau đó `delete[]` vùng nhớ cũ của `this`.
   * **Bước 3:** Trả về `return *this;`.
4. **Move Constructor (`MyClass(MyClass &&other) noexcept`):**
   * Trỏ thẳng `this->data = other.data; this->size = other.size;`
   * Đặt lại `other.data = nullptr; other.size = 0;`
5. **Move Assignment Operator (`MyClass& operator=(MyClass &&other) noexcept`):**
   * Kiểm tra `if (this != &other)`, giải phóng `delete[] this->data;` cũ, cướp con trỏ của `other`, rồi đặt `other.data = nullptr;`.

> **Tại sao hàm Move BẮT BUỘC phải có từ khóa `noexcept`?**  
> Khi `std::vector<MyClass>` bị đầy và phải mở rộng (`reallocate`), nó chỉ dám dùng **Move Constructor ($O(1)$)** để chuyển các phần tử cũ sang vùng nhớ mới nếu Move Constructor được cam kết **`noexcept`** (không ném ngoại lệ). Nếu bạn quên viết `noexcept`, `std::vector` sẽ lùi về dùng **Copy Constructor ($O(N)$)** để đảm bảo an toàn ngoại lệ, làm mất sạch hiệu năng của Move Semantics!

### 2.3. Kỹ thuật Copy-and-Swap Idiom (Tối ưu hóa `operator=`)

Thay vì viết riêng 2 hàm Copy Assignment và Move Assignment dài dòng, bạn có thể gộp chung thành **1 hàm `operator=` duy nhất** nhận tham số **theo giá trị**:

```cpp
// Nhận tham số theo giá trị: Tự động gọi Copy Ctor (nếu truyền Lvalue) hoặc Move Ctor (nếu truyền Rvalue)
MyClass& operator=(MyClass other) noexcept {
    std::swap(this->data, other.data);
    std::swap(this->size, other.size);
    return *this;
    // Khi kết thúc hàm, biến cục bộ 'other' (đang giữ con trỏ cũ của this) tự động gọi Destructor dọn sạch!
}
```

* **3 Lợi ích vàng:** Không cần kiểm tra `if (this == &other)`, không lặp code giải phóng bộ nhớ, và đạt chuẩn **Strong Exception Guarantee** (nếu việc cấp phát cho `other` thất bại do hết RAM, đối tượng `this` ban đầu vẫn nguyên vẹn 100%).

### 2.4. Rule of Zero (Triết lý C++ Hiện đại)

* **Nguyên lý:** Các lớp nghiệp vụ thông thường (như `class Student`, `class Order`) **không nên tự quản lý con trỏ thô (`new`/`delete`)**. Thay vào đó, hãy dùng các thành viên đã tự quản lý tài nguyên chuẩn RAII như `std::string`, `std::vector`, `std::unique_ptr`, `std::shared_ptr`.
* Khi đó, bạn **không cần tự viết bất kỳ hàm nào** trong 5 hàm trên (0 Destructor, 0 Copy, 0 Move) — các hàm mặc định do trình biên dịch tự sinh ra đã thực hiện Deep Copy và Move hoàn hảo 100%!

---

## 3. Thiết Kế Lớp Sở Hữu Độc Quyền (Move-Only Class)

Một số tài nguyên hệ thống mang tính độc quyền, không thể nhân bản (như File đang mở `FILE*`, kết nối mạng `Socket`, khóa luồng `std::mutex`, hoặc con trỏ thông minh `std::unique_ptr`). Ta thiết kế lớp **Move-Only** bằng cách khóa Copy (`= delete`) và chỉ mở Move:

```cpp
class UniqueFile {
public:
    // 1. CẤM SAO CHÉP (Copy = delete)
    UniqueFile(const UniqueFile&) = delete;
    UniqueFile& operator=(const UniqueFile&) = delete;

    // 2. CHỈ CHO PHÉP CHUYỂN GIAO QUYỀN SỞ HỮU (Move noexcept)
    UniqueFile(UniqueFile &&other) noexcept;
    UniqueFile& operator=(UniqueFile &&other) noexcept;
};
```

---

## 4. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_shallow_vs_deep_copy.cpp`: Minh họa Deep Copy độc lập vùng nhớ Heap với `SafeString`.
* `02_rule_of_three.cpp`: Cài đặt đầy đủ Destructor, Copy Constructor & Copy Assignment Operator (kèm chống Self-Assignment).
* `03_rule_of_five_move_semantics.cpp`: Cài đặt Move Constructor & Move Assignment Operator ($O(1)$) với `std::move` và `noexcept`.
* `04_practice_exercises.cpp`: Giải 5 bài tập tổng hợp (`MyString` Rule of 3, `Matrix2D` Rule of 5, Copy-and-Swap Idiom, `UniqueFile` Move-Only, `SimpleUniquePtr`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_shallow_vs_deep_copy.cpp -o oop_deep_copy
./oop_deep_copy

g++ -Wall -Wextra -std=c++17 02_rule_of_three.cpp -o oop_rule3
./oop_rule3

g++ -Wall -Wextra -std=c++17 03_rule_of_five_move_semantics.cpp -o oop_rule5
./oop_rule5

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o oop_res_ex
./oop_res_ex
```

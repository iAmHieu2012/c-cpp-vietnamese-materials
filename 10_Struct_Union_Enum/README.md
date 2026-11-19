# Bài 2.5: Kiểu Dữ Liệu Tự Định Nghĩa (`struct`, `union`, `enum` & Memory Padding)

Tài liệu tổng hợp cách gom nhóm dữ liệu bằng `struct`, cơ chế căn chỉnh bộ nhớ **Memory Alignment & Struct Padding** dưới phần cứng CPU, kỹ thuật nén **Bit-fields**, bản chất dùng chung RAM của `union`, và kiểu liệt kê an toàn `enum class` trong C++11.

---

## 1. Cấu Trúc (`struct`) & Quy Tắc Truy Cập Chuẩn

### 1.1. Khác biệt cú pháp giữa C và C++

* **Trong ngôn ngữ C:** Khi khai báo biến phải viết kèm từ khóa `struct`: `struct Student sv1;` (trừ khi dùng `typedef struct { ... } Student;`).
* **Trong C++:** Tên `struct` tự động trở thành một kiểu dữ liệu mới: `Student sv1;`. Ngoài ra, `struct` trong C++ hỗ trợ gán giá trị mặc định ngay khi khai báo (Default Member Initializer) và có thể chứa cả hàm thành viên (Method).

### 1.2. Toán tử dấu chấm (`.`) vs Toán tử mũi tên (`->`)

| Loại biến | Toán tử truy cập | Ví dụ | Bản chất bên dưới |
| :--- | :---: | :--- | :--- |
| **Biến trực tiếp** (`Student s`) hoặc **Tham chiếu** (`Student &r`) | **`.`** *(Dot)* | `s.gpa = 9.0;` | Truy cập trực tiếp vào vùng nhớ của đối tượng. |
| **Con trỏ** (`Student *p = &s`) | **`->`** *(Arrow)* | `p->gpa = 9.0;` | Cách viết gọn của `(*p).gpa = 9.0;` (Giải tham chiếu rồi mới truy cập). |

### 1.3. Quy tắc vàng khi truyền `struct` vào hàm

Một `struct` có thể nặng từ vài chục đến hàng trăm bytes. **Tuyệt đối tránh truyền Tham trị (`void print(Student s)`)** vì CPU sẽ tốn chi phí copy toàn bộ từng byte của `struct` lên Stack.

* **Chỉ đọc dữ liệu:** Dùng Tham chiếu hằng `void print(const Student &s)` *(Zero-copy + An toàn)*.
* **Cần sửa đổi dữ liệu:** Dùng Tham chiếu `void update(Student &s)` hoặc Con trỏ `void update(Student *s)`.

---

## 2. Memory Alignment & Struct Padding (Trọng Tâm Phỏng Vấn)

### 2.1. Tại sao `sizeof(struct)` thường LỚN HƠN tổng kích thước các biến bên trong?

CPU hiện đại đọc RAM theo từng cụm 4 bytes hoặc 8 bytes (**Word Size**), không đọc từng byte lẻ. Để CPU đọc một biến chỉ trong **1 chu kỳ xung nhịp (1 CPU cycle)**, trình biên dịch áp dụng **2 quy tắc Căn chỉnh bộ nhớ (Data Alignment)**:

1. **Quy tắc Offset:** Mọi biến có kích thước $K$ bytes (`char`=1, `short`=2, `int`=4, `double`=8) phải nằm ở vị trí địa chỉ (offset) **chia hết cho $K$**.
2. **Quy tắc Tổng kích thước:** Tổng `sizeof(struct)` phải **chia hết cho kích thước của thành viên lớn nhất** trong struct đó (để khi tạo mảng `struct arr[10]`, phần tử `arr[1]` cũng tự động được căn chỉnh đúng).

Để thỏa mãn 2 quy tắc này, trình biên dịch tự động chèn các byte trống vô dụng (**Padding Bytes**) vào giữa hoặc cuối `struct`.

### 2.2. Sơ đồ RAM: `BadLayout` (12 bytes) vs `GoodLayout` (8 bytes)

```text
  1. BadLayout { char a; int b; char c; }  ---> TỔNG: 12 BYTES (Lãng phí 6 bytes Padding!)
  Offset:   0     1    2    3     4    5    6    7     8     9   10   11
         ┌─────┬────┬────┬────┬─────────────────────┬─────┬────┬────┬────┐
         │  a  │Pad │Pad │Pad │        int b        │  c  │Pad │Pad │Pad │
         └─────┴────┴────┴────┴─────────────────────┴─────┴────┴────┴────┘
          (1B)   (3B trống)           (4B)           (1B)    (3B trống)

  2. GoodLayout { int b; char a; char c; } ---> TỔNG: 8 BYTES (Tiết kiệm ngay 33% RAM!)
  Offset:   0    1    2    3     4     5     6    7
         ┌─────────────────────┬─────┬─────┬────┬────┐
         │        int b        │  a  │  c  │Pad │Pad │
         └─────────────────────┴─────┴─────┴────┴────┘
                  (4B)          (1B)  (1B)  (2B trống)
```

* **Mẹo tối ưu RAM không tốn sức:** Luôn khai báo các biến thành viên trong `struct` theo thứ tự **kích thước giảm dần** (`double`/con trỏ 8B $\rightarrow$ `int`/`float` 4B $\rightarrow$ `short` 2B $\rightarrow$ `char`/`bool` 1B).
* **Tắt Padding bằng `#pragma pack(push, 1)`:** Ép trình biên dịch xếp khít các biến không chèn byte trống (`sizeof` = 6 bytes). **Lưu ý:** Chỉ dùng khi đóng gói gói tin mạng (Network Packet) hoặc đọc/ghi Header của file nhị phân (`.bmp`, `.wav`), vì tắt Padding làm CPU phải đọc 2 lần mới lấy đủ 1 biến `int` (giảm tốc độ thực thi).

---

## 3. Kỹ Thuật Nén Bit-Fields (Trường Bit)

Khi làm việc với lập trình nhúng (Embedded), giao thức mạng, hoặc lưu nhiều cờ trạng thái (`bool`), thay vì tốn 1 byte (8 bits) cho mỗi cờ, bạn có thể chỉ định **chính xác số bit** cho từng biến thành viên trong `struct`:

```cpp
// Thay vì tốn 3 biến unsigned int = 12 bytes, Bit-field nén tất cả vào ĐÚNG 4 BYTES (32 bits)!
struct FilePermissions {
    unsigned int read    : 1; // Chỉ tốn 1 bit  (giá trị 0 hoặc 1)
    unsigned int write   : 1; // Chỉ tốn 1 bit  (giá trị 0 hoặc 1)
    unsigned int execute : 1; // Chỉ tốn 1 bit  (giá trị 0 hoặc 1)
    unsigned int role_id : 5; // Tốn 5 bits     (lưu được số từ 0 đến 31)
};
```

* **Giới hạn của Bit-fields:** Không thể lấy địa chỉ (`&perm.read`) hoặc tạo tham chiếu tới một thành viên Bit-field, vì RAM chỉ đánh địa chỉ tới từng Byte chứ không đánh địa chỉ tới từng Bit.

---

## 4. Hợp (`union`) vs Cấu Trúc (`struct`)

| Tiêu chí | `struct` (Cấu trúc) | `union` (Hợp) |
| :--- | :--- | :--- |
| **Cấp phát bộ nhớ** | Mỗi thành viên có **một vùng nhớ riêng biệt**. | Tất cả thành viên **DÙNG CHUNG 1 vùng nhớ bắt đầu tại cùng địa chỉ**. |
| **Kích thước (`sizeof`)** | $\ge$ Tổng kích thước tất cả thành viên (cộng thêm Padding). | Bằng đúng **kích thước của thành viên lớn nhất** (có thể cộng thêm Padding). |
| **Truy cập đồng thời** | Lưu và đọc đồng thời **tất cả** các thành viên cùng lúc. | Tại một thời điểm chỉ lưu hợp lệ **duy nhất 1 thành viên** (ghi biến này sẽ đè lên biến kia). |
| **Ứng dụng thực tế** | Mô hình hóa đối tượng thực tế (Sinh viên, Điểm 2D, Hóa đơn). | Tiết kiệm RAM tối đa, tách byte (như tách IPv4 32-bit thành 4 octets), làm **Tagged Union** (tiền đề của `std::variant`). |

---

## 5. Kiểu Liệt Kê: `enum` (C) vs `enum class` (C++11)

Kiểu liệt kê giúp thay thế các "con số ma thuật" (Magic Numbers như `0, 1, 2`) bằng các tên hằng có ý nghĩa rõ ràng.

| Tiêu chí | `enum` truyền thống (C / C++98) | `enum class` (Scoped Enum - C++11 trở lên) |
| :--- | :--- | :--- |
| **Phạm vi tên (Scope)** | Rò rỉ tên ra ngoài: Hai `enum` khác nhau **không được** có phần tử trùng tên (ví dụ `Color::RED` và `TrafficLight::RED` sẽ báo lỗi trùng tên!). | **Đóng gói chặt trong phạm vi:** Bắt buộc gọi kèm tên `Color::RED` và `TrafficLight::RED` $\rightarrow$ Không bao giờ đụng độ tên. |
| **An toàn kiểu (Type Safety)** | Tự động ép kiểu ngầm sang `int` (có thể vô tình so sánh `Color::RED == Status::OK` mà trình biên dịch không báo lỗi!). | **Cấm ép kiểu ngầm sang `int`:** Muốn lấy mã số nguyên phải ép kiểu tường minh `static_cast<int>(st)`. |
| **Kiểm soát kích thước RAM** | Thường mặc định 4 bytes (`int`). | Cho phép chỉ định kiểu nền tiết kiệm RAM: `enum class Status : uint8_t { ... };` (chỉ tốn 1 byte!). |

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_struct_basics_and_pointers.cpp`: Khai báo `struct` lồng nhau, phân biệt toán tử `.` và `->`, truyền `const &` và cấp phát động mảng `struct`.
* `02_memory_alignment_padding.cpp`: Dùng `sizeof()` và `offsetof()` kiểm chứng Struct Padding trên `BadLayout`, `GoodLayout` và `#pragma pack(1)`.
* `03_union_and_enum_class.cpp`: Kiểm chứng địa chỉ trùng khớp của `union` qua bài toán tách IPv4 32-bit và sử dụng `enum class`.
* `04_practice_exercises.cpp`: Giải 5 bài tập tổng hợp (`struct Fraction`, Hình học 2D, Quản lý Sinh viên bằng `struct`, Khoảng cách thời gian `struct Time`, và `TaggedValue` kết hợp `union` + `enum class`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_struct_basics_and_pointers.cpp -o struct_basics
./struct_basics

g++ -Wall -Wextra -std=c++17 02_memory_alignment_padding.cpp -o struct_padding
./struct_padding

g++ -Wall -Wextra -std=c++17 03_union_and_enum_class.cpp -o union_enum
./union_enum

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o struct_practice
./struct_practice
```

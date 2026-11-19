# Mini-Project (Stage 2): Hệ Thống Quản Lý Sinh Viên (Heap + Struct + Binary I/O)

Dự án tổng kết **Giai đoạn 2 (Kỹ thuật lập trình C/C++)**. Kế thừa toàn bộ nghiệp vụ CRUD, thống kê và xếp loại từ Giai đoạn 1, đồng thời nâng cấp triệt để kiến trúc bên dưới: xóa bỏ hoàn toàn Mảng song song và giới hạn cứng `MAX = 100`, chuyển sang **`struct` POD**, **Mảng động tự co giãn trên Heap**, **Chuẩn hóa chuỗi**, **Con trỏ hàm (Callback)** và **Lưu/Đọc File nhị phân (`.bin`)**.

---

## 1. Cấu Trúc Thư Mục & Lệnh Biên Dịch 2

```text
MiniProject2_StudentManager/
├── README.md                  <-- Tài liệu mô tả kiến trúc, tính năng & kỹ thuật áp dụng
├── student_manager.cpp        <-- Mã nguồn chính của hệ thống (Stage 2 - Dynamic Struct Array)
└── students.bin               <-- File cơ sở dữ liệu nhị phân (tự động sinh ra khi lưu)
```

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 student_manager.cpp -o student_manager
./student_manager
```

---

## 2. Các Tính Năng Chính Của Hệ Thống 2

| Phím | Chức năng | Mô tả chi tiết nghiệp vụ & Ràng buộc dữ liệu |
| :-: | :--- | :--- |
| **`1`** | **In bảng điểm** | Kẻ bảng định dạng chuẩn bằng `<iomanip>`, hiển thị ID, Họ Tên đã chuẩn hóa, GPA và tự động xét **Xếp loại học lực**. |
| **`2`** | **Thêm sinh viên** | Tự động **nhân đôi `capacity` trên Heap** khi đầy (không giới hạn sĩ số), chống trùng lặp Mã SV (`ID`), tự chuẩn hóa Họ Tên, ép nhập GPA $\in [0.0, 10.0]$. |
| **`3`** | **Cập nhật GPA** | Tìm kiếm theo Mã SV và ghi đè điểm số mới hợp lệ. |
| **`4`** | **Xóa sinh viên** | Xóa theo Mã SV bằng thuật toán dịch trái mảng ($O(N)$), tự động giảm `size`. |
| **`5`** | **Tra cứu theo ID** | Hiển thị thông tin chi tiết (Vị trí, Mã SV, Họ Tên, GPA, Xếp loại) của một sinh viên. |
| **`6`** | **Thống kê lớp học** | Tính GPA trung bình, liệt kê **tất cả** SV đạt điểm Cao nhất / Thấp nhất (xử lý đồng điểm bằng `EPS`), thống kê số lượng & tỷ lệ `%` từng loại học lực. |
| **`7`** | **Sắp xếp bảng điểm** | Dùng chung 1 hàm sắp xếp nhận **Con trỏ hàm (Callback)** cho 2 chế độ: Giảm dần theo GPA (đồng điểm xét ID tăng dần) hoặc Tăng dần theo ID. |
| **`8`** | **Lưu dữ liệu (`.bin`)** | Ghi nguyên khối mảng `Student` từ Heap xuống file nhị phân `students.bin` trong 1 lệnh `write()`. |
| **`0`** | **Lưu & Thoát** | Tự động lưu dữ liệu xuống `students.bin`, thu hồi toàn bộ bộ nhớ Heap (`delete[]`) chống Memory Leak và thoát chương trình. |

---

## 3. Kiến Trúc Kỹ Thuật Áp Dụng (Tổng Hợp Giai Đoạn 2)

### 3.1. Chuyển đổi từ Mảng Song Song (Stage 1) sang Mảng Động Struct trên Heap (Stage 2)

Thay vì tách rời `ids[]`, `scores[]` cố định trên Stack, toàn bộ thuộc tính được đóng gói vào `struct Student` và quản lý qua con trỏ cấp phát động trên **Heap**:

```text
  Trên STACK (struct StudentList)          Trên HEAP (Mảng động tự nhân đôi Capacity)
  ┌─────────────────────────┐              ┌──────────────────────┬──────────────────────┬───────────┐
  │ data (Student*) = 0x5A0 │─────────────►│ [0] ID: 1001         │ [1] ID: 1002         │ [2] Trống │
  │ size            = 2     │              │     "Nguyen Van An"  │     "Tran Thi Binh"  │           │
  │ capacity        = 4     │              │     GPA: 8.50        │     GPA: 9.25        │           │
  └─────────────────────────┘              └──────────────────────┴──────────────────────┴───────────┘
```

### 3.2. Bản đồ kiến thức Giai đoạn 2 được sử dụng trong code

1. **Phân vùng bộ nhớ & Cấp phát động (Bài 2.1 & 2.3):**
   * Quản lý mảng sinh viên trên **Heap** qua cặp `new Student[capacity]` và `delete[] list.data; list.data = nullptr;`.
   * Áp dụng chiến lược **Nhân đôi sức chứa (Doubling Capacity)** tương tự `std::vector` để đạt độ phức tạp thêm phần tử trung bình $O(1)$.
2. **Con trỏ & Con trỏ hàm (Bài 2.2):**
   * Truyền tiêu chí sắp xếp bằng **Con trỏ hàm (Callback)** `bool (*cmp)(const Student&, const Student&)` giúp viết đúng 1 hàm sắp xếp duy nhất nhưng hỗ trợ nhiều tiêu chí khác nhau.
3. **Xử lý chuỗi (Bài 2.4):**
   * Dùng `std::stringstream` kết hợp `<cctype>` (`std::toupper`, `std::tolower`) để lọc khoảng trắng thừa và chuẩn hóa Họ Tên.
   * Dùng `std::strncpy` kèm khóa ký tự kết thúc chuỗi `'\0'` ở cuối mảng `char full_name[50]` để chống tràn bộ đệm (Buffer Overflow).
4. **Kiểu dữ liệu tự định nghĩa (Bài 2.5):**
   * Đóng gói dữ liệu bằng `struct Student` dạng **POD (Plain Old Data)** (dùng `char[50]` thay vì `std::string`) để đảm bảo an toàn tuyệt đối khi ghi/đọc nhị phân.
5. **Thao tác với Tập tin nhị phân (Bài 2.7):**
   * Tự động nạp dữ liệu từ `students.bin` lúc khởi động: dùng `seekg(0, ios::end)` và `tellg()` đo tổng dung lượng file, chia cho `sizeof(Student)` để biết chính xác số bản ghi và cấp phát Heap 1 lần duy nhất.

---

## 4. Định Hướng Nâng Cấp Ở Giai Đoạn 3 (OOP)

* Đóng gói `Student` và `StudentList` thành `class Student` và `class StudentManager` với phạm vi truy cập `private`/`public`.
* Thay thế hàm `init_list()` và `free_list()` thủ công bằng **Constructor** và **Destructor** (tuân thủ **Rule of Three / Rule of Five**).
* Nạp chồng toán tử `<<` để in thông tin sinh viên trực tiếp bằng `std::cout << sv;`.

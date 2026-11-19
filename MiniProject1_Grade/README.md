# Mini-Project (Stage 1): Hệ Thống Quản Lý Điểm Sinh Viên

Dự án tổng kết **Giai đoạn 1 (Cơ sở lập trình C/C++)**. Chương trình xây dựng hệ thống quản lý bảng điểm sinh viên chạy trên giao diện Console (CLI) với đầy đủ các nghiệp vụ **CRUD** (Create, Read, Update, Delete), thống kê dữ liệu và sắp xếp đa tiêu chí, sử dụng thuần túy kiến thức từ Bài 1.1 đến Bài 1.5.

---

## 1. Cấu Trúc Thư Mục & Lệnh Biên Dịch 1

```text
MiniProject1_Grade/
├── README.md                  <-- Tài liệu mô tả kiến trúc, tính năng & kỹ thuật áp dụng
└── student_grade_manager.cpp  <-- Mã nguồn chính của hệ thống (Stage 1 - Parallel Arrays)
```

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 student_grade_manager.cpp -o student_manager
./student_manager
```

---

## 2. Các Tính Năng Chính Của Hệ Thống 1

| Phím | Chức năng | Mô tả chi tiết nghiệp vụ & Ràng buộc dữ liệu |
| :-: | :--- | :--- |
| **`1`** | **In bảng điểm** | Kẻ bảng định dạng chuẩn bằng `<iomanip>`, tự động xét học lực theo thang điểm 10. |
| **`2`** | **Thêm sinh viên** | Kiểm tra tràn danh sách (`MAX = 100`), chống trùng lặp Mã SV (`ID`), ép nhập điểm trong đoạn $[0.0, 10.0]$. |
| **`3`** | **Cập nhật điểm** | Tìm kiếm tuyến tính theo Mã SV và ghi đè điểm số mới hợp lệ. |
| **`4`** | **Xóa sinh viên** | Xóa theo Mã SV bằng thuật toán dịch trái mảng ($O(N)$), tự động giảm sĩ số lớp (`--n`). |
| **`5`** | **Tra cứu theo ID** | Tìm và hiển thị thông tin chi tiết (Vị trí, Mã SV, Điểm số, Xếp loại) của một sinh viên. |
| **`6`** | **Thống kê lớp học** | Tính điểm trung bình, liệt kê **tất cả** SV đạt điểm Cao nhất / Thấp nhất, thống kê tỷ lệ % từng loại học lực. |
| **`7`** | **Sắp xếp bảng điểm** | Sắp xếp giảm dần theo Điểm số; nếu đồng điểm thì ưu tiên Mã SV nhỏ hơn đứng trước (Tie-breaking). |
| **`0`** | **Thoát** | Kết thúc vòng lặp `do-while` và đóng chương trình. |

---

## 3. Kiến Trúc Kỹ Thuật Áp Dụng (Tổng Hợp Giai Đoạn 1)

### 3.1. Kỹ thuật Mảng Song Song (Parallel Arrays)

Vì ở Giai đoạn 1 chưa sử dụng `struct` hay `class`, dữ liệu của mỗi sinh viên được quản lý thông qua hai mảng 1 chiều có cùng kích thước và **đồng bộ tuyệt đối qua chỉ số `i`**:

```text
  Chỉ số (Index):       i = 0       i = 1       i = 2       i = 3       i = 4
                      ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  Mảng Mã SV (ids):   │   1001    │   1002    │   1003    │   1004    │   1005    │
                      └───────────┴───────────┴───────────┴───────────┴───────────┘
                          ▲           ▲           ▲           ▲           ▲
                          │ (Đồng bộ) │           │           │           │
                          ▼           ▼           ▼           ▼           ▼
                      ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  Mảng Điểm (scores): │   8.50    │   9.25    │   6.75    │   4.50    │   7.80    │
                      └───────────┴───────────┴───────────┴───────────┴───────────┘
```

* **Nguyên tắc sống còn:** Bất kỳ thao tác **Xóa (Shift Left)** hay **Sắp xếp (Swap)** nào làm thay đổi vị trí phần tử trên mảng `scores[]` đều **bắt buộc phải thực hiện y hệt trên mảng `ids[]`** để tránh râu ông nọ cắm cằm bà kia.

### 3.2. Bản đồ kiến thức Giai đoạn 1 được sử dụng trong code

1. **Biến, Kiểu dữ liệu & Sai số số thực (Bài 1.2):**
   * Sử dụng hằng số `const double EPS = 1e-9;` để so sánh hai điểm số thực (`std::abs(a - b) < EPS`), tránh hoàn toàn lỗi sai số dấu phẩy động chuẩn IEEE 754 khi tìm nhiều thủ khoa đồng điểm hoặc khi xét đồng hạng lúc sắp xếp.
   * Sử dụng `<iomanip>` (`std::setw`, `std::left`, `std::right`, `std::fixed`, `std::setprecision(2)`) để căn lề bảng dữ liệu.
2. **Cấu trúc điều khiển (Bài 1.3):**
   * Vòng lặp `do - while` kết hợp `switch - case` làm khung Menu điều hướng liên tục và kiểm tra hợp lệ dữ liệu đầu vào (Input Validation cho điểm số $\in [0.0, 10.0]$).
3. **Hàm & Truyền tham chiếu (Bài 1.4):**
   * Tách biệt hoàn toàn tầng giao diện nhập xuất ở `main()` và tầng xử lý nghiệp vụ ở các hàm con.
   * Sử dụng **Tham chiếu (`int &n`)** ở hàm `add_student()` và `delete_student_by_id()` để cập nhật trực tiếp sĩ số lớp sau khi thêm/xóa.
   * Sử dụng từ khóa **`const`** (`const int ids[]`) ở các hàm chỉ đọc (In bảng, Tìm kiếm, Thống kê) để bảo vệ dữ liệu gốc khỏi việc bị sửa nhầm.
4. **Kỹ thuật xử lý Mảng 1 chiều (Bài 1.5):**
   * Tìm kiếm tuyến tính (Linear Search) trả về chỉ số vị trí (hoặc `-1` nếu không tồn tại).
   * Kỹ thuật dịch mảng khi xóa phần tử và thuật toán **Selection Sort** có điều kiện phụ (Tie-breaking).

---

## 4. Định Hướng Nâng Cấp Ở Các Giai Đoạn Tiếp Theo

Hệ thống hiện tại ở Giai đoạn 1 có một số giới hạn cố hữu sẽ được giải quyết triệt để khi bạn học lên các giai đoạn sau:

* **Lên Giai đoạn 2 (Kỹ thuật lập trình):**
  * Gom `id`, `full_name` (chuỗi họ tên), `gpa` vào chung một cấu trúc **`struct Student`** để xóa bỏ sự bất tiện của Mảng song song.
  * Thay mảng tĩnh `MAX = 100` bằng **Mảng cấp phát động (`new` / `delete`)** co giãn linh hoạt.
  * Bổ sung tính năng **Lưu và Đọc dữ liệu từ File (`.csv` / `.bin`)** để không bị mất dữ liệu khi tắt chương trình.
* **Lên Giai đoạn 3 (Lập trình Hướng đối tượng - OOP):**
  * Đóng gói thành lớp `class Student` và `class StudentManager` với tính đóng gói (`private`/`public`) và nạp chồng toán tử nhập/xuất (`<<`, `>>`).

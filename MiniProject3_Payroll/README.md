# Mini-Project (Stage 3): Hệ Thống Quản Lý & Tính Lương Nhân Viên (OOP)

Dự án tổng kết **Giai đoạn 3 (Lập trình Hướng đối tượng với C++)**. Hệ thống mô phỏng quản lý nhân sự và tính quỹ lương công ty cho 3 nhóm đối tượng (**Nhân viên văn phòng**, **Kỹ sư**, **Quản lý**), áp dụng triệt để 4 tính chất OOP và nguyên lý quản lý tài nguyên **Rule of Five**.

---

## 1. Cấu Trúc Thư Mục & Lệnh Biên Dịch

```text
MiniProject3_Payroll/
├── README.md                  <-- Tài liệu kiến trúc OOP, sơ đồ kế thừa & kỹ thuật áp dụng
└── payroll_system.cpp         <-- Mã nguồn chính của hệ thống (Stage 3 - Full OOP)
```

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 payroll_system.cpp -o payroll_system
./payroll_system
```

---

## 2. Sơ Đồ Kiến Trúc Kế Thừa & Công Thức Tính Lương

```text
                       ┌───────────────────────────────────┐
                       │   <<Abstract Class>> Employee     │
                       │ - id, full_name, base_salary      │
                       │ + calc_salary() const = 0         │
                       │ + get_role() const = 0            │
                       │ + clone() const = 0               │
                       │ + virtual ~Employee()             │
                       └─────────────────┬─────────────────┘
              ┌──────────────────────────┼──────────────────────────┐
              ▼                          ▼                          ▼
  ┌───────────────────────┐  ┌───────────────────────┐  ┌───────────────────────┐
  │      OfficeStaff      │  │       Engineer        │  │        Manager        │
  │ - work_days           │  │ - overtime_hours      │  │ - position_allowance  │
  │ - daily_allowance     │  │ - overtime_rate       │  │ - team_size           │
  └───────────────────────┘  └───────────────────────┘  └───────────────────────┘
```

* **Nhân viên văn phòng (`OfficeStaff`):** $\text{Lương} = \text{Lương cơ bản} + (\text{Số ngày công} \times \text{Phụ cấp ngày})$.
* **Kỹ sư (`Engineer`):** $\text{Lương} = \text{Lương cơ bản} + (\text{Giờ OT} \times \text{Đơn giá OT})$.
* **Quản lý (`Manager`):** $\text{Lương} = \text{Lương cơ bản} + \text{Phụ cấp chức vụ} + (\text{Số nhân viên quản lý} \times 500,000)$.

---

## 3. Các Tính Năng Chính Của Hệ Thống

| Phím | Chức năng | Mô tả chi tiết nghiệp vụ & Ràng buộc |
| :-: | :--- | :--- |
| **`1`** | **In bảng lương** | In toàn bộ nhân viên định dạng bảng chuẩn, tự động gọi đa hình `calc_salary()`, `get_role()` và nạp chồng toán tử `<<`. |
| **`2`** | **Thêm nhân viên** | Chọn loại nhân viên (`OfficeStaff` / `Engineer` / `Manager`), tự động cấp Mã NV tăng dần bằng biến `static`, tự động mở rộng mảng động trên Heap. |
| **`3`** | **Xóa nhân viên** | Tìm theo ID, gọi `delete` giải phóng đối tượng qua **Virtual Destructor** và dịch mảng. |
| **`4`** | **Tra cứu theo ID** | Hiển thị thông tin chi tiết của nhân viên qua con trỏ lớp cha `Employee*`. |
| **`5`** | **Thống kê quỹ lương** | Tính tổng quỹ lương, lương trung bình, liệt kê nhân viên có thu nhập cao nhất và phân bổ quỹ lương theo từng chức vụ. |
| **`6`** | **Sắp xếp bảng lương** | Sắp xếp giảm dần theo Thực lĩnh (đồng lương xếp theo ID tăng dần) hoặc tăng dần theo ID. |
| **`7`** | **Test Deep Copy** | Khởi tạo bản sao `PayrollSystem backup = sys;` để kiểm chứng **Rule of Five** + **Virtual `clone()`** hoạt động độc lập, không bị lỗi Double Free. |
| **`0`** | **Thoát** | Ra khỏi scope `main()`, Destructor của `PayrollSystem` tự động `delete` toàn bộ đối tượng trên Heap (**RAII**). |

---

## 4. Bản Đồ Kiến Thức Giai Đoạn 3 Áp Dụng Trong Code

1. **Class, Object & `static` (Bài 3.1):**
   * Đóng gói toàn bộ dữ liệu trong `private`/`protected`, cung cấp `const` method cho các hàm chỉ đọc.
   * Sử dụng biến `static int next_id` trong lớp `Employee` để tự động sinh mã nhân viên duy nhất không trùng lặp.
2. **4 Tính chất cốt lõi của OOP (Bài 3.2):**
   * **Abstract Class:** `Employee` chứa các hàm thuần ảo (`= 0`), ngăn việc tạo đối tượng nhân viên chung chung.
   * **Đa hình Runtime:** Mảng `Employee** data` lưu chung cả 3 loại nhân viên, gọi `data[i]->calc_salary()` tự động tra `vtable` chạy đúng công thức từng loại.
   * **Virtual Destructor:** `virtual ~Employee() = default;` đảm bảo giải phóng sạch lớp con khi gọi `delete data[i]`.
   * **Operator Overloading:** Nạp chồng toán tử `<<` cho `Employee` (kết hợp hàm ảo `print_details()` để in đa hình).
3. **Quản lý tài nguyên — Rule of Five & Virtual `clone()` (Bài 3.3):**
   * Lớp `PayrollSystem` quản lý mảng động con trỏ `Employee**` trên Heap, cài đặt đầy đủ **5 hàm đặc biệt** (Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment).
   * Áp dụng kỹ thuật **Virtual Copy Constructor (`clone()`)** để thực hiện Deep Copy chính xác kiểu thực tế của từng nhân viên khi sao chép cả hệ thống.

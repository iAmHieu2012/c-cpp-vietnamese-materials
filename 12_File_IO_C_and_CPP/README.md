# Bài 2.7: Làm Việc Với File Trong C & C++ (Text File & Binary File)

Tài liệu tổng hợp cơ chế đọc/ghi dữ liệu bền vững xuống ổ cứng trong C (`<stdio.h>`) và C++ (`<fstream>`), phân biệt **Text File vs Binary File**, cách tránh cạm bẫy kinh điển `while (!fin.eof())`, và kỹ thuật **Binary Serialization** kết hợp truy cập ngẫu nhiên $O(1)$ bằng `seekg`/`seekp`.

---

## 1. So Sánh Trực Diện: Text File vs Binary File

| Tiêu chí | Text File (`.txt`, `.csv`, `.json`, `.log`) | Binary File (`.bin`, `.dat`, `.exe`, `.jpg`) |
| :--- | :--- | :--- |
| **Cách lưu trữ** | Chuyển mọi dữ liệu thành **chuỗi ký tự ASCII/UTF-8** (ví dụ số `12345` tốn 5 bytes ký tự `'1'..'5'`). | Lưu **nguyên bản từng bit/byte trên RAM** (ví dụ `int 12345` luôn tốn đúng 4 bytes nhị phân). |
| **Khả năng đọc hiểu** | Con người mở trực tiếp bằng Notepad/VS Code đọc được ngay. | Mở bằng Notepad sẽ hiện ký tự rác không đọc được (chỉ đọc bằng chương trình/Hex Editor). |
| **Tốc độ & Dung lượng** | **Chậm hơn & Tốn dung lượng hơn** (tốn bước chuyển đổi số $\leftrightarrow$ chuỗi). | **Siêu nhanh & Gọn nhẹ** (đọc/ghi nguyên khối RAM trực tiếp, không cần parse). |
| **Truy cập ngẫu nhiên** | Khó (vì độ dài các dòng không đều nhau, phải duyệt từ đầu). | **Truy cập $O(1)$** với các bản ghi cố định kích thước (`seekg(index * sizeof(Record))`). |

---

## 2. Bảng Chế Độ Mở File (File Modes) Trong C & C++

| Mục đích mở file | Ngôn ngữ C (`fopen`) | Ngôn ngữ C++ (`<fstream>`) | Hành vi thực tế |
| :--- | :---: | :--- | :--- |
| **Chỉ đọc (Read)** | `"r"` | `std::ifstream f(path);` *(hoặc `ios::in`)* | Mở file có sẵn để đọc. Trả về `NULL` / `!is_open()` nếu file không tồn tại. |
| **Ghi đè (Write)** | `"w"` | `std::ofstream f(path);` *(hoặc `ios::out \| ios::trunc`)* | **Tạo mới** nếu chưa có; **xóa sạch nội dung cũ** nếu file đã tồn tại! |
| **Ghi nối đuôi (Append)** | `"a"` | `std::ofstream f(path, std::ios::app);` | Giữ nguyên nội dung cũ, mọi lệnh ghi mới tự động nối vào **cuối file**. |
| **Đọc & Ghi (Read/Write)** | `"r+"` | `std::fstream f(path, std::ios::in \| std::ios::out);` | Mở file có sẵn để vừa đọc vừa cập nhật trực tiếp bên trong (không xóa file). |
| **Chế độ Nhị phân** | Thêm chữ **`b`**: `"rb"`, `"wb"`, `"ab"`, `"r+b"` | Kết hợp cờ **`\| std::ios::binary`**: `ios::out \| ios::binary` | **Bắt buộc** khi làm việc với file nhị phân để ngăn Hệ điều hành tự đổi `'\n'` thành `'\r\n'` trên Windows. |

---

## 3. Quy Trình Chuẩn & Cạm Bẫy Chết Người Với `eof()`

### 3.1. Cơ chế RAII của `<fstream>` (C++) vs `fclose()` (C)

* **Trong C:** Bắt buộc phải gọi `fclose(fp)` sau khi dùng xong để đẩy dữ liệu từ vùng đệm (Buffer) xuống ổ cứng và giải phóng File Descriptor. Quên `fclose()` có thể làm mất dữ liệu hoặc cạn kiệt tài nguyên hệ thống.
* **Trong C++:** `std::ifstream` / `std::ofstream` tuân thủ nguyên lý **RAII** — ngay khi đối tượng ra khỏi khối `{ }`, Destructor tự động flush buffer và đóng file an toàn.

### 3.2. Tại sao TUYỆT ĐỐI KHÔNG viết `while (!fin.eof())`?

Cờ `eof()` (End-Of-File) **chỉ được bật lên SAU KHI một lệnh đọc bị thất bại do chạm cuối file**, chứ không bật trước khi đọc!

* **Cách viết SAI (Lặp dòng cuối 2 lần):**

  ```cpp
  while (!fin.eof()) {
      std::getline(fin, line); // Lần cuối đọc thất bại nhưng vẫn chạy tiếp thân vòng lặp!
      std::cout << line << "\n";
  }
  ```

* **Cách viết CHUẨN CÔNG NGHIỆP (Đặt lệnh đọc trực tiếp trong `while`):**

  ```cpp
  // Đọc từng dòng (Text / CSV):
  while (std::getline(fin, line)) { ... }

  // Đọc từng từ / từng số cách nhau bởi khoảng trắng:
  while (fin >> num) { ... }

  // Đọc từng bản ghi nhị phân (Binary):
  while (fin.read(reinterpret_cast<char*>(&record), sizeof(record))) { ... }
  ```

---

## 4. Binary Serialization & Truy Cập Ngẫu Nhiên $O(1)$ (`seekg` / `seekp`)

### 4.1. Quy tắc sống còn khi ghi `struct` xuống file nhị phân (`.bin`)

* **CẤM** ghi trực tiếp một `struct` có chứa con trỏ (`int*`, `char*`) hoặc `std::string` / `std::vector` bằng `fout.write(...)`! Vì các kiểu đó chỉ lưu **địa chỉ RAM trên Heap**; khi tắt chương trình và mở lại, địa chỉ đó trở thành rác gây **Segmentation Fault**.
* **CHUẨN:** Chỉ dùng `fout.write` / `fin.read` trực tiếp cho các **POD Struct (Plain Old Data)** — sử dụng mảng ký tự cố định kích thước như `char full_name[50];`.

### 4.2. Điều khiển con trỏ đọc (`seekg` / `tellg`) & con trỏ ghi (`seekp` / `tellp`)

```text
  Cấu trúc file students.bin (Mỗi StudentRecord nặng 64 bytes):
  Byte offset:    0            64           128          192
                ┌────────────┬────────────┬────────────┬────────────┐
  Bản ghi:      │  Record 0  │  Record 1  │  Record 2  │  Record 3  │
                └────────────┴────────────┴────────────┴────────────┘
                ▲                         ▲                         ▲
             ios::beg          seekg(2 * 64, ios::beg)           ios::end
```

* **`seekg(offset, origin)`** *(Get pointer)*: Di chuyển con trỏ **đọc** (`ifstream`).
* **`seekp(offset, origin)`** *(Put pointer)*: Di chuyển con trỏ **ghi** (`ofstream` / `fstream`).
* **`tellg()` / `tellp()`**: Trả về vị trí byte hiện tại của con trỏ đọc/ghi.
* **3 mốc gốc (`origin`):**
  * `std::ios::beg`: Tính từ **đầu file** (byte 0).
  * `std::ios::cur`: Tính từ **vị trí hiện tại** (có thể truyền số âm để lùi lại).
  * `std::ios::end`: Tính từ **cuối file** (dùng để đo tổng dung lượng file: `fin.seekg(0, ios::end); size = fin.tellg();`).

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_c_file_io_stdio.c`: Đọc/ghi file văn bản chuẩn ngôn ngữ C với `FILE*`, `fopen`, `fprintf`, `fgets`, `fclose`.
* `02_cpp_text_file_fstream.cpp`: Ghi đè, ghi nối (`ios::app`) và đọc phân tích file CSV với `std::ofstream`, `std::ifstream` và `std::stringstream`.
* `03_binary_file_serialization.cpp`: Ghi mảng `struct` POD xuống file `.bin`, đo dung lượng file bằng `tellg()` và nhảy trực tiếp tới bản ghi thứ $k$ trong $O(1)$ bằng `seekg()`.
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Thống kê file văn bản, Copy file nhị phân theo khối 4KB, Thống kê dãy số từ file, Cập nhật điểm trực tiếp trong file `.bin` bằng `seekp()`, Ghi log hệ thống).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
gcc -Wall -Wextra 01_c_file_io_stdio.c -o c_file_io
./c_file_io

g++ -Wall -Wextra -std=c++17 02_cpp_text_file_fstream.cpp -o cpp_text_io
./cpp_text_io

g++ -Wall -Wextra -std=c++17 03_binary_file_serialization.cpp -o cpp_bin_io
./cpp_bin_io

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o file_practice
./file_practice
```

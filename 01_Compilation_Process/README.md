# Bài 1.1: Môi trường & Quá trình biên dịch (Compilation Process)

Tài liệu ghi chú kiến thức nền tảng về cách trình biên dịch (Compiler) chuyển đổi mã nguồn C/C++ của con người thành mã máy mà CPU có thể thực thi được.

---

## 1. Cấu trúc thư mục bài học

```text
01_Compilation_Process/
├── README.md       # Ghi chú lý thuyết và các câu lệnh CLI
├── hello.c         # Mã nguồn mẫu ngôn ngữ C
└── hello.cpp       # Mã nguồn mẫu ngôn ngữ C++
```

---

## 2. Phân biệt trình biên dịch: `gcc` vs `g++`

Trong bộ công cụ **GCC (GNU Compiler Collection)**:

| Tiêu chí | `gcc` (GNU C Compiler) | `g++` (GNU C++ Compiler) |
| :--- | :--- | :--- |
| **Ngôn ngữ mặc định** | Ngôn ngữ **C** (`.c`) | Ngôn ngữ **C++** (`.cpp`, `.cxx`, `.cc`) |
| **Thư viện chuẩn** | Tự động liên kết thư viện chuẩn của C (`libc`). **Không** tự động liên kết thư viện chuẩn C++ (`libstdc++`). | Tự động liên kết cả thư viện chuẩn của C và C++ (`libstdc++`). |
| **Lời khuyên** | Chỉ dùng cho các dự án viết thuần bằng **C**. | Dùng cho mọi dự án **C++**. |

> **Kiểm tra phiên bản cài đặt trên máy:**
>
> ```bash
> gcc --version
> g++ --version
> ```

---

## 3. Chi tiết 4 giai đoạn của quá trình biên dịch

Khi ta bấm nút **Run** trên IDE, thực chất trình biên dịch âm thầm thực hiện **4 bước nối tiếp nhau**:

```text
[Source Code]       (1. Preprocessor)       [Expanded Code]
hello.c / .cpp  ------------------------->      hello.i
                                                   |
                                                   | (2. Compiler)
                                                   v
[Object Code]        (3. Assembler)         [Assembly Code]
hello.o / .obj  <-------------------------      hello.s
      |
      | (4. Linker + Libraries)
      v
[Executable]
hello.exe / a.out
```

### Bước 1: Tiền xử lý (Preprocessing)

* **Đầu vào:** File mã nguồn (`.c` hoặc `.cpp`).
* **Đầu ra:** File mở rộng (`.i` với C hoặc `.ii` với C++).
* **Nhiệm vụ của Preprocessor:**
  1. Xóa bỏ toàn bộ các dòng ghi chú (Comments `//` và `/* ... */`).
  2. Sao chép toàn bộ nội dung của các file tiêu đề được khai báo ở chỉ thị `#include` (như `<stdio.h>`, `<iostream>`) dán thẳng vào file mã nguồn.
  3. Thay thế toàn bộ các hằng số và hàm Macro (`#define`) bằng giá trị thực của chúng.
  4. Xử lý các chỉ thị biên dịch có điều kiện (`#ifdef`, `#ifndef`, `#endif`).
* **Câu lệnh thực thi:**

  ```bash
  # Đối với C
  gcc -E hello.c -o hello.i

  # Đối với C++
  g++ -E hello.cpp -o hello.i
  ```

### Bước 2: Biên dịch sang Hợp ngữ (Compilation)

* **Đầu vào:** File đã qua tiền xử lý (`hello.i`).
* **Đầu ra:** File mã hợp ngữ Assembly (`hello.s`).
* **Nhiệm vụ của Compiler:**
  1. Kiểm tra lỗi cú pháp (Syntax Error), lỗi kiểu dữ liệu (Type checking). Nếu code sai cú pháp, quá trình sẽ dừng lại ở bước này và báo lỗi.
  2. Tối ưu hóa mã nguồn và dịch từ ngôn ngữ bậc cao (C/C++) sang ngôn ngữ hợp ngữ (**Assembly**) phụ thuộc vào kiến trúc CPU cụ thể (x86, x64, ARM...).
* **Câu lệnh thực thi:**

  ```bash
  gcc -S hello.i -o hello.s
  # hoặc với C++: g++ -S hello.i -o hello.s
  ```

### Bước 3: Dịch sang Mã máy (Assembly)

* **Đầu vào:** File hợp ngữ (`hello.s`).
* **Đầu ra:** File đối tượng chứa mã máy nhị phân (`hello.o` trên Linux/macOS hoặc `hello.obj` trên Windows).
* **Nhiệm vụ của Assembler:**
  1. Dịch các câu lệnh Assembly (`mov`, `call`, `push`, `ret`...) thành chuỗi bit nhị phân `0` và `1` (Machine Code) mà CPU hiểu được.
  2. Chưa thể chạy độc lập vì các lời gọi hàm từ thư viện bên ngoài (như `printf`, `std::cout`) mới chỉ là các ký hiệu chờ được kết nối.
* **Câu lệnh thực thi:**

  ```bash
  gcc -c hello.s -o hello.o
  # hoặc với C++: g++ -c hello.s -o hello.o
  ```

### Bước 4: Liên kết (Linking)

* **Đầu vào:** Một hoặc nhiều file `.o` + Các thư viện chuẩn/thư viện ngoài (`.lib`, `.a`, `.dll`, `.so`).
* **Đầu ra:** File thực thi hoàn chỉnh (`hello.exe` trên Windows hoặc `hello` trên Linux/macOS).
* **Nhiệm vụ của Linker:**
  1. Gom tất cả các file `.o` trong dự án lại với nhau.
  2. Tìm địa chỉ thực sự của các hàm thư viện (`printf`, `std::cout`) để nối vào vị trí gọi hàm trong code.
  3. Kiểm tra hàm khởi đầu `main()`. Nếu thiếu hàm `main()` hoặc gọi hàm chưa được định nghĩa, Linker sẽ báo lỗi kinh điển: **`undefined reference to ...`**.
* **Câu lệnh thực thi:**

  ```bash
  gcc hello.o -o hello.exe
  # hoặc với C++: g++ hello.o -o hello.exe
  ```

---

## 4. Câu lệnh biên dịch gộp (Dùng hàng ngày)

Trong thực tế, ta không gõ lẻ 4 bước trên mà gộp chung thành **1 lệnh duy nhất** (trình biên dịch sẽ tự tạo file tạm và xóa đi sau khi tạo xong file thực thi):

### 4.1. Biên dịch nhanh cơ bản

```bash
# Biên dịch và chạy file C
gcc hello.c -o hello_c
./hello_c

# Biên dịch và chạy file C++
g++ hello.cpp -o hello_cpp
./hello_cpp
```

### 4.2. Biên dịch chuẩn kỹ sư (Bật cảnh báo lỗi & chọn chuẩn ngôn ngữ)

Khi học và đi làm, luôn khuyến khích thêm các cờ (flags) sau để bắt lỗi tiềm ẩn:

```bash
# Cờ cho C (Chuẩn C11)
gcc -std=c11 -Wall -Wextra -Werror hello.c -o hello_c

# Cờ cho C++ (Chuẩn C++17 hoặc C++20)
g++ -std=c++17 -Wall -Wextra -Werror hello.cpp -o hello_cpp
```

* `-std=c++17`: Chỉ định sử dụng chuẩn C++17.
* `-Wall` & `-Wextra`: Bật toàn bộ các cảnh báo (Warnings) về code xấu hoặc có nguy cơ gây lỗi.
* `-Werror`: Ép mọi cảnh báo (Warning) trở thành lỗi (Error) để buộc lập trình viên phải sửa sạch trước khi chạy.
* `-g`: Giữ lại thông tin ký hiệu để phục vụ gỡ lỗi (Debug bằng GDB).
* `-O2` hoặc `-O3`: Bật chế độ tối ưu hóa tốc độ chạy của chương trình.

---

## 5. Tổng hợp các lỗi biên dịch thường gặp

1. **Preprocessor Error:** Không tìm thấy file `#include` (Ví dụ: `fatal error: iostrem: No such file or directory`).
2. **Compiler Error (Syntax Error):** Quên dấu chấm phẩy `;`, sai tên biến, sai kiểu dữ liệu.
3. **Linker Error (`ld returned 1 exit status`):**
   * Quên viết hàm `main()`.
   * Khai báo hàm (Prototype) nhưng quên viết phần thân hàm (Definition).
   * Dùng `gcc` để dịch file C++ có sử dụng `std::cout` mà không liên kết thư viện `stdc++`.

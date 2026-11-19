# Bài 1.2: Biến, Kiểu dữ liệu, Nhập xuất & Toán tử (C/C++)

Tài liệu tổng hợp kiến thức cốt lõi về cách dữ liệu được biểu diễn dưới dạng nhị phân trong RAM, đối chiếu cơ chế Nhập/Xuất giữa C và C++, cùng sổ tay các kỹ thuật thao tác Bit (Bitwise).

---

## 1. Bảng Tra Cứu Kiểu Dữ Liệu Nguyên Thủy (Trên hệ thống 64-bit chuẩn)

> **Lưu ý:** Kích thước thực tế phụ thuộc vào Kiến trúc CPU và Trình biên dịch. Luôn dùng toán tử `sizeof(type)` và thư viện `<climits>` / `<cfloat>` để kiểm tra chính xác trên máy của bạn.

| Kiểu dữ liệu | Kích thước | Số bit | Miền giá trị (Xấp xỉ) | Định dạng C (`printf`/`scanf`) |
| :--- | :---: | :---: | :--- | :---: |
| `bool` *(C++ / `<stdbool.h>`)* | 1 byte | 8 bits | `false` (`0`) hoặc `true` (`1`) | `%d` |
| `char` | 1 byte | 8 bits | $-128 \rightarrow 127$ (Bảng mã ASCII) | `%c` (ký tự), `%hhd` (số) |
| `unsigned char` | 1 byte | 8 bits | $0 \rightarrow 255$ | `%hhu` |
| `short` | 2 bytes | 16 bits | $-32,768 \rightarrow 32,767$ | `%hd` |
| `unsigned short` | 2 bytes | 16 bits | $0 \rightarrow 65,535$ | `%hu` |
| `int` | 4 bytes | 32 bits | $-2 \times 10^9 \rightarrow 2 \times 10^9$ ($\approx \pm 2.14 \text{ tỷ}$) | `%d` hoặc `%i` |
| `unsigned int` | 4 bytes | 32 bits | $0 \rightarrow 4.29 \times 10^9$ ($2^{32} - 1$) | `%u` |
| `long long` | 8 bytes | 64 bits | $-9.22 \times 10^{18} \rightarrow 9.22 \times 10^{18}$ | `%lld` |
| `unsigned long long` | 8 bytes | 64 bits | $0 \rightarrow 1.84 \times 10^{19}$ ($2^{64} - 1$) | `%llu` |
| `float` *(Độ chính xác đơn)* | 4 bytes | 32 bits | Chính xác **6 – 7 chữ số** thập phân | `%f` |
| `double` *(Độ chính xác kép)* | 8 bytes | 64 bits | Chính xác **15 – 17 chữ số** thập phân | `%lf` *(khi `scanf` bắt buộc `%lf`)* |
| `size_t` *(Kiểu kích thước)* | 4 hoặc 8 bytes | 32 / 64 bits | $0 \rightarrow 2^{32}-1$ (32-bit) hoặc $2^{64}-1$ (64-bit) | `%zu` |

---

## 2. Bản Chất Lưu Trữ Trên Bộ Nhớ (Memory Representation)

### 2.1. Quy tắc Bù 2 (Two's Complement) — Cách máy tính lưu số âm

Trong kiểu số nguyên có dấu (`signed`), **bit ngoài cùng bên trái (MSB - Most Significant Bit)** được dùng làm **Bit dấu**:

* `0`: Số dương (hoặc bằng 0).
* `1`: Số âm.

**Cách chuyển số dương $X$ sang số âm $-X$ trong hệ nhị phân (ví dụ với 8-bit, chuyển `5` thành `-5`):**

1. Biểu diễn số dương `5`: `0000 0101`
2. **Bù 1 (Đảo toàn bộ bit `~`):** `1111 1010`
3. **Bù 2 (Cộng thêm `1` vào kết quả bù 1):** `1111 1010 + 1 = 1111 1011` (Đây chính là `-5` trong RAM).

### 2.2. Hiện tượng Tràn số (Integer Overflow & Underflow)

Do số lượng bit là cố định, giá trị của biến hoạt động giống như một chiếc đồng hồ hình tròn:

```text
       INT_MAX (0111...111)  +  1  ===>  INT_MIN (1000...000) [Tràn trên - Overflow]
       INT_MIN (1000...000)  -  1  ===>  INT_MAX (0111...111) [Tràn dưới - Underflow]
  Unsigned (0) (0000...000)  -  1  ===>  UINT_MAX(1111...111) [Tràn ngược về cực đại]
```

* **Nguyên tắc sống còn:** Khi nhân hai biến `int` có giá trị khoảng $10^5$ trở lên, tích của chúng ($10^{10}$) sẽ vượt quá `INT_MAX` ($2 \times 10^9$). Bắt buộc phải ép kiểu trước khi nhân: `long long res = 1LL * a * b;`

### 2.3. Sai số Dấu phẩy động (Floating-Point Precision - Chuẩn IEEE 754)

* Máy tính lưu số thực dưới dạng nhị phân ($\frac{1}{2}, \frac{1}{4}, \frac{1}{8}...$), nên những số như `0.1` hay `0.2` bị lẻ vô hạn trong hệ nhị phân, dẫn đến: `0.1 + 0.2 = 0.30000000000000004 != 0.3`.
* **Quy tắc:** **KHÔNG BAO GIỜ** dùng `a == b` để so sánh 2 số thực. Hãy kiểm tra độ lệch tuyệt đối với một hằng số rất nhỏ ($\epsilon = 10^{-9}$):

    ```cpp
    if (std::abs(a - b) < 1e-9) { /* Hai số thực bằng nhau */ }
    ```

---

## 3. Đối Chiếu Nhập / Xuất Chuẩn: C vs C++

### 3.1. Bảng chuyển đổi định dạng (`<stdio.h>` vs `<iostream>` + `<iomanip>`)

| Tác vụ | Ngôn ngữ C (`printf` / `scanf`) | Ngôn ngữ C++ (`std::cout` / `std::cin`) | Ghi chú quan trọng |
| :--- | :--- | :--- | :--- |
| **In biến cơ bản** | `printf("%d %.2lf", a, b);` | `std::cout << a << " " << b;` | C++ tự nhận diện kiểu (Type-safe). |
| **Lấy $n$ số thập phân** | `printf("%.2f", pi);` | `std::cout << std::fixed << std::setprecision(2) << pi;` | Trong C++, hiệu lực giữ nguyên cho các lệnh sau. |
| **In hệ Hex / Octal** | `printf("%#X %#o", n, n);` | `std::cout << std::showbase << std::hex << n;` | In xong nhớ gọi `std::dec` để trả về hệ 10. |
| **Canh độ rộng $w$** | `printf("[%5d] [%-5d]", n, n);` | `std::cout << std::setw(5) << n;` | `setw()` chỉ có tác dụng cho **đúng 1 biến** ngay sau nó. |
| **Bù số `0` ở đầu** | `printf("%05d", n);` | `std::cout << std::setfill('0') << std::setw(5) << n;` | Dùng xong nhớ trả lại `std::setfill(' ')`. |
| **Nhập chuỗi có dấu cách** | `fgets(str, sizeof(str), stdin);` | `std::getline(std::cin, str);` | Không dùng `scanf("%s")` hay `cin >>` vì sẽ bị ngắt ở dấu cách. |

### 3.2. Hai "Cạm bẫy" Nhập/Xuất kinh điển cần nhớ

1. **Lỗi trôi lệnh khi nhập chuỗi sau khi nhập số:**
   * *Nguyên nhân:* Khi nhập số bằng `scanf("%d", &age)` hoặc `std::cin >> age` rồi nhấn `Enter`, ký tự xuống dòng `'\n'` vẫn còn nằm lại trong bộ đệm bàn phím (`stdin`). Lệnh đọc chuỗi ngay sau đó sẽ đọc trúng ký tự `'\n'` này và kết thúc ngay lập tức.
   * *Cách khắc phục:*
     * **Trong C++:** Gọi `std::cin.ignore();` trước khi gọi `std::getline()`.
     * **Trong C:** Đặt một dấu cách trước `%c` (`scanf(" %c", &ch);`) hoặc dùng `getchar()` trước `fgets()`.
2. **Sự khác nhau giữa `%d` và `%i` trong `scanf`:**
   * `%d`: Luôn đọc số ở hệ thập phân (Base 10).
   * `%i`: Tự động nhận diện hệ cơ số (Nhập `0x10` $\rightarrow$ hiểu là `16`; nhập `010` $\rightarrow$ hiểu là `8`).

---

## 4. Các Toán Tử & Cạm Bẫy Thường Gặp

1. **Phép chia nguyên (Integer Division):**
   * Hai số nguyên chia nhau **luôn trả về phần nguyên** (bỏ phần thập phân): `5 / 2` bằng `2` (không phải `2.5`).
   * Muốn lấy kết quả thực, phải ép kiểu ít nhất 1 toán hạng: `(double)a / b` hoặc `1.0 * a / b`.
2. **Toán tử tăng/giảm (`++i` vs `i++`):**
   * **Tiền tố (`++i` - Prefix):** Tăng giá trị của `i` lên 1 trước, rồi mới trả về giá trị mới để tính toán.
   * **Hậu tố (`i++` - Postfix):** Tạo bản sao giữ giá trị cũ để tính toán trước, sau đó mới tăng `i` lên 1. *(Trong vòng lặp hoặc với đối tượng lớn, ưu tiên dùng `++i` để tối ưu hiệu năng).*
3. **Cơ chế đoản mạch (Short-circuit Evaluation) của `&&` và `||`:**
   * Với `A && B`: Nếu `A` sai (`false`), máy tính **bỏ qua không chạy `B`** vì kết quả chắc chắn là `false`.
   * Với `A || B`: Nếu `A` đúng (`true`), máy tính **bỏ qua không chạy `B`** vì kết quả chắc chắn là `true`.
   * *Ứng dụng:* Luôn đặt điều kiện kiểm tra an toàn ở vế trái, ví dụ: `if (b != 0 && a / b > 2)` (Tránh lỗi chia cho 0).

---

## 5. Toán Tử Thao Tác Bit (Bitwise Operators)

### 5.1. Bảng chân trị và chức năng

| Toán tử | Tên gọi | Quy tắc hoạt động trên từng bit | Ví dụ (`a = 5 [0101]`, `b = 3 [0011]`) |
| :---: | :--- | :--- | :--- |
| `&` | **AND** | Bằng `1` khi **cả hai** bit đều là `1`. | `5 & 3 = 1` (`0001`) |
| `\|` | **OR** | Bằng `1` khi **ít nhất một** bit là `1`. | `5 \| 3 = 7` (`0111`) |
| `^` | **XOR** | Bằng `1` khi hai bit **khác nhau** (`0^1=1`). Tính chất: `x ^ x = 0`, `x ^ 0 = x`. | `5 ^ 3 = 6` (`0110`) |
| `~` | **NOT** | Đảo ngược toàn bộ bit (`0` $\leftrightarrow$ `1`). Công thức nhanh: `~x = -x - 1`. | `~5 = -6` |
| `<<` | **Left Shift** | Dịch các bit sang trái $k$ vị trí, thêm `0` vào bên phải. Tương đương **Nhân với $2^k$**. | `5 << 1 = 10` (`1010`) |
| `>>` | **Right Shift** | Dịch các bit sang phải $k$ vị trí. Tương đương **Chia nguyên cho $2^k$**. | `5 >> 1 = 2` (`0010`) |

### 5.2. Sổ tay 8 công thức Bitwise thực chiến (Bit Manipulation Tricks)

Giả sử $n$ là số nguyên và $k$ là vị trí bit (đánh số từ $0$ từ phải sang trái):

| STT | Bài toán | Biểu thức Bitwise | Giải thích ngắn gọn |
| :-: | :--- | :--- | :--- |
| 1 | Kiểm tra $n$ là số lẻ hay chẵn | `(n & 1)` | Bit cuối cùng (bit 0) của số lẻ luôn bằng `1`. |
| 2 | Tính $2^k$ siêu tốc | `(1 << k)` *(hoặc `1ULL << k` nếu $k \ge 31$)* | Dịch số `1` sang trái $k$ bit. |
| 3 | Kiểm tra bit thứ $k$ của $n$ là `0` hay `1` | `(n >> k) & 1` | Đưa bit thứ $k$ về vị trí cuối cùng rồi `& 1`. |
| 4 | **Bật (Set)** bit thứ $k$ của $n$ lên `1` | `n \| (1 << k)` | Dùng mặt nạ có bit $k$ bằng `1` để `OR` với $n$. |
| 5 | **Tắt (Clear)** bit thứ $k$ của $n$ về `0` | `n & ~(1 << k)` | Tạo mặt nạ có bit $k$ bằng `0` (các bit khác là `1`) rồi `AND`. |
| 6 | **Đảo (Toggle)** trạng thái bit thứ $k$ | `n ^ (1 << k)` | Dùng tính chất `x ^ 1` luôn làm đảo bit `x`. |
| 7 | Tắt bit `1` ngoài cùng bên phải của $n$ | `n & (n - 1)` | Ứng dụng đếm số lượng bit `1` (Brian Kernighan's Algorithm). |
| 8 | Kiểm tra $n > 0$ có phải là lũy thừa của 2 | `(n > 0) && ((n & (n - 1)) == 0)` | Số lũy thừa của 2 (`2, 4, 8, 16...`) chỉ có duy nhất một bit `1`. |

---

## 6. Danh Sách File Thực Hành Trong Thư Mục & Lệnh Biên Dịch

* `01_sizeof_and_limits.cpp`: Đo kích thước kiểu dữ liệu, kiểm chứng hiện tượng Tràn số (Overflow) và sai số số thực.
* `02_io_comparison.c`: Tổng hợp toàn bộ Format Specifiers (`%d`, `%lld`, `%lf`, `%x`, `%p`, canh chỉnh bảng) trong C.
* `03_io_comparison.cpp`: Làm chủ `std::cin`, `std::cout`, xử lý trôi lệnh với `cin.ignore()` và định dạng bằng `<iomanip>`.
* `04_bitwise_tricks.cpp`: Trực quan hóa các phép toán trên bit bằng `std::bitset<8>` và áp dụng các công thức Bitwise.
* `05_practice_exercises.cpp`: Giải 5 bài tập tự luyện (Đổi giây sang `hh:mm:ss`, Hoán đổi bằng XOR, Tìm Max 3 số bằng toán tử 3 ngôi, Bật/Tắt/Đảo bit thứ $k$, Đổi chữ Hoa/Thường bằng Bitwise).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
gcc -Wall -Wextra 02_io_comparison.c -o io_c
./io_c

g++ -Wall -Wextra -std=c++17 01_sizeof_and_limits.cpp -o limits
./limits

g++ -Wall -Wextra -std=c++17 03_io_comparison.cpp -o io_cpp
./io_cpp

g++ -Wall -Wextra -std=c++17 04_bitwise_tricks.cpp -o bitwise
./bitwise

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o var_practice
./var_practice
```

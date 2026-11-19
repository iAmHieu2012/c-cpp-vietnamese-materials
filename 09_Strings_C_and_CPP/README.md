# Bài 2.4: Chuỗi Ký Tự Toàn Tập (C-Style Strings vs C++ `std::string`)

Tài liệu tổng hợp bản chất lưu trữ chuỗi trên RAM, sự khác biệt giữa mảng ký tự kết thúc bằng `'\0'` của C (`<cstring>`) và lớp `std::string` tự động quản lý bộ nhớ của C++ (`<string>`), cùng bộ công cụ xử lý văn bản `<cctype>` và `<sstream>`.

---

## 1. Bản Chất Của C-Style String (`char[]` / `const char*`)

### 1.1. Ký tự kết thúc chuỗi Null-Terminator (`'\0'`)

Trong ngôn ngữ C, chuỗi là một mảng 1 chiều các phần tử kiểu `char` nằm liên tiếp nhau trên RAM và **bắt buộc kết thúc bằng ký tự `'\0'` (mã ASCII bằng `0`)**:

```text
  Khai báo: char s[] = "Hello"; (5 ký tự chữ, nhưng mảng nặng 6 bytes!)

  Chỉ số (Index):     s[0]      s[1]      s[2]      s[3]      s[4]      s[5]
                    ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
  Ký tự (Char):     │   'H'   │   'e'   │   'l'   │   'l'   │   'o'   │  '\0'   │
                    ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
  Mã ASCII (Dec):   │   72    │   101   │   108   │   108   │   111   │    0    │
                    └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
                     |<------------- strlen(s) = 5 ------------->|
                     |<------------------ sizeof(s) = 6 bytes ----------------->|
```

* **Sự khác biệt sống còn giữa `sizeof(s)` và `strlen(s)`:**
  * `sizeof(s)`: Toán tử lúc biên dịch, trả về **tổng số byte của mảng được cấp phát** (tính cả byte `'\0'` và các ô trống thừa phía sau).
  * `strlen(s)`: Hàm chạy lúc thực thi ($O(N)$), đếm số ký tự từ đầu cho tới **trước ký tự `'\0'` đầu tiên**.
* **Cạm bẫy khởi tạo thiếu chỗ cho `'\0'`:**
  * Đúng: `char s[6] = "Hello";` hoặc `char s[] = "Hello";` (Trình biên dịch tự chèn `'\0'`).
  * Sai: `char s[3] = {'A', 'B', 'C'};` $\rightarrow$ Không có `'\0'` ở cuối! Khi gọi `printf("%s", s)` hoặc `strlen(s)`, CPU sẽ đọc tràn sang các vùng nhớ lân cận gây lỗi **Buffer Over-read**.

### 1.2. Bảng tra cứu các hàm xử lý chuỗi trong `<cstring>` (`<string.h>`)

| Hàm `<cstring>` | Độ phức tạp | Chức năng & Lưu ý an toàn bộ nhớ |
| :--- | :---: | :--- |
| `strlen(s)` | $O(N)$ | Trả về độ dài chuỗi (kiểu `size_t`, không tính `'\0'`). *Lưu ý: Không gọi `i < strlen(s)` trong điều kiện vòng lặp `for` vì sẽ làm vòng lặp bị chậm thành $O(N^2)$!* |
| `strcpy(dest, src)` | $O(N)$ | Sao chép chuỗi `src` (kèm cả `'\0'`) vào mảng `dest`. `dest` phải đủ rộng để tránh tràn bộ nhớ! |
| `strncpy(dest, src, n)` | $O(N)$ | Sao chép tối đa `n` ký tự từ `src` sang `dest` (an toàn hơn `strcpy`). |
| `strcat(dest, src)` | $O(N + M)$ | Nối chuỗi `src` vào ngay vị trí `'\0'` ở cuối chuỗi `dest`. |
| `strcmp(s1, s2)` | $O(\min(N,M))$ | So sánh thứ tự từ điển (ASCII) hai chuỗi: Trả về **`0`** nếu `s1` và `s2` giống hệt nhau. Trả về **`< 0`** nếu `s1` đứng trước `s2`. Trả về **`> 0`** nếu `s1` đứng sau `s2`. |
| `strchr(s, ch)` | $O(N)$ | Tìm vị trí đầu tiên của ký tự `ch` trong `s` (trả về con trỏ tới vị trí đó hoặc `nullptr`). |
| `strstr(s, sub)` | $O(N \times M)$ | Tìm chuỗi con `sub` trong `s` (trả về con trỏ tới vị trí bắt đầu hoặc `nullptr`). |
| `strtok(s, delim)` | $O(N)$ | Tách chuỗi `s` thành các từ (token) dựa theo tập ký tự phân cách `delim`. *(Làm thay đổi trực tiếp chuỗi gốc `s`)*. |

> **Cạm bẫy kinh điển:** Trong C, **KHÔNG BAO GIỜ** dùng `if (s1 == s2)` để so sánh hai chuỗi C-style, vì phép `==` đó chỉ so sánh **2 địa chỉ con trỏ** chứ không so sánh nội dung chữ bên trong! Bắt buộc phải dùng `if (strcmp(s1, s2) == 0)`.

---

## 2. Lớp Chuỗi Hiện Đại `std::string` Trong C++ (`<string>`)

### 2.1. Tại sao `std::string` vượt trội hơn mảng `char[]`?

1. **Tự động co giãn trên Heap:** Quản lý bộ nhớ động hoàn toàn tự động (không lo tràn mảng, không cần `delete[]`).
2. **Lấy độ dài trong $O(1)$:** Lưu sẵn biến độ dài `size`, gọi `s.length()` hoặc `s.size()` trả về kết quả ngay lập tức trong $O(1)$ thay vì phải đếm lại từ đầu như `strlen()` ($O(N)$).
3. **Hỗ trợ toán tử trực quan:** Gán trực tiếp `s1 = s2;`, nối chuỗi bằng `+`, `+=`, và so sánh trực tiếp bằng `==`, `!=`, `<`, `>`, `<=`, `>=`.
4. **Cơ chế tối ưu SSO (Small String Optimization):** Với các chuỗi ngắn ($\le 15$ hoặc $22$ ký tự tùy trình biên dịch), `std::string` lưu trực tiếp ký tự ngay trên **Stack** bên trong đối tượng mà không cần cấp phát **Heap**, giúp tốc độ ngang ngửa mảng `char[]` tĩnh!

### 2.2. Sổ tay các phương thức cốt lõi của `std::string`

| Phương thức / Hàm | Cú pháp mẫu | Chức năng chi tiết |
| :--- | :--- | :--- |
| **Kích thước & Kiểm tra rỗng** | `s.length()` / `s.size()` `s.empty()` | Trả về số ký tự trong $O(1)$. Trả về `true` nếu chuỗi rỗng (`""`). |
| **Thêm / Xóa ký tự cuối** | `s.push_back('a');` `s.pop_back();` | Thêm 1 ký tự vào cuối chuỗi trong $O(1)$. Xóa ký tự cuối cùng trong $O(1)$. |
| **Cắt chuỗi con (`substr`)** | `s.substr(pos, len)` `s.substr(pos)` | Lấy chuỗi con bắt đầu từ chỉ số `pos` với độ dài `len` ký tự (nếu bỏ trống `len` sẽ lấy từ `pos` đến hết chuỗi). |
| **Tìm kiếm (`find` / `rfind`)** | `size_t p = s.find("abc");` | Trả về chỉ số đầu tiên tìm thấy. **Nếu không tìm thấy, trả về hằng số `std::string::npos`**. |
| **Chèn chuỗi (`insert`)** | `s.insert(pos, "xyz");` | Chèn chuỗi `"xyz"` vào ngay vị trí chỉ số `pos`. |
| **Xóa đoạn (`erase`)** | `s.erase(pos, len);` | Xóa `len` ký tự bắt đầu từ vị trí `pos`. |
| **Thay thế (`replace`)** | `s.replace(pos, len, "new");` | Xóa `len` ký tự từ vị trí `pos` và thế chuỗi `"new"` vào chỗ đó. |
| **Tương thích ngược với C** | `const char *p = s.c_str();` | Trả về con trỏ `const char*` (có `'\0'`) để truyền vào các hàm của ngôn ngữ C. |
| **Đổi Chuỗi $\leftrightarrow$ Số** | `std::stoi(s)`, `std::stoll(s)`, `std::stod(s)` `std::to_string(num)` | Chuyển chuỗi thành `int`, `long long`, `double`. Chuyển số bất kỳ thành `std::string`. |

---

## 3. Bộ Hàm Kiểm Tra & Biến Đổi Ký Tự (`<cctype>` / `<ctype.h>`)

Thay vì phải tự nhớ mã ASCII (`'A'=65`, `'a'=97`, `'0'=48`), hãy sử dụng các hàm chuẩn trong `<cctype>`:

| Hàm `<cctype>` | Trả về khác `0` (`true`) khi ký tự `c` là: | Ghi chú ứng dụng |
| :--- | :--- | :--- |
| `std::isalpha(c)` | Chữ cái (`'A'-'Z'` hoặc `'a'-'z'`). | Lọc ký tự chữ. |
| `std::isdigit(c)` | Chữ số thập phân (`'0'-'9'`). | Đổi ký tự số sang giá trị nguyên: `int val = c - '0';`. |
| `std::isalnum(c)` | Chữ cái **hoặc** chữ số. | Rất hữu ích khi lọc bỏ dấu câu, ký tự đặc biệt. |
| `std::isspace(c)` | Khoảng trắng (`' '`, `'\t'`, `'\n'`, `'\r'`). | Kiểm tra dấu cách/xuống dòng. |
| `std::isupper(c)` / `std::islower(c)` | Chữ cái IN HOA / chữ cái in thường. | Kiểm tra trạng thái Hoa/Thường. |
| `std::toupper(c)` / `std::tolower(c)` | Chuyển `c` thành chữ HOA / chữ thường. | Nếu `c` không phải chữ cái thì hàm giữ nguyên `c`. |

---

## 4. Kỹ Thuật Tách Từ & Xử Lý CSV Với `std::stringstream` (`<sstream>`)

### Mẫu 1: Tách các từ cách nhau bởi nhiều khoảng trắng bất kỳ

```cpp
#include <sstream>
std::string line = "   Lap   trinh   C++    hien   dai  ";
std::stringstream ss(line);
std::string word;
while (ss >> word) {
    // Toán tử >> tự động bỏ qua mọi dấu cách thừa và lấy ra từng từ:
    // Lần 1: "Lap", Lần 2: "trinh", Lần 3: "C++", Lần 4: "hien", Lần 5: "dai"
}
```

### Mẫu 2: Tách các cột dữ liệu ngăn cách bởi dấu phẩy (Đọc file CSV)

```cpp
std::string csv = "1001,Nguyen Van A,8.50";
std::stringstream ss(csv);
std::string token;
while (std::getline(ss, token, ',')) {
    // Đọc từng trường dữ liệu ngăn cách bởi dấu ','
}
```

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_c_style_strings_cstring.cpp`: Thực nghiệm ký tự `'\0'`, phân biệt `sizeof` vs `strlen`, dùng `strcpy`, `strcat`, `strcmp`, `strtok`.
* `02_cpp_std_string_methods.cpp`: Thực hành các phương thức của `std::string` (`find`, `substr`, `insert`, `replace`, `stoi`, `to_string`).
* `03_stringstream_word_processing.cpp`: Chuẩn hóa Họ Tên và tách dữ liệu CSV bằng `std::stringstream`.
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Valid Palindrome, Kiểm tra Anagram, Nén chuỗi RLE, Tạo Email tự động từ Họ Tên, Tính tổng các số ẩn trong chuỗi).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_c_style_strings_cstring.cpp -o c_strings
./c_strings

g++ -Wall -Wextra -std=c++17 02_cpp_std_string_methods.cpp -o cpp_strings
./cpp_strings

g++ -Wall -Wextra -std=c++17 03_stringstream_word_processing.cpp -o sstream_demo
./sstream_demo

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o str_practice
./str_practice
```

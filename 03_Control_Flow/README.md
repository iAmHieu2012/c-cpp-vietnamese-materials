# Bài 1.3: Cấu Trúc Điều Khiển (Control Flow trong C/C++)

Tài liệu tổng hợp cách điều khiển luồng thực thi của CPU thông qua cấu trúc rẽ nhánh (`if-else`, `switch-case`), vòng lặp (`for`, `while`, `do-while`), từ khóa nhảy (`break`, `continue`, `return`) và các mẫu thuật toán số học cốt lõi.

---

## 1. Cấu Trúc Rẽ Nhánh (Branching: `if-else` & `switch-case`)

### 1.1. Quy tắc đánh giá Đúng/Sai (Truthiness) trong C/C++

Trong C và C++, mọi biểu thức số học hoặc con trỏ đặt trong điều kiện `if (...)` hay `while (...)` đều được quy đổi ngầm về kiểu logic:

* **Sai (`false`):** Giá trị bằng **`0`** (hoặc `0.0`, ký tự `'\0'`, con trỏ `NULL` / `nullptr`).
* **Đúng (`true`):** Mọi giá trị **khác `0`** (kể cả số âm như `-1`, `-99` đều là `true`!).

### 1.2. So sánh `if - else if - else` và `switch - case`

| Tiêu chí | `if - else if - else` | `switch - case` |
| :--- | :--- | :--- |
| **Kiểu dữ liệu hỗ trợ** | Mọi kiểu dữ liệu (số nguyên, số thực `float`/`double`, chuỗi, biểu thức logic phức tạp). | **Chỉ nhận kiểu số nguyên** (`int`, `char`, `short`, `long long`, `enum`). **KHÔNG** dùng được cho `float`, `double`, `string`. |
| **Loại điều kiện** | So sánh khoảng (`gpa >= 3.2 && gpa < 3.6`), kết hợp nhiều biến. | Chỉ so sánh bằng (`==`) với các **hằng số cố định lúc biên dịch (Compile-time constants)**. |
| **Tốc độ thực thi** | Kiểm tra tuần tự từng nhánh từ trên xuống dưới ($O(k)$ nhánh). | Khi có nhiều nhánh gần nhau, trình biên dịch tối ưu thành **Bảng nhảy (Jump Table)**, nhảy thẳng tới nhánh đúng trong $O(1)$. |
| **Khi nào nên dùng?** | Kiểm tra điều kiện khoảng giá trị, điều kiện kết hợp `&&`, `\|\|`. | Làm Menu điều khiển, xử lý trạng thái (State Machine), phân loại mã lỗi/tháng trong năm. |

### 1.3. Ba "Cạm bẫy" rẽ nhánh kinh điển cần tránh

1. **Nhầm lẫn giữa phép Gán (`=`) và phép So sánh bằng (`==`):**

   ```cpp
   int x = 0;
   if (x = 5) { ... } // LỖI LOGIC: Gán x = 5, biểu thức trả về 5 (!= 0) -> LUÔN ĐÚNG!
   if (x == 5) { ... } // ĐÚNG: So sánh giá trị của x với 5.
   ```

2. **Hiện tượng Trôi lệnh (Fall-through) trong `switch-case`:**
   * Nếu quên từ khóa `break;` ở cuối một `case`, CPU sẽ **chạy tuột xuống thực thi tiếp tất cả các `case` bên dưới** mà không cần kiểm tra điều kiện nữa cho đến khi gặp `break` hoặc hết `switch`.
   * *Ứng dụng tích cực:* Có thể gom nhiều `case` có chung cách xử lý lại với nhau (ví dụ: `case 1: case 3: case 5: ... break;`).
3. **Khai báo biến mới bên trong `case`:**
   * Trong C/C++, nếu bạn khởi tạo một biến mới bên trong một nhánh `case`, bạn **bắt buộc phải bọc nhánh đó bằng cặp ngoặc nhọn `{ }`** để giới hạn phạm vi biến (Scope), nếu không trình biên dịch sẽ báo lỗi `jump to case label crosses initialization`.

---

## 2. Cấu Trúc Vòng Lặp (Loops: `for`, `while`, `do-while`)

### 2.1. Bảng phân loại và chọn vòng lặp tối ưu

| Loại vòng lặp | Cú pháp tổng quát | Kiểm tra điều kiện | Số lần chạy tối thiểu | Khi nào sử dụng? |
| :--- | :--- | :---: | :---: | :--- |
| **`for`** | `for (khởi_tạo; điều_kiện; bước_nhảy)` | Trước mỗi vòng lặp | **0 lần** | Khi **biết trước số lần lặp** hoặc duyệt qua một đoạn cố định $[L, R]$, duyệt mảng. |
| **`while`** | `while (điều_kiện) { ... }` | Trước mỗi vòng lặp | **0 lần** | Khi **chưa biết trước số lần lặp** (tách chữ số của $N$, tìm GCD, đọc file đến cuối). |
| **`do - while`** | `do { ... } while (điều_kiện);` *(Nhớ dấu `;`)* | **Sau** mỗi vòng lặp | **Ít nhất 1 lần** | Khi cần thực hiện hành động trước rồi mới xét điều kiện (Menu tương tác, bắt nhập lại nếu nhập sai). |

### 2.2. Các cạm bẫy vòng lặp chết người (Loop Pitfalls)

1. **Dấu chấm phẩy `;` thừa ngay sau `for` hoặc `while`:**

   ```cpp
   for (int i = 1; i <= 5; ++i); // <-- Dấu ';' ở đây làm vòng lặp chạy rỗng 5 lần!
   {
       std::cout << i; // Lỗi biên dịch (i hết phạm vi) hoặc chỉ in ra 1 lần duy nhất!
   }
   ```

2. **Lặp vô tận với kiểu không dấu (`unsigned` / `size_t`) khi chạy lùi về 0:**
   * Không bao giờ viết `for (size_t i = n; i >= 0; --i)` vì `size_t` luôn $\ge 0$. Khi `i = 0`, lệnh `--i` làm tràn dưới về số cực đại $\rightarrow$ Lặp vô tận!
3. **Dùng số thực (`float`/`double`) làm biến đếm vòng lặp:**
   * Tránh viết `for (float x = 0.0f; x != 1.0f; x += 0.1f)` vì sai số dấu phẩy động làm `x` chỉ tiến tới `0.9999999` hoặc `1.0000001` chứ không bao giờ bằng đúng `1.0f` $\rightarrow$ Lặp vô tận!

---

## 3. Các Từ Khóa Điều Khiển Luồng (`break`, `continue`, `return`)

```text
  Trong vòng lặp (for / while / do-while):
  ┌───────────────────────────────────────────────────────────┐
  │  Lệnh 1;                                                  │
  │  if (điều_kiện_bỏ_qua) continue; ──► Nhảy thẳng lên bước  │
  │                                      lặp tiếp theo (++i)  │
  │  if (điều_kiện_dừng)   break;    ──► Thoát ngay khỏi      │
  │                                      vòng lặp hiện tại    │
  │  Lệnh 2;                                                  │
  └───────────────────────────────────────────────────────────┘
```

* **`continue`:** Bỏ qua toàn bộ các câu lệnh phía dưới nó trong lần lặp hiện tại, nhảy thẳng sang bước cập nhật biến đếm (với `for`) hoặc kiểm tra điều kiện (với `while`) cho lần lặp kế tiếp.
* **`break`:** Phá vỡ và thoát ngay lập tức khỏi vòng lặp (hoặc `switch`) **gần nhất chứa nó**. *(Lưu ý: Trong vòng lặp lồng nhau, `break` ở vòng trong chỉ thoát khỏi vòng trong, vòng ngoài vẫn chạy tiếp).*
* **`return`:** Kết thúc ngay lập tức **toàn bộ hàm hiện tại** (bất kể đang ở sâu trong bao nhiêu vòng lặp lồng nhau) và trả quyền điều khiển về nơi gọi hàm.

---

## 4. Sổ Tay 4 Mẫu Thuật Toán Vòng Lặp Kinh Điển (Algorithmic Patterns)

### Mẫu 1: Kỹ thuật tách từng chữ số của số nguyên $N$ (Dùng `while`)

Ứng dụng: Tính tổng chữ số, đếm số lượng chữ số, đảo ngược số, kiểm tra số Thuận Nghịch (Palindrome).

```cpp
int temp = n;
int reversed_n = 0, sum_digits = 0;
while (temp > 0) {
    int last_digit = temp % 10;               // 1. Lấy chữ số hàng đơn vị
    sum_digits += last_digit;                 // 2. Xử lý chữ số (cộng dồn)
    reversed_n = reversed_n * 10 + last_digit;// 3. Ghép vào số đảo ngược
    temp /= 10;                               // 4. Loại bỏ chữ số hàng đơn vị
}
```

### Mẫu 2: Kiểm tra số nguyên tố tối ưu trong $O(\sqrt{N})$ (Dùng `for` + `break`)

* **Nguyên lý toán học:** Nếu số $N$ có ước số, chắc chắn sẽ có ít nhất một ước số $\le \sqrt{N}$. Do đó chỉ cần duyệt `i` từ $2$ đến $\sqrt{N}$ (viết là `1LL * i * i <= n` để tránh sai số hàm `sqrt()` và tránh tràn số `int`).

    ```cpp
    bool is_prime = (n >= 2);
    for (int i = 2; 1LL * i * i <= n; ++i) {
        if (n % i == 0) {
            is_prime = false;
            break; // Dừng sớm ngay khi phát hiện ước số đầu tiên
        }
    }
    ```

### Mẫu 3: Tìm Ước chung lớn nhất (GCD) bằng Thuật toán Euclid (Dùng `while`)

* **Nguyên lý:** $\text{GCD}(a, b) = \text{GCD}(b, a \bmod b)$. Lặp cho đến khi $b = 0$ thì $a$ chính là $\text{GCD}$. Độ phức tạp cực nhanh: $O(\log(\min(a, b)))$.
* **Công thức Bội chung nhỏ nhất (LCM):** $\text{LCM}(a, b) = (a / \text{GCD}(a, b)) \times b$ *(Chia trước rồi mới nhân để tránh tràn số!)*.

    ```cpp
    long long a = 48, b = 18;
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    long long gcd = a; // Kết quả: 6
    ```

### Mẫu 4: Phân tích thừa số nguyên tố trong $O(\sqrt{N})$ (Kết hợp `for` và `while`)

Ứng dụng: Biểu diễn $N = p_1^{e_1} \times p_2^{e_2} \dots$ (Ví dụ: $60 = 2^2 \times 3 \times 5$).

```cpp
for (int i = 2; 1LL * i * i <= n; ++i) {
    while (n % i == 0) {
        std::cout << i << " "; // In ra thừa số nguyên tố i
        n /= i;                // Chia giảm n cho đến khi không chia hết cho i nữa
    }
}
if (n > 1) std::cout << n; // Nếu sau vòng lặp n > 1 thì phần còn lại chính là số nguyên tố cuối cùng
```

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_branching_pitfalls.cpp`: Thực hành `if-else`, `switch-case`, xử lý năm nhuận và các lỗi thường gặp.
* `02_loops_and_digits.cpp`: Thực hành `for`, `while`, `do-while`, `break`, `continue` và thuật toán tách chữ số / kiểm tra số thuận nghịch.
* `03_nested_loops_and_primes.cpp`: Kiểm tra số nguyên tố $O(\sqrt{N})$ và kỹ thuật vòng lặp lồng nhau vẽ hình học bằng dấu `*`.
* `04_practice_exercises.cpp`: Giải 5 bài tập tự luyện (Phương trình bậc 2, GCD/LCM, Fibonacci, Phân tích thừa số nguyên tố, Menu máy tính).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_branching_pitfalls.cpp -o branching
./branching

g++ -Wall -Wextra -std=c++17 02_loops_and_digits.cpp -o loops
./loops

g++ -Wall -Wextra -std=c++17 03_nested_loops_and_primes.cpp -o nested
./nested

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o cf_practice
./cf_practice
```

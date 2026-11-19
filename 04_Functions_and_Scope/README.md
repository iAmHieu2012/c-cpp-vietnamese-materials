# Bài 1.4: Hàm (Functions), Cơ Chế Truyền Tham Số & Phạm Vi Biến (C/C++)

Tài liệu tổng hợp bản chất hoạt động của hàm trên bộ nhớ **Stack**, sự khác biệt cốt lõi giữa **Tham trị (Pass by Value)**, **Con trỏ (Pass by Pointer)** và **Tham chiếu (Pass by Reference)**, cùng quy tắc quản lý vòng đời biến và Nạp chồng hàm trong C++.

---

## 1. Giải Phẫu Một Hàm & Cơ Chế Call Stack

### 1.1. Nguyên mẫu hàm (Prototype) vs Định nghĩa hàm (Definition)

Trình biên dịch C/C++ đọc mã nguồn từ trên xuống dưới. Nếu bạn viết hàm ở bên dưới `main()`, bạn bắt buộc phải khai báo **Function Prototype** ở phía trên `main()` để báo trước cho trình biên dịch biết tên hàm, kiểu trả về và danh sách kiểu tham số.

```cpp
// 1. Function Prototype (Khai báo nguyên mẫu - Chỉ cần kiểu dữ liệu, kết thúc bằng ';')
long long add(int a, int b); 

int main() {
    long long res = add(10, 20); // Lời gọi hàm (Function Call) - 10 và 20 là Đối số (Arguments)
    return 0;
}

// 2. Function Definition (Định nghĩa chi tiết phần thân hàm - a và b là Tham số / Parameters)
long long add(int a, int b) {
    return 1LL * a + b;
}
```

### 1.2. Chuyện gì xảy ra trên RAM khi gọi hàm? (Cơ chế Call Stack)

Mỗi khi một hàm được gọi, CPU sẽ cấp phát một vùng nhớ riêng biệt trên phân vùng **Stack** gọi là **Stack Frame** (Khung ngăn xếp) theo cơ chế **LIFO (Vào sau - Ra trước)**:

```text
  Bộ nhớ Stack khi main() gọi hàm swap(x, y):
  ┌──────────────────────────────────────────────────────────┐
  │ [Stack Frame của swap()]  <-- Đỉnh Stack (Đang chạy)     │
  │  • Tham số của hàm: a, b                                 │
  │  • Biến cục bộ (Local): temp                             │
  │  • Địa chỉ trả về (Return Address): Dòng tiếp theo ở main│
  ├──────────────────────────────────────────────────────────┤
  │ [Stack Frame của main()]  <-- Tạm dừng chờ swap() xong   │
  │  • Biến cục bộ của main: x = 10, y = 20                  │
  └──────────────────────────────────────────────────────────┘
  * Ngay khi hàm swap() gặp lệnh 'return' hoặc dấu '}', toàn bộ 
    Stack Frame của swap() bị THU HỒI NGAY LẬP TỨC!
```

---

## 2. Bốn Cơ Chế Truyền Tham Số Trong C/C++ (Trọng Tâm Phỏng Vấn)

| Cơ chế truyền | Cú pháp hàm | Cú pháp gọi | Tạo bản sao? | Sửa được biến gốc? | Khi nào sử dụng chuẩn công nghiệp? |
| :--- | :--- | :--- | :---: | :---: | :--- |
| **1. Tham trị** *(Pass by Value)* | `void f(int x)` | `f(a);` | **CÓ** *(Tốn RAM copy)* | **KHÔNG** | Khi truyền các kiểu dữ liệu nguyên thủy nhỏ (`int`, `char`, `double`, `bool`) và **không muốn** hàm làm thay đổi biến gốc. |
| **2. Con trỏ (C/C++)** *(Pass by Pointer)* | `void f(int *x)` | `f(&a);` | Chỉ copy địa chỉ (4/8 bytes) | **CÓ** *(Qua `*x`)* | Bắt buộc trong **ngôn ngữ C** khi muốn sửa biến gốc, truyền mảng, hoặc khi tham số có thể mang giá trị rỗng (`nullptr`). |
| **3. Tham chiếu (C++)** *(Pass by Reference)* | `void f(int &x)` | `f(a);` | **KHÔNG** *(Dùng chung ô nhớ)* | **CÓ** *(Trực tiếp)* | Vũ khí của **C++**: Dùng khi hàm cần sửa trực tiếp biến gốc (như `swap`) hoặc cần **trả về nhiều kết quả cùng lúc** qua tham số. |
| **4. Tham chiếu hằng** *(Const Reference)* | `void f(const string &s)` | `f(str);` | **KHÔNG** *(Zero-copy)* | **CẤM SỬA** *(An toàn)* | **Chuẩn vàng trong C++** khi truyền các đối tượng lớn (`std::string`, `std::vector`, `struct`, `class`) chỉ để đọc dữ liệu mà không tốn chi phí copy. |

### Cạm bẫy chết người với Tham chiếu: Không bao giờ trả về tham chiếu tới biến Local

```cpp
int& bad_function() {
    int local_x = 42; 
    return local_x; // LỖI NGHIÊM TRỌNG (Dangling Reference): 
                    // local_x bị hủy ngay khi hàm kết thúc, tham chiếu trả về trỏ vào vùng nhớ rác!
}
```

---

## 3. Phạm Vi Biến (Scope) & Vòng Đời Trên Bộ Nhớ (Storage Duration)

### 3.1. Bảng phân loại các loại biến

| Loại biến | Nơi khai báo | Vùng nhớ lưu trữ | Thời gian tồn tại (Lifetime) | Giá trị mặc định (nếu không gán) |
| :--- | :--- | :---: | :--- | :--- |
| **Biến cục bộ (Local)** | Bên trong hàm hoặc khối `{ }` | **Stack** | Sinh ra khi vào khối `{ }`, **bị hủy ngay khi ra khỏi khối `{ }`**. | **Giá trị rác (Garbage value)** |
| **Biến toàn cục (Global)** | Bên ngoài tất cả các hàm | **Data / BSS Segment** | Tồn tại **suốt thời gian chương trình chạy**. | Tự động bằng **`0`** |
| **Biến tĩnh cục bộ (`static` local)** | Bên trong hàm, có từ khóa `static` | **Data / BSS Segment** | Khởi tạo **đúng 1 lần duy nhất** ở lần gọi hàm đầu tiên, giữ nguyên giá trị giữa các lần gọi hàm. | Tự động bằng **`0`** |

### 3.2. Hiện tượng Che khuất tên (Variable Shadowing) & Toán tử `::`

* Khi một biến cục bộ đặt **trùng tên** với biến toàn cục, biến cục bộ sẽ "che khuất" biến toàn cục bên trong hàm đó.
* Trong C++, để gọi đích danh biến toàn cục khi bị trùng tên, hãy đặt toán tử phân giải phạm vi **`::`** (Scope Resolution Operator) ngay trước tên biến: `::global_var`.

---

## 4. Các Tính Năng Hàm Nâng Cao Trong C++

### 4.1. Nạp chồng hàm (Function Overloading) & Cơ chế Name Mangling

* **Định nghĩa:** C++ cho phép định nghĩa nhiều hàm **cùng tên** trong cùng một phạm vi, miễn là danh sách tham số của chúng khác nhau về **số lượng tham số**, **kiểu dữ liệu tham số**, hoặc **thứ tự kiểu tham số**.
* **Lưu ý:** Không thể nạp chồng hai hàm chỉ khác nhau mỗi **kiểu trả về (Return type)**!

  ```cpp
  int func(int x);
  double func(int x); // LỖI BIÊN DỊCH: Trình biên dịch không biết gọi hàm nào khi viết func(5)!
  ```

* **Bản chất bên dưới (Name Mangling):** Ngôn ngữ C lưu tên hàm giữ nguyên trong file Object (`.o`), nên không cho phép trùng tên. Ngược lại, trình biên dịch C++ tự động mã hóa tên hàm kèm theo chữ viết tắt của kiểu tham số (ví dụ `add(int, int)` bị đổi tên ngầm thành `_Z3addii`, còn `add(double, double)` thành `_Z3adddd`), nhờ đó Linker phân biệt được chúng.

### 4.2. Đối số mặc định (Default Arguments)

* Cho phép gán sẵn giá trị mặc định cho tham số nếu người gọi không truyền đối số đó vào.
* **2 Quy tắc bắt buộc:**
  1. Mọi tham số có giá trị mặc định **phải nằm dồn toàn bộ về phía bên phải cùng** của danh sách tham số:
     * Đúng: `void connect(string ip, int port = 8080, int timeout = 30);`
     * Sai: `void connect(string ip = "127.0.0.1", int port, int timeout = 30);` *(Lỗi biên dịch!)*
  2. Nếu tách hàm thành Prototype (ở trên) và Definition (ở dưới), **chỉ được viết giá trị mặc định ở Prototype**, tuyệt đối không viết lặp lại ở phần Definition.

### 4.3. Hàm nội tuyến (`inline` Functions)

* Mỗi lần gọi hàm thông thường, CPU tốn chi phí tạo Stack Frame, nhảy địa chỉ thanh ghi và thu hồi bộ nhớ.
* Từ khóa `inline` gợi ý cho trình biên dịch **chép thẳng mã lệnh của hàm vào ngay vị trí gọi** lúc biên dịch (giống Macro nhưng an toàn về kiểu dữ liệu), giúp tăng tốc cho các hàm cực ngắn (1–3 dòng) được gọi lặp đi lặp lại hàng triệu lần.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_pass_by_value_vs_ref.cpp`: Kiểm chứng địa chỉ bộ nhớ RAM của Tham trị, Con trỏ, Tham chiếu và `const &`.
* `02_scope_and_static.cpp`: Thực nghiệm Variable Shadowing, toán tử `::` và vòng đời của biến `static` cục bộ.
* `03_overloading_and_defaults.cpp`: Thực hành Function Overloading, Default Arguments và `inline`.
* `04_practice_exercises.cpp`: Giải 5 bài toán chuẩn hóa code bằng hàm (Số nguyên tố thuận nghịch, Phân tích số đa kết quả bằng tham chiếu, Tổ hợp $nCr$ chống tràn, Sắp xếp 3 biến, Nạp chồng hàm tính diện tích).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_pass_by_value_vs_ref.cpp -o pass_demo
./pass_demo

g++ -Wall -Wextra -std=c++17 02_scope_and_static.cpp -o scope_demo
./scope_demo

g++ -Wall -Wextra -std=c++17 03_overloading_and_defaults.cpp -o overload_demo
./overload_demo

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o func_practice
./func_practice
```

# Bài 2.2: Con Trỏ (Pointers) & Tham Chiếu (References) Toàn Tập

Tài liệu tổng hợp bản chất của Con trỏ dưới thanh RAM, sự khác biệt giữa Con trỏ và Tham chiếu, quy tắc `const` với con trỏ, Số học con trỏ (Pointer Arithmetic), Con trỏ cấp 2 (`**`), Con trỏ `void*`, Con trỏ hàm (Function Pointers) và cách phòng chống các lỗi bộ nhớ kinh điển.

---

## 1. Bản Chất Của Con Trỏ Dưới Góc Nhìn Bộ Nhớ RAM

### 1.1. Hai toán tử nền tảng: `&` (Address-of) và `*` (Dereference)

* **Con trỏ (Pointer)** là một biến mà giá trị nó lưu trữ chính là **địa chỉ ô nhớ (hệ Hex)** của một biến khác.
* **`&x` (Toán tử lấy địa chỉ):** Trả về địa chỉ trên RAM của biến `x`.
* **`*p` (Toán tử giải tham chiếu):** Đi tới địa chỉ đang lưu trong `p` để đọc hoặc ghi trực tiếp giá trị tại ô nhớ đó.

```text
  Giả sử:  int x = 42;    int *p = &x;    int **pp = &p;

  Tên biến:           pp (Con trỏ cấp 2)      p (Con trỏ cấp 1)       x (Biến nguyên)
                    ┌───────────────────┐   ┌───────────────────┐   ┌───────────────┐
  Giá trị lưu trữ:  │      0x2000       │──►│      0x1000       │──►│      42       │
                    └───────────────────┘   └───────────────────┘   └───────────────┘
  Địa chỉ riêng:      0x3000 (8 bytes)        0x2000 (8 bytes)        0x1000 (4 bytes)

  • Giá trị của x   :  x  ==  *p     ==  **pp  == 42
  • Địa chỉ của x   : &x  ==   p     ==   *pp  == 0x1000
  • Địa chỉ của p   : &p  ==   pp              == 0x2000
```

### 1.2. Tại sao mọi con trỏ đều có cùng kích thước?

Bất kể là `char*`, `int*`, `double*` hay `struct Student*`, bản thân biến con trỏ chỉ lưu một con số địa chỉ bộ nhớ ảo:

* Trên hệ điều hành / trình biên dịch **64-bit**: Mọi con trỏ đều nặng đúng **8 bytes** (64 bits).
* Trên hệ điều hành / trình biên dịch **32-bit**: Mọi con trỏ đều nặng đúng **4 bytes** (32 bits).
* **Vậy kiểu dữ liệu phía trước dấu `*` dùng để làm gì?** Để báo cho trình biên dịch biết từ địa chỉ bắt đầu đó cần **đọc bao nhiêu byte** (1 byte cho `char*`, 4 bytes cho `int*`, 8 bytes cho `double*`) và cần **nhảy bao nhiêu byte** khi thực hiện phép cộng `p + 1`!

---

## 2. Quy Tắc Đọc `const` Với Con Trỏ (Clockwise / Star Rule)

Mẹo nhớ nhanh: Hãy kẻ một vạch tưởng tượng tại dấu **`*`** và đọc từ phải sang trái:

* Nếu `const` nằm **bên trái dấu `*`** $\rightarrow$ **Giá trị tại vùng nhớ (`*p`) là hằng** (Cấm sửa `*p`).
* Nếu `const` nằm **bên phải dấu `*`** $\rightarrow$ **Bản thân địa chỉ trong `p` là hằng** (Cấm đổi hướng `p`).

| Cách khai báo | Tên gọi chuẩn | Sửa giá trị `*p = 10`? | Đổi hướng `p = &y`? | Ý nghĩa thực tế |
| :--- | :--- | :---: | :---: | :--- |
| `int *p` | Non-const pointer to non-const | **ĐƯỢC** | **ĐƯỢC** | Con trỏ thông thường, tự do hoàn toàn. |
| `const int *p` *(hoặc `int const *p`)* | **Pointer to Const** *(Con trỏ trỏ tới hằng)* | **CẤM** | **ĐƯỢC** | Dùng làm tham số hàm chỉ đọc (`const char *str`, `const int *arr`) để bảo vệ dữ liệu gốc. |
| `int * const p` | **Const Pointer** *(Con trỏ hằng)* | **ĐƯỢC** | **CẤM** | Bắt buộc khởi tạo ngay lúc khai báo; khóa chặt vào 1 ô nhớ duy nhất (bản chất giống Tham chiếu `&`). |
| `const int * const p` | Const Pointer to Const | **CẤM** | **CẤM** | Khóa cứng cả địa chỉ lẫn giá trị bên trong. |

---

## 3. So Sánh Trực Diện: Con Trỏ (`*`) vs Tham Chiếu (`&` trong C++)

| Tiêu chí | Con trỏ (`int *p`) — C & C++ | Tham chiếu (`int &r = x`) — Chỉ có ở C++ |
| :--- | :--- | :--- |
| **Bản chất** | Là một biến độc lập lưu địa chỉ ô nhớ. | Là một **tên gọi khác (Alias)** gắn chặt vào biến gốc. |
| **Khởi tạo ban đầu** | Không bắt buộc khởi tạo ngay (nên gán `= nullptr`). | **Bắt buộc** phải gắn với một biến có sẵn ngay lúc khai báo. |
| **Giá trị rỗng (`null`)** | Có thể bằng `nullptr` / `NULL`. | **Không tồn tại** tham chiếu `null` hợp lệ (an toàn hơn). |
| **Đổi hướng trỏ** | Có thể đổi sang trỏ biến khác (`p = &y;`) bất cứ lúc nào. | **Không thể** đổi sang tham chiếu biến khác sau khi đã khởi tạo. |
| **Số học địa chỉ** | Hỗ trợ `p++`, `p + i`, `p1 - p2` để duyệt mảng. | Không có số học tham chiếu (`r++` là tăng giá trị của biến gốc). |

---

## 4. Số Học Con Trỏ (Pointer Arithmetic) & Mối Quan Hệ Với Mảng

Khi thực hiện phép cộng/trừ trên con trỏ kiểu `T*`, đơn vị dịch chuyển là **`sizeof(T)` bytes** chứ không phải 1 byte:

| Phép toán | Công thức địa chỉ thực tế dưới RAM | Ý nghĩa thực thi |
| :--- | :--- | :--- |
| `p + i` | $\text{Address}(p) + i \times \text{sizeof(T)}$ | Trả về địa chỉ của phần tử cách `p` một đoạn `i` phần tử về phía sau. |
| `*(p + i)` | Đọc giá trị tại $\text{Address}(p) + i \times \text{sizeof(T)}$ | **Hoàn toàn tương đương với `p[i]` hoặc `arr[i]`!** |
| `++p` / `--p` | Tăng / giảm địa chỉ đi $\text{sizeof(T)}$ bytes | Di chuyển con trỏ sang phần tử liền kề kế tiếp hoặc liền trước. |
| `p2 - p1` | $\frac{\text{Address}(p_2) - \text{Address}(p_1)}{\text{sizeof(T)}}$ | Trả về số lượng phần tử nằm giữa 2 con trỏ cùng mảng (kiểu `ptrdiff_t`). |

> **Sự khác biệt duy nhất giữa Tên mảng `int arr[5]` và Con trỏ `int *p`:**  
> Tên mảng `arr` thực chất hoạt động giống như một **Con trỏ hằng (`int * const`)**: bạn không thể gán lại `arr = p;` hay `arr++;`, và `sizeof(arr)` trả về `20` bytes (toàn mảng), trong khi `sizeof(p)` trả về `8` bytes.

---

## 5. Con Trỏ Nâng Cao: Con Trỏ Cấp 2 (`**`), `void*` & Con Trỏ Hàm

### 5.1. Khi nào bắt buộc phải dùng Con trỏ cấp 2 (`int **pp`)?

1. Khi viết một hàm cần **thay đổi chính địa chỉ lưu trong con trỏ cấp 1** truyền từ bên ngoài vào (ví dụ: hàm cấp phát bộ nhớ cho con trỏ, hàm hoán đổi 2 con trỏ, hàm xóa node đầu tiên của Danh sách liên kết đơn trong C).
2. Khi quản lý một **mảng các con trỏ** (ví dụ: mảng các chuỗi `char *argv[]` tương đương `char **argv`) hoặc cấp phát mảng 2 chiều động trên Heap.

### 5.2. Con trỏ đa năng `void*` (Generic Pointer)

* `void*` có thể lưu địa chỉ của **bất kỳ kiểu dữ liệu nào** (được hàm `malloc()` trong C trả về).
* **2 Giới hạn của `void*`:** Vì không biết kiểu dữ liệu đích nặng bao nhiêu byte, bạn **không thể giải tham chiếu trực tiếp (`*v_ptr`)** và **không thể cộng trừ địa chỉ (`v_ptr + 1`)** nếu chưa ép kiểu tường minh (dùng `static_cast<Type*>(v_ptr)` trong C++).

### 5.3. Con trỏ hàm (Function Pointers) & Callback

* Lưu địa chỉ của mã lệnh hàm (nằm ở Text Segment).
* **Cú pháp khai báo:** `Kiểu_Trả_Về (*Tên_Con_Trỏ_Hàm)(Danh_Sách_Kiểu_Tham_Số);`
  * *Lưu ý:* Bắt buộc phải có cặp ngoặc tròn `(*ptr)` bao quanh tên con trỏ! Nếu viết `int *ptr(int, int);` thì trình biên dịch hiểu nhầm là một hàm trả về `int*`.
* **Ứng dụng:**
  1. **Callback Function:** Truyền hàm so sánh (`ascending` / `descending`) vào hàm sắp xếp tổng quát (như `qsort` trong C).
  2. **Jump Table (Mảng con trỏ hàm):** Thay thế các cấu trúc `switch-case` khổng lồ bằng mảng các hàm `ops[choice](a, b)` để gọi trong $O(1)$.

---

## 6. Bốn Lỗi Con Trỏ "Chết Người" & Quy Tắc Phòng Vệ

| Tên lỗi | Nguyên nhân xảy ra | Hậu quả | Quy tắc phòng vệ chuẩn công nghiệp |
| :--- | :--- | :--- | :--- |
| **1. Wild Pointer** *(Con trỏ hoang)* | Khai báo `int *p;` cục bộ nhưng quên khởi tạo giá trị ban đầu rồi dùng `*p = 10;`. | Ghi đè vào địa chỉ rác ngẫu nhiên $\rightarrow$ Crash ngay lập tức. | **Luôn khởi tạo `int *p = nullptr;`** ngay khi khai báo nếu chưa có địa chỉ đích. |
| **2. Null Pointer Dereference** | Cố tình truy cập `*p` khi `p` đang bằng `nullptr` hoặc `NULL`. | Sập chương trình (**Segmentation Fault**). | Luôn kiểm tra `if (p != nullptr)` trước khi giải tham chiếu `*p`. |
| **3. Dangling Pointer** *(Con trỏ treo)* | Con trỏ vẫn trỏ vào vùng nhớ **đã bị thu hồi** (trả về địa chỉ biến Local trên Stack, hoặc đã `delete p` nhưng vẫn dùng `*p`). | Đọc phải dữ liệu rác hoặc làm hỏng vùng nhớ mới cấp phát. | Không bao giờ `return &local_var;`. Ngay sau khi `delete p;`, lập tức gán **`p = nullptr;`**. |
| **4. Memory Leak** *(Rò rỉ bộ nhớ)* | Cho con trỏ `p` đang giữ vùng nhớ Heap trỏ sang địa chỉ mới (`p = &x;`) mà quên `delete` vùng nhớ cũ trước đó. | Mất hoàn toàn dấu vết vùng nhớ Heap cũ, hao hụt RAM. | Giải phóng vùng nhớ cũ bằng `delete` / `free` trước khi gán địa chỉ mới cho con trỏ. |

---

## 7. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_pointer_basics_and_const.cpp`: Thực nghiệm địa chỉ `&`, giải tham chiếu `*`, kích thước cố định 8 bytes và 3 dạng `const pointer`.
* `02_pointer_arithmetic_arrays.cpp`: Số học con trỏ `ptr + i`, duyệt mảng bằng con trỏ chạy và tính khoảng cách `p2 - p1`.
* `03_double_and_void_pointers.cpp`: Thực hành Con trỏ cấp 2 (`**pp`), hoán đổi 2 con trỏ chuỗi và ép kiểu `void*`.
* `04_function_pointers_callbacks.cpp`: Khai báo Con trỏ hàm và viết hàm sắp xếp đa năng nhận Callback Comparator.
* `05_practice_exercises.cpp`: Giải 5 bài tập thuần số học con trỏ (Tìm Min/Max, Đảo mảng, `my_strlen`, Lọc phần tử tại chỗ 2 con trỏ, Máy tính dùng Jump Table).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_pointer_basics_and_const.cpp -o ptr_basics
./ptr_basics

g++ -Wall -Wextra -std=c++17 02_pointer_arithmetic_arrays.cpp -o ptr_arith
./ptr_arith

g++ -Wall -Wextra -std=c++17 03_double_and_void_pointers.cpp -o ptr_double
./ptr_double

g++ -Wall -Wextra -std=c++17 04_function_pointers_callbacks.cpp -o ptr_func
./ptr_func

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o ptr_practice
./ptr_practice
```

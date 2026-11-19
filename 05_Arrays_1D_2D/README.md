# Bài 1.5: Mảng 1 Chiều & Mảng 2 Chiều (Arrays & Matrices trong C/C++)

Tài liệu tổng hợp bản chất lưu trữ liên tiếp trên RAM của cấu trúc mảng tĩnh, hiện tượng thoái hóa thành con trỏ (**Array Decay**), quy tắc **Row-Major Order** của ma trận 2 chiều và các mẫu thuật toán xử lý mảng tối ưu.

---

## 1. Bản Chất Bộ Nhớ Của Mảng 1 Chiều (1D Arrays)

### 1.1. Cấp phát liên tiếp (Contiguous Memory Allocation)

Khi khai báo `int a[5] = {10, 20, 30, 40, 50};` (với `sizeof(int) = 4` bytes), hệ điều hành cấp phát **1 khối 20 bytes nằm liền kề nhau không ngắt quãng** trên RAM:

```text
  Chỉ số (Index):      a[0]         a[1]         a[2]         a[3]         a[4]
                    ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  Giá trị (Value):  │    10     │    20     │    30     │    40     │    50     │
                    └───────────┴───────────┴───────────┴───────────┴───────────┘
  Địa chỉ (Hex):      0x1000      0x1004      0x1008      0x100C      0x1010
                     (4 bytes)   (4 bytes)   (4 bytes)   (4 bytes)   (4 bytes)
```

* **Công thức truy cập ngẫu nhiên (Random Access) trong $O(1)$:**
  $$\text{Address}(a[i]) = \text{Address}(a[0]) + i \times \text{sizeof(DataType)}$$
  *(Đây chính là lý do tại sao chỉ số mảng trong C/C++ bắt buộc phải bắt đầu từ **`0`**: tại `i = 0`, độ lệch địa chỉ bằng `0`, trỏ thẳng vào đầu mảng mà không tốn phép trừ `(i - 1)`).*

### 1.2. Các quy tắc khởi tạo mảng tĩnh cần nhớ

| Cách viết | Ý nghĩa & Trạng thái các phần tử trong RAM |
| :--- | :--- |
| `int a[5];` *(Khai báo cục bộ trong hàm)* | Chứa toàn bộ **giá trị rác (Garbage values)** chưa xác định. |
| `int a[5] = {10, 20};` | `a[0] = 10`, `a[1] = 20`, tất cả các phần tử còn lại **tự động bằng `0`**. |
| `int a[100] = {0};` hoặc `int a[100] = {};` | Khởi tạo nhanh **toàn bộ 100 phần tử bằng `0`**. |
| `int a[100] = {-1};` *(Cạm bẫy!)* | Chỉ có `a[0] = -1`, còn 99 phần tử phía sau **vẫn bằng `0`** (không phải `-1`!). |
| `int a[] = {1, 2, 3, 4};` | Trình biên dịch tự đếm số phần tử và ấn định kích thước là `a[4]`. |

* **Công thức lấy số lượng phần tử của mảng tĩnh (Chỉ dùng được tại nơi khai báo mảng):**

  ```cpp
  size_t n = sizeof(a) / sizeof(a[0]); // Tổng số byte của mảng chia cho số byte của 1 phần tử
  ```

---

## 2. Hiện Tượng "Array Decay" & Hai Cạm Bẫy Chết Người

### 2.1. Hiện tượng Array Decay (Mảng thoái hóa thành Con trỏ khi truyền vào hàm)

Khi bạn truyền một mảng vào hàm, C/C++ **không bao giờ sao chép toàn bộ mảng** (vì mảng có thể nặng hàng chục MB), mà chỉ truyền **địa chỉ của phần tử đầu tiên (`&a[0]`)**:

```cpp
// Hai cách khai báo hàm dưới đây hoàn toàn giống hệt nhau dưới góc nhìn của Compiler:
void process_array(int arr[], int n);
void process_array(int *arr, int n);
```

* **Hệ quả 1:** Mọi thao tác sửa đổi `arr[i]` bên trong hàm sẽ **làm thay đổi trực tiếp mảng gốc** ở bên ngoài! (Nếu muốn bảo vệ mảng gốc chỉ để đọc, bắt buộc thêm `const`: `void print(const int arr[], int n)`).
* **Hệ quả 2:** Bên trong hàm, `sizeof(arr)` chỉ trả về kích thước của biến con trỏ (**4 bytes** trên máy 32-bit hoặc **8 bytes** trên máy 64-bit), mất hoàn toàn thông tin độ dài mảng. Do đó **luôn phải truyền kèm biến kích thước `int n` vào hàm**.

### 2.2. Lỗi tràn mảng (Out-of-Bounds Access) & Tràn bộ nhớ Stack

1. **Không có cơ chế kiểm tra biên tự động (No Bounds Checking):**
   * Với mảng `int a[5]`, chỉ số hợp lệ chỉ từ `0` đến `4`. Nếu bạn truy cập `a[5]` hoặc `a[-1]`, trình biên dịch **không báo lỗi**, nhưng lúc chạy chương trình sẽ đọc/ghi đè vào vùng nhớ của biến khác trên Stack, gây sai kết quả ngầm hoặc sập chương trình (**Segmentation Fault**).
2. **Giới hạn kích thước mảng trên Stack vs Data/BSS Segment:**
   * Bộ nhớ **Stack** của một chương trình thường chỉ từ **1 MB đến 8 MB**. Nếu khai báo mảng cục bộ quá lớn trong `main()` như `int a[1000000];` ($\approx 4\text{ MB}$), chương trình sẽ bị sập ngay lập tức do **Stack Overflow**.
   * *Giải pháp:* Với mảng tĩnh lớn ($\ge 10^6$ phần tử), hãy khai báo làm **biến toàn cục (Global)** hoặc thêm từ khóa **`static`** (`static int a[1000000];`) để đưa mảng sang vùng nhớ Data/BSS Segment (hoặc dùng cấp phát động/`std::vector` ở các giai đoạn sau).

---

## 3. Sổ Tay Các Kỹ Thuật Xử Lý Mảng 1 Chiều Cốt Lõi

| Kỹ thuật | Độ phức tạp | Mô tả quy trình chuẩn |
| :--- | :---: | :--- |
| **1. Đảo ngược mảng tại chỗ** *(Two Pointers)* | $O(N)$ Time $O(1)$ Space | Đặt `left = 0`, `right = n - 1`. Lặp khi `left < right`: hoán đổi `swap(a[left], a[right])`, sau đó `++left`, `--right`. |
| **2. Chèn phần tử vào `pos`** *(Shift Right)* | $O(N)$ Time | Phải dịch các phần tử **từ cuối mảng lùi về `pos + 1`** (`for (int i = n; i > pos; --i) a[i] = a[i - 1];`), gán `a[pos] = val`, rồi tăng `++n`. |
| **3. Xóa phần tử tại `pos`** *(Shift Left)* | $O(N)$ Time | Dịch các phần tử **từ `pos` tiến về `n - 2`** (`for (int i = pos; i < n - 1; ++i) a[i] = a[i + 1];`), rồi giảm `--n`. |
| **4. Mảng đếm tần suất** *(Frequency Array)* | $O(N)$ Time | Khi giá trị `a[i]` nhỏ ($\in [0, 10^6]$), dùng mảng `cnt[] = {0}` và tăng `cnt[a[i]]++` để đếm số lần xuất hiện mà không cần 2 vòng lặp lồng nhau. |
| **5. Mảng cộng dồn 1D** *(Prefix Sum)* | Tạo: $O(N)$ Truy vấn: $O(1)$ | Xây dựng `pref[i] = pref[i - 1] + a[i]`. Tổng các phần tử trong đoạn $[L, R]$ bất kỳ được tính ngay lập tức bằng: `pref[R] - (L > 0 ? pref[L - 1] : 0)`. |
| **6. Xoay mảng sang phải $K$ bước** *(Reversal Algorithm)* | $O(N)$ Time $O(1)$ Space | Đặt `k %= n`. Thực hiện 3 lần đảo ngược: (1) Đảo toàn bộ mảng `[0, n - 1]` $\rightarrow$ (2) Đảo `[0, k - 1]` $\rightarrow$ (3) Đảo `[k, n - 1]`. |

---

## 4. Mảng 2 Chiều (Ma Trận) & Quy Tắc Row-Major Order

### 4.1. Cách mảng 2 chiều nằm trên RAM (Row-Major Order)

Phần cứng RAM là 1 chiều, không có khái niệm "hàng và cột" vật lý. Mảng 2 chiều `int mat[2][3]` thực chất được trình biên dịch C/C++ trải phẳng thành **một dải 1 chiều theo từng hàng nối tiếp nhau**:

```text
  Ma trận logic 2x3:          Bản chất lưu trữ nối tiếp trên RAM (Row-Major):
  ┌───────┬───────┬───────┐      ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
  │ [0,0] │ [0,1] │ [0,2] │ ===► │mat[0][0]│mat[0][1]│mat[0][2]│mat[1][0]│mat[1][1]│mat[1][2]│
  ├───────┼───────┼───────┤      └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
  │ [1,0] │ [1,1] │ [1,2] │      |<--------- Hàng 0 --------->| |<--------- Hàng 1 --------->|
  └───────┴───────┴───────┘
  Công thức ánh xạ địa chỉ: Address(mat[i][j]) = Base + (i * MAX_COLS + j) * sizeof(type)
```

* **Tại sao truyền mảng 2D vào hàm bắt buộc phải ghi rõ số cột `MAX_COLS`?**
  Vì trong công thức ánh xạ `i * MAX_COLS + j`, để nhảy xuống hàng thứ `i`, trình biên dịch bắt buộc phải biết mỗi hàng rộng bao nhiêu cột (`MAX_COLS`). Do đó cú pháp hàm phải là: `void func(int mat[][MAX_COLS], int rows, int cols)`.
* **Tối ưu CPU Cache (Cache Locality):** Luôn duyệt vòng lặp **Hàng (`i`) ở ngoài, Cột (`j`) ở trong**. Nếu duyệt cột ở ngoài, hàng ở trong, CPU phải nhảy cóc qua từng hàng trên RAM, gây ra hiện tượng **Cache Miss** làm chậm tốc độ xử lý gấp 5–10 lần trên ma trận lớn!

### 4.2. Các công thức tọa độ trên Ma trận vuông $N \times N$ (Chỉ số từ $0$ đến $N - 1$)

* **Đường chéo chính (Main Diagonal):** Các phần tử có `i == j` (tức là `mat[i][i]`).
* **Đường chéo phụ (Anti-Diagonal):** Các phần tử có `i + j == n - 1` (tức là `mat[i][n - 1 - i]`).
* **Tam giác trên đường chéo chính:** Các phần tử có `i <= j`.
* **Tam giác dưới đường chéo chính:** Các phần tử có `i >= j`.
* **Ma trận chuyển vị (Transpose):** Hoán đổi `swap(mat[i][j], mat[j][i])` với mọi `j > i`.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_1d_array_memory_and_decay.cpp`: Kiểm chứng địa chỉ RAM liên tiếp và hiện tượng Array Decay khi truyền mảng vào hàm.
* `02_1d_array_techniques.cpp`: Thực hành đảo mảng 2 con trỏ, chèn, xóa phần tử và mảng đếm tần suất $O(N)$.
* `03_2d_matrix_operations.cpp`: Tính tổng đường chéo chính/phụ và thuật toán nhân 2 ma trận $O(M \times N \times P)$.
* `04_practice_exercises.cpp`: Giải 5 bài tập tổng kết Giai đoạn 1 (Tìm phần tử lớn thứ 2, Prefix Sum, Xoay mảng $O(1)$ Space, Điểm yên ngựa, Ma trận xoắn ốc).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_1d_array_memory_and_decay.cpp -o array_memory
./array_memory

g++ -Wall -Wextra -std=c++17 02_1d_array_techniques.cpp -o array_tech
./array_tech

g++ -Wall -Wextra -std=c++17 03_2d_matrix_operations.cpp -o matrix_ops
./matrix_ops

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o array_practice
./array_practice
```

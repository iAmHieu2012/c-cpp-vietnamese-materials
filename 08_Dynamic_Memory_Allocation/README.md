# Bài 2.3: Cấp Phát Bộ Nhớ Động (Dynamic Memory Allocation trong C/C++)

Tài liệu tổng hợp cách xin cấp phát, co giãn và thu hồi bộ nhớ trực tiếp trên phân vùng **Heap** lúc chương trình đang chạy (Runtime), đối chiếu cơ chế của ngôn ngữ C (`<stdlib.h>`) và C++ (`new`/`delete`), cùng kiến trúc cấp phát Ma trận 2 chiều động.

---

## 1. Tại Sao Phải Dùng Cấp Phát Động Trên Heap?

| Hạn chế của Mảng tĩnh trên Stack (`int a[1000];`) | Ưu thế của Cấp phát động trên Heap (`new` / `malloc`) |
| :--- | :--- |
| Kích thước cố định lúc viết code: Khai báo thừa thì lãng phí RAM, khai báo thiếu thì tràn mảng. | **Kích thước linh hoạt theo thực tế:** Nhập `n` từ bàn phím hoặc đọc từ file bao nhiêu thì xin cấp phát đúng bấy nhiêu byte. |
| Bộ nhớ Stack rất nhỏ ($1\text{MB} - 8\text{MB}$), khai báo mảng vài triệu phần tử sẽ bị sập (**Stack Overflow**). | Bộ nhớ Heap rộng hàng chục GB (tùy dung lượng RAM máy), chứa được các cấu trúc dữ liệu khổng lồ. |
| Tự động bị hủy ngay khi hàm kết thúc (không thể trả mảng cục bộ về cho hàm gọi). | **Chủ động kiểm soát vòng đời:** Dữ liệu tồn tại xuyên suốt qua các hàm cho đến khi lập trình viên chủ động gọi `delete` hoặc `free`. |

---

## 2. Cấp Phát Động Trong Ngôn Ngữ C (`<stdlib.h>` / `<cstdlib>`)

### 2.1. Bảng phân biệt `malloc`, `calloc`, `realloc` và `free`

| Hàm | Cú pháp chuẩn | Khởi tạo giá trị? | Cơ chế hoạt động dưới bộ nhớ Heap |
| :--- | :--- | :---: | :--- |
| **`malloc`** *(Memory Allocation)* | `(int*)malloc(n * sizeof(int))` | **KHÔNG** *(Chứa rác)* | Tìm 1 khối nhớ trống liên tiếp đủ `n * sizeof(int)` bytes trên Heap. Nhanh hơn `calloc` vì không tốn bước xóa bit. |
| **`calloc`** *(Contiguous Allocation)* | `(int*)calloc(n, sizeof(int))` | **Tự động bằng `0`** | Cấp phát vùng nhớ cho `n` phần tử và đặt toàn bộ các bit trong vùng nhớ đó về `0`. |
| **`realloc`** *(Re-allocation)* | `(int*)realloc(ptr, new_n * sizeof(int))` | Phần mở rộng thêm chứa rác | Co giãn vùng nhớ `ptr`: Nếu phía sau `ptr` còn trống đủ chỗ $\rightarrow$ nối dài tại chỗ. Nếu không đủ chỗ $\rightarrow$ tìm vùng nhớ mới đủ rộng, **tự copy dữ liệu cũ sang**, **tự `free` vùng nhớ cũ**, và trả về địa chỉ mới! |
| **`free`** | `free(ptr); ptr = NULL;` | Thu hồi vùng nhớ | Trả vùng nhớ về cho Hệ điều hành quản lý. Không tự làm thay đổi địa chỉ lưu trong `ptr` nên **phải gán `ptr = NULL;` ngay sau khi `free`**. |

### 2.2. Quy tắc an toàn bắt buộc khi dùng `realloc`

Nếu hệ thống hết RAM, `realloc` sẽ trả về `NULL` nhưng **vùng nhớ cũ vẫn còn nguyên chưa bị giải phóng**. Do đó, **TUYỆT ĐỐI KHÔNG** gán trực tiếp `ptr = realloc(ptr, new_size);` (vì nếu trả về `NULL`, biến `ptr` bị ghi đè mất địa chỉ cũ gây **Memory Leak** ngay lập tức!).

```c
// CÁCH VIẾT CHUẨN CÔNG NGHIỆP VỚI REALLOC:
int *temp = (int*)realloc(ptr, new_n * sizeof(int));
if (temp != NULL) {
    ptr = temp; // Chỉ cập nhật ptr khi realloc thành công!
} else {
    // Xử lý lỗi thiếu RAM, lúc này ptr vẫn giữ an toàn dữ liệu cũ
}
```

---

## 3. Cấp Phát Động Trong C++ (`new` / `delete` & `new[]` / `delete[]`)

### 3.1. Cú pháp cấp phát và giải phóng trong C++

| Mục đích | Cú pháp Cấp phát (`new`) | Cú pháp Giải phóng (`delete`) | Ghi chú quan trọng |
| :--- | :--- | :--- | :--- |
| **1 biến đơn lẻ** *(Chứa rác)* | `int *p = new int;` | `delete p;` `p = nullptr;` | Không dùng `delete[]` cho biến đơn! |
| **1 biến đơn lẻ** *(Khởi tạo = 42)* | `int *p = new int(42);` | `delete p;` `p = nullptr;` | Có thể dùng `new int{42}` (C++11). |
| **Mảng 1D động** *(Chứa rác)* | `int *a = new int[n];` | `delete[] a;` `a = nullptr;` | **Bắt buộc có `[]`** ở `delete[] a;`! |
| **Mảng 1D động** *(Tự động = `0`)* | `int *a = new int[n]();` *(hoặc `new int[n]{}`)* | `delete[] a;` `a = nullptr;` | Thêm cặp `()` hoặc `{}` ở cuối để điền toàn bộ bằng `0` (giống `calloc`). |
| **Mảng 1D động** *(Gán sẵn đầu)* | `int *a = new int[5]{1, 2};` | `delete[] a;` `a = nullptr;` | `a[0]=1, a[1]=2`, các ô còn lại tự bằng `0`. |

### 3.2. So sánh trực diện: `new` / `delete` (C++) vs `malloc` / `free` (C)

| Tiêu chí | `new` / `delete` (Chuẩn C++) | `malloc` / `free` (Chuẩn C) |
| :--- | :--- | :--- |
| **Bản chất** | Là **Toán tử (Operators)** tích hợp sẵn trong ngôn ngữ, có thể nạp chồng (Overload). | Là **Hàm thư viện (Library Functions)** thuộc `<stdlib.h>`. |
| **Kiểu trả về & Ép kiểu** | Trả về **đúng kiểu con trỏ** (`int*`), an toàn kiểu (Type-safe), không cần ép kiểu. | Trả về con trỏ vô kiểu `void*`, bắt buộc ép kiểu trong C++. |
| **Tính toán kích thước** | Trình biên dịch **tự động tính số byte** theo kiểu dữ liệu. | Lập trình viên phải tự nhân thủ công `n * sizeof(type)`. |
| **Hàm tạo / Hàm hủy (OOP)** | **Tự động gọi Constructor** khi `new` và **gọi Destructor** khi `delete`. | **KHÔNG gọi Constructor / Destructor** (Dùng cho Object C++ sẽ làm hỏng đối tượng!). |
| **Xử lý khi hết RAM** | Mặc định ném ra ngoại lệ **`std::bad_alloc`** (hoặc trả về `nullptr` nếu dùng `new (std::nothrow)`). | Trả về giá trị `NULL`. |

> **Quy tắc sống còn:** **KHÔNG BAO GIỜ trộn lẫn** `malloc` với `delete` hoặc `new` với `free`, và không bao giờ cấp phát bằng `new int[n]` nhưng lại xóa bằng `delete a;` (thiếu dấu `[]` sẽ gây hành vi không xác định - Undefined Behavior hoặc rò rỉ bộ nhớ của các phần tử phía sau).

---

## 4. Kỹ Thuật Tự Co Giãn Mảng Động (Bản Chất Của `std::vector`)

Vì C++ không có toán tử `realloc` cho `new[]`, để lưu trữ số lượng phần tử tăng dần mà không biết trước giới hạn, ta tách làm 2 thông số:

* **`size`**: Số lượng phần tử thực tế đang lưu.
* **`capacity`**: Sức chứa tối đa hiện tại của vùng nhớ trên Heap.
* **Chiến lược Nhân Đôi (Doubling Strategy):** Khi `size == capacity` (mảng đầy), ta cấp phát mảng mới có `new_capacity = capacity * 2`, sao chép `size` phần tử cũ sang, gọi `delete[]` mảng cũ và trỏ sang mảng mới. Kỹ thuật nhân đôi này giúp độ phức tạp trung bình của mỗi lần thêm phần tử đạt **$O(1)$ (Amortized Constant Time)**.

---

## 5. Hai Kiến Trúc Cấp Phát Ma Trận 2 Chiều Động Trên Heap

Giả sử cần cấp phát ma trận động kích thước `rows` $\times$ `cols` (nhập lúc chạy):

### Cách 1: Dùng Con trỏ cấp 2 (`int **mat` — Mảng của các con trỏ hàng)

```text
  mat (int**)      Mảng con trỏ hàng (int*)         Các hàng dữ liệu nằm rời rạc trên Heap
  ┌────────┐       ┌───────────┐                    ┌──────┬──────┬──────┬──────┐
  │ 0x5000 │──────►│ mat[0]    │───────────────────►│ [0,0]│ [0,1]│ [0,2]│ [0,3]│
  └────────┘       ├───────────┤                    └──────┴──────┴──────┴──────┘
                   │ mat[1]    │──────┐             ┌──────┬──────┬──────┬──────┐
                   ├───────────┤      └────────────►│ [1,0]│ [1,1]│ [1,2]│ [1,3]│
                   │ mat[2]    │───────────────┐    └──────┴──────┴──────┴──────┘
                   └───────────┘               │    ┌──────┬──────┬──────┬──────┐
                                               └───►│ [2,0]│ [2,1]│ [2,2]│ [2,3]│
                                                    └──────┴──────┴──────┴──────┘
```

* **Ưu điểm:** Dùng cú pháp tự nhiên `mat[i][j]`; cho phép tạo **Mảng răng cưa (Jagged Array)** nơi mỗi hàng có số cột khác nhau (ví dụ Tam giác Pascal).
* **Nhược điểm:** Tốn `rows + 1` lần gọi `new[]` và `delete[]`; các hàng nằm rời rạc trên RAM nên dễ bị **Cache Miss** khi duyệt.
* **Quy tắc giải phóng:** Phải xóa **từ trong ra ngoài** (vòng lặp `delete[] mat[i];` trước, rồi mới `delete[] mat;` sau).

### Cách 2: Trải phẳng thành 1 mảng 1D liên tiếp (`int *flat = new int[rows * cols]`)

* **Cơ chế:** Chỉ gọi đúng 1 lệnh `new int[rows * cols]` và truy cập phần tử hàng `i`, cột `j` qua công thức ánh xạ: **`flat[i * cols + j]`**.
* **Ưu điểm vượt trội:** Toàn bộ ma trận nằm **liền kề 1 khối duy nhất** trên Heap, tối ưu bộ nhớ đệm CPU Cache nhanh gấp nhiều lần Cách 1, không phân mảnh bộ nhớ và chỉ cần 1 dòng `delete[] flat;` để thu hồi.

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_c_malloc_calloc_realloc.c`: Thực hành bộ tứ `malloc`, `calloc`, `realloc` (với con trỏ tạm an toàn) và `free` trong C.
* `02_cpp_new_delete_and_resize.cpp`: Thực hành `new`, `delete`, `new[]`, `delete[]` và viết hàm tự động nhân đôi `capacity` khi mảng đầy.
* `03_2d_dynamic_matrix.cpp`: So sánh trực tiếp 2 cách cấp phát và giải phóng Ma trận 2D động trên Heap (`int**` vs Contiguous 1D).
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Lọc số dương trả về mảng Heap, Trộn 2 mảng tăng dần $O(N+M)$, Nhập mảng tự co giãn, Chuyển vị ma trận động, Tam giác Pascal bằng Jagged Array).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
# Biên dịch file C
gcc -Wall -Wextra 01_c_malloc_calloc_realloc.c -o c_alloc
./c_alloc

# Biên dịch các file C++
g++ -Wall -Wextra -std=c++17 02_cpp_new_delete_and_resize.cpp -o cpp_alloc
./cpp_alloc

g++ -Wall -Wextra -std=c++17 03_2d_dynamic_matrix.cpp -o dyn_matrix
./dyn_matrix

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o dyn_practice
./dyn_practice
```

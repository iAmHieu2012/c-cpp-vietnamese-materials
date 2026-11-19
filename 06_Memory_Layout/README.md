# Bài 2.1: Phân Vùng Bộ Nhớ Trong C/C++ (Memory Layout)

Tài liệu giải mã cách Hệ điều hành tổ chức bộ nhớ RAM khi một chương trình C/C++ thực thi, phân tích chi tiết 5 phân vùng (**Text, Data, BSS, Heap, Stack**), so sánh hiệu năng giữa **Stack và Heap**, và cách phòng tránh các lỗi bộ nhớ nghiêm trọng.

---

## 1. Sơ Đồ Tổng Quan 5 Phân Vùng Bộ Nhớ (Virtual Memory Layout)

Khi bạn chạy một file thực thi (`.exe`), Hệ điều hành cấp cho tiến trình một không gian bộ nhớ được chia làm **5 phân vùng chính** sắp xếp từ địa chỉ thấp đến địa chỉ cao:

```text
  ĐỊA CHỈ CAO (High Memory Address - ví dụ: 0x7FFF...)
  ┌──────────────────────────────────────────────────────────┐
  │ 5. STACK SEGMENT (Ngăn xếp - Cấp phát tĩnh cục bộ)       │
  │    • Chứa: Biến cục bộ (Local), Tham số hàm, Return Addr │
  │    • Quản lý: Tự động bởi CPU (qua thanh ghi ESP/RSP)    │
  │    • Hướng mở rộng: Phình từ TRÊN XUỐNG DƯỚI             │
  ├────────────────────────────┬─────────────────────────────┤
  │                            │                             │
  │                            ▼ (Stack grows downward)      │
  │                    (Vùng trống tự do)                    │
  │                            ▲ (Heap grows upward)         │
  │                            │                             │
  ├────────────────────────────┴─────────────────────────────┤
  │ 4. HEAP SEGMENT (Đống - Cấp phát động)                   │
  │    • Chứa: Vùng nhớ xin cấp phát lúc chạy (malloc / new) │
  │    • Quản lý: Thủ công bởi Lập trình viên (free / delete)│
  │    • Hướng mở rộng: Phình từ DƯỚI LÊN TRÊN               │
  ├──────────────────────────────────────────────────────────┤
  │ 3. BSS SEGMENT (Block Started by Symbol)                 │
  │    • Chứa: Biến Global & Static CHƯA khởi tạo (hoặc = 0) │
  │    • Đặc điểm: Được Hệ điều hành tự động điền toàn bộ = 0│
  ├──────────────────────────────────────────────────────────┤
  │ 2. DATA SEGMENT (Initialized Data & Read-Only .rodata)   │
  │    • Chứa: Biến Global & Static ĐÃ khởi tạo khác 0,      │
  │            biến hằng toàn cục, hằng chuỗi ("Hello")      │
  ├──────────────────────────────────────────────────────────┤
  │ 1. TEXT / CODE SEGMENT                                   │
  │    • Chứa: Mã máy nhị phân (Machine Code) của các hàm    │
  │    • Đặc điểm: Chỉ đọc (Read-Only) để chống tự sửa lệnh  │
  └──────────────────────────────────────────────────────────┘
  ĐỊA CHỈ THẤP (Low Memory Address - ví dụ: 0x0040...)
```

---

## 2. Chi Tiết Từng Phân Vùng & Quy Tắc Phân Loại Biến

| Phân vùng | Lưu trữ thành phần nào? | Quyền truy cập | Giá trị mặc định | Thời gian tồn tại (Lifetime) |
| :--- | :--- | :---: | :--- | :--- |
| **1. Text / Code** | Mã lệnh thực thi của các hàm (`main`, các hàm con). | Chỉ đọc & Thực thi (`R-X`) | Mã máy nhị phân | Suốt thời gian chương trình chạy. |
| **2a. `.rodata`** *(Read-Only Data)* | Hằng chuỗi (`"Hello"`), biến `const` toàn cục. | Chỉ đọc (`R--`) | Giá trị khởi tạo | Suốt thời gian chương trình chạy. |
| **2b. Data** | Biến `global` và biến `static` (cả trong/ngoài hàm) **đã khởi tạo $\ne 0$**. | Đọc & Ghi (`RW-`) | Giá trị khởi tạo | Suốt thời gian chương trình chạy. |
| **3. BSS** | Biến `global` và biến `static` **chưa khởi tạo** (hoặc khởi tạo bằng `0`). | Đọc & Ghi (`RW-`) | **Tự động bằng `0`** | Suốt thời gian chương trình chạy. |
| **4. Heap** | Vùng nhớ cấp phát động qua `malloc`, `calloc`, `new`, `new[]`. | Đọc & Ghi (`RW-`) | Rác (`malloc`/`new`) hoặc `0` (`calloc`) | Từ lúc gọi `new`/`malloc` đến khi gọi `delete`/`free` (hoặc tắt app). |
| **5. Stack** | Biến cục bộ (Local), tham số hàm, địa chỉ trả về của hàm. | Đọc & Ghi (`RW-`) | **Giá trị rác (Garbage)** | Sinh ra khi vào hàm/khối `{ }`, **tự hủy ngay khi ra khỏi khối `{ }`**. |

> **Câu hỏi phỏng vấn kinh điển:** *Tại sao phải tách riêng Data Segment và BSS Segment?*  
> **Trả lời:** Để giảm dung lượng file thực thi `.exe` trên ổ cứng! Các biến ở **Data Segment** có giá trị cụ thể (như `int a = 10;`, mảng khởi tạo sẵn) nên phải lưu trực tiếp giá trị đó vào file `.exe`. Ngược lại, các biến ở **BSS** (ví dụ `static int arr[1000000];` chưa khởi tạo) đều bằng `0`, nên file `.exe` chỉ cần ghi chú một dòng *"Cần 4MB toàn số 0"*, khi nào người dùng bấm chạy thì Hệ điều hành mới cấp RAM và điền số `0` vào.

---

## 3. So Sánh Trực Diện: STACK vs HEAP (Trọng Tâm Kỹ Thuật)

| Tiêu chí | Bộ nhớ STACK (Ngăn xếp) | Bộ nhớ HEAP (Đống) |
| :--- | :--- | :--- |
| **Cơ chế cấp phát** | **Tự động** lúc biên dịch/gọi hàm (LIFO). | **Thủ công** lúc chương trình đang chạy (Runtime). |
| **Tốc độ cấp phát** | **Cực kỳ nhanh** (Chỉ tốn 1 lệnh CPU dịch chuyển thanh ghi con trỏ Stack `RSP`). | **Chậm hơn nhiều** (Hệ điều hành phải tìm khối nhớ trống phù hợp, cập nhật bảng quản lý). |
| **Kích thước tối đa** | **Rất nhỏ và cố định** (Thường chỉ từ **1 MB – 8 MB** tùy hệ điều hành). | **Rất lớn và linh hoạt** (Chỉ bị giới hạn bởi dung lượng RAM vật lý và bộ nhớ ảo của máy). |
| **Phân mảnh bộ nhớ** | **Không bao giờ bị phân mảnh** (Do cấp phát/thu hồi liền kề theo thứ tự LIFO). | **Dễ bị phân mảnh (Fragmentation)** nếu cấp phát và giải phóng nhiều khối nhỏ liên tục. |
| **Lỗi nguy hiểm** | **Stack Overflow** (Tràn ngăn xếp do đệ quy vô tận hoặc khai báo mảng cục bộ quá lớn). | **Memory Leak** (Rò rỉ bộ nhớ do quên `delete`/`free`), **Dangling Pointer**, **Double Free**. |

---

## 4. Ba Tình Huống Thực Chiến Dễ Nhầm Lẫn Nhất

### 4.1. Phân biệt "Bản thân biến con trỏ" và "Vùng nhớ con trỏ trỏ tới"

Khi bạn viết câu lệnh sau bên trong hàm `main()`:

```cpp
int *ptr = new int(2026);
```

Có **2 vùng nhớ ở 2 phân vùng hoàn toàn khác nhau** được tạo ra:

1. Biến con trỏ `ptr` (nặng 8 bytes trên máy 64-bit, dùng để lưu địa chỉ): Là biến cục bộ của `main()` nên nằm trên **STACK** (`&ptr`).
2. Ô nhớ nguyên 4 bytes chứa giá trị `2026`: Do toán tử `new` tạo ra nên nằm trên **HEAP** (`ptr`).

### 4.2. Phân biệt `char s[] = "Hello";` và `const char *p = "Hello";`

```text
  1. char s[] = "Hello";         --> Tạo mảng 6 bytes trên STACK, copy {'H','e','l','l','o','\0'} vào.
                                     Sửa s[0] = 'h' HỢP LỆ 100%!
  2. const char *p = "Hello";    --> Con trỏ p (trên Stack) trỏ thẳng vào chuỗi "Hello" ở vùng .rodata.
                                     Cố tình sửa p[0] = 'h' sẽ gây CRASH (Segmentation Fault)!
```

### 4.3. Ba lỗi bộ nhớ chết người cần tuyệt đối tránh

1. **Dangling Pointer (Trả về địa chỉ của biến Local trên Stack):**
   * Biến cục bộ trên Stack bị thu hồi ngay khi hàm kết thúc. Không bao giờ `return &local_var;` từ một hàm! Nếu muốn dữ liệu sống sót sau khi hàm kết thúc, phải cấp phát trên **Heap** (`new`), dùng biến `static`, hoặc truyền tham chiếu từ ngoài vào.
2. **Stack Overflow (Tràn bộ nhớ Stack):**
   * Xảy ra khi gọi đệ quy không có điều kiện dừng, hoặc khai báo mảng cục bộ quá lớn (ví dụ `int a[2000000];` tốn 8MB trong `main`). Với mảng lớn, hãy chuyển sang **Heap** (`new int[2000000]`) hoặc đặt ở **BSS** (biến Global/`static`).
3. **Memory Leak (Rò rỉ bộ nhớ trên Heap):**
   * Mỗi lệnh `new` phải đi kèm một lệnh `delete`, mỗi lệnh `new[]` phải đi kèm `delete[]`, và mỗi `malloc()` phải đi kèm `free()`. Nếu cấp phát trên Heap trong vòng lặp mà quên giải phóng, RAM sẽ bị ăn mòn cho đến khi treo máy.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_memory_segments_address.cpp`: In địa chỉ hệ Hex của cả 5 phân vùng (Text, Data, BSS, Heap, Stack) để kiểm chứng thứ tự sắp xếp trên RAM.
* `02_stack_vs_heap_behavior.cpp`: Thực nghiệm hướng phát triển ngược chiều nhau của Stack (từ cao xuống thấp) và Heap (từ thấp lên cao), cấp phát mảng lớn trên Heap.
* `03_practice_exercises.cpp`: Phân biệt chuỗi trên Stack vs `.rodata`, sửa lỗi trả về địa chỉ biến Local và phòng chống Memory Leak.

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_memory_segments_address.cpp -o mem_segments
./mem_segments

g++ -Wall -Wextra -std=c++17 02_stack_vs_heap_behavior.cpp -o stack_vs_heap
./stack_vs_heap

g++ -Wall -Wextra -std=c++17 03_practice_exercises.cpp -o mem_practice
./mem_practice
```

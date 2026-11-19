# Bài 6.3: Quản Lý Bộ Nhớ Tự Động (RAII & Smart Pointers: `unique_ptr`, `shared_ptr`, `weak_ptr`)

Tài liệu hệ thống hóa triết lý cốt lõi của C++ hiện đại — **RAII (Resource Acquisition Is Initialization)** — và bộ ba Con trỏ thông minh trong thư viện `<memory>`: **`std::unique_ptr`** (sở hữu độc quyền), **`std::shared_ptr`** (sở hữu chia sẻ qua đếm tham chiếu) và **`std::weak_ptr`** (phá vỡ vòng lặp tham chiếu chống rò rỉ bộ nhớ).

---

## 1. Triết Lý RAII & Tại Sao C++ Hiện Đại Xóa Bỏ `new` / `delete` Thủ Công?

### 1.1. Bản chất của RAII (Resource Acquisition Is Initialization)

* **Vấn đề của con trỏ thô (`Raw Pointer`):** Khi bạn gọi `int *p = new int[100];`, nếu ở giữa hàm có lệnh `return` sớm hoặc một hàm con ném ra ngoại lệ (**Exception**), dòng `delete[] p;` ở cuối hàm sẽ **bị bỏ qua vĩnh viễn** $\implies$ **Memory Leak**!
* **Giải pháp RAII:** Bọc tài nguyên Heap (hoặc File, Socket, Mutex) vào bên trong một **đối tượng nằm trên Stack**. Vì C++ cam kết **100% gọi Destructor của mọi biến trên Stack khi ra khỏi khối `{ }`** (bất kể thoát bình thường hay do Exception — *Stack Unwinding*), tài nguyên luôn được thu hồi tự động!

### 1.2. Bảng tổng quan 3 loại Smart Pointers trong `<memory>`

| Tiêu chí | `std::unique_ptr<T>` | `std::shared_ptr<T>` | `std::weak_ptr<T>` |
| :--- | :--- | :--- | :--- |
| **Quyền sở hữu** | **Độc quyền (1-1):** Tại một thời điểm chỉ duy nhất 1 `unique_ptr` giữ vùng nhớ. | **Chia sẻ (N-1):** Nhiều `shared_ptr` cùng trỏ chung 1 đối tượng trên Heap. | **Không sở hữu (Quan sát):** Nhìn vào đối tượng của `shared_ptr` mà không giữ mạng nó. |
| **Sao chép (Copy)?** | **CẤM (`= delete`)** — Chỉ cho phép **Move (`std::move`)**! | **ĐƯỢC** — Mỗi lần Copy tăng bộ đếm tham chiếu `++use_count`. | **ĐƯỢC** — Không làm tăng `use_count` chính. |
| **Kích thước (`sizeof`)** | **8 bytes** *(Bằng đúng con trỏ thô `T*`, 0 chi phí phụ!)*. | **16 bytes** *(Lưu 2 con trỏ: 1 trỏ tới Object, 1 trỏ tới Control Block)*. | **16 bytes** *(Giống `shared_ptr`)*. |
| **Hàm khởi tạo chuẩn** | **`std::make_unique<T>(args...)`** *(C++14)* | **`std::make_shared<T>(args...)`** *(C++11)* | Khởi tạo từ một `std::shared_ptr<T>` có sẵn. |
| **Khi nào sử dụng?** | **Mặc định cho 90% trường hợp** cần cấp phát Heap hoặc Đa hình OOP. | Khi một tài nguyên thực sự cần dùng chung bởi nhiều hệ thống độc lập. | Phá vỡ **Vòng lặp tham chiếu (Circular Reference)** của `shared_ptr`, hoặc làm Cache. |

---

## 2. `std::unique_ptr` — Con Trỏ Độc Quyền Zero-Overhead (`01_raii_and_unique_ptr.cpp`)

```cpp
// 1. Luôn khởi tạo bằng std::make_unique (An toàn ngoại lệ & không phải gõ từ khóa 'new'):
auto p1 = std::make_unique<Student>(1001, "Nguyen Van An");

// 2. Cấm Copy, chỉ có thể chuyển giao quyền sở hữu bằng std::move:
// auto p2 = p1;            // LỖI BIÊN DỊCH!
auto p2 = std::move(p1);    // HỢP LỆ: p1 trở thành nullptr, p2 nắm quyền sở hữu duy nhất!
```

* **Các phương thức quan trọng của `unique_ptr`:**
  * `*p` và `p->method()`: Truy cập đối tượng giống hệt con trỏ thường.
  * `p.get()`: Trả về địa chỉ con trỏ thô `T*` (chỉ dùng để truyền vào các hàm C cũ đọc dữ liệu, **tuyệt đối không gọi `delete p.get()`**!).
  * `p.reset()`: Hủy ngay đối tượng hiện tại và đưa `p` về `nullptr`.
  * `p.release()`: Trả về con trỏ thô và **từ bỏ quyền quản lý** (không xóa vùng nhớ).
* **Custom Deleter (Bộ hủy tùy chỉnh):** Không chỉ quản lý bộ nhớ `delete`, `unique_ptr` có thể tự động đóng `FILE*` bằng `std::fclose` hoặc đóng Socket khi ra khỏi hàm!

---

## 3. `std::shared_ptr` & Kiến Trúc Control Block Dưới Thanh RAM (`02_shared_ptr_reference_counting.cpp`)

```text
  Trên STACK (2 biến shared_ptr cùng chia sẻ)        Trên HEAP (Khi tạo bằng std::make_shared<T>())
  ┌─────────────────────────┐                        ┌────────────────────────────┬────────────────────────┐
  │ sp1 (16 bytes):         │                        │ Control Block (Atomic):    │ Vùng nhớ Object T:     │
  │  • ptr_data ────────────┼───────────────────────►│  • Strong Ref Count = 2    │  • id = 101            │
  │  • ptr_control ─────────┼───────────────────┐    │  • Weak Ref Count   = 0    │  • name = "Shared"     │
  ├─────────────────────────┤                   └───►│  • Deleter / Allocator     │                        │
  │ sp2 = sp1 (16 bytes)    │───────────────────────►└────────────────────────────┴────────────────────────┘
  └─────────────────────────┘
  * Mỗi khi copy `sp2 = sp1`, Strong Ref Count tăng lên 2 (bằng biến nguyên tử Atomic thread-safe).
  * Khi một shared_ptr ra khỏi scope, Strong Ref Count giảm đi 1. Chỉ khi Strong Ref Count chạm 0, Object T mới bị hủy!
```

* **Tại sao luôn dùng `std::make_shared<T>(...)` thay vì `std::shared_ptr<T>(new T(...))`?**
  * Nếu dùng `new T`, hệ thống phải cấp phát Heap **2 lần rời rạc** (1 lần cho `T`, 1 lần cho `Control Block`).
  * Dùng **`std::make_shared<T>`** gộp cả `Control Block` và `Object T` vào **1 khối nhớ liên tiếp duy nhất trên Heap**, nhanh gấp đôi và tối ưu bộ nhớ đệm CPU Cache!

---

## 4. `std::weak_ptr` & Cách Phá Vỡ Vòng Lặp Tham Chiếu (Circular Reference) (`03_weak_ptr_circular_reference.cpp`)

### 4.1. Thảm họa rò rỉ bộ nhớ khi hai `shared_ptr` trỏ vòng tròn vào nhau

```text
  LỖI CIRCULAR REFERENCE (2 Node trỏ chéo bằng shared_ptr):
  Node A (Ref Count = 2)  ────(shared_ptr next)────►  Node B (Ref Count = 2)
         ▲────────────────────(shared_ptr prev)────────────────────┘
  * Khi ra khỏi hàm main(), con trỏ ngoài bị hủy làm Ref Count của A và B giảm từ 2 xuống 1.
  * Vì A đợi B hủy trước mới giảm về 0, còn B lại đợi A hủy trước ===> KHÔNG BAO GIỜ VỀ 0 (Memory Leak 100%)!
```

### 4.2. Giải pháp: Dùng `std::weak_ptr` cho chiều ngược lại (`prev` hoặc `parent`)

* Thay con trỏ chiều ngược `std::shared_ptr<Node> prev` bằng **`std::weak_ptr<Node> prev`**.
* Vì `weak_ptr` **không làm tăng `Strong Ref Count`**, khi thoát khỏi hàm, `Ref Count` của cả hai Node đều giảm về `0` và tự động giải phóng sạch sẽ!
* **Cách truy cập an toàn qua `weak_ptr`:** Vì đối tượng đích có thể đã bị hủy bất cứ lúc nào, `weak_ptr` không có toán tử `*` hay `->`. Bắt buộc phải gọi **`if (auto sp = wp.lock())`** để nâng cấp tạm thời lên `shared_ptr` trong lúc đọc dữ liệu!

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_raii_philosophy.cpp`: **Triết lý RAII (Resource Acquisition Is Initialization)** — Tự xây dựng `ScopedFileGuard` và `ScopedIntBuffer` đảm bảo thu hồi tài nguyên 100% kể cả khi ném Exception (Stack Unwinding).
* `02_unique_ptr_exclusive_ownership.cpp`: **`std::unique_ptr`** — Quyền sở hữu độc quyền Zero-Overhead, `std::make_unique`, chuyển quyền sở hữu bằng `std::move`, mảng `unique_ptr<int[]>` và Custom Deleter cho `FILE*`.
* `03_shared_ptr_reference_counting.cpp`: **`std::shared_ptr`** — `std::make_shared`, Control Block và kiểm chứng sự thay đổi của `use_count()` qua các scope lồng nhau.
* `04_weak_ptr_circular_reference.cpp`: **`std::weak_ptr`** — Thực nghiệm lỗi Memory Leak do Circular Reference giữa 2 `shared_ptr` và cách phá vòng bằng `std::weak_ptr` + `.lock()`.
* `05_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Đa hình OOP với `vector<unique_ptr<Shape>>`, Cây BST hoàn toàn bằng `unique_ptr` không cần viết Destructor, Quan hệ 2 chiều `Department-Worker` với `shared_ptr`/`weak_ptr`, Bộ nhớ đệm `AssetCache` bằng `weak_ptr`, Exception Safety với RAII).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_raii_philosophy.cpp -o 01_raii_philosophy
./01_raii_philosophy

g++ -Wall -Wextra -std=c++17 02_unique_ptr_exclusive_ownership.cpp -o 02_unique_ptr_exclusive_ownership
./02_unique_ptr_exclusive_ownership

g++ -Wall -Wextra -std=c++17 03_shared_ptr_reference_counting.cpp -o 03_shared_ptr_reference_counting
./03_shared_ptr_reference_counting

g++ -Wall -Wextra -std=c++17 04_weak_ptr_circular_reference.cpp -o 04_weak_ptr_circular_reference
./04_weak_ptr_circular_reference

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

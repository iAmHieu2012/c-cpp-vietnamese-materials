# Bài 4.3: Cấu Trúc Dữ Liệu Tuyến Tính (Linked List, Stack, Queue, Deque & Monotonic Patterns)

Tài liệu hệ thống hóa kiến trúc bộ nhớ và cơ chế cài đặt thủ công (từ con trỏ & mảng động trên Heap, tuân thủ RAII) của các cấu trúc dữ liệu tuyến tính cốt lõi: **Singly / Doubly / Circular Linked List**, **Stack (LIFO)**, **Circular Queue / Ring Buffer (FIFO)**, **Deque (Hàng đợi 2 đầu)** và kỹ thuật **Monotonic Stack / Monotonic Deque**.

---

## 1. So Sánh Trực Diện: Mảng (Contiguous Array) vs Danh Sách Liên Kết (Linked List)

| Tiêu chí | Mảng tĩnh / Mảng động (`std::vector`) | Danh sách liên kết (`Singly` / `Doubly Linked List`) |
| :--- | :--- | :--- |
| **Bố cục trên RAM** | Các phần tử nằm **liền kề nhau thành 1 khối duy nhất**. | Các Node nằm **rải rác ngẫu nhiên trên Heap**, nối nhau bằng con trỏ. |
| **Truy cập chỉ số `a[i]`** | **$O(1)$** (Công thức địa chỉ: `Base + i * sizeof(T)`). | **$O(N)$** (Bắt buộc phải đi bộ qua từng con trỏ `next` từ `head`). |
| **Chèn / Xóa ở Đầu** | **$O(N)$** (Phải dịch chuyển toàn bộ các phần tử phía sau). | **$O(1)$** (Chỉ cần cập nhật 1–2 liên kết con trỏ). |
| **Chèn / Xóa ở Cuối** | **$O(1)$** *(Amortized)* khi chưa vượt `capacity`. | Chèn cuối **$O(1)$** (nếu giữ `tail`); Xóa cuối: **$O(N)$** (Singly) hoặc **$O(1)$** (Doubly). |
| **Chèn / Xóa ở Giữa** *(khi đã có con trỏ tại Node)* | **$O(N)$** (Do phải dịch mảng để tạo/lấp chỗ trống). | **$O(1)$** (Đổi dây con trỏ tại chỗ mà không làm dịch chuyển Node nào khác). |
| **CPU Cache Locality** | **Cực kỳ xuất sắc** (CPU nạp sẵn cả dải ô nhớ kề nhau vào L1/L2 Cache). | **Kém** (Nhảy địa chỉ Heap ngẫu nhiên gây **Cache Miss** liên tục). |
| **Chi phí bộ nhớ phụ** | `0 bytes` phụ trên mỗi phần tử. | Tốn thêm **8 bytes/Node** (`next`) hoặc **16 bytes/Node** (`prev + next`) trên hệ 64-bit. |

---

## 2. Giải Phẫu 3 Loại Danh Sách Liên Kết

### 2.1. Singly Linked List (`01_singly_linked_list.cpp`) & Kỹ thuật Đảo ngược tại chỗ

```text
  Cấu trúc Singly Linked List (có cả head và tail):
  head                                            tail
   │                                               │
   ▼                                               ▼
  ┌──────┬──────┐    ┌──────┬──────┐    ┌──────┬─────────┐
  │  10  │ next ┼───►│  20  │ next ┼───►│  30  │ nullptr │
  └──────┴──────┘    └──────┴──────┘    └──────┴─────────┘

  Kỹ thuật Đảo ngược tại chỗ (In-place Reverse — O(N) Time, O(1) Space) bằng 3 con trỏ trượt:
     prev       curr     next_node
      │          │          │
   nullptr ◄─── [10]       [20] ───► [30] ───► nullptr
                 (1) Lưu: next_node = curr->next;
                 (2) Đảo: curr->next = prev;
                 (3) Trượt lên: prev = curr; curr = next_node;
```

* **Tại sao xóa cuối (`pop_back`) trong Singly List vẫn tốn $O(N)$ dù đã có con trỏ `tail`?**
  Vì để xóa `tail`, ta phải cập nhật con trỏ `next` của **Node đứng ngay trước `tail`** thành `nullptr`. Nhưng liên kết chỉ đi 1 chiều từ trái sang phải, không có con trỏ lùi, nên bắt buộc phải duyệt từ `head` để tìm Node áp chót!

### 2.2. Doubly Linked List & Circular Linked List (`02_doubly_and_circular_list.cpp`)

```text
  1. Doubly Linked List (Liên kết 2 chiều `prev` và `next` — Xóa đầu, cuối, giữa đều O(1)):
            ┌──────┬──────┬──────┐      ┌──────┬──────┬──────┐
  nullptr ◄─┼ prev │  10  │ next ┼─────►│ prev │  20  │ next ┼─► nullptr
            └──────┴──────┴──────┘◄─────┼──────┴──────┴──────┘

  2. Circular Linked List (Danh sách liên kết vòng — Chỉ cần giữ 1 con trỏ `tail` duy nhất!):
       ┌────────────────────────────────────────────────┐
       ▼                                                │ (tail->next chính là head!)
     ┌──────┬──────┐    ┌──────┬──────┐    ┌──────┬─────┴┐
     │  10  │ next ┼───►│  20  │ next ┼───►│  30  │ next │ ◄── tail
     └──────┴──────┘    └──────┴──────┘    └──────┴──────┘
```

* **Hai kỹ thuật thực chiến với Linked List:**
  1. **Dummy Node (Node giả ở đầu):** Khi viết hàm gộp 2 danh sách (`merge`) hoặc xóa các node thỏa điều kiện, hãy tạo `Node dummy(0); Node *tail = &dummy;` trên Stack để không phải viết `if (head == nullptr)` xử lý riêng cho Node đầu tiên.
  2. **Thuật toán Rùa và Thỏ (Floyd's Cycle / Fast & Slow Pointers):** Cho `slow` nhảy 1 bước (`slow->next`), `fast` nhảy 2 bước (`fast->next->next`). Nếu `fast` chạm cuối thì `slow` đang nằm chính giữa danh sách (**Middle Node**); nếu `slow == fast` thì danh sách bị **dính chu trình vòng (Cycle)**!

---

## 3. Stack (LIFO), Circular Queue (FIFO) & Deque (`03_stack_and_queue.cpp`)

### 3.1. Stack (Ngăn xếp — Last In, First Out)

* **Nguyên lý:** Vào sau ra trước. Mọi thao tác `push(x)`, `pop()`, `top()` chỉ diễn ra tại **1 đầu duy nhất** trong **$O(1)$**.
* **Ứng dụng:** Quản lý Call Stack hàm/đệ quy, khử đệ quy DFS, kiểm tra cặp ngoặc đóng mở hợp lệ, tính giá trị biểu thức Hậu tố (**Reverse Polish Notation / Postfix**), tính năng Undo/Redo.

### 3.2. Circular Queue / Ring Buffer (Hàng đợi vòng — First In, First Out)

Nếu cài đặt Queue trên mảng 1D bằng cách chỉ tăng `front_idx++` khi lấy ra và `rear_idx++` khi thêm vào, các ô trống phía trước `front_idx` sẽ bị bỏ phí vĩnh viễn (**False Overflow**). Giải pháp chuẩn công nghiệp là **Ring Buffer** dùng toán tử chia lấy dư `% capacity`:

```text
  Mảng tĩnh/động kích thước capacity = 5, cuộn tròn chỉ số bằng `(idx + 1) % capacity`:
                 front_idx = 2                    rear_idx = 0 (Đã cuộn vòng về đầu mảng!)
                       ▼                                ▼
  Chỉ số:     [0]             [1]             [2]             [3]             [4]
         ┌───────────┬───────────────┬───────────────┬───────────────┬───────────────┐
         │    40     │    (Trống)    │      10       │      20       │      30       │
         └───────────┴───────────────┴───────────────┴───────────────┴───────────────┘
           ▲ (Mới vào)                 ▲ (Sẽ ra trước) ──────────────────────────────┘
```

* **Công thức tiến chỉ số vòng $O(1)$:**
  * Thêm phần tử (`enqueue`): `rear_idx = (rear_idx + 1) % capacity; data[rear_idx] = val; ++count;`
  * Lấy phần tử (`dequeue`): `front_idx = (front_idx + 1) % capacity; --count;`

### 3.3. Deque (Double-Ended Queue — Hàng đợi 2 đầu) & Kỹ thuật Monotonic Stack/Queue

* **Deque (Hàng đợi 2 đầu):** Là sự kết hợp của cả Stack và Queue, cho phép thêm và xóa phần tử ở **cả đầu trước (`push_front`, `pop_front`) lẫn đầu sau (`push_back`, `pop_back`) trong thời gian $O(1)$**.
* **Monotonic Stack (Ngăn xếp đơn điệu — $O(N)$):** Duy trì các phần tử trong Stack luôn theo thứ tự tăng dần (hoặc giảm dần) bằng cách `pop()` các phần tử vi phạm trước khi `push(a[i])`. Giải quyết bài toán **Next Greater Element (Phần tử lớn hơn gần nhất bên phải)** trong $O(N)$ thay vì $O(N^2)$.
* **Monotonic Deque (Hàng đợi 2 đầu đơn điệu — $O(N)$):** Lưu chỉ số trong `deque` theo thứ tự giá trị giảm dần; loại bỏ chỉ số rơi khỏi cửa sổ ở `front` và loại bỏ phần tử nhỏ hơn phần tử mới ở `back`. Giải quyết bài toán **Sliding Window Maximum (Max trên từng cửa sổ trượt kích thước $K$)** trong đúng **$O(N)$**!

---

## 4. Bảng Độ Phức Tạp Tổng Hợp

| Cấu trúc dữ liệu | Truy cập `i` | Thêm Đầu | Thêm Cuối | Xóa Đầu | Xóa Cuối | Tìm kiếm |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Singly Linked List** *(có `tail`)* | $O(N)$ | **$O(1)$** | **$O(1)$** | **$O(1)$** | $O(N)$ | $O(N)$ |
| **Doubly Linked List** *(có `tail`)* | $O(N)$ | **$O(1)$** | **$O(1)$** | **$O(1)$** | **$O(1)$** | $O(N)$ |
| **Circular Linked List** *(giữ `tail`)* | $O(N)$ | **$O(1)$** | **$O(1)$** | **$O(1)$** | $O(N)$ | $O(N)$ |
| **Stack (LIFO)** | — | **$O(1)$** (`push`) | — | **$O(1)$** (`pop`) | — | $O(N)$ |
| **Circular Queue (FIFO)** | — | — | **$O(1)$** (`enqueue`) | **$O(1)$** (`dequeue`) | — | $O(N)$ |
| **Deque (Hàng đợi 2 đầu)** | $O(1)$ | **$O(1)$** | **$O(1)$** | **$O(1)$** | **$O(1)$** | $O(N)$ |

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_singly_linked_list.cpp`: Singly Linked List (`push_front`, `push_back`, `pop_front`, `remove_value`, `reverse` tại chỗ $O(1)$ Space).
* `02_doubly_and_circular_list.cpp`: Doubly Linked List ($O(1)$ 2 đầu) & Circular Linked List (cắt vòng trong Destructor chống lặp vô tận).
* `03_stack_and_applications.cpp`: **Stack (LIFO)** (`LinkedStack`, `MinStack` truy vấn Min trong $O(1)$, và **Monotonic Stack** giải bài toán Next Greater Element $O(N)$).
* `04_queue_and_circular_queue.cpp`: **Queue (FIFO)** (`CircularQueue` / Ring Buffer mảng vòng $O(1)$ và **Monotonic Deque** giải bài toán Sliding Window Maximum $O(N)$).
* `05_practice_exercises.cpp`: 5 bài tập kinh điển (Valid Parentheses, Postfix RPN Evaluator, Floyd's Cycle & Middle Node, Merge Sorted Lists với Dummy Node, Cài đặt Queue bằng 2 Stack Amortized $O(1)$).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_singly_linked_list.cpp -o 01_singly_linked_list
./01_singly_linked_list

g++ -Wall -Wextra -std=c++17 02_doubly_and_circular_list.cpp -o 02_doubly_and_circular_list
./02_doubly_and_circular_list

g++ -Wall -Wextra -std=c++17 03_stack_and_applications.cpp -o 03_stack_and_applications
./03_stack_and_applications

g++ -Wall -Wextra -std=c++17 04_queue_and_circular_queue.cpp -o 04_queue_and_circular_queue
./04_queue_and_circular_queue

g++ -Wall -Wextra -std=c++17 05_practice_exercises.cpp -o 05_practice_exercises
./05_practice_exercises
```

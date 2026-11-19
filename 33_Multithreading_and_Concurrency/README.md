# Bài 7.1: Lập Trình Đa Luồng & Đồng Bộ Hóa (`<thread>`, `<mutex>`, `<condition_variable>`, `<atomic>`, `<future>`)

Tài liệu hệ thống hóa kiến trúc lập trình song song và đa luồng trong C++ hiện đại: quản lý vòng đời **`std::thread` (`join`, `detach`)**, bản chất của **Race Condition** và cách bảo vệ vùng găng (**Critical Section**) bằng **`std::mutex` (`lock_guard`, `unique_lock`, `scoped_lock`)** & **`std::atomic`**, phòng chống **Deadlock**, đồng bộ hóa **Producer-Consumer** bằng **`std::condition_variable`**, và lập trình bất đồng bộ với **`std::async`, `std::future`, `std::promise`**.

---

## 1. Kiến Trúc Bộ Nhớ Đa Luồng & Quản Lý `std::thread` (`01_threads_and_race_condition.cpp`)

### 1.1. Các luồng (Threads) chia sẻ những gì và sở hữu riêng những gì?

Trong cùng một tiến trình (Process), khi bạn tạo thêm các luồng con bằng `std::thread`, chúng tổ chức trên RAM như sau:

```text
  ┌─────────────────────────────────────────────────────────────────────┐
  │ VÙNG NHỚ DÙNG CHUNG CHO MỌI LUỒNG (Shared Memory -> Dễ xung đột!):  │
  │   • Text Segment (Mã máy)   • Data / BSS (Biến Global / Static)     │
  │   • HEAP Segment (Toàn bộ dữ liệu cấp phát động new / malloc)       │
  ├──────────────────────┬──────────────────────┬───────────────────────┤
  │ Luồng 1 (Main)       │ Luồng 2 (Worker A)   │ Luồng 3 (Worker B)    │
  │  • STACK RIÊNG       │  • STACK RIÊNG       │  • STACK RIÊNG        │
  │  • Thanh ghi CPU     │  • Thanh ghi CPU     │  • Thanh ghi CPU      │
  └──────────────────────┴──────────────────────┴───────────────────────┘
```

### 1.2. Quy tắc sống còn với `join()`, `detach()` và `std::ref()`

1. **Bắt buộc phải chọn `join()` hoặc `detach()` trước khi đối tượng `std::thread` bị hủy:**
   * **`t.join()`:** Luồng cha (như `main`) **đứng đợi** cho tới khi luồng con `t` chạy xong hoàn toàn rồi mới đi tiếp.
   * **`t.detach()`:** Tách luồng con ra chạy ngầm độc lập (Daemon thread). *(Nguy hiểm nếu luồng con đang giữ tham chiếu tới biến cục bộ trên Stack của hàm cha!)*.
   * **Cạm bẫy chết người:** Nếu một biến `std::thread t(...)` ra khỏi scope `{ }` mà bạn chưa gọi `t.join()` hay `t.detach()`, Destructor `~thread()` sẽ lập tức gọi **`std::terminate()` đánh sập toàn bộ chương trình!**
2. **Truyền tham chiếu vào `std::thread` bắt buộc phải bọc `std::ref()`:**
   * Mặc định Constructor của `std::thread` **luôn copy hoặc move** mọi đối số vào bộ nhớ riêng của luồng mới. Dù hàm của bạn khai báo `void work(int &x)`, nếu gọi `std::thread t(work, a);` sẽ bị lỗi biên dịch! Bắt buộc phải viết: **`std::thread t(work, std::ref(a));`**.

---

## 2. Xung Đột Dữ Liệu (Race Condition), `<mutex>` & `<atomic>` (`01_threads_and_race_condition.cpp`)

### 2.1. Tại sao `++counter` bị sai khi chạy đa luồng? (Data Race)

Phép tăng `++counter` dưới phần cứng CPU không phải là 1 bước đơn lẻ, mà gồm **3 lệnh Assembly tách rời**: (1) Đọc `counter` từ RAM vào thanh ghi $\rightarrow$ (2) Cộng thêm 1 trong thanh ghi $\rightarrow$ (3) Ghi kết quả từ thanh ghi về RAM. Khi 2 luồng cùng đọc giá trị `100` một lúc, cả hai cùng cộng lên `101` và ghi đè lên nhau $\implies$ Tăng 2 lần nhưng kết quả chỉ lên `101`!

### 2.2. Bảng đối chiếu bộ công cụ bảo vệ dữ liệu đa luồng

| Công cụ | Thư viện | Cơ chế vận hành | Khi nào nên sử dụng? |
| :--- | :---: | :--- | :--- |
| **`std::mutex`** *(Thủ công)* | `<mutex>` | Gọi `m.lock()` và `m.unlock()` thủ công. | **KHÔNG NÊN gọi trực tiếp** (vì nếu giữa `lock` và `unlock` có `return` hoặc Exception, mutex sẽ bị khóa vĩnh viễn!). |
| **`std::lock_guard`** | `<mutex>` | Lớp bọc **RAII**: tự động `lock()` khi tạo và tự động `unlock()` khi ra khỏi khối `{ }`. | **Chuẩn mặc định** khi chỉ cần khóa 1 mutex đơn giản trong một phạm vi hàm/khối lệnh. |
| **`std::unique_lock`** | `<mutex>` | Lớp bọc RAII linh hoạt: cho phép `unlock()` sớm, `lock()` lại, hỗ trợ `std::move`. | **Bắt buộc dùng** khi kết hợp với **`std::condition_variable`**. |
| **`std::scoped_lock`** *(C++17)* | `<mutex>` | Khóa đồng thời **nhiều mutex cùng lúc** (`std::scoped_lock lk(m1, m2);`) bằng thuật toán tránh Deadlock. | **Chuẩn vàng** khi một tác vụ cần khóa từ **2 mutex trở lên** (ví dụ: chuyển tiền giữa 2 tài khoản). |
| **`std::atomic<T>`** | `<atomic>` | Thực thi lệnh nguyên tử trực tiếp ở **tầng phần cứng CPU (Lock-Free)**, không làm treo luồng. | Khi chỉ thao tác trên biến đơn (`int`, `long long`, `bool`, con trỏ) như bộ đếm `++cnt` hoặc cờ dừng `stop_flag`. |

---

## 3. Phòng Chống Deadlock & `std::condition_variable` (`02_deadlock_and_condition_variable.cpp`)

### 3.1. Hiện tượng Khóa chết (Deadlock) & Giải pháp `std::scoped_lock`

```text
  Kịch bản Deadlock kinh điển (Chuyển tiền chéo giữa Tài khoản A và B):
  • Luồng 1 (Chuyển A -> B):  Đã khóa Mutex_A ───(Đang chờ mở)───► Mutex_B
  • Luồng 2 (Chuyển B -> A):  Đã khóa Mutex_B ───(Đang chờ mở)───► Mutex_A
  ===> Cả 2 luồng đứng chờ nhau vĩnh viễn (Treo cứng chương trình)!
```

* **Giải pháp triệt để trong C++17:** Không bao giờ gọi 2 lệnh `lock_guard` rời rạc cho 2 mutex. Hãy dùng **`std::scoped_lock lock(accA.mtx, accB.mtx);`** — C++ tự động áp dụng thuật toán *Deadlock Avoidance* để khóa cả hai an toàn bất kể thứ tự truyền vào!

### 3.2. Đồng bộ hóa Luồng với `std::condition_variable` (Producer - Consumer)

Thay vì dùng vòng lặp `while (queue.empty()) {}` đốt 100% CPU vô ích (**Busy Waiting**), `std::condition_variable` cho phép luồng Consumer **đi ngủ (0% CPU)** cho đến khi luồng Producer đẩy dữ liệu mới vào và bấm chuông đánh thức (`notify_one()` / `notify_all()`):

```cpp
std::unique_lock<std::mutex> lk(mtx);
// Luôn truyền Lambda điều kiện vào tham số thứ 2 của wait() để chống Đánh thức giả (Spurious Wakeup)!
cv.wait(lk, [&]() { return !data_queue.empty() || is_finished; });
```

---

## 4. Lập Trình Bất Đồng Bộ Với `std::async`, `std::future` & `std::promise` (`03_async_future_and_promise.cpp`)

`std::thread` không có cơ chế trả về giá trị `return` trực tiếp từ hàm con. Thư viện `<future>` giải quyết gọn nhẹ bài toán này:

```text
  Luồng sản xuất (std::async / std::promise)                Luồng tiêu thụ (Main Thread)
  ┌────────────────────────────────────────┐                ┌────────────────────────────────────────┐
  │ Tính toán ngầm trên lõi CPU khác...    │ ══(Kênh nối)═► │ std::future<Result> fut;               │
  │ • return kết quả (hoặc promise.set_val)│                │ Result r = fut.get(); // Đợi & lấy KQ! │
  └────────────────────────────────────────┘                └────────────────────────────────────────┘
```

* **`std::async(std::launch::async, func, args...)`:** Tự động tạo luồng chạy hàm `func` ngay lập tức ở hậu trường và trả về đối tượng `std::future<T>`. Khi cần lấy kết quả, chỉ cần gọi **`fut.get()`** (hàm `get()` chỉ được gọi **đúng 1 lần duy nhất**).
* **`std::promise<T>` & `std::future<T>`:** Khi bạn tự quản lý `std::thread` và muốn gửi một giá trị (hoặc gửi cả một Exception bằng `set_exception`) từ luồng con về luồng cha tại một thời điểm tùy ý.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_thread_lifecycle_and_management.cpp`: Thư viện `<thread>` — Tạo luồng, `join()`, truyền tham chiếu với `std::ref`, `hardware_concurrency()` và lớp bọc RAII `ScopedThread`.
* `02_mutex_and_lock_guards.cpp`: Tránh xung đột dữ liệu (Race Condition) với `<mutex>` — `std::mutex`, `std::lock_guard`, và mở khóa sớm bằng `std::unique_lock`.
* `03_deadlock_and_atomic.cpp`: Phòng chống **Deadlock** khi khóa nhiều mutex bằng `std::scoped_lock` (C++17) và biến nguyên tử Lock-Free **`std::atomic`** (`fetch_add`, Compare-And-Swap).
* `04_condition_variable_producer_consumer.cpp`: Đồng bộ hóa luồng với **`std::condition_variable`** (`wait` với Predicate chống Spurious Wakeup, `notify_one`, `notify_all`).
* `05_async_future_and_promise.cpp`: Lập trình bất đồng bộ với **`std::async`**, **`std::future`** và **`std::promise`** (kèm chuyển tiếp Exception qua `future.get()`).
* `06_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Tính tổng mảng song song đa luồng, Thread-Safe Blocking Queue, Đồng bộ Chẵn-Lẻ luân phiên, Lock-Free Atomic Metrics, Thiết kế **Thread Pool**).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 -pthread 01_thread_lifecycle_and_management.cpp -o 01_thread_lifecycle_and_management
./01_thread_lifecycle_and_management

g++ -Wall -Wextra -std=c++17 -pthread 02_mutex_and_lock_guards.cpp -o 02_mutex_and_lock_guards
./02_mutex_and_lock_guards

g++ -Wall -Wextra -std=c++17 -pthread 03_deadlock_and_atomic.cpp -o 03_deadlock_and_atomic
./03_deadlock_and_atomic

g++ -Wall -Wextra -std=c++17 -pthread 04_condition_variable_producer_consumer.cpp -o 04_condition_variable_producer_consumer
./04_condition_variable_producer_consumer

g++ -Wall -Wextra -std=c++17 -pthread 05_async_future_and_promise.cpp -o 05_async_future_and_promise
./05_async_future_and_promise

g++ -Wall -Wextra -std=c++17 -pthread 06_practice_exercises.cpp -o 06_practice_exercises
./06_practice_exercises
```

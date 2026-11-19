# Bài 4.5: Bảng Băm Toàn Tập (Hash Table — Hash Functions, Separate Chaining, Open Addressing & Rehashing)

Tài liệu hệ thống hóa kiến trúc bên dưới của **Bảng băm (Hash Table)** theo đúng mục 4.5 của Roadmap: nguyên lý thiết kế **Hàm băm (Hash Function: `djb2`, `FNV-1a`, Polynomial Rolling Hash)**, đối chiếu trực diện 2 trường phái xử lý đụng độ (**Separate Chaining** vs **Open Addressing: Linear Probing, Quadratic Probing**), cơ chế **Tombstone (Mộ giả)** khi xóa, và quy tắc **Load Factor & Dynamic Rehashing** để duy trì tốc độ tra cứu **$O(1)$**.

---

## 1. Nguyên Lý Hoạt Động & Thiết Kế Hàm Băm (Hash Function)

### 1.1. Bản chất ánh xạ $O(1)$ của Bảng băm

Mảng 1 chiều cho phép truy cập `a[i]` trong $O(1)$, nhưng chỉ dùng được khi khóa là số nguyên nhỏ `0..N-1`. **Bảng băm** mở rộng khả năng truy cập $O(1)$ đó cho **mọi kiểu khóa bất kỳ** (Chuỗi `"Nguyen Van A"`, số điện thoại, tọa độ `(x, y)`) thông qua 2 bước:

```text
  Khóa (Key = "An") ──► [1. Hash Function: h = hash("An")] ──► [2. Nén chỉ số: idx = h % M] ──► Bucket[idx]
                         (Biến chuỗi thành số 64-bit)           (Ánh xạ vào mảng M ô nhớ)       (Tra cứu O(1))
```

### 1.2. Ba tiêu chuẩn của một Hàm băm tốt & Các thuật toán băm kinh điển

1. **Tính tất định (Deterministic):** Cùng một `Key` đầu vào phải luôn cho ra đúng một mã băm duy nhất.
2. **Tốc độ tính toán nhanh:** Chỉ tốn $O(1)$ với số nguyên hoặc $O(L)$ với chuỗi độ dài $L$.
3. **Phân bố đều (Uniform Distribution / Avalanche Effect):** Chỉ thay đổi 1 bit ở `Key` cũng làm mã băm thay đổi ngẫu nhiên toàn bộ, tránh dồn cục vào một vài bucket.

| Thuật toán băm | Công thức cốt lõi | Đặc điểm ứng dụng |
| :--- | :--- | :--- |
| **`djb2` (Dan Bernstein)** | `h = 5381;` `for (c : s) h = ((h << 5) + h) + c;` *(tức `h * 33 + c`)* | Cực nhanh, cài đặt 3 dòng, phân bố rất tốt cho chuỗi ký tự ASCII thông thường. |
| **`FNV-1a` (64-bit)** | `h = 14695981039346656037ULL;` `for (c : s) { h ^= c; h *= 1099511628211ULL; }` | Chuẩn công nghiệp được sử dụng trong nhiều trình biên dịch cho `std::hash<std::string>`. |
| **Fibonacci / Knuth Multiplicative** *(Cho số nguyên)* | `h = (key * 2654435769U) >> (32 - p);` | Phá vỡ tính tuần hoàn khi các khóa số nguyên là bội số chẵn của nhau. |

---

## 2. Xử Lý Đụng Độ (Collision Handling): Separate Chaining vs Open Addressing

Khi hai khóa khác nhau ($K_1 \ne K_2$) có cùng chỉ số `hash(K1) % M == hash(K2) % M`, hiện tượng **Đụng độ (Hash Collision)** xảy ra. Có 2 phương pháp giải quyết kinh điển:

```text
  1. SEPARATE CHAINING (Phương pháp Dây chuyền — Mỗi ô Bucket là đầu của 1 Singly Linked List):
     Buckets:
      [0] ──► nullptr
      [1] ──► ["An": 85] ──► ["Dung": 95] ──► nullptr   (Đụng độ tại ô [1] -> nối thêm Node vào chuỗi!)
      [2] ──► ["Binh": 92] ──► nullptr

  2. OPEN ADDRESSING (Phương pháp Thăm dò mở — Toàn bộ phần tử nằm gói gọn trong 1 mảng phẳng duy nhất):
     Giả sử Key 10, 17, 24 đều băm vào ô [3] (với M = 7):
     Chỉ số:   [0]       [1]       [2]         [3]             [4]             [5]       [6]
            ┌───────┬─────────┬─────────┬───────────────┬───────────────┬───────────────┬───────┐
     Mảng:  │ EMPTY │  EMPTY  │  EMPTY  │   Key = 10    │   Key = 17    │   Key = 24    │ EMPTY │
            └───────┴─────────┴─────────┴───────────────┴───────────────┴───────────────┴───────┘
                                         (Gốc: step = 0) (Trượt: step=1) (Trượt: step=2)
```

| Tiêu chí đối chiếu | 1. Separate Chaining *(Dây chuyền — `01_...cpp`)* | 2. Open Addressing *(Thăm dò mở — `02_...cpp`)* |
| :--- | :--- | :--- |
| **Cấu trúc bộ nhớ** | Mảng con trỏ `Node**` trên Heap + các Node cấp phát rời rạc. | **Một mảng phẳng liên tiếp duy nhất** (`std::vector<Slot>`), không dùng con trỏ Node. |
| **Hệ số tải ($\alpha = N / M$)** | Cho phép $\alpha \ge 1.0$ (thường Rehash khi **$\alpha > 0.75$**). | Bắt buộc **$\alpha < 1.0$** (thường Rehash ngay khi **$\alpha > 0.5 - 0.7$**). |
| **Cơ chế Xóa (`remove`)** | Xóa trực tiếp Node khỏi Linked List (`delete curr`). | **CẤM đặt lại ô về `EMPTY`!** Bắt buộc đánh dấu trạng thái **`DELETED` (Tombstone / Mộ giả)**. |
| **CPU Cache Locality** | Trung bình (do nhảy qua các địa chỉ con trỏ trên Heap). | **Cực kỳ xuất sắc** (dò trên các ô nhớ liền kề trong cùng dòng L1 Cache). |

---

## 3. Ba Chiến Lược Thăm Dò Trong Open Addressing & Vai Trò Của Tombstone

Khi ô gốc `h = hash(key) % M` đã bị chiếm, Open Addressing dò tìm ô tiếp theo theo công thức `idx = (h + f(i)) % M` (với lần thử $i = 1, 2, 3, \dots$):

1. **Thăm dò tuyến tính (Linear Probing — $f(i) = i$):**
   * Dò tuần tự từng ô kề bên: `h, h + 1, h + 2, ...`
   * *Ưu điểm:* Tận dụng CPU Cache tốt nhất. *Nhược điểm:* Dễ bị hiện tượng **Dính chùm sơ cấp (Primary Clustering)** — các ô đầy dính liền nhau thành một dải dài làm chậm các lần chèn sau.
2. **Thăm dò bậc hai (Quadratic Probing — $f(i) = i^2$):**
   * Nhảy theo bình phương khoảng cách: `h + 1^2, h + 2^2, h + 3^2, ...` (`+1, +4, +9, +16...`).
   * *Ưu điểm:* Phá vỡ hiện tượng dính chùm sơ cấp của Linear Probing! *(Lưu ý: Để đảm bảo luôn tìm được ô trống khi $\alpha < 0.5$, kích thước bảng $M$ nên là **số nguyên tố**).*
3. **Băm kép (Double Hashing — $f(i) = i \times \text{hash}_2(\text{key})$):**
   * Bước nhảy phụ thuộc vào hàm băm thứ hai $\text{hash}_2(\text{key}) \ne 0$, triệt tiêu cả dính chùm sơ cấp lẫn thứ cấp.

### Tại sao khi Xóa trong Open Addressing BẮT BUỘC phải dùng Tombstone (`DELETED`)?

* Giả sử `10`, `17`, `24` cùng băm ra ô `[3]`, nên nằm lần lượt ở các ô `[3]`, `[4]`, `[5]`.
* Nếu bạn xóa `17` ở ô `[4]` mà đặt thẳng ô `[4]` về **`EMPTY`**, thì ngay sau đó khi tìm kiếm `get(24)`: thuật toán kiểm tra ô `[3]` (là `10`), trượt sang ô `[4]` thấy `EMPTY` sẽ **tưởng lầm là `24` không tồn tại và dừng tìm kiếm ngay lập tức!**
* **Quy tắc Tombstone:** Khi xóa ở ô `[4]`, ta đổi trạng thái ô đó thành **`DELETED` (Mộ giả)**:
  * Khi **Tìm kiếm (`get`)**: Gặp `DELETED` thì **bước qua đi tìm tiếp**, chỉ dừng khi gặp `EMPTY` thật.
  * Khi **Chèn mới (`put`)**: Có thể **tái sử dụng** ngay ô `DELETED` đầu tiên bắt gặp để không làm lãng phí RAM!

---

## 4. Hệ Số Tải (Load Factor) & Cơ Chế Dynamic Rehashing

* **Hệ số tải (Load Factor):** $\alpha = \frac{\text{Số phần tử hiện có } (N)}{\text{Tổng số Bucket } (M)}$.
* Khi $\alpha$ vượt ngưỡng cho phép ($0.75$ với Chaining hoặc $0.5 - 0.7$ với Open Addressing), bảng băm tự động gọi **`rehash(new_M)`**:
  1. Cấp phát mảng Bucket mới có kích thước gấp đôi (hoặc số nguyên tố kế tiếp $\approx 2M$).
  2. Tính lại chỉ số `new_idx = hash(key) % new_M` cho toàn bộ $N$ phần tử cũ và chuyển sang bảng mới.
  3. Giải phóng mảng Bucket cũ. Chi phí Rehashing là $O(N)$ nhưng chỉ xảy ra sau $O(N)$ lần chèn, nên độ phức tạp trung bình mỗi thao tác vẫn đạt **$O(1)$ Amortized**!

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_hash_functions_and_chaining.cpp`: So sánh hàm băm `djb2` & `FNV-1a`, cài đặt `ChainingHashTable` kèm Dynamic Rehashing.
* `02_open_addressing_probing.cpp`: Cài đặt `LinearProbingHashTable` và `QuadraticProbingHashTable` kèm cơ chế xóa Tombstone (`DELETED`) và Rehashing.
* `03_hash_table_applications.cpp`: 3 ứng dụng thực chiến (Đếm tần suất từ, Phát hiện phần tử trùng trong cửa sổ $K$, và Memoization Cache).
* `04_practice_exercises.cpp`: Giải 5 bài tập phỏng vấn kinh điển về Hash Table (Two Sum $O(N)$, First Unique Character, Subarray Sum Equals K, Longest Consecutive Sequence $O(N)$, Isomorphic Strings).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_hash_functions_and_chaining.cpp -o hash_chaining
./hash_chaining

g++ -Wall -Wextra -std=c++17 02_open_addressing_probing.cpp -o hash_probing
./hash_probing

g++ -Wall -Wextra -std=c++17 03_hash_table_applications.cpp -o hash_apps
./hash_apps

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o hash_exercises
./hash_exercises
```

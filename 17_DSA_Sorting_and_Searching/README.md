# Bài 4.2: Giải Thuật Sắp Xếp & Tìm Kiếm (Sorting & Searching)

Tài liệu hệ thống hóa cơ chế vận hành chi tiết của các thuật toán tìm kiếm (**Linear Search**, **Binary Search**, **Lower Bound vs Upper Bound**, **Binary Search on Answer**) và 8 thuật toán sắp xếp cốt lõi từ cơ bản ($O(N^2)$) đến nâng cao ($O(N \log N)$ & tuyến tính $O(N)$).

---

## 1. Nhóm Thuật Toán Tìm Kiếm (`01_searching_and_bs_on_answer.cpp`)

### 1.1. Linear Search (Tìm kiếm tuyến tính) — $O(N)$

* **Nguyên lý:** Duyệt tuần tự từ đầu đến cuối mảng (`i = 0 -> n - 1`), so sánh `a[i] == target`.
* **Khi nào dùng:** Mảng **chưa sắp xếp**, mảng rất nhỏ ($N < 32$), hoặc cấu trúc không hỗ trợ truy cập ngẫu nhiên (như Linked List).

### 1.2. Binary Search (Tìm kiếm nhị phân) — $O(\log N)$

* **Điều kiện bắt buộc:** Mảng phải **đã được sắp xếp** (hoặc không gian tìm kiếm có tính đơn điệu).
* **Nguyên lý:** Chia đôi không gian tìm kiếm `[left, right]` tại mỗi bước:
  1. Tính điểm giữa chống tràn số nguyên: **`mid = left + (right - left) / 2;`** *(Nếu viết `(left + right) / 2` sẽ bị tràn `int` khi `left + right > 2 * 10^9`)*.
  2. Nếu `a[mid] == target` $\rightarrow$ trả về `mid`.
  3. Nếu `a[mid] < target` $\rightarrow$ nửa trái đều nhỏ hơn `target`, thu hẹp sang nửa phải (`left = mid + 1`).
  4. Ngược lại $\rightarrow$ thu hẹp sang nửa trái (`right = mid - 1`).

### 1.3. Phân biệt `Lower Bound` ($\ge x$) và `Upper Bound` ($> x$) — $O(\log N)$

Khi mảng có nhiều phần tử trùng nhau (ví dụ `a = [10, 20, 20, 20, 30]`, tìm `x = 20`), Binary Search cơ bản chỉ trả về một vị trí bất kỳ. Ta dùng khoảng nửa mở `[left, right)` với `left = 0, right = n` để tìm chính xác ranh giới:

```text
  Chỉ số:          0     1     2     3     4      5 (n)
                ┌─────┬─────┬─────┬─────┬─────┐
  Mảng đã sort: │ 10  │ 20  │ 20  │ 20  │ 30  │
                └─────┴─────┴─────┴─────┴─────┘
                        ▲                 ▲
               lower_bound(20) = 1   upper_bound(20) = 4
               (Đầu tiên >= 20)      (Đầu tiên > 20)
  ==> Số lần xuất hiện của 20 trong mảng = upper_bound(20) - lower_bound(20) = 4 - 1 = 3 lần!
```

* **Lower Bound (`a[mid] >= target`):** Tìm vị trí nhỏ nhất có giá trị **$\ge \text{target}$**. Khi `a[mid] >= target`, gán `right = mid`, ngược lại `left = mid + 1`.
* **Upper Bound (`a[mid] > target`):** Tìm vị trí nhỏ nhất có giá trị **$> \text{target}$**. Khi `a[mid] > target`, gán `right = mid`, ngược lại `left = mid + 1`.

### 1.4. Binary Search on Answer (Tìm kiếm nhị phân trên tập kết quả)

* **Dấu hiệu nhận biết:** Đề bài yêu cầu *"Tìm giá trị nhỏ nhất của giá trị lớn nhất"* (Minimize the Maximum) hoặc *"Tìm giá trị lớn nhất thỏa mãn điều kiện..."*.
* **Nguyên lý:** Không tìm trên mảng đầu vào mà tìm nhị phân trên **miền đáp án khả dĩ `[MinAns, MaxAns]`**. Với mỗi giá trị thử `mid`, gọi hàm kiểm tra `check(mid)` (thường chạy trong $O(N)$):
  * Nếu hàm `check(mid)` có dạng **`T T T T F F F`** (muốn tìm `T` lớn nhất, ví dụ bài toán Cắt gỗ EKO): khi `check(mid) == true`, lưu `ans = mid` rồi đẩy `left = mid + 1`.
  * Nếu hàm `check(mid)` có dạng **`F F F T T T T`** (muốn tìm `T` nhỏ nhất, ví dụ bài toán Vận chuyển hàng trong $D$ ngày): khi `check(mid) == true`, lưu `ans = mid` rồi ép `right = mid - 1`.

---

## 2. Nhóm Sắp Xếp Cơ Bản $O(N^2)$ (`02_basic_sorts.cpp`)

### 2.1. Selection Sort (Sắp xếp chọn)

* **Cơ chế:** Tại mỗi vị trí `i` từ `0` đến `n - 2`, quét toàn bộ đoạn chưa sắp xếp `[i..n-1]` để tìm chỉ số của phần tử nhỏ nhất `min_idx`, sau đó hoán đổi `swap(a[i], a[min_idx])`.
* **Đặc điểm:** Luôn chạy đúng $\frac{N(N-1)}{2}$ phép so sánh $\rightarrow$ **$O(N^2)$ trong mọi trường hợp**; **Không ổn định (Unstable)**; ưu điểm duy nhất là số lần `swap` tối đa chỉ là **$N - 1$** (hữu ích khi chi phí ghi bộ nhớ cực đắt).

### 2.2. Insertion Sort (Sắp xếp chèn)

* **Cơ chế:** Giống cách xếp bài trên tay. Lấy phần tử `key = a[i]`, dịch toàn bộ các phần tử lớn hơn `key` ở đoạn đã sắp xếp `[0..i-1]` sang phải 1 vị trí, rồi đặt `key` vào ô trống đúng thứ tự.
* **Đặc điểm:** **Ổn định (Stable)**; nếu mảng đã gần như sắp xếp sẵn thì vòng lặp `while` bên trong hầu như không chạy $\rightarrow$ đạt **$O(N)$ Best-Case**. Cực nhanh trên mảng nhỏ ($N \le 32$), được tích hợp bên trong thuật toán **Introsort** của `std::sort`.

### 2.3. Bubble Sort (Sắp xếp nổi bọt)

* **Cơ chế:** So sánh từng cặp kề nhau `(a[j], a[j+1])`, nếu `a[j] > a[j+1]` thì đổi chỗ để phần tử lớn nhất "nổi" về cuối mảng sau mỗi vòng lặp.
* **Tối ưu:** Gắn cờ `bool swapped = false;`. Nếu kết thúc 1 lượt quét mà `swapped == false`, mảng đã hoàn toàn tăng dần $\rightarrow$ `break` dừng sớm (đạt $O(N)$ Best-Case). **Ổn định (Stable)**.

---

## 3. Nhóm Sắp Xếp Nâng Cao (`03_advanced_sorts.cpp`)

### 3.1. Merge Sort (Sắp xếp trộn) — $O(N \log N)$ | $O(N)$ Space | Stable

```text
  Chia đôi đệ quy (Divide):        [38, 27, 43, 3]  ===>  [38, 27]  và  [43, 3]
  Trộn 2 nửa đã sort (Merge):      [27, 38] + [3, 43] ===(2 con trỏ i, j)===> [3, 27, 38, 43]
```

* **Cơ chế (Chia để trị - Divide & Conquer):**
  1. **Chia:** Tách đôi mảng tại `mid` cho tới khi mảng con chỉ còn 1 phần tử.
  2. **Trộn (`merge_halves`):** Dùng 2 con trỏ `i` (nửa trái) và `j` (nửa phải) chọn phần tử nhỏ hơn đưa vào mảng tạm `temp[]`, sau đó chép trả lại `a[left..right]`.
* **Điểm cốt tử:** Dùng điều kiện `if (a[i] <= a[j])` (có dấu `=`) khi trộn để bảo toàn thứ tự ban đầu của các phần tử bằng nhau (**Stable**). Ứng dụng thêm để đếm số cặp nghịch thế (**Inversion Count**) trong $O(N \log N)$.

### 3.2. Quick Sort (Sắp xếp nhanh) — $O(N \log N)$ avg | $O(\log N)$ Space | Unstable

```text
  Bước Partition (Phân hoạch Lomuto với pivot = a[high]):
  ┌─────────────────────────┬───────────────┬─────────────────────────┐
  │   Các phần tử <= pivot  │     PIVOT     │   Các phần tử > pivot   │
  │      [low ... p-1]      │      [p]      │     [p+1 ... high]      │
  └─────────────────────────┴───────────────┴─────────────────────────┘
   (Đệ quy sắp xếp nửa trái)   (Đã cố định)  (Đệ quy sắp xếp nửa phải)
```

* **Điểm cốt tử:** Sắp xếp trực tiếp tại chỗ (In-place) không tốn mảng tạm $O(N)$, tận dụng cực tốt bộ nhớ đệm **CPU Cache Locality** nên chạy nhanh hơn Merge Sort và Heap Sort trong thực tế.
* **Nhược điểm & Khắc phục:** Rơi vào Worst-Case **$O(N^2)$** nếu mảng đã sắp xếp sẵn mà luôn chọn `pivot` ở đầu/cuối. Thực tế khắc phục bằng cách chọn `pivot` ngẫu nhiên hoặc **Median-of-Three** (trung vị của `low, mid, high`).

### 3.3. Heap Sort (Sắp xếp vun đống) — $O(N \log N)$ | $O(1)$ Space | Unstable

* **Cơ chế:** Xem mảng 1D là một Cây nhị phân hoàn chỉnh (con trái của `i` là `2*i + 1`, con phải là `2*i + 2`):
  1. **Build Max-Heap ($O(N)$):** Gọi `heapify` từ node cha cuối cùng (`n/2 - 1`) lùi về gốc `0` để đưa số lớn nhất lên đỉnh `a[0]`.
  2. **Trích xuất liên tiếp ($O(N \log N)$):** Hoán đổi đỉnh `a[0]` xuống cuối mảng `a[i]`, giảm kích thước Heap còn `i`, rồi gọi `heapify(a, i, 0)` để đẩy số lớn tiếp theo lên đỉnh.
* **Đặc điểm:** Kết hợp được cả ưu điểm **$O(N \log N)$ mọi trường hợp** của Merge Sort và **$O(1)$ Space tại chỗ** của Quick Sort.

### 3.4. Counting Sort & Radix Sort (Sắp xếp tuyến tính không dựa trên so sánh)

* **Counting Sort — $O(N + K)$ Time & Space | Stable:**
  * Dùng mảng đếm tần suất `cnt[0..K]` (với $K = \max(a)$), cộng dồn Prefix Sum trên `cnt[]` để xác định vị trí xuất hiện cuối cùng của từng giá trị, rồi duyệt **ngược** từ `n - 1` về `0` để điền vào mảng kết quả (giữ tính **Stable**). Chỉ áp dụng khi $K$ nhỏ ($K \le 10^6$).
* **Radix Sort (LSD) — $O(d \cdot (N + 10))$ Time | $O(N + 10)$ Space | Stable:**
  * Khắc phục giới hạn $K$ lớn của Counting Sort bằng cách sắp xếp lần lượt theo từng chữ số từ hàng đơn vị (`exp = 1`), hàng chục (`10`), hàng trăm (`100`)... Tại mỗi hàng, gọi **Counting Sort ổn định** với 10 thùng (`0..9`).

---

## 4. Bảng Tổng Hợp So Sánh 8 Thuật Toán Sắp Xếp

| Thuật toán | Best Time | Average Time | Worst Time | Auxiliary Space | Stable (Ổn định)? | Khi nào nên dùng? |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Selection Sort** | $O(N^2)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Không | Khi mảng rất nhỏ và muốn tối thiểu hóa số lần ghi (`swap`). |
| **Insertion Sort** | **$O(N)$** | $O(N^2)$ | $O(N^2)$ | $O(1)$ | **Có** | Khi mảng nhỏ ($N \le 32$) hoặc mảng **gần như đã sắp xếp**. |
| **Bubble Sort** | **$O(N)$** | $O(N^2)$ | $O(N^2)$ | $O(1)$ | **Có** | Mục đích giảng dạy hoặc kiểm tra nhanh mảng đã sort ($O(N)$). |
| **Merge Sort** | $O(N \log N)$ | $O(N \log N)$ | **$O(N \log N)$** | $O(N)$ | **Có** | Khi bắt buộc cần tính **Stable** (`std::stable_sort`) hoặc sort Linked List. |
| **Quick Sort** | $O(N \log N)$ | **$O(N \log N)$** | $O(N^2)$ | $O(\log N)$ | Không | Nhanh nhất trên mảng trong thực tế nhờ Cache-friendly. |
| **Heap Sort** | $O(N \log N)$ | $O(N \log N)$ | **$O(N \log N)$** | **$O(1)$** | Không | Khi cần cam kết $O(N \log N)$ tuyệt đối mà không được tốn thêm RAM. |
| **Counting Sort** | $O(N + K)$ | $O(N + K)$ | $O(N + K)$ | $O(N + K)$ | **Có** | Sắp xếp số nguyên có miền giá trị $K$ nhỏ ($K \le 10^6$). |
| **Radix Sort** | $O(d(N+10))$ | $O(d(N+10))$ | $O(d(N+10))$ | $O(N + 10)$ | **Có** | Sắp xếp lượng lớn số nguyên 32/64-bit hoặc chuỗi cố định độ dài. |

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_searching_and_bs_on_answer.cpp`: Linear Search, Binary Search, Lower Bound / Upper Bound và Binary Search on Answer (bài toán cắt gỗ EKO).
* `02_basic_sorts.cpp`: Selection Sort, Insertion Sort, Bubble Sort (có cờ dừng sớm).
* `03_merge_sort.cpp`: **Merge Sort** (Top-Down đệ quy, Bottom-Up khử đệ quy, tính ổn định Stable & đếm cặp nghịch thế Inversion Count).
* `04_quick_sort.cpp`: **Quick Sort** (Phân hoạch Lomuto vs Hoare, chọn chốt Median-of-Three chống $O(N^2)$ và 3-Way Dutch National Flag Partition).
* `05_heap_sort_and_counting_sort.cpp`: **Heap Sort** (In-place $O(1)$ Space), **Counting Sort** và **Radix Sort (LSD)**.
* `06_practice_exercises.cpp`: 5 bài tập thực chiến (First/Last Position, Search in Rotated Sorted Array, Inversion Count bằng Merge Sort, Quickselect tìm phần tử lớn thứ $K$ trong $O(N)$, Ship Packages Within $D$ Days).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_searching_and_bs_on_answer.cpp -o 01_searching_and_bs_on_answer
./01_searching_and_bs_on_answer

g++ -Wall -Wextra -std=c++17 02_basic_sorts.cpp -o 02_basic_sorts
./02_basic_sorts

g++ -Wall -Wextra -std=c++17 03_merge_sort.cpp -o 03_merge_sort
./03_merge_sort

g++ -Wall -Wextra -std=c++17 04_quick_sort.cpp -o 04_quick_sort
./04_quick_sort

g++ -Wall -Wextra -std=c++17 05_heap_sort_and_counting_sort.cpp -o 05_heap_sort_and_counting_sort
./05_heap_sort_and_counting_sort

g++ -Wall -Wextra -std=c++17 06_practice_exercises.cpp -o 06_practice_exercises
./06_practice_exercises
```

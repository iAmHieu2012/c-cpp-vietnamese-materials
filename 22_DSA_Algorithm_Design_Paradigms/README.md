# Bài 4.7: Các Chiến Lược Thiết Kế Giải Thuật (Two Pointers, Sliding Window, Divide & Conquer, Backtracking, Greedy & DP)

Tài liệu hệ thống hóa toàn bộ 6 chiến lược thiết kế giải thuật quan trọng nhất trong DSA (theo đúng mục 4.7 của Roadmap): **Hai con trỏ (Two Pointers)**, **Cửa sổ trượt (Sliding Window)**, **Chia để trị (Divide & Conquer)**, **Quay lui (Backtracking)**, **Tham lam (Greedy)** và **Quy hoạch động (Dynamic Programming)**.

---

## 1. Bảng Tổng Quan Chọn Chiến Lược Giải Thuật

| Chiến lược | Dấu hiệu nhận biết bài toán | Độ phức tạp mục tiêu | Ví dụ kinh điển |
| :--- | :--- | :---: | :--- |
| **1. Two Pointers** *(Hai con trỏ)* | Tìm cặp/bộ phần tử trên mảng **đã sắp xếp**, đảo/lọc mảng tại chỗ, kiểm tra đối xứng, gộp 2 mảng. | $O(N)$ Time $O(1)$ Space | Two Sum Sorted, Valid Palindrome, Remove Duplicates, Merge Sorted Arrays. |
| **2. Sliding Window** *(Cửa sổ trượt)* | Tìm đoạn con liên tiếp (**Subarray / Substring**) dài nhất/ngắn nhất hoặc có tổng/số lượng thỏa điều kiện. | $O(N)$ Time | Max Sum Subarray of Size $K$, Longest Substring Without Repeating Chars. |
| **3. Divide & Conquer** *(Chia để trị)* | Bài toán tách được thành các phần con **độc lập không giao nhau**, giải riêng rồi gộp kết quả. | $O(\log N)$ hoặc $O(N \log N)$ | Binary Search, Merge Sort, Quick Sort, Lũy thừa nhị phân $a^n$, Inversion Count. |
| **4. Backtracking** *(Quay lui)* | Yêu cầu liệt kê **tất cả các cấu hình/nghiệm** thỏa mãn ràng buộc ($N$ nhỏ $\le 20$). | $O(2^N)$ hoặc $O(N!)$ | Subsets, Permutations, Combination Sum, N-Queens, Giải Sudoku. |
| **5. Greedy** *(Tham lam)* | Bài toán tối ưu có tính chất **Chọn tối ưu cục bộ tại mỗi bước luôn dẫn tới tối ưu toàn cục** (thường sau khi Sort). | $O(N)$ hoặc $O(N \log N)$ | Activity Selection, Fractional Knapsack, Jump Game, Dijkstra, Kruskal. |
| **6. Dynamic Programming** *(Quy hoạch động)* | Bài toán tối ưu (Min/Max/Đếm số cách) mà các bài toán con **gối chồng lặp đi lặp lại** (Greedy bị sai). | $O(N)$, $O(N^2)$, $O(N \times W)$ | 0/1 Knapsack, Coin Change, Dãy con tăng dài nhất (LIS), Xâu con chung (LCS). |

---

## 2. Hai Con Trỏ (Two Pointers) & Cửa Sổ Trượt (Sliding Window)

### 2.1. Kỹ thuật Hai Con Trỏ (Two Pointers — $O(N)$)

Thay vì dùng 2 vòng lặp lồng nhau $O(N^2)$ để xét mọi cặp `(i, j)`, ta điều khiển 2 chỉ số `left` và `right` di chuyển đơn điệu theo 1 trong 2 mô hình:

1. **Hai con trỏ ngược chiều (`left = 0, right = n - 1` tiến vào giữa):** Dùng cho mảng đã sắp xếp (như *Two Sum Sorted*: nếu `a[left] + a[right] > target` thì giảm `--right`, nếu `< target` thì tăng `++left`), hoặc kiểm tra Palindrome, đảo mảng.
2. **Hai con trỏ cùng chiều (`write_idx` và `read_idx` cùng xuất phát từ `0`):** Dùng để lọc/nén mảng tại chỗ trong $O(1)$ Space (như xóa phần tử trùng lặp, dồn số `0` về cuối mảng).

### 2.2. Kỹ thuật Cửa Sổ Trượt (Sliding Window — $O(N)$)

```text
  Mảng:   [ 2 | 1 | 5 | 1 | 3 | 2 ]   (Cửa sổ trượt [left ... right])
                ▲           ▲
               left       right
  • Bước 1: Mở rộng biên phải `++right`, nạp `a[right]` vào trạng thái cửa sổ.
  • Bước 2: Khi cửa sổ vi phạm điều kiện (hoặc vượt kích thước K), thu hẹp biên trái `++left`.
```

* **Khuôn mẫu chuẩn cho Cửa sổ trượt kích thước biến thiên (Variable-size Sliding Window):**

  ```cpp
  int left = 0, best_len = 0;
  for (int right = 0; right < n; ++right) {
      add_to_window(a[right]);                 // 1. Mở rộng cửa sổ sang phải
      while (window_is_invalid()) {            // 2. Co cửa sổ từ bên trái nếu vi phạm
          remove_from_window(a[left]);
          ++left;
      }
      best_len = std::max(best_len, right - left + 1); // 3. Cập nhật kết quả tối ưu
  }
  ```

---

## 3. Chia Để Trị (Divide & Conquer) vs Quy Hoạch Động (DP)

Cả hai đều chia bài toán lớn thành các bài toán con nhỏ hơn, nhưng sự khác biệt cốt tử nằm ở **sự trùng lặp của các bài toán con**:

* **Divide & Conquer (Bài toán con ĐỘC LẬP):** Nửa trái `a[0..mid]` và nửa phải `a[mid+1..n-1]` trong Merge Sort hoàn toàn tách biệt, không bao giờ tính lại cùng một đoạn $\rightarrow$ Giải bằng đệ quy chia đôi trực tiếp.
* **Dynamic Programming (Bài toán con GỐI NHAU — Overlapping Subproblems):** Khi tính `fib(5) = fib(4) + fib(3)`, nhánh `fib(4)` lại gọi `fib(3) + fib(2)` $\rightarrow$ `fib(3)` và `fib(2)` bị tính đi tính lại hàng triệu lần nếu đệ quy thuần. DP giải quyết bằng cách **lưu kết quả mỗi bài toán con đúng 1 lần vào bảng nhớ**.

---

## 4. Quay Lui (Backtracking) & Tham Lam (Greedy) (`01_backtracking.cpp`, `02_greedy_algorithms.cpp`)

### 4.1. Quay lui (Backtracking: Choose $\rightarrow$ Explore $\rightarrow$ Unchoose)

* Duyệt cây không gian trạng thái theo chiều sâu (DFS) để tìm mọi nghiệm hợp lệ.
* **Kỹ thuật Cắt tỉa nhánh (Branch Pruning):** Kiểm tra điều kiện hợp lệ ngay trước khi gọi đệ quy (ví dụ: trong *Combination Sum*, nếu tổng hiện tại `> target` thì dừng nhánh đó ngay; trong *N-Queens*, dùng 3 mảng đánh dấu `col`, `diag1[r - c + n - 1]`, `diag2[r + c]` để kiểm tra ô đặt hậu hợp lệ trong **$O(1)$**).

### 4.2. Tham lam (Greedy Algorithms)

* **Activity Selection ($O(N \log N)$):** Sắp xếp các cuộc họp/hoạt động theo **thời gian kết thúc tăng dần (`end`)**, luôn chọn hoạt động kết thúc sớm nhất không đè lên hoạt động trước đó để dành nhiều thời gian trống nhất cho phía sau.
* **Fractional Knapsack ($O(N \log N)$):** Khi đồ vật có thể chia nhỏ (như vàng cám, gạo), sắp xếp giảm dần theo **đơn giá trên mỗi kg (`value / weight`)** và lấy tối đa từ món đắt nhất luôn cho giá trị cao nhất. *(Nếu đồ vật nguyên khối không được cắt — **0/1 Knapsack** — Tham lam sẽ cho kết quả SAI và bắt buộc phải dùng Quy hoạch động!).*

---

## 5. Quy Hoạch Động (Dynamic Programming — DP) (`03_dynamic_programming.cpp`)

### 5.1. Quy trình 4 bước giải mọi bài toán Quy hoạch động

1. **Định nghĩa trạng thái `dp[...]`:** Xác định rõ ý nghĩa của `dp[i]` hoặc `dp[i][j]` (ví dụ: `dp[x]` là số tờ tiền ít nhất để tạo ra số tiền `x`).
2. **Xác định trường hợp cơ sở (Base Cases):** Giá trị khởi điểm không cần tính (ví dụ: `dp[0] = 0`).
3. **Lập công thức chuyển trạng thái (State Transition):** Biểu diễn `dp` hiện tại qua các `dp` nhỏ hơn đã biết.
4. **Quyết định thứ tự tính & tối ưu bộ nhớ (Top-Down Memoization hoặc Bottom-Up Tabulation).**

### 5.2. Ba mẫu Quy hoạch động kinh điển bắt buộc nắm vững

| Bài toán DP | Định nghĩa & Công thức chuyển trạng thái cốt lõi | Độ phức tạp chuẩn |
| :--- | :--- | :---: |
| **1. 0/1 Knapsack** *(Cái túi 0/1 - Mỗi đồ vật lấy tối đa 1 lần)* | Tối ưu xuống mảng 1D `dp[cap]` (duyệt `cap` **ngược từ $W$ về $w_i$** để mỗi vật chỉ dùng 1 lần): `dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i]);` | Time: $O(N \times W)$ Space: **$O(W)$** |
| **2. Coin Change** *(Đổi tiền - Mỗi mệnh giá dùng vô hạn lần)* | Mảng 1D `dp[x]` (duyệt `x` **xuôi từ `coin` lên `amount`** để cho phép lấy lặp lại): `dp[x] = min(dp[x], dp[x - coin] + 1);` | Time: $O(N \times \text{Amount})$ Space: **$O(\text{Amount})$** |
| **3. LIS & LCS / Edit Distance** *(Dãy con & Xâu con)* | **LIS ($O(N \log N)$):** Duy trì mảng `tails[]` tăng dần + `std::lower_bound`. **LCS ($O(N \times M)$):** Nếu `A[i-1] == B[j-1]` thì `dp[i][j] = dp[i-1][j-1] + 1`, ngược lại `max(dp[i-1][j], dp[i][j-1])`. | LIS: **$O(N \log N)$** LCS: $O(N \times M)$ |

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_backtracking.cpp`: Sinh toàn bộ tập con (Subsets) & bài toán N-Queens (tối ưu mảng đánh dấu $O(1)$).
* `02_greedy_algorithms.cpp`: Activity Selection (chọn lịch họp) & Fractional Knapsack.
* `03_dynamic_programming.cpp`: 0/1 Knapsack (so sánh Top-Down Memoization vs Bottom-Up 1D Space $O(W)$) & LIS tối ưu $O(N \log N)$.
* `04_practice_exercises.cpp`: 5 bài tập kinh điển (Combination Sum có cắt tỉa nhánh, Jump Game Greedy $O(N)$, Coin Change DP, Longest Common Subsequence LCS, Edit Distance - Levenshtein).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_backtracking.cpp -o dsa_backtrack
./dsa_backtrack

g++ -Wall -Wextra -std=c++17 02_greedy_algorithms.cpp -o dsa_greedy
./dsa_greedy

g++ -Wall -Wextra -std=c++17 03_dynamic_programming.cpp -o dsa_dp
./dsa_dp

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o dsa_paradigms_ex
./dsa_paradigms_ex
```

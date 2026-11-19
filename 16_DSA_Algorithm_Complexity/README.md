# Bài 4.1: Độ Phức Tạp Thuật Toán (Algorithm Complexity & Big-O Analysis)

Tài liệu tổng hợp phương pháp đánh giá hiệu năng thuật toán độc lập với phần cứng thông qua ký hiệu tiệm cận (**Big-O, $\Omega$, $\Theta$**), phân tích **Time Complexity**, **Space Complexity**, **Amortized Complexity (Độ phức tạp khấu hao)**, Định lý Master cho đệ quy và bảng ước lượng giới hạn $N$ chuẩn trong phỏng vấn kỹ thuật & lập trình thi đấu.

---

## 1. Bản Chất Của Ký Hiệu Tiệm Cận: Big-O ($O$), Omega ($\Omega$) & Theta ($\Theta$)

Khi kích thước dữ liệu đầu vào $N \to \infty$, tốc độ chạy của thuật toán phụ thuộc vào **tốc độ tăng trưởng số phép toán cơ bản** chứ không phải số giây tuyệt đối trên một máy tính đơn lẻ:

| Ký hiệu | Tên gọi | Ý nghĩa toán học & Thực tế | Ví dụ trên thuật toán Linear Search ($N$ phần tử) |
| :---: | :--- | :--- | :--- |
| **$O(f(N))$** | **Big-O** *(Cận trên - Upper Bound)* | Đánh giá **Trường hợp xấu nhất (Worst-Case)** — cam kết thuật toán không bao giờ chậm quá ngưỡng này. *(Dùng 95% trong phỏng vấn & thi đấu)*. | **$O(N)$** — Phần tử cần tìm nằm ở cuối mảng hoặc không tồn tại. |
| **$\Omega(f(N))$** | **Big-Omega** *(Cận dưới - Lower Bound)* | Đánh giá **Trường hợp tốt nhất (Best-Case)** — số phép toán tối thiểu phải thực hiện. | **$\Omega(1)$** — Phần tử cần tìm nằm ngay ở vị trí đầu tiên `a[0]`. |
| **$\Theta(f(N))$** | **Big-Theta** *(Cận chặt - Tight Bound)* | Khi cả cận trên và cận dưới trùng nhau ($O$ và $\Omega$ cùng bậc tăng trưởng). | Duyệt tính tổng cả mảng luôn tốn đúng **$\Theta(N)$** mọi trường hợp. |

---

## 2. Bảng Thứ Bậc Big-O & Ước Lượng Giới Hạn $N$ (Chuẩn $1\text{ Giây} \approx 10^8$ Phép Toán)

Thứ tự tăng trưởng từ nhanh nhất đến chậm nhất:
$$O(1) < O(\log N) < O(\sqrt{N}) < O(N) < O(N \log N) < O(N^2) < O(N^3) < O(2^N) < O(N!)$$

| Độ phức tạp | Tên gọi | Ví dụ thuật toán kinh điển | Giới hạn $N$ tối đa chạy trong $\le 1\text{s}$ |
| :--- | :--- | :--- | :---: |
| **$O(1)$** | Hằng số *(Constant)* | Truy xuất mảng `a[i]`, phép toán bit, `push_back`, công thức tổng Gauss. | Không giới hạn ($N \le 10^{18}$) |
| **$O(\log N)$** | Logarit *(Logarithmic)* | Binary Search, Lũy thừa nhị phân ($a^b$), Euclid GCD, thao tác trên BST cân bằng / Heap. | $N \le 10^{18}$ *(vì $\log_2(10^{18}) \approx 60$ bước)* |
| **$O(\sqrt{N})$** | Căn bậc hai *(Square Root)* | Kiểm tra số nguyên tố, phân tích thừa số nguyên tố, liệt kê mọi ước số của $N$. | $N \le 10^{14} - 10^{16}$ |
| **$O(N)$** | Tuyến tính *(Linear)* | Linear Search, Kadane's Algorithm, Two Pointers, Sliding Window, Prefix Sum, Hash Map duyệt. | $N \le 10^7 - 10^8$ |
| **$O(N \log N)$** | Tuyến tính - Logarit | Merge Sort, Quick Sort (avg), Heap Sort, `std::sort`, Dijkstra, Kruskal, LIS tối ưu. | $N \le 5 \times 10^5 - 2 \times 10^6$ |
| **$O(N^2)$** | Bậc hai *(Quadratic)* | Bubble / Insertion / Selection Sort, 2 vòng lặp lồng nhau, Quy hoạch động 2D ($N \times N$). | $N \le 5,000 - 10,000$ |
| **$O(N^3)$** | Bậc ba *(Cubic)* | Nhân 2 ma trận $N \times N$ cơ bản, thuật toán đường đi ngắn nhất Floyd-Warshall. | $N \le 300 - 500$ |
| **$O(2^N)$** | Hàm mũ *(Exponential)* | Fibonacci đệ quy thuần, vét cạn mọi tập con (Bitmask / Backtracking Subsets), QHĐ trạng thái bit. | $N \le 20 - 24$ *(vì $2^{24} \approx 1.6 \times 10^7$)* |
| **$O(N!)$** | Giai thừa *(Factorial)* | Sinh tất cả hoán vị, bài toán Người du lịch (TSP) vét cạn. | $N \le 10 - 11$ *(vì $11! \approx 4 \times 10^7$)* |

---

## 3. Sổ Tay Quy Tắc Tính Nhanh Time Complexity & Các Bẫy Phỏng Vấn

### 3.1. Năm quy tắc rút gọn cơ bản

1. **Bỏ qua mọi hằng số nhân và hằng số cộng:** $O(5N + 1000) \rightarrow O(N)$; $O(\frac{1}{2}N^2) \rightarrow O(N^2)$.
2. **Chỉ giữ lại số hạng có bậc tăng trưởng lớn nhất:** $O(N^2 + N \log N + 50N) \rightarrow O(N^2)$.
3. **Các khối lệnh chạy nối tiếp nhau $\rightarrow$ Phép Cộng:** Khối 1 tốn $O(N)$, khối 2 tốn $O(M)$ $\rightarrow$ Tổng là $O(N + M)$.
4. **Các vòng lặp lồng nhau độc lập $\rightarrow$ Phép Nhân:** Vòng ngoài chạy $N$ lần, vòng trong chạy $M$ lần $\rightarrow$ Tổng là $O(N \times M)$.
5. **Biến lặp tăng/giảm theo cấp số nhân (`i *= 2` hoặc `n /= 2`) $\rightarrow$ Logarit:** Tốn $O(\log_2 N)$ bước.

### 3.2. Ba "Cạm bẫy" đánh giá sai độ phức tạp của Vòng lặp Lồng nhau

Không phải cứ thấy 2 vòng lặp `for` / `while` lồng nhau là kết luận ngay $O(N^2)$! Hãy chú ý 3 trường hợp kinh điển sau:

```cpp
// BẪY 1: Kỹ thuật Hai con trỏ (Two Pointers / Sliding Window) ---> ĐỘ PHỨC TẠP: O(N), KHÔNG PHẢI O(N^2)!
int left = 0;
for (int right = 0; right < n; ++right) {
    while (left < right && need_shrink(left, right)) {
        ++left; // Biến `left` chỉ tăng tiến từ 0 đến n - 1 tối đa đúng N lần trong suốt cả chương trình!
    }
}

// BẪY 2: Chuỗi điều hòa (Harmonic Series - Sàng Eratosthenes / Bội số) ---> ĐỘ PHỨC TẠP: O(N log N)!
for (int i = 1; i <= n; ++i) {
    for (int j = i; j <= n; j += i) { // Bước nhảy là `+ i` chứ không phải `+ 1`!
        // Số lần chạy: N/1 + N/2 + N/3 + ... + N/N = N * (1 + 1/2 + 1/3 + ... + 1/N) = O(N log N)
    }
}

// BẪY 3: Gọi hàm strlen(s) trong điều kiện vòng lặp C-String ---> VÔ TÌNH BIẾN O(N) THÀNH O(N^2)!
for (size_t i = 0; i < std::strlen(s); ++i) { ... } // SAI: Mỗi bước lặp lại đếm lại strlen O(N) -> Tổng O(N^2)!
```

### 3.3. Đánh giá độ phức tạp Đệ quy (Cây đệ quy & Định lý Master)

Với phương trình đệ quy chia để trị dạng $T(N) = a \cdot T\left(\frac{N}{b}\right) + O(N^d)$ (chia bài toán kích thước $N$ thành $a$ bài toán con kích thước $N/b$, tốn $O(N^d)$ chi phí gộp):

* **Binary Search** ($a = 1, b = 2, d = 0$): $T(N) = T(N/2) + O(1) \implies \mathbf{O(\log N)}$.
* **Merge Sort** ($a = 2, b = 2, d = 1$): $T(N) = 2T(N/2) + O(N) \implies \mathbf{O(N \log N)}$.
* **Duyệt cây nhị phân** ($a = 2, b = 2, d = 0$): $T(N) = 2T(N/2) + O(1) \implies \mathbf{O(N)}$.
* **Fibonacci đệ quy thuần** ($T(N) = T(N-1) + T(N-2) + O(1)$): Cây nhị phân sâu $N$ tầng $\implies \mathbf{O(2^N)}$ (chính xác là $O(1.618^N)$).

### 3.4. Amortized Complexity (Độ phức tạp khấu hao)

* **Ví dụ điển hình:** Thao tác `push_back()` của `std::vector` (hoặc `insert` của Hash Table có Rehashing).
* Hầu hết các lần `push_back()` chỉ tốn $O(1)$. Chỉ khi mảng đầy (`size == capacity`, xảy ra tại các mốc lũy thừa của 2: $1, 2, 4, 8, \dots, N$), vector mới tốn $O(N)$ để cấp phát gấp đôi và chép dữ liệu sang.
* Tổng chi phí cho $N$ lần chèn liên tiếp là: $N + (1 + 2 + 4 + \dots + N) \approx 3N$ phép toán. Chia trung bình cho $N$ lần thao tác, mỗi lần `push_back()` có độ phức tạp khấu hao là **$O(1)$ Amortized**.

---

## 4. Phân Tích Space Complexity (Độ Phức Tạp Không Gian Bộ Nhớ)

**Auxiliary Space Complexity** đo lường lượng bộ nhớ phụ tối đa mà thuật toán xin cấp phát thêm (trên cả **Heap** lẫn **Call Stack**) trong quá trình chạy:

1. **Thuật toán tại chỗ (In-place — $O(1)$ Space):** Chỉ dùng một số lượng biến đơn cố định (`left`, `right`, `sum`, `temp`), không phụ thuộc vào $N$.
2. **Cấu trúc phụ trợ ($O(N)$ hoặc $O(N^2)$ Space):** Cấp phát mảng tạm `temp[N]` trong Merge Sort, bảng `dp[N][W]`, hoặc `std::unordered_map`.
3. **Bộ nhớ Call Stack của Đệ quy ($O(D)$ Space với $D$ là độ sâu đệ quy tối đa):**
   * Đệ quy tuyến tính `factorial(N)` nằm chồng $N$ Stack Frames cùng lúc $\rightarrow$ Tốn **$O(N)$ Space** (dễ gây Stack Overflow nếu $N > 10^5$).
   * Merge Sort và Binary Search đệ quy chỉ đi sâu tối đa $\log_2 N$ tầng tại một thời điểm $\rightarrow$ Chỉ tốn **$O(\log N)$ Call Stack Space**.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_time_complexity_benchmark.cpp`: Dùng `<chrono>` đo thời gian thực thi thực tế của $O(1)$, $O(\log N)$, $O(N)$, $O(N \log N)$, $O(N^2)$, $O(2^N)$.
* `02_space_complexity_analysis.cpp`: So sánh trực diện thuật toán In-place $O(1)$ Space vs Mảng phụ $O(N)$ Space và Call Stack đệ quy.
* `03_practice_exercises.cpp`: 5 bài tập tối ưu hóa từ thuật toán ngây thơ xuống chuẩn Big-O (Prime $O(\sqrt{N})$, Missing Number XOR $O(N)$ Time / $O(1)$ Space, Two Sum Sorted $O(N)$, Kadane's Algorithm $O(N)$, Fibonacci $O(N)$ Time / $O(1)$ Space).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 -O2 01_time_complexity_benchmark.cpp -o bench_bigo
./bench_bigo

g++ -Wall -Wextra -std=c++17 02_space_complexity_analysis.cpp -o space_analysis
./space_analysis

g++ -Wall -Wextra -std=c++17 03_practice_exercises.cpp -o bigo_exercises
./bigo_exercises
```

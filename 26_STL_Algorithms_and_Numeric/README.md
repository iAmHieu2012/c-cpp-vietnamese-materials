# Bài 5.4: Thuật Toán Chuẩn STL (`<algorithm>` & `<numeric>`) Toàn Tập

Tài liệu hệ thống hóa hơn 30 thuật toán cốt lõi trong hai thư viện `<algorithm>` và `<numeric>` của C++17: từ **Sắp xếp & Phân hoạch ($O(N \log N)$ / $O(N)$)**, **Tìm kiếm nhị phân ($O(\log N)$)**, **Biến đổi & Tích lũy số học**, đến **Hoán vị, Xoay mảng & Phép toán Tập hợp**, kèm 4 cạm bẫy chết người cần tuyệt đối tránh.

---

## 1. Nhóm Sắp Xếp, Phân Hoạch & Tìm Kiếm Nhị Phân (`01_sorting_and_searching_stl.cpp`)

### 1.1. So sánh 4 thuật toán Sắp xếp & Chọn phần tử trong STL

```text
  Cho mảng gốc: [ 40 | 10 | 60 | 20 | 50 | 30 | 70 ] (N = 7, giả sử K = 3)

  1. std::sort(begin, end)                    --> [ 10 | 20 | 30 | 40 | 50 | 60 | 70 ]  (Sắp toàn bộ: O(N log N))
  2. std::partial_sort(begin, begin + 3, end) --> [ 10 | 20 | 30 | 60 | 50 | 40 | 70 ]  (Chỉ sắp đúng Top 3: O(N log K))
  3. std::nth_element(begin, begin + 2, end)  --> [ 20 | 10 | 30 | 60 | 50 | 40 | 70 ]  (Chỉ chốt đúng ô [2]=30: O(N)!)
                                                   |<-- <=30 -->|      |<--- >=30 --->|
```

| Hàm thuật toán | Độ phức tạp | Cơ chế lõi bên dưới | Khi nào sử dụng chuẩn công nghiệp? |
| :--- | :---: | :--- | :--- |
| **`std::sort`** | $O(N \log N)$ | **Introsort** *(Lai giữa Quick Sort + Heap Sort + Insertion Sort)* | Sắp xếp thông thường nhanh nhất (Yêu cầu **Random Access Iterator**: `vector`, `array`, `deque`). |
| **`std::stable_sort`** | $O(N \log N)$ | **Merge Sort** *(Tốn thêm $O(N)$ RAM)* | Khi cần **giữ nguyên thứ tự ban đầu** của các phần tử có giá trị bằng nhau (ví dụ: sắp theo Điểm, đồng điểm giữ nguyên thứ tự nhập). |
| **`std::partial_sort`** | **$O(N \log K)$** | **Binary Heap** kích thước $K$ | Khi chỉ cần lấy ra **Top $K$ phần tử đầu tiên đã sắp xếp** (với $K \ll N$, nhanh hơn nhiều so với sort cả mảng). |
| **`std::nth_element`** | **$O(N)$** *(avg)* | **Introselect / Quickselect** | Tìm **Trung vị (Median)** hoặc **Phần tử lớn thứ $K$ / nhỏ thứ $K$** trong thời gian tuyến tính $O(N)$ mà không cần sắp xếp các phần tử khác! |

### 1.2. Bộ tứ Tìm kiếm nhị phân trên mảng đã sắp xếp — $O(\log N)$

* **`std::binary_search(begin, end, val)`:** Chỉ trả về `true` / `false` (phần tử `val` có tồn tại hay không).
* **`std::lower_bound(begin, end, val)`:** Trả về Iterator trỏ tới phần tử **đầu tiên $\ge \text{val}$** (nếu không có, trả về `end()`).
* **`std::upper_bound(begin, end, val)`:** Trả về Iterator trỏ tới phần tử **đầu tiên $> \text{val}$** (nếu không có, trả về `end()`).
* **`std::equal_range(begin, end, val)`:** Trả về một `std::pair{lower_bound, upper_bound}` trong 1 lần gọi.
  * **Công thức đổi Iterator sang chỉ số mảng (Index):** `int idx = static_cast<int>(it - v.begin());`
  * **Công thức đếm số lần xuất hiện của `val` trong $O(\log N)$:** `auto [lo, hi] = std::equal_range(v.begin(), v.end(), val); int count = hi - lo;`

---

## 2. Nhóm Toán Học `<numeric>` & Biến Đổi Dữ Liệu (`02_transform_and_numeric.cpp`)

| Hàm `<numeric>` / `<algorithm>` | Độ phức tạp | Cú pháp mẫu & Chức năng thực chiến |
| :--- | :---: | :--- |
| **`std::iota`** | $O(N)$ | `std::iota(v.begin(), v.end(), 1);` $\rightarrow$ Điền dãy tăng dần liên tiếp `1, 2, 3, ..., N` (rất hay dùng khởi tạo mảng `parent` trong DSU hoặc mảng chỉ số). |
| **`std::accumulate`** | $O(N)$ | `long long s = std::accumulate(v.begin(), v.end(), 0LL);` $\rightarrow$ Tính tổng (hoặc tích, chuỗi gộp) toàn mảng. |
| **`std::partial_sum`** | $O(N)$ | `std::partial_sum(v.begin(), v.end(), pref.begin());` $\rightarrow$ Tạo mảng cộng dồn **Prefix Sum** trong 1 dòng lệnh! |
| **`std::adjacent_difference`** | $O(N)$ | `std::adjacent_difference(v.begin(), v.end(), diff.begin());` $\rightarrow$ Tạo mảng hiệu `diff[i] = v[i] - v[i-1]` (Difference Array). |
| **`std::inner_product`** | $O(N)$ | `std::inner_product(a.begin(), a.end(), b.begin(), 0LL);` $\rightarrow$ Tính tích vô hướng của 2 vector $\sum a_i \cdot b_i$. |
| **`std::gcd` & `std::lcm`** *(C++17)* | $O(\log(\min(a,b)))$ | Có sẵn trong `<numeric>`: `std::gcd(48, 18)` trả về `6`; `std::lcm(12, 18)` trả về `36`. |
| **`std::transform`** | $O(N)$ | Áp dụng hàm Lambda lên từng phần tử (giống hàm `map()` trong Python/JS): `std::transform(v.begin(), v.end(), out.begin(), [](int x) { return x * x; });` |
| **`std::clamp`** *(C++17)* | $O(1)$ | `val = std::clamp(val, min_v, max_v);` $\rightarrow$ Kẹp giá trị `val` nằm gọn trong đoạn `[min_v, max_v]`. |
| **`all_of` / `any_of` / `none_of`** | $O(N)$ | Kiểm tra logic toàn mảng với Lambda: Tất cả đều thỏa mãn (`all_of`), Có ít nhất 1 phần tử thỏa (`any_of`), Không có phần tử nào thỏa (`none_of`). |

---

## 3. Hoán Vị, Xoay Mảng & Phép Toán Tập Hợp (`03_permutations_and_set_ops.cpp`)

### 3.1. Sinh hoán vị kế tiếp (`std::next_permutation`) & Xoay mảng (`std::rotate`)

```text
  1. std::rotate(v.begin(), v.begin() + 2, v.end()) — Xoay trái 2 vị trí tại chỗ trong O(N) Time, O(1) Space:
     Trước: [ 1 | 2 | 3 | 4 | 5 ]   (Đưa phần tử tại begin() + 2 là số '3' lên làm đầu mảng mới!)
     Sau  : [ 3 | 4 | 5 | 1 | 2 ]   (Mẹo xoay PHẢI k bước: std::rotate(v.rbegin(), v.rbegin() + k, v.rend()))

  2. std::unique(v.begin(), v.end()) — Khử các phần tử trùng lặp ĐỨNG LIỀN KỀ NHAU trong O(N):
     Sau khi sort: [ 1 | 2 | 2 | 3 | 3 | 3 | 4 ] ===(unique + erase)===> [ 1 | 2 | 3 | 4 ]
```

* **`std::next_permutation(begin, end)`:** Biến đổi mảng hiện tại thành hoán vị từ điển lớn hơn kế tiếp trong $O(N)$ và trả về `true` (nếu đã là hoán vị giảm dần lớn nhất, nó đảo về nhỏ nhất và trả về `false`).

### 3.2. Bốn phép toán Tập hợp trên 2 mảng ĐÃ SẮP XẾP — $O(N + M)$

Không cần tốn bộ nhớ tạo `std::set` ($O(N \log N)$), nếu hai `std::vector` `A` và `B` đã được sắp xếp tăng dần, STL xử lý các phép toán tập hợp trong thời gian tuyến tính **$O(N + M)$** bằng kỹ thuật 2 con trỏ:

* **`std::merge`:** Trộn 2 mảng đã sắp xếp thành 1 mảng sắp xếp (giữ toàn bộ phần tử).
* **`std::set_union`:** Hợp $A \cup B$ (lấy các phần tử thuộc $A$ hoặc $B$, không lặp).
* **`std::set_intersection`:** Giao $A \cap B$ (chỉ lấy các phần tử xuất hiện ở **cả $A$ lẫn $B$**).
* **`std::set_difference`:** Hiệu $A \setminus B$ (lấy các phần tử **có trong $A$ nhưng không có trong $B$**).
* **`std::set_symmetric_difference`:** Hiệu đối xứng $(A \setminus B) \cup (B \setminus A)$.

---

## 4. Bốn "Cạm Bẫy" Chết Người Khi Dùng STL Algorithms (Trọng Tâm Phỏng Vấn)

1. **Vi phạm quy tắc *Strict Weak Ordering* trong Comparator của `std::sort`:**
   * Hàm so sánh `cmp(a, b)` **BẮT BUỘC phải trả về `false` khi `a == b`** (chỉ dùng `<` hoặc `>`, **TUYỆT ĐỐI CẤM dùng `<=` hoặc `>=`**!).
   * Nếu bạn viết `return a.score >= b.score;`, khi mảng có nhiều phần tử bằng nhau, thuật toán Introsort sẽ đọc vượt biên mảng gây **Segmentation Fault** ngay lúc chạy!
2. **Tràn số `int` ngầm trong `std::accumulate`:**
   * Kiểu dữ liệu trả về của `std::accumulate(v.begin(), v.end(), init)` được quyết định hoàn toàn bởi **kiểu của tham số thứ ba (`init`)**!
   * Nếu truyền số `0` (kiểu `int`), tổng vượt $2 \times 10^9$ sẽ bị tràn số dù bạn hứng kết quả bằng biến `long long` hay `double`.
   * **Cách viết chuẩn:** Truyền **`0LL`** cho tổng `long long`, và truyền **`0.0`** cho tổng số thực `double`.
3. **Quên `std::sort` trước vòng lặp `do { ... } while (std::next_permutation(...))`:**
   * `next_permutation` chỉ sinh các hoán vị **lớn hơn** cấu hình hiện tại theo thứ tự từ điển. Nếu mảng ban đầu là `{3, 1, 2}`, nó chỉ sinh tiếp từ `{3, 1, 2}` đến `{3, 2, 1}` rồi dừng (bỏ sót mất 4 hoán vị đầu!). **Bắt buộc phải gọi `std::sort(v.begin(), v.end())` trước vòng lặp `do-while`**.
4. **Ghi vào Vector rỗng mà quên `std::back_inserter`:**
   * Khi gọi `std::transform(a.begin(), a.end(), b.begin(), ...)` hoặc `std::set_intersection(..., res.begin())`, nếu vector đích `res` chưa được cấp phát `size` (`std::vector<int> res;`), lệnh ghi vào `res.begin()` sẽ ghi đè vào vùng nhớ chưa cấp phát gây **Crash**!
   * **Giải pháp:** Hoặc khởi tạo sẵn kích thước `std::vector<int> res(a.size());`, hoặc truyền **`std::back_inserter(res)`** (thuộc `<iterator>`) để tự động gọi `res.push_back()`.

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_sorting_and_searching_stl.cpp`: `std::sort`, `std::stable_sort`, `std::partial_sort`, `std::nth_element` ($O(N)$), `equal_range` và `minmax_element`.
* `02_transform_and_numeric.cpp`: `std::iota`, `std::accumulate` (`0LL`), `std::partial_sum`, `std::inner_product`, `std::gcd`/`std::lcm`, `std::transform` và `all_of`/`any_of`/`count_if`.
* `03_permutations_and_set_ops.cpp`: `std::next_permutation`, `std::rotate` (trái/phải $O(1)$ Space), `std::set_union`, `std::set_intersection`, `std::set_difference`.
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Bảng tổng sắp Huy chương đa tiêu chí, Truy vấn Prefix Sum & Chuẩn hóa điểm bằng `clamp`, Bài toán Người du lịch TSP bằng `next_permutation`, Phân tích tập khách hàng 2 chi nhánh $O(N+M)$, Tính Trung vị & Trimmed Mean $O(N)$ bằng `nth_element`).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_sorting_and_searching_stl.cpp -o stl_sort_search
./stl_sort_search

g++ -Wall -Wextra -std=c++17 02_transform_and_numeric.cpp -o stl_numeric
./stl_numeric

g++ -Wall -Wextra -std=c++17 03_permutations_and_set_ops.cpp -o stl_perm_set
./stl_perm_set

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o stl_algo_ex
./stl_algo_ex
```

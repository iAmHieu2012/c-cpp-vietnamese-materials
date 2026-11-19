# Bài 2.6: Kỹ Thuật Đệ Quy (Recursion) & Quay Lui (Backtracking)

Tài liệu hệ thống hóa cơ chế **Call Stack** khi gọi đệ quy, phân loại 3 dạng đệ quy cốt lõi (Tuyến tính, Nhị phân, Đệ quy đuôi), khuôn mẫu tư duy **Quay lui (Backtracking)** và kỹ thuật cắt tỉa nhánh (Pruning) qua các bài toán kinh điển: Hoán vị, Tổ hợp, N-Queens và Sudoku.

---

## 1. Bản Chất Của Đệ Quy & Cơ Chế Call Stack

### 1.1. Hai thành phần bắt buộc của mọi hàm Đệ quy

1. **Điều kiện dừng (Base Case):** Trường hợp nhỏ nhất có thể trả về kết quả trực tiếp mà không cần gọi tiếp hàm đệ quy. *(Thiếu Base Case hoặc Base Case không bao giờ chạm tới sẽ gây tràn ngăn xếp **Stack Overflow**).*
2. **Bước đệ quy (Recursive Step):** Chia nhỏ bài toán hiện tại thành bài toán con cùng bản chất nhưng kích thước nhỏ hơn, tiến dần về Base Case.

### 1.2. Quá trình Push / Pop trên Call Stack (Ví dụ `factorial(3)`)

Mỗi lần hàm tự gọi lại chính nó, CPU tạo ra một **Stack Frame** mới chứa tham số, biến cục bộ và địa chỉ trả về (Return Address) đẩy lên đỉnh **Stack**:

```text
  Giai đoạn 1: ĐI XUỐNG (Push Stack)          Giai đoạn 2: TRẢ VỀ (Pop Stack)
  ┌──────────────────────────────┐            ┌──────────────────────────────┐
  │ factorial(1) -> Base Case: 1 │ ─────────► │ return 1                     │
  ├──────────────────────────────┤            ├──────────────────────────────┤
  │ factorial(2) -> chờ 2 * f(1) │            │ 2 * 1 = 2  -> return 2       │
  ├──────────────────────────────┤            ├──────────────────────────────┤
  │ factorial(3) -> chờ 3 * f(2) │            │ 3 * 2 = 6  -> return 6       │
  ├──────────────────────────────┤            ├──────────────────────────────┤
  │ main()                       │            │ Nhận kết quả = 6             │
  └──────────────────────────────┘            └──────────────────────────────┘
```

---

## 2. Phân Loại 3 Dạng Đệ Quy Cốt Lõi

| Loại đệ quy | Đặc điểm cấu trúc | Độ phức tạp tiêu biểu | Ví dụ & Lưu ý thực chiến |
| :--- | :--- | :---: | :--- |
| **Đệ quy tuyến tính** *(Linear Recursion)* | Mỗi lần gọi chỉ phát sinh **1 lời gọi đệ quy** duy nhất. | Time: $O(N)$ Space: $O(N)$ | Tính $N!$, duyệt danh sách liên kết, tìm kiếm nhị phân ($O(\log N)$). |
| **Đệ quy nhị phân** *(Binary Recursion)* | Mỗi lần gọi rẽ thành **2 lời gọi đệ quy** (tạo cây nhị phân). | Time: $O(2^N)$ Space: $O(N)$ | Fibonacci thuần (`f(n-1) + f(n-2)`), Tháp Hà Nội. **Rất chậm** nếu có bài toán con lặp lại $\rightarrow$ cần Quy hoạch động (Memoization). |
| **Đệ quy đuôi** *(Tail Recursion)* | Lời gọi đệ quy là **thao tác cuối cùng** trong hàm, không còn phép toán nào chờ sau nó. | Time: $O(N)$ Space: $O(1)$ *(với `-O2`)* | `return fact_tail(n - 1, n * acc);`. Trình biên dịch bật tối ưu **TCO (Tail Call Optimization)** sẽ tái sử dụng 1 Stack Frame duy nhất, chống Stack Overflow. |

---

## 3. Thuật Toán Quay Lui (Backtracking)

### 3.1. Khuôn mẫu 3 bước kinh điển (Choose $\rightarrow$ Explore $\rightarrow$ Unchoose)

Quay lui là kỹ thuật duyệt cây tìm kiếm theo chiều sâu (DFS) để thử các cấu hình khả dĩ. Nếu một nhánh dẫn vào ngõ cụt hoặc vi phạm ràng buộc, ta **khôi phục lại trạng thái trước đó** để thử nhánh khác:

```cpp
void backtrack(int step) {
    // 1. Điều kiện dừng: Đã xây dựng xong 1 cấu hình hoàn chỉnh
    if (is_solution(step)) {
        process_solution();
        return;
    }

    // 2. Duyệt tất cả các ứng cử viên khả dĩ tại bước hiện tại
    for (auto candidate : get_candidates(step)) {
        if (is_valid(candidate)) {       // Cắt tỉa nhánh (Pruning) nếu vi phạm ràng buộc
            make_move(candidate);        // BƯỚC 1: CHỌN (Choose)
            backtrack(step + 1);         // BƯỚC 2: ĐI SÂU (Explore)
            undo_move(candidate);        // BƯỚC 3: QUAY LUI / BỎ CHỌN (Unchoose)
        }
    }
}
```

### 3.2. Kỹ thuật tối ưu kiểm tra $O(1)$ trong bài toán N-Queens

Thay vì duyệt lại toàn bộ bàn cờ $N \times N$ mỗi khi đặt quân hậu mới (tốn $O(N)$), ta dùng 3 mảng đánh dấu `bool` để kiểm tra xung đột trong **$O(1)$**:

* **Cột:** `col_used[col]`
* **Đường chéo chính** *(hiệu `row - col` không đổi)*: `diag1_used[row - col + n]` *(cộng `n` để tránh chỉ số âm)*
* **Đường chéo phụ** *(tổng `row + col` không đổi)*: `diag2_used[row + col]`

---

## 4. Khi Nào Nên Dùng Đệ Quy vs Vòng Lặp?

| Tiêu chí | Đệ quy (Recursion) | Vòng lặp (Iteration) |
| :--- | :--- | :--- |
| **Điểm mạnh** | Code cực ngắn gọn, tự nhiên cho các cấu trúc phân nhánh (Cây, Đồ thị DFS, Quay lui, Chia để trị). | Tốc độ nhanh hơn, **không tốn bộ nhớ Call Stack**, không bao giờ bị Stack Overflow. |
| **Điểm yếu** | Tốn chi phí gọi hàm (Stack Frame), dễ tràn Stack nếu độ sâu $N > 10^4 - 10^5$. | Khó viết và rối code hơn với các bài toán quay lui nhiều trạng thái (phải tự quản lý `std::stack` thủ công). |

---

## 5. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_call_stack_and_types.cpp`: In truy vết Push/Pop Call Stack của Giai thừa, so sánh Đệ quy tuyến tính, Đệ quy đuôi và Đệ quy nhị phân.
* `02_backtracking_permutations_combinations.cpp`: Áp dụng khuôn mẫu Quay lui sinh Hoán vị $N!$ và Tổ hợp $C(N, K)$.
* `03_n_queens_and_sudoku.cpp`: Giải bài toán 8-Queens (kiểm tra $O(1)$) và giải bàn cờ Sudoku $9 \times 9$.
* `04_practice_exercises.cpp`: Giải 5 bài tập thực chiến (Lũy thừa nhị phân $O(\log N)$, Tháp Hà Nội, Chuỗi nhị phân không có hai số `1` kề nhau, Subset Sum, Chuột thoát Mê cung).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_call_stack_and_types.cpp -o rec_types
./rec_types

g++ -Wall -Wextra -std=c++17 02_backtracking_permutations_combinations.cpp -o backtrack_basic
./backtrack_basic

g++ -Wall -Wextra -std=c++17 03_n_queens_and_sudoku.cpp -o nqueens_sudoku
./nqueens_sudoku

g++ -Wall -Wextra -std=c++17 04_practice_exercises.cpp -o rec_practice
./rec_practice
```

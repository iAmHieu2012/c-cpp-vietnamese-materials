# Bài 4.4: Cấu Trúc Cây Toàn Tập (Binary Tree, BST, AVL/Red-Black, Binary Heap & Trie)

Tài liệu hệ thống hóa kiến trúc, cơ chế vận hành và cài đặt thủ công (chuẩn C++17 + RAII) của toàn bộ 5 cấu trúc cây cốt lõi trong mục 4.4 của Roadmap: **Cây nhị phân (Binary Tree)**, **Cây nhị phân tìm kiếm (BST)**, **Cây tự cân bằng (AVL & Red-Black Tree)**, **Đống nhị phân (Binary Heap / Priority Queue)** và **Cây tiền tố (Trie / Prefix Tree)**.

---

## 1. Cây Nhị Phân (Binary Tree) & Cây Nhị Phân Tìm Kiếm (BST) (`01_binary_tree_and_bst.cpp`)

### 1.1. Bất biến của BST & 4 Phép Duyệt Cây Kinh Điển

* **Tính chất bất biến của BST (Binary Search Tree Property):** Tại mọi node `u`, tất cả các node thuộc **cây con bên trái** đều có giá trị **`< u->val`**, và tất cả các node thuộc **cây con bên phải** đều có giá trị **`> u->val`**.
* **Cạm bẫy khi kiểm tra BST (Validate BST):** Không chỉ so sánh `u` với con trực tiếp `u->left` và `u->right`, mà phải truyền khoảng giới hạn `(min_bound, max_bound)` xuống từng nhánh đệ quy!

| Phép duyệt cây | Thứ tự thăm Node | Cơ chế cài đặt | Ứng dụng thực tế cốt lõi |
| :--- | :---: | :---: | :--- |
| **1. Pre-order (NLR)** | `Node -> Left -> Right` | Đệ quy / Stack | Sao chép cây (Clone), tuần tự hóa cây ra file (Serialize). |
| **2. In-order (LNR)** | `Left -> Node -> Right` | Đệ quy / Stack | Duyệt BST theo thứ tự **tăng dần hoàn hảo** ($O(N)$). |
| **3. Post-order (LRN)** | `Left -> Right -> Node` | Đệ quy / Stack | **Giải phóng bộ nhớ cây (`~Destructor`)** (xóa hết con rồi mới xóa cha), tính chiều cao/kích thước cây con. |
| **4. Level-order (BFS)** | Từng tầng từ trên xuống | `std::queue` | Duyệt theo độ sâu, tìm khoảng cách ngắn nhất từ gốc. |

### 1.2. Thuật toán Xóa Node trong BST (`remove_rec`) — 3 Trường Hợp

```text
  Trường hợp 3 (Khó nhất — Node cần xóa [50] có đủ cả 2 con Trái và Phải):
           [50] (Cần xóa)                                [60] (Chép In-order Successor lên!)
          ╱    ╲           1. Tìm Min của nhánh phải     ╱    ╲
       [30]    [70]        ==> In-order Successor [60] [30]    [70]
              ╱    ╲       2. Đè 60 lên 50                    ╱    ╲
           [60]    [80]    3. Đệ quy xóa [60] ở nhánh phải  (Xóa)  [80]
```

1. **Node lá (0 con):** `delete node;` và trả về `nullptr` để ngắt liên kết từ cha.
2. **Node chỉ có 1 con (Trái hoặc Phải):** Lưu lại con duy nhất `child`, `delete node;`, rồi trả `child` về nối thẳng lên ông.
3. **Node có đủ 2 con:** Tìm **In-order Successor** (node nhỏ nhất của cây con phải: `find_min(node->right)`), chép giá trị của nó vào `node->val`, rồi gọi đệ quy xóa node Successor đó ở cây con bên phải.

---

## 2. Cây Nhị Phân Tự Cân Bằng: AVL Tree & Red-Black Tree (`03_avl_tree_and_trie.cpp`)

Nếu chèn một dãy số đã tăng dần (`10, 20, 30, 40, 50`) vào BST thường, cây sẽ bị lệch hẳn sang một bên và **suy biến thành Singly Linked List** với chiều cao $H = N \implies$ mọi thao tác bị chậm xuống **$O(N)$**. Cây tự cân bằng giữ cho chiều cao luôn xấp xỉ **$H = O(\log_2 N)$**:

### 2.1. Cây AVL (Cân bằng nghiêm ngặt — Hệ số cân bằng $|\text{BF}| \le 1$)

```text
  Ví dụ Lệch Phải - Phải (RR: BF(10) = -2) ===> Quay Trái Đơn (Single Left Rotation) tại [10]:
       [10] (BF = -2)                                  [20] (BF = 0)
           ╲               rotate_left(10)            ╱    ╲
           [20]            ==============►         [10]    [30]
               ╲
               [30]
```

* **Hệ số cân bằng (Balance Factor):** $\text{BF}(u) = \text{Height}(\text{Left}) - \text{Height}(\text{Right}) \in \{-1, 0, 1\}$.
* Khi sau thao tác chèn/xóa mà $|\text{BF}(u)| == 2$, cây AVL khôi phục cân bằng trong **$O(1)$** bằng **4 phép quay (Rotations)**:
  1. **Lệch Trái - Trái (LL — $\text{BF}(u) > 1$ và $\text{BF}(\text{left}) \ge 0$):** Quay phải đơn `rotate_right(u)`.
  2. **Lệch Phải - Phải (RR — $\text{BF}(u) < -1$ và $\text{BF}(\text{right}) \le 0$):** Quay trái đơn `rotate_left(u)`.
  3. **Lệch Trái - Phải (LR — $\text{BF}(u) > 1$ và $\text{BF}(\text{left}) < 0$):** Quay trái ở con trái `u->left = rotate_left(u->left)` $\rightarrow$ Quay phải ở `u`.
  4. **Lệch Phải - Trái (RL — $\text{BF}(u) < -1$ và $\text{BF}(\text{right}) > 0$):** Quay phải ở con phải `u->right = rotate_right(u->right)` $\rightarrow$ Quay trái ở `u`.

### 2.2. Cây Đỏ - Đen (Red-Black Tree — Nền tảng của `std::set` & `std::map`)

* Mỗi node mang thêm 1 bit màu (**Đỏ** hoặc **Đen**), đảm bảo đường đi dài nhất từ gốc xuống lá không bao giờ vượt quá **2 lần** đường đi ngắn nhất ($H \le 2 \log_2(N + 1)$).
* **So sánh thực chiến AVL vs Red-Black Tree:**
  * **AVL Tree:** Cân bằng chặt hơn nên **tra cứu (`search`) nhanh hơn** (phù hợp hệ thống đọc nhiều, ít ghi).
  * **Red-Black Tree:** Cân bằng lỏng hơn nhưng khi Chèn/Xóa chỉ tốn **tối đa 2–3 phép quay**, nên **chèn/xóa nhanh hơn AVL** $\rightarrow$ được C++ STL chọn làm cấu trúc lõi cho `std::set`, `std::map`, `std::multiset`, `std::multimap`.

---

## 3. Đống Nhị Phân (Binary Heap / Priority Queue) (`02_binary_heap_priority_queue.cpp`)

### 3.1. Ánh xạ Cây nhị phân hoàn chỉnh lên Mảng 1D (0 Con trỏ!)

Vì Binary Heap luôn được lấp đầy từ trên xuống dưới, từ trái sang phải, ta lưu trực tiếp trên mảng động 1D mà không tốn 16 bytes con trỏ cho mỗi node:

```text
            [0]: 90                 Với node tại chỉ số i (0-indexed):
           ╱       ╲                • Chỉ số nút Cha (Parent)  : (i - 1) / 2
      [1]: 70     [2]: 80           • Chỉ số Con Trái (Left)   : 2 * i + 1
      ╱     ╲     ╱                 • Chỉ số Con Phải (Right)  : 2 * i + 2
  [3]:30  [4]:50 [5]:60             ==> Mảng RAM: [ 90 | 70 | 80 | 30 | 50 | 60 ]
```

* **`push(val)` — $O(\log N)$:** Thêm vào cuối mảng `data[size++]`, rồi gọi **`sift_up()`** bơi ngược lên trên nếu lớn hơn cha (với Max-Heap).
* **`pop()` — $O(\log N)$:** Ghi đè phần tử cuối mảng lên đỉnh `data[0]`, giảm `--size`, rồi gọi **`sift_down()`** chìm xuống dưới để khôi phục tính chất Heap.
* **Floyd's Build Heap — $O(N)$:** Biến một mảng lộn xộn có sẵn thành Heap bằng cách gọi `sift_down()` từ node cha cuối cùng (`size / 2 - 1`) lùi về `0`. Độ phức tạp hội tụ về tuyến tính **$O(N)$** (nhanh hơn nhiều so với `push` từng phần tử tốn $O(N \log N)$).

---

## 4. Cây Tiền Tố (Trie / Prefix Tree) (`03_avl_tree_and_trie.cpp`)

* **Bản chất:** Là cây $K$-phân (với bảng chữ cái tiếng Anh thường là $K = 26$ nhánh `'a'..'z'`), trong đó mỗi cạnh đại diện cho 1 ký tự và mỗi đường đi từ gốc xuống một node đánh dấu `is_end_of_word = true` tạo thành một từ.
* **Ưu thế tuyệt đối trên bài toán chuỗi:**
  * Tìm kiếm một từ hoặc kiểm tra tiền tố (`starts_with(prefix)`) chỉ tốn **$O(L)$** (với $L$ là độ dài từ, độc lập với tổng số từ $N$ trong từ điển!).
  * Hỗ trợ **Gợi ý từ khóa tự động (Autocomplete)** theo thứ tự từ điển, kiểm tra chính tả, lọc từ khóa và bài toán **Maximum XOR Pair** (Bitwise Trie 0/1).

---

## 5. Bảng Độ Phức Tạp Tổng Hợp Các Cấu Trúc Cây

| Cấu trúc Cây | Tìm kiếm (Avg / Worst) | Chèn (Avg / Worst) | Xóa (Avg / Worst) | Lấy Min / Max | Duyệt có thứ tự |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **BST thường** | $O(\log N)$ / $O(N)$ | $O(\log N)$ / $O(N)$ | $O(\log N)$ / $O(N)$ | $O(\log N)$ / $O(N)$ | **Có** (In-order $O(N)$) |
| **AVL / Red-Black Tree** | **$O(\log N)$ / $O(\log N)$** | **$O(\log N)$ / $O(\log N)$** | **$O(\log N)$ / $O(\log N)$** | **$O(\log N)$** | **Có** (In-order $O(N)$) |
| **Binary Heap** | $O(N)$ | **$O(\log N)$** | **$O(\log N)$** *(đỉnh)* | **$O(1)$** (`top`) | Không |
| **Trie (Độ dài từ $L$)** | **$O(L)$** | **$O(L)$** | **$O(L)$** | — | **Có** (Thứ tự từ điển) |

---

## 6. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_binary_tree_traversals.cpp`: **Cây nhị phân (Binary Tree)** — Duyệt Pre/In/Post-order đệ quy, khử đệ quy bằng Stack, BFS Level-order theo tầng, tính Chiều cao & Đường kính cây (Diameter).
* `02_binary_search_tree_bst.cpp`: **Cây nhị phân tìm kiếm (BST)** — `insert`, `search`, `remove` đủ 3 trường hợp với In-order Successor, Validate BST và tìm LCA.
* `03_binary_heap_priority_queue.cpp`: **Đống nhị phân (`MaxHeap`)** trên mảng động 1D (`sift_up`, `sift_down`, `push`, `pop`, `top`, Floyd's Build Heap $O(N)$).
* `04_avl_self_balancing_tree.cpp`: **Cây tự cân bằng (`AVLTree`)** — Đủ 4 phép xoay LL, RR, LR, RL cho cả `insert` lẫn `remove` trong $O(\log N)$.
* `05_trie_prefix_tree.cpp`: **Cây tiền tố (`Trie`)** — `insert`, `search`, `startsWith`, gợi ý từ khóa `autocomplete` và xóa từ `removeWord` trong $O(L)$.
* `06_practice_exercises.cpp`: 5 bài tập kinh điển về Cây (Max Depth & Invert Binary Tree, Validate BST, Lowest Common Ancestor trong BST, Kth Largest bằng Min-Heap, Hệ thống Autocomplete gợi ý từ khóa bằng Trie).

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_binary_tree_traversals.cpp -o 01_binary_tree_traversals
./01_binary_tree_traversals

g++ -Wall -Wextra -std=c++17 02_binary_search_tree_bst.cpp -o 02_binary_search_tree_bst
./02_binary_search_tree_bst

g++ -Wall -Wextra -std=c++17 03_binary_heap_priority_queue.cpp -o 03_binary_heap_priority_queue
./03_binary_heap_priority_queue

g++ -Wall -Wextra -std=c++17 04_avl_self_balancing_tree.cpp -o 04_avl_self_balancing_tree
./04_avl_self_balancing_tree

g++ -Wall -Wextra -std=c++17 05_trie_prefix_tree.cpp -o 05_trie_prefix_tree
./05_trie_prefix_tree

g++ -Wall -Wextra -std=c++17 06_practice_exercises.cpp -o 06_practice_exercises
./06_practice_exercises
```

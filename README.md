# Roadmap C/C++

## Giai đoạn 1: Cơ sở lập trình (Nhập môn với C/C++ cơ bản)

*Mục tiêu: Hình thành tư duy lập trình (Programming Mindset), hiểu cách máy tính thực thi mã nguồn và nắm vững cú pháp nền tảng.*

1. **Môi trường & Quá trình biên dịch (Compilation Process):**
   * Cài đặt IDE/Compiler: GCC/MinGW, Clang, VS Code hoặc Visual Studio.
   * Hiểu 4 bước dịch mã nguồn: **Preprocessing** (Tiền xử lý `.i`) → **Compilation** (Biên dịch sang Assembly `.s`) → **Assembly** (Dịch sang mã máy `.o`/`.obj`) → **Linking** (Liên kết tạo file thực thi `.exe`).

2. **Biến, Kiểu dữ liệu & Toán tử:**
   * Kiểu dữ liệu nguyên thủy: `int`, `long long`, `float`, `double`, `char`, `bool` (C++), từ khóa `unsigned`, `const`, `sizeof()`.
   * Cách dữ liệu lưu trữ trên bộ nhớ: Hệ nhị phân (Binary), hệ thập lục phân (Hex), quy tắc bù 2 (Two's complement), tràn số (Overflow).
   * Nhập xuất chuẩn: `scanf`/`printf` (C) và `std::cin`/`std::cout` (C++).
   * Toán tử: Số học, quan hệ, logic (`&&`, `||`, `!`), toán tử 3 ngôi (`?:`), và **toán tử thao tác bit (Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`)**.

3. **Cấu trúc điều khiển (Control Flow):**
   * Rẽ nhánh: `if - else if - else`, `switch - case`.
   * Vòng lặp: `for`, `while`, `do - while`.
   * Điều khiển luồng: `break`, `continue`, `return`.

4. **Hàm (Functions) & Phạm vi biến:**
   * Khai báo (Prototype), định nghĩa (Definition) và lời gọi hàm.
   * Cơ chế truyền tham trị (Pass by value) và truyền tham chiếu (Pass by reference `&` trong C++).
   * Phạm vi biến: Biến cục bộ (Local), biến toàn cục (Global), biến tĩnh (`static`).
   * Nạp chồng hàm (Function Overloading - C++).

5. **Mảng 1 chiều & Mảng 2 chiều (Arrays):**
   * Khai báo, khởi tạo, truy xuất phần tử thông qua chỉ số (index).
   * Các bài toán kinh điển trên mảng: Duyệt mảng, tìm Min/Max, tính tổng, đếm tần suất, chèn/xóa phần tử, đảo ngược mảng.
   * Mảng 2 chiều (Ma trận): Duyệt hàng, cột, đường chéo chính/phụ, nhân hai ma trận.

> **Bài tập tổng kết Giai đoạn 1:** Viết các hàm kiểm tra số nguyên tố, số thuận nghịch, số Fibonacci; viết chương trình quản lý điểm số sinh viên bằng mảng 1 chiều và thực hiện các phép toán ma trận.

---

## Giai đoạn 2: Kỹ thuật lập trình (Con trỏ, Bộ nhớ & Đệ quy)

*Mục tiêu: Làm chủ "linh hồn" của C/C++ — hiểu sâu về quản lý bộ nhớ cấp thấp và cách tối ưu hóa mã nguồn.*

1. **Phân vùng bộ nhớ (Memory Layout):**
   * Hiểu rõ 5 phân vùng khi chương trình chạy: **Text/Code Segment** (mã máy), **Data Segment** (biến toàn cục/static đã khởi tạo), **BSS** (chưa khởi tạo), **Stack** (biến cục bộ, lời gọi hàm - cấp phát tĩnh), **Heap** (cấp phát động).

2. **Con trỏ (Pointers) & Tham chiếu (References):**
   * Bản chất địa chỉ bộ nhớ (`&`) và toán tử giải tham chiếu (`*`).
   * Số học con trỏ (Pointer Arithmetic: `ptr + 1`, `ptr - 1`), mối liên hệ mật thiết giữa Con trỏ và Mảng.
   * Con trỏ cấp 2 (`int **p`), con trỏ `void*`, con trỏ `NULL` / `nullptr` (C++11).
   * Con trỏ hàm (Function Pointers) và ứng dụng làm hàm Callback.
   * Các lỗi kinh điển cần tránh: Dangling Pointer (con trỏ treo), Wild Pointer (con trỏ hoang), Memory Leak (rò rỉ bộ nhớ).

3. **Cấp phát bộ nhớ động (Dynamic Memory Allocation):**
   * Trong C: `malloc()`, `calloc()`, `realloc()`, và giải phóng bằng `free()`.
   * Trong C++: Toán tử `new`, `new[]` và giải phóng bằng `delete`, `delete[]`.

4. **Chuỗi ký tự (Strings):**
   * Chuỗi kiểu C (C-style string): Mảng `char` kết thúc bằng ký tự `\0`, thư viện `<cstring>` (`strlen`, `strcpy`, `strcmp`, `strcat`, `strtok`).
   * Chuỗi trong C++ (`std::string`): Các phương thức xử lý chuỗi hiện đại (`substr`, `find`, `replace`, `stringstream`).

5. **Kiểu dữ liệu tự định nghĩa (User-defined Types):**
   * `struct` (Cấu trúc): Gom nhóm dữ liệu, căn chỉnh bộ nhớ (**Struct Alignment & Padding**).
   * `union` (Hợp) và `enum` (Liệt kê), từ khóa `typedef` / `using`.

6. **Kỹ thuật Đệ quy (Recursion) & Quay lui (Backtracking):**
   * Cơ chế Call Stack khi gọi đệ quy, điều kiện dừng (Base case) và bước đệ quy.
   * Đệ quy tuyến tính, đệ quy nhị phân, đệ quy đuôi (Tail Recursion).
   * Thuật toán Quay lui (Backtracking): Sinh hoán vị, tổ hợp, bài toán N-Queens (N quân hậu), giải Sudoku.

7. **Thao tác với Tập tin (File I/O):**
   * File văn bản (`.txt`, `.csv`) và File nhị phân (`.bin`, `.dat`).
   * Thư viện `<cstdio>` (`FILE*`, `fopen`, `fread`, `fwrite`, `fclose`) và `<fstream>` (`ifstream`, `ofstream`, `fstream`).

> **Bài tập tổng kết Giai đoạn 2:** Xây dựng chương trình Quản lý Thư viện hoặc Quản lý Sinh viên chạy trên Console, sử dụng mảng cấp phát động kết hợp `struct`, có chức năng lưu/đọc dữ liệu ra file nhị phân.

---

## Giai đoạn 3: Lập trình Hướng đối tượng với C++ (Object-Oriented Programming)

*Mục tiêu: Chuyển từ tư duy lập trình hướng thủ tục (Procedural) của C sang tư duy thiết kế hệ thống theo mô hình đối tượng của C++.*

1. **Class & Object (Lớp và Đối tượng):**
   * Thuộc tính (Attributes/Properties) và Phương thức (Methods/Behaviors).
   * Phạm vi truy cập (Access Modifiers): `public`, `private`, `protected`.
   * Hàm khởi tạo (Constructor: Default, Parameterized, Copy Constructor) và Hàm hủy (Destructor `~ClassName()`).
   * Con trỏ `this`, thành viên tĩnh (`static` member variables & functions), hàm bạn (`friend function`) và lớp bạn (`friend class`).

2. **4 Tính chất cốt lõi của OOP:**
   * **Đóng gói (Encapsulation):** Che giấu dữ liệu nội bộ, giao tiếp qua Getter/Setter.
   * **Kế thừa (Inheritance):** Lớp cơ sở (Base class) và lớp dẫn xuất (Derived class), đơn kế thừa, đa kế thừa (Multiple Inheritance) và cách giải quyết **Diamond Problem** (Kế thừa ảo `virtual inheritance`).
   * **Đa hình (Polymorphism):**
     * Đa hình lúc biên dịch (Compile-time): Function Overloading & **Operator Overloading** (Nạp chồng toán tử `+`, `-`, `<<`, `>>`, `[]`, `=`).
     * Đa hình lúc chạy (Runtime): Ghi đè phương thức (Overriding), phương thức ảo (`virtual function`), cơ chế **vtable (Virtual Table) và vptr** bên dưới bộ nhớ, **Virtual Destructor** (tránh rò rỉ bộ nhớ khi xóa qua con trỏ lớp cha).
   * **Trừu tượng (Abstraction):** Phương thức thuần ảo (`pure virtual function: virtual void func() = 0;`), Lớp trừu tượng (Abstract Class) và Giao diện (Interface).

3. **Quản lý tài nguyên trong Class:**
   * Shallow Copy (Sao chép nông) vs Deep Copy (Sao chép sâu).
   * Nguyên lý **Rule of Three** (Destructor, Copy Constructor, Copy Assignment Operator) và **Rule of Five** (bổ sung Move Constructor, Move Assignment).

> **Bài tập tổng kết Giai đoạn 3:** Thiết kế hệ thống tính lương nhân viên công ty (Nhân viên văn phòng, Kỹ sư, Quản lý) áp dụng Kế thừa, Đa hình, Abstract Class, hoặc tự viết lại lớp `MyString` / `MyVector` có nạp chồng toán tử đầy đủ.

---

## Giai đoạn 4: Cấu trúc dữ liệu và Giải thuật (DSA)

*Mục tiêu: Nền tảng quan trọng nhất để viết code hiệu năng cao và vượt qua các vòng phỏng vấn kỹ thuật (Coding Interview).*

1. **Độ phức tạp thuật toán**
   * Ký hiệu Big-O ($O(1)$, $O(\log n)$, $O(n)$, $O(n \log n)$, $O(n^2)$, $O(2^n)$).
   * Đánh giá Time Complexity (Thời gian) và Space Complexity (Không gian bộ nhớ).

2. **Giải thuật Sắp xếp & Tìm kiếm**
   * **Tìm kiếm:** Tuyến tính (Linear Search), Nhị phân (Binary Search trên mảng và trên kết quả).
   * **Sắp xếp cơ bản:** Selection, Insertion, Bubble Sort.
   * **Sắp xếp nâng cao:** Merge Sort, Quick Sort, Heap Sort, Counting Sort, Radix Sort.

3. **Cấu trúc dữ liệu tuyến tính**
   * **Danh sách liên kết (Linked List):** Đơn (Singly), Đôi (Doubly), Vòng (Circular).
   * **Ngăn xếp (Stack - LIFO):** Cài đặt bằng mảng và Linked List. Ứng dụng: Khử đệ quy, tính giá trị biểu thức hậu tố (Postfix), kiểm tra dấu ngoặc.
   * **Hàng đợi (Queue - FIFO):** Queue thường, Circular Queue, Deque (Hàng đợi 2 đầu).

4. **Cấu trúc Cây (Trees)**
   * **Cây nhị phân (Binary Tree):** Duyệt cây (Pre-order, In-order, Post-order, Level-order).
   * **Cây nhị phân tìm kiếm (BST):** Thêm, xóa, tìm kiếm node.
   * **Cây cân bằng:** Cây AVL (các phép quay đơn/kép), Red-Black Tree (hiểu nguyên lý).
   * **Heap (Đống):** Min-Heap, Max-Heap và cấu trúc Hàng đợi ưu tiên (Priority Queue).
   * **Cây tiền tố (Trie):** Ứng dụng tìm kiếm chuỗi nhanh, gợi ý từ khóa.

5. **Bảng băm (Hash Table)**
   * Hàm băm (Hash Function).
   * Xử lý đụng độ (Collision Handling) bằng phương pháp Chaining (Dây chuyền) và Open Addressing (Thăm dò tuyến tính/bậc hai).

6. **Lý thuyết Đồ thị (Graphs)**
   * **Biểu diễn đồ thị:** Ma trận kề, Danh sách kề, Danh sách cạnh.
   * **Duyệt đồ thị:** BFS (Chiều rộng), DFS (Chiều sâu).
   * **Đường đi ngắn nhất:** Dijkstra, Bellman-Ford, Floyd-Warshall.
   * **Cây khung nhỏ nhất (MST):** Prim, Kruskal (kết hợp cấu trúc Disjoint Set Union - DSU).
   * **Sắp xếp Tô-pô** (Topological Sort).

7. **Các chiến lược giải thuật**
   * **Hai con trỏ** (Two Pointers) & **Cửa sổ trượt** (Sliding Window).
   * **Chia để trị** (Divide and Conquer).
   * **Tham lam** (Greedy Algorithms).
   * **Quy hoạch động** (Dynamic Programming - DP): Memoization (Top-down) và Tabulation (Bottom-up) — Bài toán cái túi (Knapsack), Dãy con tăng dài nhất (LIS), Xâu con chung dài nhất (LCS).

---

## Giai đoạn 5: Lập trình Tổng quát (Templates) & Thư viện chuẩn STL

*Mục tiêu: Ngừng "phát minh lại bánh xe", sử dụng thành thạo công cụ có sẵn của C++ để tăng tốc độ code lên gấp nhiều lần.*

1. **Templates (Khuôn mẫu):**
   * Function Template và Class Template (viết code độc lập với kiểu dữ liệu).
   * Template Specialization (Chuyên biệt hóa) và Variadic Templates.

2. **Thư viện STL (Standard Template Library) — 4 thành phần cốt lõi:**
   * **Containers (Cấu trúc lưu trữ):**
     * *Sequence:* `std::vector`, `std::deque`, `std::list`, `std::forward_list`, `std::array`.
     * *Adapters:* `std::stack`, `std::queue`, `std::priority_queue`.
     * *Associative (Cây đỏ-đen - có thứ tự, $O(\log n)$):* `std::set`, `std::multiset`, `std::map`, `std::multimap`.
     * *Unordered Associative (Bảng băm - không thứ tự, trung bình $O(1)$):* `std::unordered_set`, `std::unordered_map`.

   * **Iterators (Bộ lặp):** `begin()`, `end()`, `rbegin()`, `rend()`, phân loại Random Access, Bidirectional, Forward Iterators.
   * **Algorithms (`<algorithm>` & `<numeric>`):** `std::sort`, `std::stable_sort`, `std::binary_search`, `std::lower_bound`, `std::upper_bound`, `std::find`, `std::count`, `std::accumulate`, `std::next_permutation`.
   * **Functors (Function Objects):** Đối tượng hàm và bộ so sánh tùy chỉnh (Custom Comparator cho `sort` và `priority_queue`).

---

## Giai đoạn 6: C++ Hiện đại (Modern C++: C++11 / C++14 / C++17 / C++20)

*Mục tiêu: Viết code C++ theo chuẩn công nghiệp hiện nay — an toàn bộ nhớ, hiệu năng tối đa và súc tích.*

1. **Tiện ích cú pháp & Suy luận kiểu:**
   * Từ khóa `auto`, `decltype`, vòng lặp Range-based `for (const auto& x : container)`.
   * Khởi tạo đồng nhất (Uniform Initialization `{}`), `nullptr`, `constexpr` (tính toán ngay lúc biên dịch).
   * Structured Bindings (C++17: `auto [key, val] = pair;`), `std::optional`, `std::variant`, `std::any`, `std::string_view`.

2. **Biểu thức Lambda (Lambda Expressions):**
   * Cú pháp `[capture](parameters) -> return_type { body }`, ứng dụng truyền hàm ẩn danh vào STL Algorithms.

3. **Quản lý bộ nhớ tự động (RAII & Smart Pointers - `<memory>`):**
   * Triết lý **RAII (Resource Acquisition Is Initialization)**: Gắn vòng đời tài nguyên với vòng đời đối tượng trên Stack.
   * `std::unique_ptr`: Quyền sở hữu độc quyền (không thể copy, chỉ có thể move).
   * `std::shared_ptr`: Quyền sở hữu chia sẻ (đếm tham chiếu - Reference Counting).
   * `std::weak_ptr`: Phá vỡ vòng lặp tham chiếu (Circular Reference) của `shared_ptr`.

4. **Move Semantics & Tối ưu hiệu năng:**
   * Phân biệt **Lvalue** (có định danh, lấy được địa chỉ) và **Rvalue** (giá trị tạm thời).
   * Tham chiếu Rvalue (`&&`), hàm `std::move()` và `std::forward()` (Perfect Forwarding).
   * **Move Constructor & Move Assignment Operator**: Chuyển quyền sở hữu tài nguyên thay vì sao chép tốn kém.

5. **Tính năng C++20 đáng chú ý:**
   * **Concepts:** Ràng buộc kiểu dữ liệu cho Template rõ ràng hơn.
   * **Ranges (`<ranges>`):** Kết hợp các thao tác xử lý tập dữ liệu dạng đường ống (Pipeline).
   * **Coroutines & Modules:** Lập trình bất đồng bộ và thay thế cơ chế `#include` truyền thống.

---

## Giai đoạn 7: Lập trình Hệ thống, Công cụ & Định hướng chuyên sâu

*Mục tiêu: Bước ra khỏi các bài toán trên giấy, làm việc với hệ thống đa luồng, công cụ quản lý dự án chuẩn doanh nghiệp.*

### 1. Lập trình Đa luồng & Đồng bộ hóa (Multithreading & Concurrency)

* Thư viện `<thread>`: Tạo và quản lý luồng (`join()`, `detach()`).
* Tránh xung đột dữ liệu (Race Condition) với `<mutex>`: `std::mutex`, `std::lock_guard`, `std::unique_lock`.
* Tránh khoá chết (Deadlock), sử dụng `std::condition_variable`, `std::atomic` và lập trình bất đồng bộ với `std::async`, `std::future`, `std::promise`.

### 2. Hệ sinh thái Công cụ (Tooling & Best Practices)

* **Build System:** Làm chủ **CMake** và **Makefile** để biên dịch dự án lớn nhiều thư mục.
* **Trình gỡ lỗi & Phân tích bộ nhớ:** Sử dụng **GDB** để debug, **Valgrind** hoặc **AddressSanitizer (ASan)** để phát hiện Memory Leak.
* **Kiểm thử & Quản lý mã nguồn:** Git/GitHub, viết Unit Test với **GoogleTest (GTest)**.
* **Mẫu thiết kế (Design Patterns - 22 mẫu GoF / Refactoring.Guru):**
  * **Creational Patterns (Nhóm Khởi tạo):** Abstract Factory, Builder, Factory Method, Prototype, Singleton.
  * **Structural Patterns (Nhóm Cấu trúc):** Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy.
  * **Behavioral Patterns (Nhóm Hành vi):** Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Template Method, Visitor.
* **Nguyên lý thiết kế SOLID:** Single Responsibility (SRP), Open/Closed (OCP), Liskov Substitution (LSP), Interface Segregation (ISP), Dependency Inversion (DIP).

### 3. Các hướng rẽ chuyên ngành sau khi hoàn thành

* **Game Development:** Học **Unreal Engine** (C++), hoặc thư viện đồ họa cấp thấp (SDL2, SFML, OpenGL, Vulkan, DirectX).
* **Embedded Systems & IoT (Nhúng & Vi điều khiển):** Quay lại đào sâu C/C++ cận phần cứng (STM32, ESP32, Arduino, RTOS, Linux Kernel, Device Driver).
* **High-Performance Computing / Backend / Finance:** Lập trình mạng với Socket (TCP/IP), thư viện Boost.Asio, gRPC, hệ thống giao dịch tần suất cao (HFT).
* **Desktop / Cross-platform App:** Học **Qt Framework**.
* **Computer Vision / AI Core:** Thư viện **OpenCV**, tối ưu hóa mô hình với CUDA C++, TensorRT.

---

## Tài liệu & Nền tảng luyện tập khuyên dùng

* **Sách gối đầu giường:**
* *C Programming: A Modern Approach* (K. N. King) — Chuẩn mực nhất cho ngôn ngữ C.
* *C++ Primer* (Stanley B. Lippman) — Sách toàn diện nhất cho C++ cơ bản đến trung cấp.
* *Effective Modern C++* (Scott Meyers) — Bắt buộc đọc để hiểu sâu C++11/14.

* **Website tham khảo chuẩn:** `learncpp.com` (Kênh tự học C++ miễn phí tốt nhất thế giới hiện tại) và `cppreference.com` (Từ điển tra cứu thư viện chuẩn).
* **Nền tảng luyện code DSA:** LeetCode, Codeforces, HackerRank, CSES Problem Set (rất tốt cho DSA bằng C++).

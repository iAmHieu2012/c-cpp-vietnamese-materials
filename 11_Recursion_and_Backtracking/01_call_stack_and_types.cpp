#include <iostream>

// 1. ĐỆ QUY TUYẾN TÍNH (Linear Recursion) - Có in truy vết quá trình Đẩy vào (Push) & Rút ra (Pop) Call Stack
long long factorial_trace(int n) {
    std::cout << "  [PUSH Stack] Goi factorial(" << n << ")\n";
    
    // Base Case (Điều kiện dừng)
    if (n <= 1) {
        std::cout << "  [BASE CASE]  factorial(" << n << ") tra ve 1\n";
        return 1;
    }

    // Recursive Step (Vẫn còn phép nhân 'n *' chờ đợi sau khi hàm con trả về -> Không phải Tail Recursion)
    long long res = n * factorial_trace(n - 1);
    std::cout << "  [POP Stack]  factorial(" << n << ") = " << n << " * factorial(" << n - 1 << ") = " << res << "\n";
    return res;
}

// 2. ĐỆ QUY ĐUÔI (Tail Recursion) - Không còn phép toán nào chờ sau lời gọi đệ quy
// Truyền kết quả tích lũy 'acc' đi theo từng bước gọi
long long factorial_tail(int n, long long acc = 1) {
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc); // Lệnh return gọi thẳng hàm, không làm gì thêm!
}

// 3. ĐỆ QUY NHỊ PHÂN (Binary Recursion) - Rẽ 2 nhánh ở mỗi bước: O(2^n)
long long fib_binary(int n) {
    if (n <= 1) return n;
    return fib_binary(n - 1) + fib_binary(n - 2);
}

int main() {
    std::cout << "=== 1. TRUY VET CALL STACK CUA DE QUY TUYEN TINH (4!) ===\n";
    long long f4 = factorial_trace(4);
    std::cout << "-> Ket qua 4! = " << f4 << "\n\n";

    std::cout << "=== 2. DE QUY DUOI (TAIL RECURSION) ===\n";
    std::cout << "-> factorial_tail(10) = " << factorial_tail(10) << "\n\n";

    std::cout << "=== 3. DE QUY NHI PHAN (BINARY RECURSION) ===\n";
    std::cout << "-> fib_binary(10) = " << fib_binary(10) << "\n";

    return 0;
}
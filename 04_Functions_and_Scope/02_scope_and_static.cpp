#include <iostream>

// Biến toàn cục (Global variable) - Nằm ở Data Segment, tồn tại suốt chương trình
int counter = 100; 

void demo_static_variable() {
    int normal_var = 0;        // Biến Local thường: Tạo mới và reset về 0 mỗi lần gọi hàm
    static int static_var = 0; // Biến Static Local: Chỉ khởi tạo = 0 ở LẦN GỌI ĐẦU TIÊN!

    ++normal_var;
    ++static_var;

    std::cout << "  normal_var = " << normal_var 
              << " | static_var = " << static_var << " (Nho gia tri cu!)\n";
}

int main() {
    // =========================================================================
    // 1. HIỆN TƯỢNG CHE KHUẤT TÊN (VARIABLE SHADOWING) & TOÁN TỬ PHẠM VI (::)
    // =========================================================================
    std::cout << "=== 1. VARIABLE SHADOWING & SCOPE RESOLUTION (::) ===\n";
    int counter = 10; // Biến Local trùng tên với biến Global

    std::cout << "Bien counter (Local trong main):  " << counter << "\n";
    // Dùng toán tử :: (Scope Resolution Operator) của C++ để gọi đích danh biến Global
    std::cout << "Bien ::counter (Global ben ngoai): " << ::counter << "\n\n";

    { // Một khối lệnh (Block scope) con bên trong main
        int counter = 1;
        std::cout << "Bien counter (Trong khoi { } con): " << counter << "\n";
        std::cout << "Bien ::counter (Global):           " << ::counter << "\n\n";
    } // Ra khỏi dấu '}' này, biến counter = 1 bị hủy ngay lập tức!


    // =========================================================================
    // 2. BIẾN TĨNH CỤC BỘ (STATIC LOCAL VARIABLE)
    // =========================================================================
    std::cout << "=== 2. BIEN STATIC TRONG HAM ===\n";
    demo_static_variable(); // Lần gọi 1
    demo_static_variable(); // Lần gọi 2
    demo_static_variable(); // Lần gọi 3

    return 0;
}
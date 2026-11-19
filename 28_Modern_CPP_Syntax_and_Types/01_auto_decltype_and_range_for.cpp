// ============================================================================
// File: 01_auto_decltype_and_range_for.cpp
// Bài 6.1: Từ khóa auto, decltype & Vòng lặp Range-based for (const auto& x : c)
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

// Hàm Template dùng auto + decltype để suy luận kiểu trả về của phép cộng
template <typename T, typename U>
auto addValues(T a, U b) -> decltype(a + b) {
    return a + b;
}

int main() {
    std::cout << "=== 1. SUY LUAN KIEU AUTO & VONG LAP RANGE-BASED FOR ===\n";
    std::vector<std::string> langs = {"C++11", "C++14", "C++17"};

    // 1. Dùng auto& khi muốn sửa trực tiếp phần tử gốc trong container
    for (auto &s : langs) {
        s += " (Modern)";
    }

    // 2. Dùng const auto& khi chỉ đọc để đạt Zero-Copy + An toàn tuyệt đối
    std::cout << "  Danh sach sau khi sua qua auto&: ";
    for (const auto &s : langs) {
        std::cout << "[" << s << "] ";
    }
    std::cout << "\n\n";

    std::cout << "=== 2. TU KHOA DECLTYPE (SUY LUAN KIEU BIEU THUC LUC BIEN DICH) ===\n";
    int sample = 100;
    decltype(sample) clonedVar = 250; // clonedVar mang đúng kiểu int của sample
    std::cout << "  Bien khai bao bang decltype(sample): " << clonedVar << "\n";

    auto sumResult = addValues(10, 3.14159); // int + double -> double
    std::cout << "  addValues(10, 3.14159) = " << sumResult
              << " (sizeof = " << sizeof(sumResult) << " bytes)\n";

    return 0;
}

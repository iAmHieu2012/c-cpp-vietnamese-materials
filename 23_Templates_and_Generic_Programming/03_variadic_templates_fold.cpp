#include <iostream>
#include <string>
#include <type_traits>

// =============================================================================
// 1. VARIADIC TEMPLATES + C++17 FOLD EXPRESSIONS
// =============================================================================

// Cộng tổng số lượng tham số bất kỳ (hỗ trợ cả trường hợp 0 tham số nhờ Binary Fold `+ 0`)
template <typename... Args>
auto sum_all(Args... args) {
    return (args + ... + 0);
}

// Kiểm tra tất cả các điều kiện truyền vào có đồng thời đúng không
template <typename... Args>
bool all_true(Args... args) {
    return (... && args);
}

// In số lượng tham số bất kỳ với kiểu dữ liệu hỗn hợp trên 1 dòng
template <typename... Args>
void print_line(const Args&... args) {
    std::cout << "  [Log] ";
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

// =============================================================================
// 2. COMPILE-TIME BRANCHING VỚI IF CONSTEXPR & <TYPE_TRAITS> (C++17)
// =============================================================================
template <typename T>
std::string inspect_value(const T &val) {
    if constexpr (std::is_same_v<T, bool>) {
        return std::string("Boolean: ") + (val ? "true" : "false");
    } else if constexpr (std::is_integral_v<T>) {
        return "Integer (" + std::to_string(sizeof(T)) + "B): " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "Float (" + std::to_string(sizeof(T)) + "B): " + std::to_string(val);
    } else {
        return "String/Object: " + std::string(val);
    }
}

int main() {
    std::cout << "=== 1. VARIADIC TEMPLATES & FOLD EXPRESSIONS (C++17) ===\n";
    std::cout << "  sum_all()                 = " << sum_all() << "\n";
    std::cout << "  sum_all(10, 20, 30, 40)   = " << sum_all(10, 20, 30, 40) << "\n";
    std::cout << "  sum_all(1, 2.5, 3, 4.25)  = " << sum_all(1, 2.5, 3, 4.25) << "\n";
    std::cout << "  all_true(true, 5 > 2, 1)  = " << (all_true(true, 5 > 2, 1) ? "True" : "False") << "\n";

    print_line("User:", "Nguyen Van A", "| ID:", 1001, "| GPA:", 3.85);
    std::cout << "\n";

    std::cout << "=== 2. IF CONSTEXPR & TYPE_TRAITS (RE NHANH LUC BIEN DICH) ===\n";
    std::cout << "  " << inspect_value(42) << "\n";
    std::cout << "  " << inspect_value(3.14159) << "\n";
    std::cout << "  " << inspect_value(true) << "\n";
    std::cout << "  " << inspect_value("Modern C++17") << "\n";

    return 0;
}

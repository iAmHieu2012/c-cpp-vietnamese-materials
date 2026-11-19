#include <iostream>
#include <concepts>
#include <compare> // Toán tử 3 chiều <=> (Spaceship operator)
#include <string>
#include <vector>

// =============================================================================
// 1. C++20 CONCEPTS: RÀNG BUỘC KIỂU DỮ LIỆU RÕ RÀNG CHO TEMPLATE
// =============================================================================
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Concept kiểm tra một kiểu T có hỗ trợ toán tử xuất `std::cout << x` hay không
template <typename T>
concept Printable = requires(T x) {
    { std::cout << x } -> std::same_as<std::ostream&>;
};

// Cú pháp rút gọn C++20 (Abbreviated Function Template với Concept)
Numeric auto compute_average(Numeric auto a, Numeric auto b) {
    return (a + b) / 2.0;
}

template <Printable T>
void log_item(const T &item) {
    std::cout << "  [Printable Concept] " << item << "\n";
}

// =============================================================================
// 2. TOÁN TỬ SO SÁNH 3 CHIỀU SPACESHIP (<=>) - TỰ SINH 6 PHÉP SO SÁNH
// =============================================================================
struct AppVersion {
    int major;
    int minor;
    int patch;

    // Chỉ cần 1 dòng duy nhất: Trình biên dịch tự sinh ==, !=, <, <=, >, >=
    auto operator<=>(const AppVersion &other) const = default;
};

int main() {
    std::cout << "=== 1. C++20 CONCEPTS (NUMERIC & PRINTABLE) ===\n";
    std::cout << "  compute_average(10, 25)     = " << compute_average(10, 25) << "\n";
    std::cout << "  compute_average(3.5, 8.5)   = " << compute_average(3.5, 8.5) << "\n";
    // compute_average("abc", "def"); // LỖI BIÊN DỊCH RÕ RÀNG: const char* không thỏa mãn concept 'Numeric'!

    log_item(2026);
    log_item("C++20 Concepts Rock!");

    std::cout << "\n=== 2. SPACESHIP OPERATOR (<=>) TU SINH 6 TOAN TU SO SANH ===\n";
    AppVersion v1{2, 4, 1};
    AppVersion v2{2, 5, 0};
    AppVersion v3{2, 4, 1};

    std::cout << "  v1 (2.4.1) <  v2 (2.5.0)? " << (v1 < v2  ? "True" : "False") << "\n";
    std::cout << "  v2 (2.5.0) >  v1 (2.4.1)? " << (v2 > v1  ? "True" : "False") << "\n";
    std::cout << "  v1 (2.4.1) == v3 (2.4.1)? " << (v1 == v3 ? "True" : "False") << "\n";
    std::cout << "  v1 (2.4.1) != v2 (2.5.0)? " << (v1 != v2 ? "True" : "False") << "\n";

    return 0;
}

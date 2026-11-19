// ============================================================================
// File: 02_uniform_init_nullptr_constexpr.cpp
// Bài 6.1: Uniform Initialization {}, nullptr & constexpr (Tính lúc biên dịch)
// ============================================================================

#include <iostream>
#include <type_traits>
#include <vector>

// ============================================================================
// 1. CONSTEXPR FUNCTION: TÍNH TOÁN NGAY LÚC BIÊN DỊCH (COMPILE-TIME)
// ============================================================================
constexpr long long compileTimeFactorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

// ============================================================================
// 2. PHÂN BIỆT QUÁ TẢI HÀM VỚI NULLPTR VS SỐ NGUYÊN 0
// ============================================================================
void checkPointer(int x) {
    std::cout << "  Goi checkPointer(int) voi so nguyen: " << x << "\n";
}

void checkPointer(int *p) {
    std::cout << "  Goi checkPointer(int*) voi con tro : " << p << "\n";
}

int main() {
    std::cout << "=== 1. UNIFORM INITIALIZATION {} (CHONG NARROWING CONVERSION) ===\n";
    int safeInt{42}; // Nếu viết int bad{3.14}; trình biên dịch sẽ chặn lỗi ngay!
    std::vector<int> primes{2, 3, 5, 7, 11};
    std::cout << "  safeInt{42} = " << safeInt
              << " | primes.size() = " << primes.size() << "\n\n";

    std::cout << "=== 2. TU KHOA NULLPTR (THAY THE MACRO NULL / 0) ===\n";
    checkPointer(0);       // Gọi bản int
    checkPointer(nullptr); // Luôn gọi đúng bản con trỏ int*!

    std::cout << "\n=== 3. CONSTEXPR & STATIC_ASSERT (TINH NGAY LUC BIEN DICH) ===\n";
    constexpr long long fact10 = compileTimeFactorial(10);
    static_assert(fact10 == 3628800LL, "Loi tinh giai thua!");
    std::cout << "  constexpr 10! (da tinh san trong ma may) = " << fact10 << "\n";

    return 0;
}

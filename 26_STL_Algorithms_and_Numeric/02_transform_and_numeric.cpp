#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    std::cout << "=== 1. STD::IOTA, STD::ACCUMULATE & STD::GCD / STD::LCM (C++17) ===\n";
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 1); // Điền tự động: 1, 2, 3, ..., 10

    // Truyền 0LL để ép kiểu tích lũy là long long (chống tràn int!)
    long long sum = std::accumulate(v.begin(), v.end(), 0LL);
    long long prod = std::accumulate(v.begin(), v.begin() + 5, 1LL, std::multiplies<long long>());
    std::cout << "  Tong 1..10 = " << sum << " | Tich 1..5 (5!) = " << prod << "\n";
    std::cout << "  std::gcd(48, 18) = " << std::gcd(48, 18)
              << " | std::lcm(12, 18) = " << std::lcm(12, 18) << "\n\n";

    std::cout << "=== 2. STD::PARTIAL_SUM (PREFIX SUM) & STD::INNER_PRODUCT ===\n";
    std::vector<int> prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());
    std::cout << "  Prefix sum: ";
    for (int x : prefix) std::cout << x << " ";
    std::cout << "\n";

    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    long long dot = std::inner_product(a.begin(), a.end(), b.begin(), 0LL);
    std::cout << "  Tich vo huong {1,2,3} . {4,5,6} = " << dot << "\n\n";

    std::cout << "=== 3. STD::TRANSFORM, STD::CLAMP & PREDICATES (ALL_OF / ANY_OF / COUNT_IF) ===\n";
    std::vector<int> squared(v.size());
    std::transform(v.begin(), v.end(), squared.begin(), [](int x) {
        return std::clamp(x * x, 0, 60); // Bình phương và kẹp trần tối đa ở 60
    });

    std::cout << "  Binh phuong (clamp <= 60): ";
    for (int x : squared) std::cout << x << " ";
    std::cout << "\n";

    int even_cnt = static_cast<int>(std::count_if(squared.begin(), squared.end(),
                                                  [](int x) { return x % 2 == 0; }));
    bool all_pos = std::all_of(squared.begin(), squared.end(), [](int x) { return x > 0; });
    bool any_eq60 = std::any_of(squared.begin(), squared.end(), [](int x) { return x == 60; });

    std::cout << "  So luong phan tu chan : " << even_cnt << "\n";
    std::cout << "  Tat ca deu duong?     : " << (all_pos ? "Co" : "Khong") << "\n";
    std::cout << "  Co so nao cham tran 60: " << (any_eq60 ? "Co" : "Khong") << "\n";

    return 0;
}

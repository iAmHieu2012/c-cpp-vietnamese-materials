#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

using Clock = std::chrono::high_resolution_clock;

// 1. O(1) - Hằng số: Không phụ thuộc kích thước N
long long sum_o1(long long n) {
    return n * (n + 1) / 2;
}

// 2. O(log N) - Logarit: Chia đôi không gian tìm kiếm mỗi bước
long long count_halvings_ologn(long long n) {
    long long steps = 0;
    while (n > 1) {
        n /= 2;
        ++steps;
    }
    return steps;
}

// 3. O(N) - Tuyến tính: Duyệt 1 vòng lặp
long long sum_on(long long n) {
    long long s = 0;
    for (long long i = 1; i <= n; ++i) s += i;
    return s;
}

// 4. O(N log N) - Tuyến tính nhân Logarit (chuẩn của Merge Sort / Quick Sort)
long long work_onlogn(int n) {
    long long ops = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = n; j > 1; j /= 2) {
            ++ops;
        }
    }
    return ops;
}

// 5. O(N^2) - Bậc hai: Hai vòng lặp lồng nhau
long long work_on2(int n) {
    long long ops = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ++ops;
        }
    }
    return ops;
}

// 6. O(2^N) - Hàm mũ: Đệ quy nhị phân không nhớ (Fibonacci thuần)
long long fib_o2n(int n) {
    if (n <= 1) return n;
    return fib_o2n(n - 1) + fib_o2n(n - 2);
}

int main() {
    auto measure = [](const char *label, auto func) {
        auto start = Clock::now();
        long long res = func();
        auto end = Clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  " << std::left << std::setw(28) << label
                  << " | Ket qua: " << std::setw(14) << res
                  << " | Thoi gian: " << us << " us\n";
    };

    std::cout << "=== BENCHMARK THOI GIAN THUC TE CÁC BAC BIG-O ===\n";
    measure("O(1)       N = 10,000,000", []() { return sum_o1(10000000); });
    measure("O(log N)   N = 10,000,000", []() { return count_halvings_ologn(10000000); });
    measure("O(N)       N = 10,000,000", []() { return sum_on(10000000); });
    measure("O(N log N) N = 200,000",    []() { return work_onlogn(200000); });
    measure("O(N^2)     N = 10,000",     []() { return work_on2(10000); });
    measure("O(2^N)     N = 38",         []() { return fib_o2n(38); });

    return 0;
}
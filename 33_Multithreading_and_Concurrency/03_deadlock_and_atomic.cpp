// ============================================================================
// File: 03_deadlock_and_atomic.cpp
// Bài 7.1: Phòng chống Deadlock (std::scoped_lock) & Biến nguyên tử Lock-Free (<atomic>)
// ============================================================================

#include <atomic>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// ============================================================================
// 1. PHÒNG CHỐNG DEADLOCK KHI KHÓA 2 MUTEX BẰNG STD::SCOPED_LOCK (C++17)
// ============================================================================
struct BankAccount {
    std::string owner;
    long long balance;
    std::mutex mtx;

    BankAccount(std::string name, long long initBal)
        : owner(std::move(name)), balance(initBal) {}
};

void safeTransfer(BankAccount &from, BankAccount &to, long long amount) {
    if (&from == &to) return;
    // std::scoped_lock khóa đồng thời cả 2 mutex bằng thuật toán chống Deadlock tuyệt đối
    std::scoped_lock lock(from.mtx, to.mtx);
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
    }
}

int main() {
    std::cout << "=== 1. CHONG DEADLOCK KHI CHUYEN TIEN CHEO BANG STD::SCOPED_LOCK ===\n";
    BankAccount accA("An", 10000);
    BankAccount accB("Binh", 10000);

    std::thread t1([&]() {
        for (int i = 0; i < 10000; ++i) safeTransfer(accA, accB, 1);
    });
    std::thread t2([&]() {
        for (int i = 0; i < 10000; ++i) safeTransfer(accB, accA, 2);
    });

    t1.join();
    t2.join();
    std::cout << "  So du sau 20,000 giao dich cheo: An = " << accA.balance
              << " | Binh = " << accB.balance
              << " (Tong bao toan = " << (accA.balance + accB.balance) << ")\n\n";

    std::cout << "=== 2. BIEN NGUYEN TU LOCK-FREE STD::ATOMIC & COMPARE-AND-SWAP ===\n";
    std::atomic<long long> atomicCounter{0};
    std::atomic<int> peakValue{0};

    auto updatePeakLockFree = [&](int candidate) {
        int currentPeak = peakValue.load();
        while (currentPeak < candidate &&
               !peakValue.compare_exchange_weak(currentPeak, candidate)) {
        }
    };

    std::vector<std::thread> pool;
    for (int id = 1; id <= 4; ++id) {
        pool.emplace_back([&, id]() {
            for (int i = 0; i < 50000; ++i) {
                atomicCounter.fetch_add(1, std::memory_order_relaxed);
            }
            updatePeakLockFree(id * 250);
        });
    }
    for (auto &t : pool) t.join();

    std::cout << "  Atomic Counter (4 x 50,000) = " << atomicCounter.load() << "\n";
    std::cout << "  Atomic Peak Value (CAS)     = " << peakValue.load() << "\n";

    return 0;
}

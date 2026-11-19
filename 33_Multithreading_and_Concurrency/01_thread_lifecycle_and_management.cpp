// ============================================================================
// File: 01_thread_lifecycle_and_management.cpp
// Bài 7.1: Thư viện <thread> (Tạo luồng, join, detach, std::ref & RAII ScopedThread)
// ============================================================================

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

// Hàm nhận tham chiếu: khi gọi qua std::thread bắt buộc bọc đối số bằng std::ref()!
void computeChunkByRef(int workerId, int &sharedOutput) {
    sharedOutput += workerId * 100;
}

// ============================================================================
// LỚP BỌC RAII CHO STD::THREAD (TỰ ĐỘNG JOIN TRONG DESTRUCTOR, CHỐNG STD::TERMINATE)
// ============================================================================
class ScopedThread {
    std::thread th_;

public:
    explicit ScopedThread(std::thread t) : th_(std::move(t)) {}

    ~ScopedThread() {
        if (th_.joinable()) {
            th_.join();
        }
    }

    ScopedThread(const ScopedThread &) = delete;
    ScopedThread &operator=(const ScopedThread &) = delete;
};

int main() {
    std::cout << "=== 1. THONG TIN PHAN CUNG & VONG DOI STD::THREAD ===\n";
    std::cout << "  So luong luong phan cung (hardware_concurrency) = "
              << std::thread::hardware_concurrency() << " threads\n";
    std::cout << "  ID cua Main Thread = " << std::this_thread::get_id() << "\n\n";

    std::cout << "=== 2. TRUYEN THAM CHIEU VOI STD::REF & GOI JOIN() ===\n";
    int result1 = 0, result2 = 0;
    std::thread t1(computeChunkByRef, 1, std::ref(result1));
    std::thread t2(computeChunkByRef, 2, std::ref(result2));

    t1.join();
    t2.join();
    std::cout << "  Ket qua sau khi t1, t2 join(): result1 = " << result1
              << ", result2 = " << result2 << "\n\n";

    std::cout << "=== 3. RAII SCOPED_THREAD (TU DONG JOIN KHI RA KHOI SCOPE) ===\n";
    int autoJoinedResult = 0;
    {
        ScopedThread safeWorker(
            std::thread([&autoJoinedResult]() { autoJoinedResult = 42; }));
    } // safeWorker ra khỏi khối {} -> Tự động gọi join() an toàn!
    std::cout << "  Gia tri autoJoinedResult sau khi thoat scope = "
              << autoJoinedResult << "\n";

    return 0;
}

// ============================================================================
// File: 02_mutex_and_lock_guards.cpp
// Bài 7.1: Tránh Race Condition với <mutex> (std::mutex, lock_guard, unique_lock)
// ============================================================================

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class SharedAuditLog {
    std::mutex mtx_;
    std::vector<std::string> entries_;

public:
    // 1. Dùng std::lock_guard khi cần khóa trọn vẹn cả hàm
    void appendSimple(const std::string &msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        entries_.push_back(msg);
    }

    // 2. Dùng std::unique_lock khi muốn chủ động unlock() sớm sau khi xong Critical Section
    void formatAndAppend(int workerId, int taskNum) {
        // Bước chuẩn bị chuỗi không chạm vào dữ liệu chung -> Không cần khóa Mutex
        std::string formatted = "Worker#" + std::to_string(workerId) +
                                "_Task#" + std::to_string(taskNum);

        std::unique_lock<std::mutex> ulock(mtx_);
        entries_.push_back(formatted);
        ulock.unlock(); // Mở khóa sớm ngay khi ghi xong để luồng khác vào ngay!
    }

    size_t size() const { return entries_.size(); }
};

int main() {
    std::cout << "=== 1. RACE CONDITION VS STD::LOCK_GUARD<STD::MUTEX> ===\n";
    const int NUM_THREADS = 4;
    const int INCREMENTS = 50000;

    int unsafeCounter = 0;
    int safeMutexCounter = 0;
    std::mutex counterMtx;

    std::vector<std::thread> workers;
    for (int i = 0; i < NUM_THREADS; ++i) {
        workers.emplace_back([&]() {
            for (int step = 0; step < INCREMENTS; ++step) {
                ++unsafeCounter; // Data Race!
                {
                    std::lock_guard<std::mutex> lk(counterMtx);
                    ++safeMutexCounter;
                }
            }
        });
    }
    for (auto &t : workers) t.join();

    std::cout << "  Ky vong chinh xac              : " << (NUM_THREADS * INCREMENTS) << "\n";
    std::cout << "  Unsafe Counter (Bi Data Race!) : " << unsafeCounter << "\n";
    std::cout << "  Safe Mutex Counter (lock_guard): " << safeMutexCounter << "\n\n";

    std::cout << "=== 2. LINH HOAT MO KHOA SOM VOI STD::UNIQUE_LOCK ===\n";
    SharedAuditLog auditLog;
    std::vector<std::thread> logWorkers;
    for (int w = 1; w <= 4; ++w) {
        logWorkers.emplace_back([w, &auditLog]() {
            for (int task = 1; task <= 250; ++task) {
                auditLog.formatAndAppend(w, task);
            }
        });
    }
    for (auto &t : logWorkers) t.join();
    std::cout << "  Tong so ban ghi AuditLog an toan = " << auditLog.size()
              << " (Ky vong: 1000)\n";

    return 0;
}

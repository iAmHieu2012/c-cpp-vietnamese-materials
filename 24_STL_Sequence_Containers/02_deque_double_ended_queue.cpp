// ============================================================================
// File: 02_deque_double_ended_queue.cpp
// Bài 5.2: std::deque (Hàng đợi 2 đầu: Map of Chunks, O(1) 2 đầu + Random Access)
// ============================================================================

#include <deque>
#include <iostream>
#include <string>

int main() {
    std::cout << "=== 1. STD::DEQUE (O(1) PUSH/POP O CA 2 DAU + RANDOM ACCESS) ===\n";
    std::deque<int> dq = {20, 30};

    dq.push_front(10); // O(1) - std::vector không có push_front()!
    dq.push_back(40);  // O(1)
    dq.push_front(5);  // O(1)

    std::cout << "  Truy cap chi so ngau nhien dq[i] trong O(1): ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << "\n";

    dq.pop_front(); // Xóa 5 ở đầu trong O(1)
    dq.pop_back();  // Xóa 40 ở cuối trong O(1)
    std::cout << "  Sau khi pop_front() va pop_back(): front = "
              << dq.front() << ", back = " << dq.back() << "\n\n";

    std::cout << "=== 2. UNG DUNG DEQUE LAM BO DEM LICH SU CO GIOI HAN (BOUNDED LOG) ===\n";
    std::deque<std::string> recentLogs;
    const size_t MAX_LOGS = 3;

    auto addLog = [&](const std::string &entry) {
        if (recentLogs.size() == MAX_LOGS) {
            recentLogs.pop_front(); // Đẩy bản ghi cũ nhất ra khỏi đầu hàng đợi trong O(1)
        }
        recentLogs.push_back(entry);
    };

    addLog("10:00 - Server Booted");
    addLog("10:01 - DB Connected");
    addLog("10:02 - User Login");
    addLog("10:03 - Payment Processed"); // Tự động đẩy "10:00 - Server Booted" ra ngoài

    for (const auto &log : recentLogs) {
        std::cout << "  * " << log << "\n";
    }

    return 0;
}

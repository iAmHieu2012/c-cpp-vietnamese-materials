// ============================================================================
// File: 04_condition_variable_producer_consumer.cpp
// Bài 7.1: Đồng bộ hóa Luồng với std::condition_variable (Mô hình Producer - Consumer)
// ============================================================================

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main() {
    std::cout << "=== DONG BO HOA PRODUCER - CONSUMER VOI STD::CONDITION_VARIABLE ===\n\n";

    std::queue<int> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

    // Luồng Producer: Sản xuất 5 gói tin rồi thông báo kết thúc
    std::thread producer([&]() {
        for (int item = 1; item <= 5; ++item) {
            {
                std::lock_guard<std::mutex> lk(mtx);
                buffer.push(item * 10);
                std::cout << "  [Producer] Da day goi tin: " << (item * 10) << "\n";
            }
            cv.notify_one(); // Đánh thức Consumer đang ngủ dậy xử lý
        }
        {
            std::lock_guard<std::mutex> lk(mtx);
            finished = true;
        }
        cv.notify_all();
    });

    // Luồng Consumer: Ngủ chờ trên condition_variable (0% CPU) cho tới khi có dữ liệu
    std::thread consumer([&]() {
        while (true) {
            std::unique_lock<std::mutex> lk(mtx);
            // Tham số thứ 2 là Predicate chống đánh thức giả (Spurious Wakeup)
            cv.wait(lk, [&]() { return !buffer.empty() || finished; });

            while (!buffer.empty()) {
                int val = buffer.front();
                buffer.pop();
                std::cout << "    -> [Consumer] Da xu ly goi tin: " << val << "\n";
            }
            if (finished && buffer.empty()) break;
        }
    });

    producer.join();
    consumer.join();

    return 0;
}

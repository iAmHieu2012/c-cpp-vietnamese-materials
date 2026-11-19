#include <iostream>
#include <future> // std::async, std::future, std::promise
#include <thread>
#include <numeric>
#include <vector>
#include <stdexcept>

// Hàm tính tổng đoạn [L, R] trả về kết quả trực tiếp qua std::future
long long range_sum_worker(long long L, long long R) {
    long long s = 0;
    for (long long i = L; i <= R; ++i) s += i;
    return s;
}

int main() {
    std::cout << "=== 1. TINH TOAN BAT DONG BO SONG SONG VOI STD::ASYNC & STD::FUTURE ===\n";
    // std::launch::async ép hệ thống chạy ngay trên luồng riêng biệt
    std::future<long long> fut1 = std::async(std::launch::async, range_sum_worker, 1, 500000);
    std::future<long long> fut2 = std::async(std::launch::async, range_sum_worker, 500001, 1000000);

    // Luồng main có thể làm việc khác trong lúc fut1, fut2 đang tính trên các nhân CPU khác
    // Khi gọi .get(), nếu luồng con chưa xong thì main sẽ đợi và nhận giá trị trả về
    long long total = fut1.get() + fut2.get();
    std::cout << "  Tong 1..1,000,000 (chia 2 luong async) = " << total << "\n\n";

    std::cout << "=== 2. TRUYEN DU LIEU & BAT EXCEPTION QUA CAP STD::PROMISE - STD::FUTURE ===\n";
    std::promise<std::string> msg_promise;
    std::future<std::string>  msg_future = msg_promise.get_future();

    std::thread worker([](std::promise<std::string> p) {
        // Đẩy kết quả vào đầu promise -> đầu future ở luồng main sẽ nhận được ngay lập tức!
        p.set_value("Du lieu da tai xong tu Worker Thread!");
    }, std::move(msg_promise));

    std::cout << "  [Main nhan tu future.get()]: " << msg_future.get() << "\n";
    worker.join();

    // Minh họa std::async tự động bắt và chuyển tiếp Exception từ luồng con về luồng main khi gọi .get()
    auto failing_future = std::async(std::launch::async, []() -> int {
        throw std::runtime_error("Loi chia cho 0 trong luong Async!");
    });

    try {
        failing_future.get();
    } catch (const std::exception &e) {
        std::cout << "  [Bat Exception tu luong con an toan qua future.get()]: " << e.what() << "\n";
    }

    return 0;
}

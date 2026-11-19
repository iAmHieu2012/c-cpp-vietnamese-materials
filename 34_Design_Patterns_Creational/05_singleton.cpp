#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

// =============================================================================
// SINGLETON PATTERN (THREAD-SAFE MEYERS' SINGLETON CHUẨN C++11/17)
// =============================================================================
class DatabaseConfig {
private:
    std::string connection_url;
    int max_connections;
    mutable std::mutex mtx;

    // 1. Đặt Constructor vào private để cấm bên ngoài tự gọi 'new' hoặc khai báo biến
    DatabaseConfig() : connection_url("postgres://localhost:5432/prod"), max_connections(50) {
        std::cout << "  [Singleton Constructor] Khoi tao DatabaseConfig DUY NHAT 1 LAN!\n";
    }

public:
    // 2. Cấm tuyệt đối Copy và Move
    DatabaseConfig(const DatabaseConfig&) = delete;
    DatabaseConfig& operator=(const DatabaseConfig&) = delete;

    // 3. Điểm truy cập toàn cục: C++11 đảm bảo khởi tạo static local là Thread-Safe 100%!
    static DatabaseConfig& get_instance() {
        static DatabaseConfig instance;
        return instance;
    }

    void set_max_connections(int n) {
        std::lock_guard<std::mutex> lk(mtx);
        max_connections = n;
    }

    void print_status(const char *caller) const {
        std::lock_guard<std::mutex> lk(mtx);
        std::cout << "  [" << caller << " | Addr: " << this << "] URL = " << connection_url
                  << " | MaxConn = " << max_connections << "\n";
    }
};

int main() {
    std::cout << "=== SINGLETON PATTERN (MEYERS' THREAD-SAFE SINGLETON) ===\n";
    std::vector<std::thread> threads;

    // Cho 4 luồng cùng gọi get_instance() đồng thời -> Constructor vẫn chỉ chạy ĐÚNG 1 LẦN!
    for (int i = 1; i <= 4; ++i) {
        threads.emplace_back([i]() {
            DatabaseConfig &cfg = DatabaseConfig::get_instance();
            if (i == 2) cfg.set_max_connections(200);
            std::string label = "Thread #" + std::to_string(i);
            cfg.print_status(label.c_str());
        });
    }

    for (auto &t : threads) t.join();

    return 0;
}

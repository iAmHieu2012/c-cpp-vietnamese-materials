// ============================================================================
// File: 02_unique_ptr_exclusive_ownership.cpp
// Bài 6.3: std::unique_ptr (Quyền sở hữu độc quyền, Move-only, Custom Deleter)
// ============================================================================

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

class ServerConnection {
    std::string endpoint_;

public:
    explicit ServerConnection(std::string ep) : endpoint_(std::move(ep)) {
        std::cout << "  [+] Mo ket noi toi: " << endpoint_ << "\n";
    }
    ~ServerConnection() {
        std::cout << "  [-] Tu dong dong ket noi (RAII): " << endpoint_ << "\n";
    }
    void sendPing() const {
        std::cout << "  [~] Sending PING to " << endpoint_ << "...\n";
    }
};

// Hàm nhận quyền sở hữu độc quyền từ nơi gọi (Sink Function)
void transferOwnership(std::unique_ptr<ServerConnection> conn) {
    std::cout << "  [Trong ham transferOwnership] Da nhan quyen so huu:\n";
    conn->sendPing();
}

int main() {
    std::cout << "=== 1. STD::UNIQUE_PTR & STD::MAKE_UNIQUE (ZERO-OVERHEAD) ===\n";
    std::cout << "  sizeof(ServerConnection*)                = "
              << sizeof(ServerConnection *) << " bytes\n";
    std::cout << "  sizeof(std::unique_ptr<ServerConnection>) = "
              << sizeof(std::unique_ptr<ServerConnection>)
              << " bytes (0 byte chi phi phu!)\n\n";

    {
        auto p1 = std::make_unique<ServerConnection>("192.168.1.10:8080");
        p1->sendPing();

        // Chuyển giao quyền sở hữu từ p1 sang hàm transferOwnership bằng std::move
        transferOwnership(std::move(p1));

        std::cout << "  Sau khi std::move(p1), p1 == nullptr? "
                  << (p1 == nullptr ? "Dung (p1 da rong)" : "Sai") << "\n";
    }

    std::cout << "\n=== 2. UNIQUE_PTR QUAN LY MANG DONG & CUSTOM DELETER ===\n";
    auto arr = std::make_unique<int[]>(4);
    for (int i = 0; i < 4; ++i) arr[i] = (i + 1) * 100;
    std::cout << "  Mang unique_ptr<int[]>: "
              << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << "\n";

    auto fileCloser = [](std::FILE *fp) {
        if (fp) {
            std::fclose(fp);
            std::cout << "  [Custom Deleter] Da tu dong fclose(FILE*) an toan!\n";
        }
    };
    {
        std::unique_ptr<std::FILE, decltype(fileCloser)> fp(
            std::fopen("smart_test.txt", "w"), fileCloser);
        if (fp) {
            std::fprintf(fp.get(), "Hello from std::unique_ptr Custom Deleter!\n");
        }
    }
    std::remove("smart_test.txt");

    return 0;
}

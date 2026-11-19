// ============================================================================
// File: 05_dependency_inversion_dip.cpp
// Bài 7.5: SOLID Principles - [D] Dependency Inversion Principle (Đảo ngược Phụ thuộc)
// 1. High-level modules should not depend on low-level modules. Both should depend on abstractions.
// 2. Abstractions should not depend on details. Details should depend on abstractions.
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// 1. VI PHẠM DIP (Module cấp cao khởi tạo cứng đối tượng cấp thấp bên trong)
// ============================================================================
namespace BadDIP {

class MySqlDatabase {
public:
    void insertOrder(const std::string &orderId) {
        std::cout << "  [MySQL Hardcoded] Saved order " << orderId << "\n";
    }
};

class SmtpEmailSender {
public:
    void sendEmail(const std::string &msg) {
        std::cout << "  [SMTP Hardcoded] Sent email: " << msg << "\n";
    }
};

// OrderService bị dính chặt (Tight Coupling) vào MySqlDatabase và SmtpEmailSender.
// Không thể viết Unit Test offline, không thể đổi sang PostgreSQL hay SMS/Telegram!
class RigidOrderService {
    MySqlDatabase db_;
    SmtpEmailSender mailer_;

public:
    void checkout(const std::string &orderId) {
        db_.insertOrder(orderId);
        mailer_.sendEmail("Order " + orderId + " confirmed!");
    }
};

} // namespace BadDIP

// ============================================================================
// 2. TUÂN THỦ DIP (Cả High-Level và Low-Level cùng phụ thuộc vào Abstraction)
// ============================================================================
namespace GoodDIP {

// --- ABSTRACTIONS (Interfaces do tầng Nghiệp vụ cấp cao định nghĩa) ---
class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual void saveOrder(const std::string &orderId, double amount) = 0;
};

class INotificationSender {
public:
    virtual ~INotificationSender() = default;
    virtual void notifyCustomer(const std::string &recipient,
                                const std::string &message) = 0;
};

// --- HIGH-LEVEL MODULE (Nhận phụ thuộc qua Constructor Dependency Injection) ---
class OrderCheckoutUseCase {
    std::shared_ptr<IOrderRepository> repo_;
    std::shared_ptr<INotificationSender> notifier_;

public:
    OrderCheckoutUseCase(std::shared_ptr<IOrderRepository> repo,
                         std::shared_ptr<INotificationSender> notifier)
        : repo_(std::move(repo)), notifier_(std::move(notifier)) {}

    void processOrder(const std::string &orderId,
                      const std::string &customerContact,
                      double amount) {
        std::cout << "  [CheckoutUseCase] Dang xu ly don hang " << orderId << "...\n";
        repo_->saveOrder(orderId, amount);
        notifier_->notifyCustomer(customerContact,
                                  "Don hang " + orderId + " ($" +
                                      std::to_string(static_cast<int>(amount)) +
                                      ") da thanh toan thanh cong!");
    }
};

// --- LOW-LEVEL MODULES (Các cài đặt chi tiết cho Production & Unit Testing) ---
class PostgresOrderRepository : public IOrderRepository {
public:
    void saveOrder(const std::string &orderId, double amount) override {
        std::cout << "    -> [PostgreSQL Repo] INSERT order='" << orderId
                  << "', amount=$" << amount << "\n";
    }
};

class TelegramBotNotifier : public INotificationSender {
public:
    void notifyCustomer(const std::string &recipient,
                        const std::string &message) override {
        std::cout << "    -> [Telegram Bot -> " << recipient << "] \"" << message << "\"\n";
    }
};

// Mock / Fake dùng để chạy Unit Test cực nhanh trong RAM mà không cần DB thật hay Internet
class InMemoryMockRepository : public IOrderRepository {
public:
    int savedCount = 0;
    void saveOrder(const std::string &orderId, double) override {
        ++savedCount;
        std::cout << "    -> [InMemory Mock DB] Ghi nhan vao RAM don hang: " << orderId << "\n";
    }
};

class SilentMockNotifier : public INotificationSender {
public:
    void notifyCustomer(const std::string &recipient,
                        const std::string &message) override {
        std::cout << "    -> [Mock Notifier] Gia lap gui tin toi " << recipient
                  << " (" << message.size() << " chars)\n";
    }
};

} // namespace GoodDIP

int main() {
    std::cout << "=== [D] DEPENDENCY INVERSION PRINCIPLE (DIP & DI) ===\n\n";

    std::cout << "--- 1. Truoc khi Refactor (Tight Coupling) ---\n";
    BadDIP::RigidOrderService badService;
    badService.checkout("ORD-LEGACY-01");

    std::cout << "\n--- 2. Sau khi Refactor: Cau hinh Production (PostgreSQL + Telegram) ---\n";
    auto prodRepo = std::make_shared<GoodDIP::PostgresOrderRepository>();
    auto prodNotifier = std::make_shared<GoodDIP::TelegramBotNotifier>();
    GoodDIP::OrderCheckoutUseCase prodCheckout(prodRepo, prodNotifier);
    prodCheckout.processOrder("ORD-PROD-101", "@iamhieu2012", 450.0);

    std::cout << "\n--- 3. Sau khi Refactor: Cau hinh Unit Test (In-Memory Mock) ---\n";
    auto mockRepo = std::make_shared<GoodDIP::InMemoryMockRepository>();
    auto mockNotifier = std::make_shared<GoodDIP::SilentMockNotifier>();
    GoodDIP::OrderCheckoutUseCase testCheckout(mockRepo, mockNotifier);
    testCheckout.processOrder("ORD-TEST-999", "test_user", 99.0);
    std::cout << "  [Unit Test Assert] So don hang da luu trong Mock = "
              << mockRepo->savedCount << " (PASS!)\n";

    return 0;
}

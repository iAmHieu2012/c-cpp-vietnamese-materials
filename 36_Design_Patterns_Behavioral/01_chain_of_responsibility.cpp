// ============================================================================
// File: 01_chain_of_responsibility.cpp
// Bài 7.4: Behavioral Design Patterns - Chain of Responsibility (Chuỗi trách nhiệm)
// Tham khảo: https://refactoring.guru/design-patterns/chain-of-responsibility
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// 1. ĐỐI TƯỢNG YÊU CẦU (HTTP Request)
// ============================================================================
struct HttpRequest {
    std::string endpoint;
    std::string authToken;
    std::string role;
    int requestsPerMinute;
    std::string payload;
};

// ============================================================================
// 2. HANDLER INTERFACE & BASE HANDLER
// ============================================================================
class MiddlewareHandler {
protected:
    std::shared_ptr<MiddlewareHandler> nextHandler_;

public:
    virtual ~MiddlewareHandler() = default;

    // Trả về chính handler tiếp theo để nối chuỗi dạng: h1->setNext(h2)->setNext(h3)
    std::shared_ptr<MiddlewareHandler> setNext(std::shared_ptr<MiddlewareHandler> next) {
        nextHandler_ = std::move(next);
        return nextHandler_;
    }

    virtual bool handle(const HttpRequest &req) {
        if (nextHandler_) {
            return nextHandler_->handle(req);
        }
        return true; // Đã đi qua toàn bộ chuỗi thành công
    }
};

// ============================================================================
// 3. CONCRETE HANDLERS (Các mắt xích xử lý cụ thể)
// ============================================================================

// Mắt xích 1: Kiểm tra giới hạn tần suất (Rate Limiting / Anti-DDoS)
class RateLimitMiddleware : public MiddlewareHandler {
    int maxRpm_;

public:
    explicit RateLimitMiddleware(int maxRpm = 60) : maxRpm_(maxRpm) {}

    bool handle(const HttpRequest &req) override {
        if (req.requestsPerMinute > maxRpm_) {
            std::cout << "  [RateLimit] TU CHOI (429 Too Many Requests): "
                      << req.requestsPerMinute << " req/min vuot nguong "
                      << maxRpm_ << "!\n";
            return false;
        }
        std::cout << "  [RateLimit] PASS (" << req.requestsPerMinute << " req/min).\n";
        return MiddlewareHandler::handle(req);
    }
};

// Mắt xích 2: Xác thực danh tính (Authentication)
class AuthMiddleware : public MiddlewareHandler {
public:
    bool handle(const HttpRequest &req) override {
        if (req.authToken.empty() || req.authToken.find("Bearer ") != 0) {
            std::cout << "  [Auth] TU CHOI (401 Unauthorized): Token khong hop le!\n";
            return false;
        }
        std::cout << "  [Auth] PASS (Token hop le).\n";
        return MiddlewareHandler::handle(req);
    }
};

// Mắt xích 3: Phân quyền (Role-Based Authorization)
class RoleAuthorizationMiddleware : public MiddlewareHandler {
public:
    bool handle(const HttpRequest &req) override {
        if (req.endpoint.find("/admin") == 0 && req.role != "ADMIN") {
            std::cout << "  [RoleAuth] TU CHOI (403 Forbidden): Endpoint '"
                      << req.endpoint << "' yeu cau quyen ADMIN (hien tai: "
                      << req.role << ")!\n";
            return false;
        }
        std::cout << "  [RoleAuth] PASS (Role: " << req.role << ").\n";
        return MiddlewareHandler::handle(req);
    }
};

// Mắt xích 4: Kiểm tra an toàn dữ liệu đầu vào (SQL Injection / XSS Sanitizer)
class PayloadValidationMiddleware : public MiddlewareHandler {
public:
    bool handle(const HttpRequest &req) override {
        if (req.payload.find("DROP TABLE") != std::string::npos ||
            req.payload.find("<script>") != std::string::npos) {
            std::cout << "  [PayloadCheck] TU CHOI (400 Bad Request): Phat hien ma doc trong payload!\n";
            return false;
        }
        std::cout << "  [PayloadCheck] PASS (Payload an toan).\n";
        return MiddlewareHandler::handle(req);
    }
};

void processRequest(MiddlewareHandler &chain, const HttpRequest &req) {
    std::cout << "\n---> Nhan Request toi: " << req.endpoint << "\n";
    if (chain.handle(req)) {
        std::cout << "===> [Controller] XU LY THANH CONG (200 OK) cho " << req.endpoint << "!\n";
    } else {
        std::cout << "===> [Controller] YEU CAU BI CHAN BOI MIDDLEWARE CHAIN!\n";
    }
}

int main() {
    std::cout << "=== CHAIN OF RESPONSIBILITY PATTERN (HTTP MIDDLEWARE PIPELINE) ===\n";

    // Khởi tạo các mắt xích
    auto rateLimit = std::make_shared<RateLimitMiddleware>(100);
    auto auth = std::make_shared<AuthMiddleware>();
    auto roleAuth = std::make_shared<RoleAuthorizationMiddleware>();
    auto payloadCheck = std::make_shared<PayloadValidationMiddleware>();

    // Nối chuỗi: RateLimit -> Auth -> RoleAuth -> PayloadCheck
    rateLimit->setNext(auth)->setNext(roleAuth)->setNext(payloadCheck);

    // Kịch bản 1: Request hợp lệ hoàn toàn
    processRequest(*rateLimit, {"/admin/users", "Bearer jwt_token_123", "ADMIN", 25, "{\"name\":\"Hieu\"}"});

    // Kịch bản 2: Bị chặn ngay ở mắt xích 1 (Rate Limit)
    processRequest(*rateLimit, {"/api/orders", "Bearer jwt_token_123", "USER", 250, "{}"});

    // Kịch bản 3: Bị chặn ở mắt xích 3 (User thường cố truy cập /admin)
    processRequest(*rateLimit, {"/admin/config", "Bearer jwt_token_456", "USER", 10, "{}"});

    // Kịch bản 4: Bị chặn ở mắt xích 4 (Payload chứa SQL Injection)
    processRequest(*rateLimit, {"/api/search", "Bearer jwt_token_789", "USER", 15, "q=1; DROP TABLE users;--"});

    return 0;
}

// ============================================================================
// File: 07_state.cpp
// Bài 7.4: Behavioral Design Patterns - State Pattern (Máy trạng thái)
// Tham khảo: https://refactoring.guru/design-patterns/state
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

class OrderContext;

// ============================================================================
// 1. STATE INTERFACE (Giao diện chung cho mọi trạng thái của Đơn hàng)
// ============================================================================
class IOrderState {
public:
    virtual ~IOrderState() = default;
    virtual std::string getName() const = 0;
    virtual void payOrder(OrderContext &order) = 0;
    virtual void shipOrder(OrderContext &order) = 0;
    virtual void deliverOrder(OrderContext &order) = 0;
    virtual void cancelOrder(OrderContext &order) = 0;
};

// ============================================================================
// 2. CONTEXT (Đối tượng Đơn hàng ủy quyền hành vi cho State hiện tại)
// ============================================================================
class OrderContext {
    std::string orderId_;
    std::unique_ptr<IOrderState> currentState_;

public:
    OrderContext(std::string id, std::unique_ptr<IOrderState> initialState)
        : orderId_(std::move(id)), currentState_(std::move(initialState)) {
        std::cout << "[Order " << orderId_ << "] Khoi tao o trang thai: ["
                  << currentState_->getName() << "]\n";
    }

    void transitionTo(std::unique_ptr<IOrderState> newState) {
        std::cout << "  => [Order " << orderId_ << "] Chuyen trang thai: ["
                  << currentState_->getName() << "] ---> ["
                  << newState->getName() << "]\n";
        currentState_ = std::move(newState);
    }

    void pay() { currentState_->payOrder(*this); }
    void ship() { currentState_->shipOrder(*this); }
    void deliver() { currentState_->deliverOrder(*this); }
    void cancel() { currentState_->cancelOrder(*this); }
};

// ============================================================================
// 3. CONCRETE STATES (Các trạng thái cụ thể tự quyết định hành vi & chuyển tiếp)
// ============================================================================
class CancelledState : public IOrderState {
public:
    std::string getName() const override { return "CANCELLED"; }
    void payOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang da huy, khong the thanh toan!\n";
    }
    void shipOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang da huy, khong the giao hang!\n";
    }
    void deliverOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang da huy!\n";
    }
    void cancelOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang da o trang thai CANCELLED roi.\n";
    }
};

class DeliveredState : public IOrderState {
public:
    std::string getName() const override { return "DELIVERED"; }
    void payOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang da thanh toan va giao thanh cong.\n";
    }
    void shipOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang da giao toi tay khach hang.\n";
    }
    void deliverOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang da hoan tat giao nhan.\n";
    }
    void cancelOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang da giao thanh cong, khong the huy (vui long tao yeu cau Doi/Tra)!\n";
    }
};

class ShippedState : public IOrderState {
public:
    std::string getName() const override { return "SHIPPED"; }
    void payOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang da thanh toan truoc khi giao.\n";
    }
    void shipOrder(OrderContext &) override {
        std::cout << "  [Thong bao] Don hang dang tren duong van chuyen.\n";
    }
    void deliverOrder(OrderContext &order) override {
        std::cout << "  [Shipper] Khach hang da ky nhan kien hang!\n";
        order.transitionTo(std::make_unique<DeliveredState>());
    }
    void cancelOrder(OrderContext &) override {
        std::cout << "  [Loi] Hang da xuat kho giao cho Shipper, khong the huy ngang!\n";
    }
};

class PaidState : public IOrderState {
public:
    std::string getName() const override { return "PAID"; }
    void payOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang da duoc thanh toan roi (tranh tru tien 2 lan)!\n";
    }
    void shipOrder(OrderContext &order) override {
        std::cout << "  [Kho hang] Dong goi xong va ban giao cho don vi van chuyen.\n";
        order.transitionTo(std::make_unique<ShippedState>());
    }
    void deliverOrder(OrderContext &) override {
        std::cout << "  [Loi] Chua xuat kho (SHIPPED) thi chua the xac nhan giao thanh cong!\n";
    }
    void cancelOrder(OrderContext &order) override {
        std::cout << "  [He thong] Huy don va tu dong hoan tien (Refund) ve the cho khach!\n";
        order.transitionTo(std::make_unique<CancelledState>());
    }
};

class PendingPaymentState : public IOrderState {
public:
    std::string getName() const override { return "PENDING_PAYMENT"; }
    void payOrder(OrderContext &order) override {
        std::cout << "  [Cong thanh toan] Xac nhan thanh toan thanh cong!\n";
        order.transitionTo(std::make_unique<PaidState>());
    }
    void shipOrder(OrderContext &) override {
        std::cout << "  [Loi] Khong the xuat kho khi khach chua thanh toan!\n";
    }
    void deliverOrder(OrderContext &) override {
        std::cout << "  [Loi] Don hang chua thanh toan va chua van chuyen!\n";
    }
    void cancelOrder(OrderContext &order) override {
        std::cout << "  [He thong] Huy don hang chua thanh toan.\n";
        order.transitionTo(std::make_unique<CancelledState>());
    }
};

int main() {
    std::cout << "=== STATE PATTERN (MAY TRANG THAI DON HANG E-COMMERCE) ===\n\n";

    OrderContext order1("ORD-2026-01", std::make_unique<PendingPaymentState>());
    order1.ship();    // Bị chặn vì chưa thanh toán
    order1.pay();     // Chuyển sang PAID
    order1.pay();     // Chống thanh toán đúp
    order1.ship();    // Chuyển sang SHIPPED
    order1.cancel();  // Bị chặn vì đang vận chuyển
    order1.deliver(); // Chuyển sang DELIVERED

    std::cout << "\n----------------------------------------------------------\n";
    OrderContext order2("ORD-2026-02", std::make_unique<PendingPaymentState>());
    order2.pay();    // Chuyển sang PAID
    order2.cancel(); // Hủy khi mới PAID -> Hoàn tiền và chuyển sang CANCELLED
    order2.ship();   // Bị chặn vì đơn đã CANCELLED

    return 0;
}

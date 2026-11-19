// ============================================================================
// File: 11_practice_exercises.cpp
// Bài 7.4: Bài tập thực hành tổng hợp - Nhóm Mẫu Hành Vi (Behavioral Patterns)
// ============================================================================

#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// BÀI TẬP 1: Chain of Responsibility - Phê duyệt chi phí doanh nghiệp
// ============================================================================
class ApprovalHandler {
protected:
    std::shared_ptr<ApprovalHandler> next_;

public:
    virtual ~ApprovalHandler() = default;
    std::shared_ptr<ApprovalHandler> setNext(std::shared_ptr<ApprovalHandler> next) {
        next_ = std::move(next);
        return next_;
    }
    virtual void approveExpense(const std::string &item, double amountUSD) = 0;
};

class TeamLeadApprover : public ApprovalHandler {
public:
    void approveExpense(const std::string &item, double amountUSD) override {
        if (amountUSD <= 500.0) {
            std::cout << "  [Team Lead] PHE DUYET '" << item << "' ($" << amountUSD << ").\n";
        } else if (next_) {
            std::cout << "  [Team Lead] Vuot han muc $500 -> Chuyen len Department Manager...\n";
            next_->approveExpense(item, amountUSD);
        }
    }
};

class ManagerApprover : public ApprovalHandler {
public:
    void approveExpense(const std::string &item, double amountUSD) override {
        if (amountUSD <= 5000.0) {
            std::cout << "  [Dept Manager] PHE DUYET '" << item << "' ($" << amountUSD << ").\n";
        } else if (next_) {
            std::cout << "  [Dept Manager] Vuot han muc $5,000 -> Chuyen len CEO...\n";
            next_->approveExpense(item, amountUSD);
        }
    }
};

class CeoApprover : public ApprovalHandler {
public:
    void approveExpense(const std::string &item, double amountUSD) override {
        if (amountUSD <= 50000.0) {
            std::cout << "  [CEO] PHE DUYET '" << item << "' ($" << amountUSD << ").\n";
        } else {
            std::cout << "  [Board of Directors] Yeu cau '" << item << "' ($" << amountUSD
                      << ") can hop Dai hoi co dong!\n";
        }
    }
};

void exercise1() {
    std::cout << "\n--- BAI 1: CHAIN OF RESPONSIBILITY (PHE DUYET NGAN SACH) ---\n";
    auto lead = std::make_shared<TeamLeadApprover>();
    auto manager = std::make_shared<ManagerApprover>();
    auto ceo = std::make_shared<CeoApprover>();
    lead->setNext(manager)->setNext(ceo);

    lead->approveExpense("Ban phim co cho Dev", 180.0);
    lead->approveExpense("Cluster GPU Cloud 1 thang", 3200.0);
    lead->approveExpense("Hop dong ban quyen Enterprise", 28000.0);
    lead->approveExpense("Mo chi nhanh moi", 150000.0);
}

// ============================================================================
// BÀI TẬP 2: Command Pattern - Điều khiển Smart Home có Undo
// ============================================================================
class ISmartCommand {
public:
    virtual ~ISmartCommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class AirConditioner {
    int temp_ = 26;

public:
    int getTemp() const { return temp_; }
    void setTemp(int t) {
        temp_ = t;
        std::cout << "  [Dieu hoa] Nhiet do hien tai: " << temp_ << " C\n";
    }
};

class SetTemperatureCommand : public ISmartCommand {
    AirConditioner &ac_;
    int prevTemp_;
    int newTemp_;

public:
    SetTemperatureCommand(AirConditioner &ac, int target)
        : ac_(ac), prevTemp_(ac.getTemp()), newTemp_(target) {}

    void execute() override {
        prevTemp_ = ac_.getTemp();
        ac_.setTemp(newTemp_);
    }

    void undo() override {
        std::cout << "  [Undo Dieu hoa] Khoi phuc nhiet do cu...\n";
        ac_.setTemp(prevTemp_);
    }
};

void exercise2() {
    std::cout << "\n--- BAI 2: COMMAND PATTERN (SMART HOME REMOTE UNDO) ---\n";
    AirConditioner livingRoomAC;
    std::vector<std::unique_ptr<ISmartCommand>> history;

    auto runCmd = [&](std::unique_ptr<ISmartCommand> cmd) {
        cmd->execute();
        history.push_back(std::move(cmd));
    };

    runCmd(std::make_unique<SetTemperatureCommand>(livingRoomAC, 22));
    runCmd(std::make_unique<SetTemperatureCommand>(livingRoomAC, 18));

    if (!history.empty()) {
        history.back()->undo();
        history.pop_back();
    }
}

// ============================================================================
// BÀI TẬP 3: Strategy Pattern - Chiến lược chiết khấu giỏ hàng (Pricing Engine)
// ============================================================================
class IDiscountStrategy {
public:
    virtual ~IDiscountStrategy() = default;
    virtual double applyDiscount(double subtotal) const = 0;
    virtual std::string getName() const = 0;
};

class VipPercentageDiscount : public IDiscountStrategy {
    double percent_;

public:
    explicit VipPercentageDiscount(double pct) : percent_(pct) {}
    double applyDiscount(double subtotal) const override {
        return subtotal * (1.0 - percent_ / 100.0);
    }
    std::string getName() const override {
        return "VIP Discount (-" + std::to_string(static_cast<int>(percent_)) + "%)";
    }
};

class FlashSaleThresholdDiscount : public IDiscountStrategy {
public:
    double applyDiscount(double subtotal) const override {
        return (subtotal >= 1000.0) ? (subtotal - 250.0) : subtotal;
    }
    std::string getName() const override {
        return "Flash Sale (Giam $250 cho don tu $1000)";
    }
};

void exercise3() {
    std::cout << "\n--- BAI 3: STRATEGY PATTERN (CHIEN LUOC GIA GIO HANG) ---\n";
    double cartTotal = 1200.0;
    std::vector<std::unique_ptr<IDiscountStrategy>> strategies;
    strategies.push_back(std::make_unique<VipPercentageDiscount>(15.0));
    strategies.push_back(std::make_unique<FlashSaleThresholdDiscount>());

    std::cout << "  Tong tien goc: $" << cartTotal << "\n";
    for (const auto &strat : strategies) {
        std::cout << "  + Ap dung [" << strat->getName() << "] -> Con lai: $"
                  << std::fixed << std::setprecision(2)
                  << strat->applyDiscount(cartTotal) << "\n";
    }
}

// ============================================================================
// BÀI TẬP 4: State Pattern - Máy bán nước tự động (Vending Machine)
// ============================================================================
class VendingMachine {
    enum class State { NO_COIN, HAS_COIN, SOLD_OUT };
    State state_ = State::NO_COIN;
    int stock_ = 1;

public:
    void insertCoin() {
        if (state_ == State::NO_COIN) {
            std::cout << "  [Vending] Da nhan xu. Vui long bam nut chon nuoc.\n";
            state_ = State::HAS_COIN;
        } else if (state_ == State::HAS_COIN) {
            std::cout << "  [Vending] Ban da bo xu roi!\n";
        } else {
            std::cout << "  [Vending] May da het hang, tra lai xu!\n";
        }
    }

    void pressButton() {
        if (state_ == State::HAS_COIN) {
            std::cout << "  [Vending] Dang nha 1 lon Coca-Cola...\n";
            --stock_;
            state_ = (stock_ > 0) ? State::NO_COIN : State::SOLD_OUT;
        } else if (state_ == State::NO_COIN) {
            std::cout << "  [Vending] Vui long bo xu truoc khi bam nut!\n";
        } else {
            std::cout << "  [Vending] May da het hang (SOLD_OUT)!\n";
        }
    }
};

void exercise4() {
    std::cout << "\n--- BAI 4: STATE PATTERN (MAY BAN NUOC TU DONG) ---\n";
    VendingMachine vm;
    vm.pressButton(); // Chưa bỏ xu
    vm.insertCoin();  // Bỏ xu
    vm.pressButton(); // Nhả lon cuối cùng -> SOLD_OUT
    vm.insertCoin();  // Bị từ chối vì đã hết hàng
}

// ============================================================================
// BÀI TẬP 5: Observer + Mediator - Event Bus thông báo đa kênh
// ============================================================================
class EventBus {
    std::vector<std::function<void(const std::string &)>> listeners_;

public:
    void subscribe(std::function<void(const std::string &)> callback) {
        listeners_.push_back(std::move(callback));
    }

    void publish(const std::string &eventPayload) const {
        std::cout << "  [EventBus] Phat su kien: \"" << eventPayload << "\"\n";
        for (const auto &cb : listeners_) {
            cb(eventPayload);
        }
    }
};

void exercise5() {
    std::cout << "\n--- BAI 5: MODERN C++ EVENT BUS (OBSERVER + LAMBDA) ---\n";
    EventBus bus;
    bus.subscribe([](const std::string &ev) {
        std::cout << "    -> [EmailService] Gui email xac nhan cho su kien: " << ev << "\n";
    });
    bus.subscribe([](const std::string &ev) {
        std::cout << "    -> [AuditLogger] Ghi log he thong: " << ev << "\n";
    });

    bus.publish("USER_REGISTERED: iamhieu2012@example.com");
}

// ============================================================================
// MENU CHÍNH (do-while + switch-case)
// ============================================================================
int main() {
    int choice = -1;
    do {
        std::cout << "\n=========================================================\n"
                  << "   BAI TAP THUC HANH: BEHAVIORAL DESIGN PATTERNS (7.4)\n"
                  << "=========================================================\n"
                  << "  1. Chain of Responsibility (Phe duyet ngan sach 3 cap)\n"
                  << "  2. Command Pattern (Dieu khien Dieu hoa & Undo)\n"
                  << "  3. Strategy Pattern (Chien luoc tinh gia & chiet khau)\n"
                  << "  4. State Pattern (May ban nuoc tu dong)\n"
                  << "  5. Observer Pattern (Modern C++ EventBus voi Lambda)\n"
                  << "  6. Chay tat ca bai tap (1 -> 5)\n"
                  << "  0. Thoat\n"
                  << "---------------------------------------------------------\n"
                  << "Nhap lua chon cua ban: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[Canh bao] Dau vao khong hop le. Ket thuc chuong trinh.\n";
            break;
        }

        switch (choice) {
            case 1: exercise1(); break;
            case 2: exercise2(); break;
            case 3: exercise3(); break;
            case 4: exercise4(); break;
            case 5: exercise5(); break;
            case 6:
                exercise1();
                exercise2();
                exercise3();
                exercise4();
                exercise5();
                break;
            case 0:
                std::cout << "Tam biet!\n";
                break;
            default:
                std::cout << "Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

// ============================================================================
// File: 06_observer.cpp
// Bài 7.4: Behavioral Design Patterns - Observer Pattern (Pub-Sub / Người quan sát)
// Tham khảo: https://refactoring.guru/design-patterns/observer
// ============================================================================

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// 1. OBSERVER INTERFACE (Giao diện cho các bên đăng ký nhận thông báo)
// ============================================================================
class IStockObserver {
public:
    virtual ~IStockObserver() = default;
    virtual void onPriceUpdated(const std::string &symbol, double newPrice) = 0;
};

// ============================================================================
// 2. SUBJECT / PUBLISHER (Nguồn phát sự kiện sử dụng std::weak_ptr chống Dangling)
// ============================================================================
class StockTickerSubject {
    std::string symbol_;
    double price_;
    std::vector<std::weak_ptr<IStockObserver>> subscribers_;

public:
    StockTickerSubject(std::string symbol, double initialPrice)
        : symbol_(std::move(symbol)), price_(initialPrice) {}

    void subscribe(const std::shared_ptr<IStockObserver> &observer) {
        subscribers_.push_back(observer);
    }

    void setPrice(double newPrice) {
        price_ = newPrice;
        std::cout << "\n[StockTicker] Ma co phieu " << symbol_
                  << " cap nhat gia moi: $" << std::fixed << std::setprecision(2)
                  << price_ << "\n";
        notifyAll();
    }

private:
    void notifyAll() {
        // Vừa thông báo cho các Observer còn sống, vừa tự động dọn các weak_ptr đã hết hạn
        subscribers_.erase(
            std::remove_if(subscribers_.begin(), subscribers_.end(),
                           [this](const std::weak_ptr<IStockObserver> &weakObs) {
                               if (auto obs = weakObs.lock()) {
                                   obs->onPriceUpdated(symbol_, price_);
                                   return false; // Giữ lại
                               }
                               std::cout << "  (StockTicker: Tu dong don 1 subscriber da huy)\n";
                               return true; // Xóa khỏi danh sách
                           }),
            subscribers_.end());
    }
};

// ============================================================================
// 3. CONCRETE OBSERVERS (Các ứng dụng theo dõi biến động giá)
// ============================================================================
class MobileAlertObserver : public IStockObserver {
    std::string username_;

public:
    explicit MobileAlertObserver(std::string user) : username_(std::move(user)) {}

    void onPriceUpdated(const std::string &symbol, double newPrice) override {
        std::cout << "  -> [Mobile Push -> " << username_ << "] "
                  << symbol << " hien co gia $" << newPrice << "\n";
    }
};

class AlgoTradingBotObserver : public IStockObserver {
    double buyThreshold_;
    double sellThreshold_;

public:
    AlgoTradingBotObserver(double buyAt, double sellAt)
        : buyThreshold_(buyAt), sellThreshold_(sellAt) {}

    void onPriceUpdated(const std::string &symbol, double newPrice) override {
        if (newPrice <= buyThreshold_) {
            std::cout << "  -> [AlgoTradingBot] TIN HIEU MUA (BUY) " << symbol
                      << " tai gia $" << newPrice << " (<= $" << buyThreshold_ << ")!\n";
        } else if (newPrice >= sellThreshold_) {
            std::cout << "  -> [AlgoTradingBot] TIN HIEU CHOT LOI (SELL) " << symbol
                      << " tai gia $" << newPrice << " (>= $" << sellThreshold_ << ")!\n";
        } else {
            std::cout << "  -> [AlgoTradingBot] Giu nguyen vi the (HOLD) cho " << symbol << ".\n";
        }
    }
};

int main() {
    std::cout << "=== OBSERVER PATTERN (PUB-SUB VOI STD::WEAK_PTR AN TOAN) ===\n";

    StockTickerSubject nvdaTicker("NVDA", 120.00);

    auto mobileUser1 = std::make_shared<MobileAlertObserver>("HieuDev");
    auto tradingBot = std::make_shared<AlgoTradingBotObserver>(115.00, 135.00);

    nvdaTicker.subscribe(mobileUser1);
    nvdaTicker.subscribe(tradingBot);

    {
        // Một Observer tạm thời chỉ tồn tại trong phạm vi khối lệnh này
        auto tempMobileUser = std::make_shared<MobileAlertObserver>("TempSessionUser");
        nvdaTicker.subscribe(tempMobileUser);

        nvdaTicker.setPrice(114.50); // Cả 3 Observer đều nhận được thông báo
    } // tempMobileUser tự giải phóng khi ra khỏi scope

    // Khi giá thay đổi lần tiếp theo, StockTicker tự động dọn weak_ptr của tempMobileUser
    nvdaTicker.setPrice(138.25);

    return 0;
}

// ============================================================================
// File: 08_strategy.cpp
// Bài 7.4: Behavioral Design Patterns - Strategy Pattern (Chiến lược hoán đổi được)
// Tham khảo: https://refactoring.guru/design-patterns/strategy
// ============================================================================

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

// ============================================================================
// 1. STRATEGY INTERFACE (Giao diện chung cho họ các thuật toán tìm đường)
// ============================================================================
class IRouteStrategy {
public:
    virtual ~IRouteStrategy() = default;
    virtual std::string getStrategyName() const = 0;
    virtual void buildRoute(const std::string &origin,
                            const std::string &destination,
                            double baseDistanceKm) const = 0;
};

// ============================================================================
// 2. CONCRETE STRATEGIES (Các thuật toán định tuyến khác nhau)
// ============================================================================
class FastestHighwayStrategy : public IRouteStrategy {
public:
    std::string getStrategyName() const override {
        return "Cao toc nhanh nhat (Fastest Highway)";
    }

    void buildRoute(const std::string &origin,
                    const std::string &destination,
                    double baseDistanceKm) const override {
        double timeHours = baseDistanceKm / 95.0; // Tốc độ trung bình 95 km/h
        double tollCost = baseDistanceKm * 1500;  // Phí BOT 1,500đ/km
        std::cout << "  [Highway Route] " << origin << " ===(Cao toc)===> " << destination << "\n"
                  << "    + Quang duong : " << baseDistanceKm << " km\n"
                  << "    + Thoi gian   : " << std::fixed << std::setprecision(2) << timeHours << " gio\n"
                  << "    + Phi BOT     : " << std::fixed << std::setprecision(0) << tollCost << " VND\n";
    }
};

class EconomicalNoTollStrategy : public IRouteStrategy {
public:
    std::string getStrategyName() const override {
        return "Tiet kiem khong thu phi (Avoid Tolls)";
    }

    void buildRoute(const std::string &origin,
                    const std::string &destination,
                    double baseDistanceKm) const override {
        double actualKm = baseDistanceKm * 1.18;  // Đi đường quốc lộ xa hơn 18%
        double timeHours = actualKm / 55.0;       // Tốc độ trung bình 55 km/h
        std::cout << "  [Economical Route] " << origin << " ---(Quoc lo)---> " << destination << "\n"
                  << "    + Quang duong : " << std::fixed << std::setprecision(1) << actualKm << " km\n"
                  << "    + Thoi gian   : " << std::fixed << std::setprecision(2) << timeHours << " gio\n"
                  << "    + Phi BOT     : 0 VND\n";
    }
};

class PublicTransitStrategy : public IRouteStrategy {
public:
    std::string getStrategyName() const override {
        return "Giao thong cong cong (Metro / Bus)";
    }

    void buildRoute(const std::string &origin,
                    const std::string &destination,
                    double baseDistanceKm) const override {
        double timeHours = (baseDistanceKm / 40.0) + 0.35; // Cộng thời gian chờ tàu
        std::cout << "  [Public Transit] " << origin << " ~~~(Metro + Bus)~~~> " << destination << "\n"
                  << "    + Quang duong : " << baseDistanceKm << " km\n"
                  << "    + Thoi gian   : " << std::fixed << std::setprecision(2) << timeHours << " gio\n"
                  << "    + Gia ve      : 25,000 VND (Ve luot co dinh)\n";
    }
};

// ============================================================================
// 3. CONTEXT (Ứng dụng Bản đồ giữ tham chiếu tới Strategy và hoán đổi lúc chạy)
// ============================================================================
class NavigationAppContext {
    std::unique_ptr<IRouteStrategy> strategy_;

public:
    explicit NavigationAppContext(std::unique_ptr<IRouteStrategy> initialStrategy)
        : strategy_(std::move(initialStrategy)) {}

    void setRouteStrategy(std::unique_ptr<IRouteStrategy> newStrategy) {
        strategy_ = std::move(newStrategy);
        std::cout << "\n[Navigator] Da chuyen chien luoc dinh tuyen sang: ["
                  << strategy_->getStrategyName() << "]\n";
    }

    void navigate(const std::string &from, const std::string &to, double distanceKm) const {
        if (!strategy_) {
            std::cout << "[Navigator] Chua chon chien luoc dinh tuyen!\n";
            return;
        }
        strategy_->buildRoute(from, to, distanceKm);
    }
};

int main() {
    std::cout << "=== STRATEGY PATTERN (HOAN DOI THUAT TOAN DINH TUYEN LUC CHAY) ===\n\n";

    // Bắt đầu với chiến lược Cao tốc nhanh nhất
    NavigationAppContext navApp(std::make_unique<FastestHighwayStrategy>());
    navApp.navigate("Ha Noi", "Hai Phong", 105.0);

    // Người dùng muốn tiết kiệm phí BOT -> Hoán đổi Strategy ngay lúc chạy
    navApp.setRouteStrategy(std::make_unique<EconomicalNoTollStrategy>());
    navApp.navigate("Ha Noi", "Hai Phong", 105.0);

    // Người dùng đi chuyển trong nội đô bằng phương tiện công cộng
    navApp.setRouteStrategy(std::make_unique<PublicTransitStrategy>());
    navApp.navigate("Ben Thanh", "Suoi Tien", 19.7);

    return 0;
}

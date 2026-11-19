// ============================================================================
// File: 04_mediator.cpp
// Bài 7.4: Behavioral Design Patterns - Mediator Pattern (Trung gian điều phối)
// Tham khảo: https://refactoring.guru/design-patterns/mediator
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

class Aircraft;

// ============================================================================
// 1. MEDIATOR INTERFACE (Giao diện Đài kiểm soát không lưu)
// ============================================================================
class IAirTrafficControl {
public:
    virtual ~IAirTrafficControl() = default;
    virtual void registerAircraft(Aircraft *aircraft) = 0;
    virtual void broadcastAlert(const Aircraft *sender, const std::string &message) = 0;
    virtual bool requestLanding(const Aircraft *requester) = 0;
    virtual void notifyRunwayCleared(const Aircraft *aircraft) = 0;
};

// ============================================================================
// 2. COLLEAGUE / COMPONENT (Máy bay chỉ giao tiếp với Đài kiểm soát ATC)
// ============================================================================
class Aircraft {
protected:
    std::string callsign_;
    IAirTrafficControl *atc_;

public:
    Aircraft(std::string callsign, IAirTrafficControl *atc)
        : callsign_(std::move(callsign)), atc_(atc) {
        if (atc_) {
            atc_->registerAircraft(this);
        }
    }

    virtual ~Aircraft() = default;

    const std::string &getCallsign() const { return callsign_; }

    void sendRadioMessage(const std::string &msg) const {
        std::cout << "  [" << callsign_ << " -> ATC]: \"" << msg << "\"\n";
        atc_->broadcastAlert(this, msg);
    }

    void receiveRadioMessage(const std::string &fromCallsign, const std::string &msg) const {
        std::cout << "    -> [" << callsign_ << " nhan tin tu " << fromCallsign << "]: \""
                  << msg << "\"\n";
    }

    bool tryLand() const {
        std::cout << "  [" << callsign_ << "] Xin phep ha canh...\n";
        return atc_->requestLanding(this);
    }

    void vacateRunway() const {
        atc_->notifyRunwayCleared(this);
    }
};

// ============================================================================
// 3. CONCRETE MEDIATOR (Đài kiểm soát không lưu quản lý đường băng & liên lạc)
// ============================================================================
class AirportControlTower : public IAirTrafficControl {
    std::vector<Aircraft *> airspaceAircrafts_;
    bool runwayOccupied_ = false;
    std::string currentRunwayHolder_;

public:
    void registerAircraft(Aircraft *aircraft) override {
        airspaceAircrafts_.push_back(aircraft);
        std::cout << "[ATC Tower] Da ghi nhan may bay " << aircraft->getCallsign()
                  << " vao vung troi.\n";
    }

    void broadcastAlert(const Aircraft *sender, const std::string &message) override {
        for (Aircraft *plane : airspaceAircrafts_) {
            if (plane != sender) { // Không gửi ngược lại cho chính máy bay phát tin
                plane->receiveRadioMessage(sender->getCallsign(), message);
            }
        }
    }

    bool requestLanding(const Aircraft *requester) override {
        if (!runwayOccupied_) {
            runwayOccupied_ = true;
            currentRunwayHolder_ = requester->getCallsign();
            std::cout << "[ATC Tower] CHAP THUAN cho " << currentRunwayHolder_
                      << " ha canh xuong Duong bang 01R.\n";
            return true;
        } else {
            std::cout << "[ATC Tower] TU CHOI " << requester->getCallsign()
                      << "! Duong bang dang ban boi " << currentRunwayHolder_
                      << ". Vui long bay vong cho (Holding Pattern).\n";
            return false;
        }
    }

    void notifyRunwayCleared(const Aircraft *aircraft) override {
        if (runwayOccupied_ && currentRunwayHolder_ == aircraft->getCallsign()) {
            std::cout << "[ATC Tower] " << currentRunwayHolder_
                      << " da roi duong bang. Duong bang 01R HIEN DA TRONG!\n";
            runwayOccupied_ = false;
            currentRunwayHolder_.clear();
        }
    }
};

int main() {
    std::cout << "=== MEDIATOR PATTERN (DAI KIEM SOAT KHONG LUU ATC) ===\n\n";

    AirportControlTower noiBaiTower;

    Aircraft vn123("VN-123 (Boeing 787)", &noiBaiTower);
    Aircraft vj456("VJ-456 (Airbus A321)", &noiBaiTower);
    Aircraft qh789("QH-789 (Embraer 190)", &noiBaiTower);

    std::cout << "\n--- 1. Canh bao thoi tiet qua trung gian ATC ---\n";
    vn123.sendRadioMessage("Phat hien nhieu dong (Turbulence) o do cao 10,000ft!");

    std::cout << "\n--- 2. Dieu phoi ha canh tren duong bang duy nhat ---\n";
    vn123.tryLand();
    vj456.tryLand(); // Bị từ chối vì VN-123 đang chiếm đường băng

    vn123.vacateRunway();
    vj456.tryLand(); // Bây giờ được phép hạ cánh

    return 0;
}

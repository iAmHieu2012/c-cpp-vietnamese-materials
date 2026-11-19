#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// =============================================================================
// PROTOTYPE PATTERN (NHÂN BẢN ĐỐI TƯỢNG ĐA HÌNH QUA VIRTUAL CLONE & REGISTRY)
// =============================================================================
class RobotPrototype {
public:
    virtual ~RobotPrototype() = default;
    // Phương thức clone() cho phép tạo bản sao sâu mà không cần biết lớp con cụ thể
    virtual std::unique_ptr<RobotPrototype> clone() const = 0;
    virtual void set_serial(int sn) = 0;
    virtual void print() const = 0;
};

class CombatDrone : public RobotPrototype {
private:
    std::string model;
    int armor;
    int serial_num;

public:
    CombatDrone(std::string m, int a, int sn = 0)
        : model(std::move(m)), armor(a), serial_num(sn) {}

    std::unique_ptr<RobotPrototype> clone() const override {
        return std::make_unique<CombatDrone>(*this);
    }

    void set_serial(int sn) override { serial_num = sn; }

    void print() const override {
        std::cout << "  [CombatDrone] Model: " << model << " | Armor: " << armor
                  << " | Serial #" << serial_num << " (Addr: " << this << ")\n";
    }
};

class MedicalBot : public RobotPrototype {
private:
    int heal_rate;
    int serial_num;

public:
    explicit MedicalBot(int rate, int sn = 0) : heal_rate(rate), serial_num(sn) {}

    std::unique_ptr<RobotPrototype> clone() const override {
        return std::make_unique<MedicalBot>(*this);
    }

    void set_serial(int sn) override { serial_num = sn; }

    void print() const override {
        std::cout << "  [MedicalBot]  HealRate: +" << heal_rate << " HP/s"
                  << " | Serial #" << serial_num << " (Addr: " << this << ")\n";
    }
};

int main() {
    std::cout << "=== PROTOTYPE PATTERN (VIRTUAL CLONE & PROTOTYPE REGISTRY) ===\n";
    std::unordered_map<std::string, std::unique_ptr<RobotPrototype>> registry;
    registry["HeavyDrone"] = std::make_unique<CombatDrone>("Valkyrie-X9", 800, 0);
    registry["Medic"]      = std::make_unique<MedicalBot>(150, 0);

    // Nhân bản hàng loạt từ mẫu gốc trong Registry mà không cần biết lớp cụ thể bên dưới
    auto unit1 = registry["HeavyDrone"]->clone();
    unit1->set_serial(101);

    auto unit2 = registry["HeavyDrone"]->clone();
    unit2->set_serial(102);

    auto unit3 = registry["Medic"]->clone();
    unit3->set_serial(201);

    unit1->print();
    unit2->print();
    unit3->print();

    return 0;
}

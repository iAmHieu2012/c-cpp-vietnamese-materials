#include <iostream>
#include <memory>
#include <string>

// =============================================================================
// FACTORY METHOD PATTERN (NHƯỜNG QUYỀN KHỞI TẠO ĐỐI TƯỢNG CHO LỚP CON)
// =============================================================================
class ITransport {
public:
    virtual ~ITransport() = default;
    virtual void deliver(const std::string &cargo) const = 0;
};

class Truck : public ITransport {
public:
    void deliver(const std::string &cargo) const override {
        std::cout << "  [Truck - Duong bo] Van chuyen '" << cargo << "' bang xe tai container.\n";
    }
};

class Ship : public ITransport {
public:
    void deliver(const std::string &cargo) const override {
        std::cout << "  [Ship - Duong bien] Van chuyen '" << cargo << "' bang tau vien duong.\n";
    }
};

class Airplane : public ITransport {
public:
    void deliver(const std::string &cargo) const override {
        std::cout << "  [Airplane - Hang khong] Van chuyen hoa toc '" << cargo << "' bang may bay.\n";
    }
};

// Lớp Creator chứa quy trình nghiệp vụ chung, gọi tới Factory Method create_transport()
class Logistics {
public:
    virtual ~Logistics() = default;
    virtual std::unique_ptr<ITransport> create_transport() const = 0; // FACTORY METHOD

    void plan_delivery(const std::string &cargo) const {
        std::unique_ptr<ITransport> transport = create_transport();
        transport->deliver(cargo);
    }
};

class RoadLogistics : public Logistics {
public:
    std::unique_ptr<ITransport> create_transport() const override {
        return std::make_unique<Truck>();
    }
};

class SeaLogistics : public Logistics {
public:
    std::unique_ptr<ITransport> create_transport() const override {
        return std::make_unique<Ship>();
    }
};

class AirLogistics : public Logistics {
public:
    std::unique_ptr<ITransport> create_transport() const override {
        return std::make_unique<Airplane>();
    }
};

int main() {
    std::cout << "=== FACTORY METHOD PATTERN (LOGISTICS CREATOR) ===\n";
    RoadLogistics road;
    SeaLogistics  sea;
    AirLogistics  air;

    road.plan_delivery("Linh kien dien tu");
    sea.plan_delivery("10,000 tan gao xuat khau");
    air.plan_delivery("Vaccine bao quan lanh");

    return 0;
}

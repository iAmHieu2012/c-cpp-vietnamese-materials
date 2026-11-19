#include <iostream>
#include <string>

// =============================================================================
// BUILDER PATTERN (KHỞI TẠO ĐỐI TƯỢNG PHỨC TẠP TỪNG BƯỚC + DIRECTOR)
// =============================================================================
struct Computer {
    std::string cpu = "Standard CPU";
    std::string ram = "8GB DDR4";
    std::string storage = "256GB SSD";
    std::string gpu = "Integrated Graphics";
    bool has_liquid_cooling = false;

    void show_specs(const char *title) const {
        std::cout << "  [" << title << "]\n"
                  << "    - CPU     : " << cpu << "\n"
                  << "    - RAM     : " << ram << "\n"
                  << "    - Storage : " << storage << "\n"
                  << "    - GPU     : " << gpu << "\n"
                  << "    - Cooling : " << (has_liquid_cooling ? "Liquid AIO" : "Stock Air") << "\n";
    }
};

// Fluent Builder: Cho phép gọi nối chuỗi (Method Chaining) các bước lắp ráp tùy chọn
class ComputerBuilder {
private:
    Computer pc;

public:
    ComputerBuilder& reset() {
        pc = Computer{};
        return *this;
    }

    ComputerBuilder& set_cpu(std::string c)     { pc.cpu = std::move(c); return *this; }
    ComputerBuilder& set_ram(std::string r)     { pc.ram = std::move(r); return *this; }
    ComputerBuilder& set_storage(std::string s) { pc.storage = std::move(s); return *this; }
    ComputerBuilder& set_gpu(std::string g)     { pc.gpu = std::move(g); return *this; }
    ComputerBuilder& enable_liquid_cooling(bool flag = true) {
        pc.has_liquid_cooling = flag;
        return *this;
    }

    Computer build() {
        Computer result = std::move(pc);
        reset();
        return result;
    }
};

// Director: Đóng gói các cấu hình lắp ráp tiêu chuẩn thường dùng
class ComputerDirector {
public:
    static Computer construct_high_end_workstation(ComputerBuilder &builder) {
        return builder.reset()
                      .set_cpu("AMD Ryzen 9 9950X")
                      .set_ram("128GB DDR5 ECC")
                      .set_storage("4TB NVMe Gen5 SSD")
                      .set_gpu("NVIDIA RTX 4090 24GB")
                      .enable_liquid_cooling(true)
                      .build();
    }

    static Computer construct_budget_office_pc(ComputerBuilder &builder) {
        return builder.reset()
                      .set_cpu("Intel Core i3-14100")
                      .set_ram("16GB DDR4")
                      .set_storage("512GB NVMe SSD")
                      .build();
    }
};

int main() {
    std::cout << "=== BUILDER PATTERN (FLUENT BUILDER & DIRECTOR) ===\n";
    ComputerBuilder builder;

    Computer workstation = ComputerDirector::construct_high_end_workstation(builder);
    Computer office_pc   = ComputerDirector::construct_budget_office_pc(builder);

    workstation.show_specs("AI Workstation (via Director)");
    office_pc.show_specs("Office PC (via Director)");

    Computer custom_pc = builder.set_cpu("Intel Core i7-14700K")
                                .set_ram("32GB DDR5")
                                .set_gpu("RTX 4070 Super")
                                .build();
    custom_pc.show_specs("Custom Gaming PC (Direct Fluent Builder)");

    return 0;
}

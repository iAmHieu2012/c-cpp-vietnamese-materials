#include <iostream>
#include <string>

// =============================================================================
// 1. GIẢI QUYẾT DIAMOND PROBLEM BẰNG VIRTUAL INHERITANCE
// =============================================================================
class Device {
protected:
    int device_id;
public:
    Device(int id = 0) : device_id(id) {
        std::cout << "  [Constructor] Device(" << device_id << ")\n";
    }
    void show_id() const {
        std::cout << "  Device ID: " << device_id << "\n";
    }
};

// Kế thừa ảo (virtual public) đảm bảo chỉ có 1 bản sao Device duy nhất khi đa kế thừa
class Printer : virtual public Device {
public:
    Printer(int id = 0) : Device(id) {
        std::cout << "  [Constructor] Printer\n";
    }
    void print_doc(const std::string &doc) const {
        std::cout << "  [Printer] Dang in: " << doc << "\n";
    }
};

class Scanner : virtual public Device {
public:
    Scanner(int id = 0) : Device(id) {
        std::cout << "  [Constructor] Scanner\n";
    }
    void scan_doc() const {
        std::cout << "  [Scanner] Dang quet tai lieu...\n";
    }
};

// Đa kế thừa (Multiple Inheritance): AllInOne vừa là Printer vừa là Scanner
class AllInOne : public Printer, public Scanner {
public:
    // Với virtual inheritance, lớp con cuối cùng (AllInOne) trực tiếp gọi Constructor của lớp gốc Device
    AllInOne(int id) : Device(id), Printer(id), Scanner(id) {
        std::cout << "  [Constructor] AllInOne\n";
    }
};

int main() {
    std::cout << "=== KHOI TAO DOI TUONG DA KE THUA (VIRTUAL INHERITANCE) ===\n";
    AllInOne machine(9001);

    std::cout << "\n=== GOI PHUONG THUC KHONG BI NHAP NHANG (NO AMBIGUITY) ===\n";
    machine.show_id(); // Nếu không có 'virtual', dòng này sẽ lỗi biên dịch vì có 2 bản sao Device!
    machine.print_doc("Bao_cao_Q4.pdf");
    machine.scan_doc();

    return 0;
}
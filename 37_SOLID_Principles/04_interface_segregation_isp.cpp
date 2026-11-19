// ============================================================================
// File: 04_interface_segregation_isp.cpp
// Bài 7.5: SOLID Principles - [I] Interface Segregation Principle (Phân tách Interface)
// "Clients should not be forced to depend upon interfaces that they do not use."
// ============================================================================

#include <iostream>
#include <stdexcept>
#include <string>

// ============================================================================
// 1. VI PHẠM ISP ("Fat Interface" ép lớp con phải cài đặt những hàm vô nghĩa)
// ============================================================================
namespace BadISP {

class IMultiFunctionDevice {
public:
    virtual ~IMultiFunctionDevice() = default;
    virtual void printDocument(const std::string &doc) = 0;
    virtual void scanDocument(const std::string &doc) = 0;
    virtual void faxDocument(const std::string &doc) = 0;
};

// Máy in giá rẻ chỉ biết In, nhưng bị ép phải override cả scan() và fax()!
class BasicInkjetPrinter : public IMultiFunctionDevice {
public:
    void printDocument(const std::string &doc) override {
        std::cout << "  [BasicInkjet] Dang in: " << doc << "\n";
    }

    void scanDocument(const std::string &) override {
        // Bị ép cài đặt hàm không hỗ trợ -> Quăng ngoại lệ lúc chạy!
        throw std::runtime_error("May in co ban KHONG ho tro Scan!");
    }

    void faxDocument(const std::string &) override {
        throw std::runtime_error("May in co ban KHONG ho tro Fax!");
    }
};

} // namespace BadISP

// ============================================================================
// 2. TUÂN THỦ ISP (Chia nhỏ Fat Interface thành các Role Interfaces chuyên biệt)
// ============================================================================
namespace GoodISP {

class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string &doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string &doc) = 0;
};

class IFaxMachine {
public:
    virtual ~IFaxMachine() = default;
    virtual void sendFax(const std::string &doc) = 0;
};

// Máy in cơ bản chỉ cần kế thừa duy nhất IPrinter
class SimpleLaserPrinter : public IPrinter {
public:
    void print(const std::string &doc) override {
        std::cout << "  [SimpleLaserPrinter] In tai lieu: " << doc << "\n";
    }
};

// Máy photocopy đa năng văn phòng kế thừa kết hợp cả 3 giao diện nhỏ
class EnterprisePhotocopier : public IPrinter, public IScanner, public IFaxMachine {
public:
    void print(const std::string &doc) override {
        std::cout << "  [EnterprisePhotocopier] In toc do cao: " << doc << "\n";
    }

    void scan(const std::string &doc) override {
        std::cout << "  [EnterprisePhotocopier] Quet PDF 600dpi: " << doc << "\n";
    }

    void sendFax(const std::string &doc) override {
        std::cout << "  [EnterprisePhotocopier] Gui Fax quoc te: " << doc << "\n";
    }
};

void executePrintJob(IPrinter &printer, const std::string &doc) {
    printer.print(doc);
}

} // namespace GoodISP

int main() {
    std::cout << "=== [I] INTERFACE SEGREGATION PRINCIPLE (ISP) ===\n\n";

    std::cout << "--- 1. Hau qua cua Fat Interface (Vi pham ISP) ---\n";
    BadISP::BasicInkjetPrinter badPrinter;
    badPrinter.printDocument("Hop_dong.pdf");
    try {
        badPrinter.scanDocument("CCCD.png");
    } catch (const std::exception &ex) {
        std::cout << "  [Runtime Exception] " << ex.what() << "\n";
    }

    std::cout << "\n--- 2. Sau khi phan tach thanh IPrinter, IScanner, IFaxMachine ---\n";
    GoodISP::SimpleLaserPrinter simplePrinter;
    GoodISP::EnterprisePhotocopier officeAllInOne;

    // Cả hai đều dùng được với hàm yêu cầu IPrinter
    GoodISP::executePrintJob(simplePrinter, "Bao_cao_tai_chinh.docx");
    GoodISP::executePrintJob(officeAllInOne, "Thiet_ke_kien_truc.pdf");

    // Chỉ máy đa năng mới có giao diện IScanner và IFaxMachine (an toàn ngay lúc biên dịch!)
    officeAllInOne.scan("Ho_chieu.jpg");
    officeAllInOne.sendFax("Don_dat_hang_PO.pdf");

    return 0;
}

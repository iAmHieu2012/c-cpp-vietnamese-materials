// ============================================================================
// File: 10_visitor.cpp
// Bài 7.4: Behavioral Design Patterns - Visitor Pattern (Double Dispatch)
// Tham khảo: https://refactoring.guru/design-patterns/visitor
// ============================================================================

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ResidentialBuilding;
class BankBranch;
class IndustrialFactory;

// ============================================================================
// 1. VISITOR INTERFACE (Khai báo hàm visit riêng cho từng Concrete Element)
// ============================================================================
class IBuildingVisitor {
public:
    virtual ~IBuildingVisitor() = default;
    virtual void visitResidential(const ResidentialBuilding &b) = 0;
    virtual void visitBank(const BankBranch &b) = 0;
    virtual void visitFactory(const IndustrialFactory &b) = 0;
};

// ============================================================================
// 2. ELEMENT INTERFACE & CONCRETE ELEMENTS (Các công trình kiến trúc trên bản đồ)
// ============================================================================
class IBuildingElement {
public:
    virtual ~IBuildingElement() = default;
    // Kỹ thuật Double Dispatch: phần tử tự gọi visitor.visitXxx(*this)
    virtual void accept(IBuildingVisitor &visitor) const = 0;
};

class ResidentialBuilding : public IBuildingElement {
    std::string address_;
    int familyCount_;

public:
    ResidentialBuilding(std::string addr, int families)
        : address_(std::move(addr)), familyCount_(families) {}

    const std::string &getAddress() const { return address_; }
    int getFamilyCount() const { return familyCount_; }

    void accept(IBuildingVisitor &visitor) const override {
        visitor.visitResidential(*this);
    }
};

class BankBranch : public IBuildingElement {
    std::string bankName_;
    double vaultCashUSD_;

public:
    BankBranch(std::string name, double vaultCash)
        : bankName_(std::move(name)), vaultCashUSD_(vaultCash) {}

    const std::string &getBankName() const { return bankName_; }
    double getVaultCashUSD() const { return vaultCashUSD_; }

    void accept(IBuildingVisitor &visitor) const override {
        visitor.visitBank(*this);
    }
};

class IndustrialFactory : public IBuildingElement {
    std::string factoryName_;
    int hazardLevel_; // 1 -> 5

public:
    IndustrialFactory(std::string name, int hazard)
        : factoryName_(std::move(name)), hazardLevel_(hazard) {}

    const std::string &getFactoryName() const { return factoryName_; }
    int getHazardLevel() const { return hazardLevel_; }

    void accept(IBuildingVisitor &visitor) const override {
        visitor.visitFactory(*this);
    }
};

// ============================================================================
// 3. CONCRETE VISITORS (Thêm nghiệp vụ mới mà không sửa các lớp Building)
// ============================================================================

// Nghiệp vụ 1: Định giá gói bảo hiểm phù hợp cho từng loại công trình
class InsuranceQuoteVisitor : public IBuildingVisitor {
    double totalAnnualPremium_ = 0.0;

public:
    void visitResidential(const ResidentialBuilding &b) override {
        double quote = b.getFamilyCount() * 120.0;
        totalAnnualPremium_ += quote;
        std::cout << "  [Bao hiem Y te & Nha o] Toa chung cu '" << b.getAddress()
                  << "' (" << b.getFamilyCount() << " ho): $"
                  << std::fixed << std::setprecision(2) << quote << "/nam\n";
    }

    void visitBank(const BankBranch &b) override {
        double quote = b.getVaultCashUSD() * 0.015; // 1.5% giá trị két sắt
        totalAnnualPremium_ += quote;
        std::cout << "  [Bao hiem Chong Trom & Ket sat] Ngan hang '" << b.getBankName()
                  << "': $" << std::fixed << std::setprecision(2) << quote << "/nam\n";
    }

    void visitFactory(const IndustrialFactory &b) override {
        double quote = 2500.0 * b.getHazardLevel();
        totalAnnualPremium_ += quote;
        std::cout << "  [Bao hiem Chay no Cong nghiep] Nha may '" << b.getFactoryName()
                  << "' (Rui ro cap " << b.getHazardLevel() << "): $"
                  << std::fixed << std::setprecision(2) << quote << "/nam\n";
    }

    double getTotalAnnualPremium() const { return totalAnnualPremium_; }
};

// Nghiệp vụ 2: Xuất dữ liệu bản đồ thành định dạng XML
class XmlExportVisitor : public IBuildingVisitor {
public:
    void visitResidential(const ResidentialBuilding &b) override {
        std::cout << "  <residential address=\"" << b.getAddress()
                  << "\" families=\"" << b.getFamilyCount() << "\" />\n";
    }

    void visitBank(const BankBranch &b) override {
        std::cout << "  <bank name=\"" << b.getBankName()
                  << "\" vault_usd=\"" << std::fixed << std::setprecision(0)
                  << b.getVaultCashUSD() << "\" />\n";
    }

    void visitFactory(const IndustrialFactory &b) override {
        std::cout << "  <factory name=\"" << b.getFactoryName()
                  << "\" hazard_level=\"" << b.getHazardLevel() << "\" />\n";
    }
};

int main() {
    std::cout << "=== VISITOR PATTERN (DOUBLE DISPATCH TREN DANH SACH CONG TRINH) ===\n\n";

    std::vector<std::unique_ptr<IBuildingElement>> cityBuildings;
    cityBuildings.push_back(std::make_unique<ResidentialBuilding>("Landmark 81 Tower", 450));
    cityBuildings.push_back(std::make_unique<BankBranch>("Vietcombank HQ", 5000000.0));
    cityBuildings.push_back(std::make_unique<IndustrialFactory>("VinFast Paint Shop", 4));

    std::cout << "--- 1. Chay InsuranceQuoteVisitor tren toan thanh pho ---\n";
    InsuranceQuoteVisitor insuranceVisitor;
    for (const auto &building : cityBuildings) {
        building->accept(insuranceVisitor);
    }
    std::cout << "  => TONG DOANH THU BAO HIEM DU KIEN: $"
              << insuranceVisitor.getTotalAnnualPremium() << "/nam\n\n";

    std::cout << "--- 2. Chay XmlExportVisitor tren cung danh sach ---\n";
    XmlExportVisitor xmlVisitor;
    std::cout << "<city_map>\n";
    for (const auto &building : cityBuildings) {
        building->accept(xmlVisitor);
    }
    std::cout << "</city_map>\n";

    return 0;
}

// ============================================================================
// File: 01_single_responsibility_srp.cpp
// Bài 7.5: SOLID Principles - [S] Single Responsibility Principle (Đơn trách nhiệm)
// "A class should have one, and only one, reason to change."
// ============================================================================

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct LineItem {
    std::string productName;
    int quantity;
    double unitPrice;
};

// ============================================================================
// 1. VI PHẠM SRP (ANTI-PATTERN: "God Class" ôm đồm 3 trách nhiệm khác nhau)
// ============================================================================
namespace BadSRP {

class GodInvoice {
    std::string invoiceId_;
    std::vector<LineItem> items_;

public:
    explicit GodInvoice(std::string id) : invoiceId_(std::move(id)) {}

    void addItem(const std::string &name, int qty, double price) {
        items_.push_back({name, qty, price});
    }

    // Trách nhiệm 1: Logic nghiệp vụ kế toán (Thay đổi khi luật thuế thay đổi)
    double calculateTotalWithTax() const {
        double subtotal = 0.0;
        for (const auto &item : items_) {
            subtotal += item.quantity * item.unitPrice;
        }
        return subtotal * 1.10; // VAT 10%
    }

    // Trách nhiệm 2: Định dạng hiển thị UI/Report (Thay đổi khi đổi mẫu in HTML/JSON)
    void printInvoiceConsole() const {
        std::cout << "  [Bad GodInvoice] ID: " << invoiceId_
                  << " | Total (VAT 10%): $" << calculateTotalWithTax() << "\n";
    }

    // Trách nhiệm 3: Lưu trữ cơ sở dữ liệu (Thay đổi khi chuyển từ MySQL sang PostgreSQL)
    void saveToMySqlDatabase() const {
        std::cout << "  [Bad GodInvoice] INSERT INTO invoices VALUES ('"
                  << invoiceId_ << "', " << calculateTotalWithTax() << ");\n";
    }
};

} // namespace BadSRP

// ============================================================================
// 2. TUÂN THỦ SRP (Tách mỗi lý do thay đổi thành một lớp chuyên biệt)
// ============================================================================
namespace GoodSRP {

// Lớp 1: Chỉ phụ trách Dữ liệu & Nghiệp vụ tính toán hóa đơn (Domain Entity)
class Invoice {
    std::string invoiceId_;
    std::vector<LineItem> items_;
    double taxRate_;

public:
    Invoice(std::string id, double taxRate = 0.10)
        : invoiceId_(std::move(id)), taxRate_(taxRate) {}

    void addItem(const std::string &name, int qty, double price) {
        items_.push_back({name, qty, price});
    }

    const std::string &getId() const { return invoiceId_; }
    const std::vector<LineItem> &getItems() const { return items_; }

    double calculateSubtotal() const {
        double sum = 0.0;
        for (const auto &item : items_) {
            sum += item.quantity * item.unitPrice;
        }
        return sum;
    }

    double calculateTotal() const {
        return calculateSubtotal() * (1.0 + taxRate_);
    }
};

// Lớp 2: Chỉ phụ trách Định dạng & Xuất báo cáo (Presentation / Formatter)
class InvoiceFormatter {
public:
    static std::string toJson(const Invoice &inv) {
        return "{ \"invoice_id\": \"" + inv.getId() + "\", \"subtotal\": " +
               std::to_string(inv.calculateSubtotal()) + ", \"total\": " +
               std::to_string(inv.calculateTotal()) + " }";
    }

    static void printReceipt(const Invoice &inv) {
        std::cout << "  +--------------------------------------------+\n";
        std::cout << "  | HOA DON THANH TOAN: " << std::setw(22) << std::left << inv.getId() << " |\n";
        std::cout << "  +--------------------------------------------+\n";
        for (const auto &item : inv.getItems()) {
            std::cout << "  | * " << std::setw(18) << std::left << item.productName
                      << " x" << item.quantity << " : $"
                      << std::fixed << std::setprecision(2) << (item.quantity * item.unitPrice) << "\n";
        }
        std::cout << "  | -> Tong thanh toan (sau VAT): $" << inv.calculateTotal() << "\n";
        std::cout << "  +--------------------------------------------+\n";
    }
};

// Lớp 3: Chỉ phụ trách Lưu trữ dữ liệu bền vững (Persistence / Repository)
class InvoiceRepository {
public:
    void saveToDatabase(const Invoice &inv) const {
        std::cout << "  [InvoiceRepository] Da luu hoa don '" << inv.getId()
                  << "' ($" << inv.calculateTotal() << ") vao PostgreSQL.\n";
    }
};

} // namespace GoodSRP

int main() {
    std::cout << "=== [S] SINGLE RESPONSIBILITY PRINCIPLE (SRP) ===\n\n";

    std::cout << "--- 1. Truoc khi Refactor (GodInvoice om dom 3 trach nhiem) ---\n";
    BadSRP::GodInvoice badInv("INV-BAD-01");
    badInv.addItem("Mechanical Keyboard", 2, 120.0);
    badInv.printInvoiceConsole();
    badInv.saveToMySqlDatabase();

    std::cout << "\n--- 2. Sau khi Refactor (Tach Domain, Formatter, Repository) ---\n";
    GoodSRP::Invoice goodInv("INV-GOOD-2026", 0.08); // VAT 8%
    goodInv.addItem("Mechanical Keyboard", 2, 120.0);
    goodInv.addItem("4K Monitor", 1, 350.0);

    GoodSRP::InvoiceFormatter::printReceipt(goodInv);
    std::cout << "  [InvoiceFormatter JSON] " << GoodSRP::InvoiceFormatter::toJson(goodInv) << "\n";

    GoodSRP::InvoiceRepository repo;
    repo.saveToDatabase(goodInv);

    return 0;
}

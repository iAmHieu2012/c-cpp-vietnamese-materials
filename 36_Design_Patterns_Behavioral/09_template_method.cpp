// ============================================================================
// File: 09_template_method.cpp
// Bài 7.4: Behavioral Design Patterns - Template Method (Phương thức mẫu)
// Tham khảo: https://refactoring.guru/design-patterns/template-method
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// 1. ABSTRACT CLASS (Lớp cơ sở định nghĩa khung xương thuật toán - Template Method)
// ============================================================================
class DocumentDataMiner {
public:
    virtual ~DocumentDataMiner() = default;

    // TEMPLATE METHOD (Không cho lớp con override quy trình khung này)
    void mineDocument(const std::string &filePath) const {
        std::cout << "====================================================\n";
        std::cout << "[Pipeline] Bat dau khai phá du lieu: " << filePath << "\n";

        openFile(filePath);
        std::string rawData = extractRawContent();
        std::vector<std::string> records = parseRecords(rawData);

        // Hook Method: Bước kiểm tra tùy chọn trước khi phân tích
        if (shouldRunDeepValidation()) {
            std::cout << "  [Hook] Dang chay kiem tra toan ven du lieu nang cao...\n";
        }

        analyzeData(records);
        sendReport(filePath, records.size());
        closeFile(filePath);
    }

protected:
    // Các bước bắt buộc lớp con phải tự định nghĩa (Pure Virtual Steps)
    virtual void openFile(const std::string &filePath) const = 0;
    virtual std::string extractRawContent() const = 0;
    virtual std::vector<std::string> parseRecords(const std::string &raw) const = 0;
    virtual void closeFile(const std::string &filePath) const = 0;

    // Các bước dùng chung đã có cài đặt chuẩn ở lớp cha (Common Steps)
    void analyzeData(const std::vector<std::string> &records) const {
        std::cout << "  [Common Analyzer] Dang thong ke " << records.size()
                  << " ban ghi:\n";
        for (const auto &rec : records) {
            std::cout << "    * Record: " << rec << "\n";
        }
    }

    void sendReport(const std::string &filePath, size_t count) const {
        std::cout << "  [Common Reporter] Xuat bao cao JSON cho '" << filePath
                  << "' (Tong so: " << count << " items).\n";
    }

    // Hook Method (Phương thức móc nối có hành vi mặc định, lớp con có thể override)
    virtual bool shouldRunDeepValidation() const {
        return false;
    }
};

// ============================================================================
// 2. CONCRETE CLASSES (Các lớp con chỉ nạp chồng những bước đặc thù)
// ============================================================================
class CsvDataMiner : public DocumentDataMiner {
protected:
    void openFile(const std::string &filePath) const override {
        std::cout << "  [CSV] Mo luong van ban UTF-8: " << filePath << "\n";
    }

    std::string extractRawContent() const override {
        return "101,Laptop,25000000;102,Keyboard,1500000;103,Mouse,800000";
    }

    std::vector<std::string> parseRecords(const std::string &raw) const override {
        std::cout << "  [CSV] Tach cac dong theo dau ';' va cot theo dau ','...\n";
        std::vector<std::string> result;
        size_t start = 0, end = 0;
        while ((end = raw.find(';', start)) != std::string::npos) {
            result.push_back(raw.substr(start, end - start));
            start = end + 1;
        }
        result.push_back(raw.substr(start));
        return result;
    }

    void closeFile(const std::string &filePath) const override {
        std::cout << "  [CSV] Dong file CSV: " << filePath << "\n";
    }
};

class PdfInvoiceDataMiner : public DocumentDataMiner {
protected:
    void openFile(const std::string &filePath) const override {
        std::cout << "  [PDF] Khoi tao Poppler PDF Engine de doc: " << filePath << "\n";
    }

    std::string extractRawContent() const override {
        return "PDF_OBJ_STREAM(INV-01: $450 | INV-02: $1200)";
    }

    std::vector<std::string> parseRecords(const std::string &) const override {
        std::cout << "  [PDF] Quet OCR va trich xuat bang hoa don tu PDF...\n";
        return {"Invoice #INV-01 ($450.00)", "Invoice #INV-02 ($1,200.00)"};
    }

    // Bật Hook kiểm tra nâng cao riêng cho tài liệu hóa đơn PDF
    bool shouldRunDeepValidation() const override {
        return true;
    }

    void closeFile(const std::string &filePath) const override {
        std::cout << "  [PDF] Giai phong bo nho PDF Engine cho: " << filePath << "\n";
    }
};

int main() {
    std::cout << "=== TEMPLATE METHOD PATTERN (BO KHUNG KHAI PHA DU LIEU) ===\n\n";

    CsvDataMiner csvMiner;
    csvMiner.mineDocument("sales_q4_2026.csv");

    std::cout << "\n";

    PdfInvoiceDataMiner pdfMiner;
    pdfMiner.mineDocument("enterprise_invoices.pdf");

    return 0;
}

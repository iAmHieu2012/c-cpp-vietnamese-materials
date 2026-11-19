// ============================================================================
// File: 04_abstraction_and_interfaces.cpp
// Bài 3.2: Tính Trừu tượng (Abstract Base Class vs Pure Interface)
// ============================================================================

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// 1. PURE INTERFACES (Giao diện thuần túy: 100% hàm thuần ảo '= 0', không dữ liệu)
// ============================================================================
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual std::string toJson() const = 0;
};

class IAuditable {
public:
    virtual ~IAuditable() = default;
    virtual void printAuditTrail() const = 0;
};

// ============================================================================
// 2. ABSTRACT BASE CLASS (Lớp cơ sở trừu tượng: Có thuộc tính chung + hàm '= 0')
// ============================================================================
class PaymentProcessor {
protected:
    std::string merchantId_;
    double transactionFeeRate_;

public:
    PaymentProcessor(std::string merchantId, double feeRate)
        : merchantId_(std::move(merchantId)), transactionFeeRate_(feeRate) {}

    virtual ~PaymentProcessor() = default;

    // Phương thức dùng chung đã có thân hàm ở Abstract Class
    double calculateFee(double amount) const {
        return amount * transactionFeeRate_;
    }

    // Phương thức thuần ảo (= 0) bắt buộc mọi lớp con phải định nghĩa
    virtual bool authorizeAndCapture(double amount) const = 0;
    virtual std::string getProviderName() const = 0;
};

// ============================================================================
// 3. CONCRETE CLASSES (Kế thừa 1 Abstract Class + Đa kế thừa nhiều Interface)
// ============================================================================
class StripeCreditCardProcessor : public PaymentProcessor,
                                  public ISerializable,
                                  public IAuditable {
    std::string maskedCard_;

public:
    StripeCreditCardProcessor(std::string merchantId, std::string maskedCard)
        : PaymentProcessor(std::move(merchantId), 0.029), // Phí 2.9%
          maskedCard_(std::move(maskedCard)) {}

    std::string getProviderName() const override { return "Stripe Visa/Mastercard"; }

    bool authorizeAndCapture(double amount) const override {
        double fee = calculateFee(amount);
        std::cout << "  [Stripe] Thu $" << std::fixed << std::setprecision(2)
                  << amount << " tu the " << maskedCard_
                  << " (Phi dich vu: $" << fee << ")\n";
        return true;
    }

    std::string toJson() const override {
        return "{\"provider\":\"Stripe\",\"merchant\":\"" + merchantId_ +
               "\",\"card\":\"" + maskedCard_ + "\"}";
    }

    void printAuditTrail() const override {
        std::cout << "  [AuditLog] PCI-DSS verified for merchant " << merchantId_
                  << " via Stripe Gateway.\n";
    }
};

class VnPayQrProcessor : public PaymentProcessor,
                         public ISerializable {
    std::string bankCode_;

public:
    VnPayQrProcessor(std::string merchantId, std::string bankCode)
        : PaymentProcessor(std::move(merchantId), 0.011), // Phí 1.1%
          bankCode_(std::move(bankCode)) {}

    std::string getProviderName() const override { return "VNPay QR Banking"; }

    bool authorizeAndCapture(double amount) const override {
        double fee = calculateFee(amount);
        std::cout << "  [VNPay QR] Quet ma ngan hang " << bankCode_
                  << " thanh toan $" << std::fixed << std::setprecision(2)
                  << amount << " (Phi dich vu: $" << fee << ")\n";
        return true;
    }

    std::string toJson() const override {
        return "{\"provider\":\"VNPay\",\"merchant\":\"" + merchantId_ +
               "\",\"bank\":\"" + bankCode_ + "\"}";
    }
};

// Hàm cấp cao chỉ làm việc với lớp trừu tượng PaymentProcessor (Không cần biết cổng nào)
void runCheckout(const PaymentProcessor &processor, double orderTotal) {
    std::cout << "-> Khoi tao giao dich qua [" << processor.getProviderName() << "]:\n";
    processor.authorizeAndCapture(orderTotal);
}

int main() {
    std::cout << "=== TINH TRUU TUONG: ABSTRACT CLASS VS PURE INTERFACE ===\n\n";

    StripeCreditCardProcessor stripe("MERCH-VN-01", "****-****-****-4242");
    VnPayQrProcessor vnpay("MERCH-VN-01", "VCB");

    std::cout << "--- 1. Goi qua Abstract Base Class (PaymentProcessor&) ---\n";
    runCheckout(stripe, 250.0);
    runCheckout(vnpay, 120.0);

    std::cout << "\n--- 2. Goi qua Pure Interface (ISerializable&) ---\n";
    const ISerializable *serializables[] = {&stripe, &vnpay};
    for (const auto *item : serializables) {
        std::cout << "  JSON: " << item->toJson() << "\n";
    }

    std::cout << "\n--- 3. Goi qua Pure Interface (IAuditable&) ---\n";
    const IAuditable &auditableStripe = stripe;
    auditableStripe.printAuditTrail();

    return 0;
}

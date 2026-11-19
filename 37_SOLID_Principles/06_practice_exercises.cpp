// ============================================================================
// File: 06_practice_exercises.cpp
// Bài 7.5: Bài tập thực hành tổng hợp - 5 Nguyên lý SOLID
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// BÀI TẬP 1: Refactor theo SRP (Tách UserValidator, PasswordHasher, UserRepo)
// ============================================================================
class PasswordHasher {
public:
    static std::string hashArgon2(const std::string &rawPassword) {
        return "$argon2id$v=19$hashed_" + rawPassword;
    }
};

class UserEmailValidator {
public:
    static bool isValid(const std::string &email) {
        return email.find('@') != std::string::npos && email.find('.') != std::string::npos;
    }
};

class UserRegistrationService {
public:
    void registerUser(const std::string &email, const std::string &rawPass) const {
        if (!UserEmailValidator::isValid(email)) {
            std::cout << "  [SRP Registration] Email khong hop le: " << email << "\n";
            return;
        }
        std::string passwordHash = PasswordHasher::hashArgon2(rawPass);
        std::cout << "  [SRP Registration] Dang ky thanh cong cho '" << email
                  << "' voi hash: " << passwordHash << "\n";
    }
};

void exercise1() {
    std::cout << "\n--- BAI 1: SINGLE RESPONSIBILITY PRINCIPLE (USER REGISTRATION) ---\n";
    UserRegistrationService service;
    service.registerUser("invalid_email", "123456");
    service.registerUser("hieu.dev@google.com", "SuperSecret2026!");
}

// ============================================================================
// BÀI TẬP 2: Refactor theo OCP (Hệ thống tính phí vận chuyển đa quốc gia)
// ============================================================================
class IShippingRule {
public:
    virtual ~IShippingRule() = default;
    virtual std::string getRegion() const = 0;
    virtual double calculateFee(double weightKg) const = 0;
};

class DomesticShippingRule : public IShippingRule {
public:
    std::string getRegion() const override { return "Noi dia (Domestic)"; }
    double calculateFee(double weightKg) const override { return 2.0 + weightKg * 0.8; }
};

class InternationalExpressRule : public IShippingRule {
public:
    std::string getRegion() const override { return "Quoc te (DHL Express)"; }
    double calculateFee(double weightKg) const override { return 18.0 + weightKg * 6.5; }
};

void exercise2() {
    std::cout << "\n--- BAI 2: OPEN/CLOSED PRINCIPLE (SHIPPING CALCULATOR) ---\n";
    std::vector<std::unique_ptr<IShippingRule>> rules;
    rules.push_back(std::make_unique<DomesticShippingRule>());
    rules.push_back(std::make_unique<InternationalExpressRule>());

    double packageWeight = 3.5;
    for (const auto &rule : rules) {
        std::cout << "  * Goi " << packageWeight << "kg [" << rule->getRegion()
                  << "] -> Phi ship: $" << rule->calculateFee(packageWeight) << "\n";
    }
}

// ============================================================================
// BÀI TẬP 3: Refactor theo LSP (Tài khoản Ngân hàng: Rút được vs Tiết kiệm kỳ hạn)
// ============================================================================
class IBankAccount {
public:
    virtual ~IBankAccount() = default;
    virtual void deposit(double amount) = 0;
    virtual double getBalance() const = 0;
};

// Chỉ những tài khoản cho phép rút tiền tức thời mới kế thừa IWithdrawableAccount
class IWithdrawableAccount : public IBankAccount {
public:
    virtual bool withdraw(double amount) = 0;
};

class CheckingAccount : public IWithdrawableAccount {
    double balance_ = 0.0;

public:
    void deposit(double amount) override { balance_ += amount; }
    double getBalance() const override { return balance_; }
    bool withdraw(double amount) override {
        if (amount > balance_) return false;
        balance_ -= amount;
        return true;
    }
};

// Sổ tiết kiệm kỳ hạn cố định không bị ép cài đặt hàm withdraw() quăng lỗi
class FixedTermDepositAccount : public IBankAccount {
    double balance_ = 0.0;

public:
    void deposit(double amount) override { balance_ += amount; }
    double getBalance() const override { return balance_; }
};

void exercise3() {
    std::cout << "\n--- BAI 3: LISKOV SUBSTITUTION PRINCIPLE (BANK ACCOUNTS) ---\n";
    CheckingAccount checking;
    checking.deposit(1000.0);
    checking.withdraw(300.0);
    std::cout << "  [CheckingAccount] So du sau khi rut $300: $" << checking.getBalance() << "\n";

    FixedTermDepositAccount savings;
    savings.deposit(5000.0);
    std::cout << "  [FixedTermDeposit] So du tiet kiem ky han (khong cho rut ngang): $"
              << savings.getBalance() << "\n";
}

// ============================================================================
// BÀI TẬP 4: Refactor theo ISP (Phân tách quyền Đọc / Ghi kho lưu trữ Cloud)
// ============================================================================
class IReadableStorage {
public:
    virtual ~IReadableStorage() = default;
    virtual std::string readFile(const std::string &path) const = 0;
};

class IWritableStorage {
public:
    virtual ~IWritableStorage() = default;
    virtual void writeFile(const std::string &path, const std::string &data) = 0;
};

class ReadOnlyCdRomArchive : public IReadableStorage {
public:
    std::string readFile(const std::string &path) const override {
        return "Noi dung tu CD-ROM: " + path;
    }
};

class CloudS3Bucket : public IReadableStorage, public IWritableStorage {
public:
    std::string readFile(const std::string &path) const override {
        return "Noi dung tu AWS S3: " + path;
    }
    void writeFile(const std::string &path, const std::string &data) override {
        std::cout << "  [AWS S3] Da upload '" << path << "' (" << data.size() << " bytes)\n";
    }
};

void exercise4() {
    std::cout << "\n--- BAI 4: INTERFACE SEGREGATION PRINCIPLE (READ/WRITE STORAGE) ---\n";
    ReadOnlyCdRomArchive cdRom;
    CloudS3Bucket s3;

    std::cout << "  * " << cdRom.readFile("/archive/2020.log") << "\n";
    s3.writeFile("/backups/db.sql", "CREATE TABLE users...");
    std::cout << "  * " << s3.readFile("/backups/db.sql") << "\n";
}

// ============================================================================
// BÀI TẬP 5: Refactor theo DIP (Hệ thống Cảnh báo IoT với Dependency Injection)
// ============================================================================
class IAlarmSiren {
public:
    virtual ~IAlarmSiren() = default;
    virtual void triggerAlarm(const std::string &reason) = 0;
};

class SmartHomeSecuritySirens : public IAlarmSiren {
public:
    void triggerAlarm(const std::string &reason) override {
        std::cout << "  [Coi bao dong & SMS] CANH BAO KHAN CAP: " << reason << "!\n";
    }
};

class SmokeDetectorMonitor {
    IAlarmSiren &siren_;

public:
    explicit SmokeDetectorMonitor(IAlarmSiren &siren) : siren_(siren) {}

    void checkSmokeLevel(double ppm) {
        std::cout << "  [SmokeDetector] Nong do khoi do duoc: " << ppm << " ppm.\n";
        if (ppm > 150.0) {
            siren_.triggerAlarm("Nong do khoi vuot nguong an toan (" +
                                std::to_string(static_cast<int>(ppm)) + " ppm)");
        }
    }
};

void exercise5() {
    std::cout << "\n--- BAI 5: DEPENDENCY INVERSION PRINCIPLE (IOT ALARM MONITOR) ---\n";
    SmartHomeSecuritySirens realSiren;
    SmokeDetectorMonitor detector(realSiren);
    detector.checkSmokeLevel(45.0);
    detector.checkSmokeLevel(280.0);
}

// ============================================================================
// MENU CHÍNH (do-while + switch-case)
// ============================================================================
int main() {
    int choice = -1;
    do {
        std::cout << "\n=========================================================\n"
                  << "   BAI TAP THUC HANH: 5 NGUYEN LY SOLID (BAI 7.5)\n"
                  << "=========================================================\n"
                  << "  1. [S] Single Responsibility Principle (User Registration)\n"
                  << "  2. [O] Open/Closed Principle (Shipping Fee Rules)\n"
                  << "  3. [L] Liskov Substitution Principle (Bank Accounts)\n"
                  << "  4. [I] Interface Segregation Principle (Cloud Storage)\n"
                  << "  5. [D] Dependency Inversion Principle (IoT Smoke Detector)\n"
                  << "  6. Chay tat ca bai tap (1 -> 5)\n"
                  << "  0. Thoat\n"
                  << "---------------------------------------------------------\n"
                  << "Nhap lua chon cua ban: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[Canh bao] Dau vao khong hop le. Ket thuc chuong trinh.\n";
            break;
        }

        switch (choice) {
            case 1: exercise1(); break;
            case 2: exercise2(); break;
            case 3: exercise3(); break;
            case 4: exercise4(); break;
            case 5: exercise5(); break;
            case 6:
                exercise1();
                exercise2();
                exercise3();
                exercise4();
                exercise5();
                break;
            case 0:
                std::cout << "Tam biet!\n";
                break;
            default:
                std::cout << "Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

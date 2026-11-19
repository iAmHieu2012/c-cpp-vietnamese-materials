#include <iostream>
#include <string>
#include <iomanip>

class BankAccount {
private:
    // Thuộc tính (Attributes) luôn để private để bảo vệ dữ liệu khỏi sửa bậy
    std::string account_number;
    std::string owner_name;
    double balance;

public:
    // Khởi tạo dữ liệu hợp lệ qua phương thức public
    void init(const std::string &acc_num, const std::string &owner, double initial_balance) {
        account_number = acc_num;
        owner_name = owner;
        balance = (initial_balance >= 0.0) ? initial_balance : 0.0;
    }

    // Getter (Luôn có từ khóa 'const' ở cuối vì không sửa dữ liệu)
    std::string get_owner() const { return owner_name; }
    double get_balance() const { return balance; }

    // Các phương thức nghiệp vụ (Methods / Behaviors)
    void deposit(double amount) {
        if (amount <= 0.0) {
            std::cout << "  [Loi] So tien nap phai > 0!\n";
            return;
        }
        balance += amount;
        std::cout << "  [+] Da nap " << amount << " VND. So du moi: " << balance << "\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0.0 || amount > balance) {
            std::cout << "  [Loi] Rut tien that bai (So tien khong hop le hoac vuot so du)!\n";
            return false;
        }
        balance -= amount;
        std::cout << "  [-] Da rut " << amount << " VND. So du con lai: " << balance << "\n";
        return true;
    }

    void print_info() const {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "  STK: " << account_number << " | Chu TK: " << owner_name
                  << " | So du: " << balance << " VND\n";
    }
};

int main() {
    BankAccount acc1; // Tạo một Object (Instance) trên Stack
    acc1.init("VCB-001", "Nguyen Van An", 5000000);

    // acc1.balance = -999999; // LỖI BIÊN DỊCH! balance là private

    std::cout << "=== THONG TIN TAI KHOAN BAN DAU ===\n";
    acc1.print_info();

    std::cout << "\n=== THUC HIEN GIAO DICH ===\n";
    acc1.deposit(1500000);
    acc1.withdraw(2000000);
    acc1.withdraw(9000000); // Bị chặn bởi logic kiểm tra bên trong class

    return 0;
}
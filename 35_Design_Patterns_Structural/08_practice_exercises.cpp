#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

// =============================================================================
// BÀI 1: ADAPTER — TÍCH HỢP CỔNG THANH TOÁN NGOẠI TỆ (USD -> VND ADAPTER)
// =============================================================================
class IVndPayment {
public:
    virtual ~IVndPayment() = default;
    virtual void pay_vnd(long long amount_vnd) const = 0;
};

class StripeUsdService {
public:
    void charge_usd(double dollars) const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  [Stripe USD Service] Charged $" << dollars << " USD.\n";
    }
};

class StripeVndAdapter : public IVndPayment {
private:
    StripeUsdService stripe;
    double exchange_rate = 25000.0;
public:
    void pay_vnd(long long amount_vnd) const override {
        double usd = static_cast<double>(amount_vnd) / exchange_rate;
        std::cout << "  [Adapter] Chuyen doi " << amount_vnd << " VND -> $" << usd << " USD:\n";
        stripe.charge_usd(usd);
    }
};

void exercise_1_adapter() {
    std::cout << "\n[Bai 1 - Adapter] Thanh toan VND qua thu vien Stripe USD:\n";
    StripeVndAdapter adapter;
    adapter.pay_vnd(500000);
}


// =============================================================================
// BÀI 2: BRIDGE — ĐIỀU KHIỂN TỪ XA (REMOTE) & THIẾT BỊ (TV / RADIO)
// =============================================================================
class IDevice {
public:
    virtual ~IDevice() = default;
    virtual void set_volume(int percent) = 0;
};

class SmartTV : public IDevice {
public: void set_volume(int p) override { std::cout << "  [SmartTV] Am luong = " << p << "%\n"; }
};

class Soundbar : public IDevice {
public: void set_volume(int p) override { std::cout << "  [Soundbar] Am luong = " << p << "%\n"; }
};

class RemoteControl {
protected:
    IDevice &device;
public:
    explicit RemoteControl(IDevice &dev) : device(dev) {}
    void mute() { device.set_volume(0); }
};

void exercise_2_bridge() {
    std::cout << "\n[Bai 2 - Bridge] Dung chung RemoteControl cho SmartTV va Soundbar:\n";
    SmartTV tv;
    Soundbar bar;
    RemoteControl r1(tv), r2(bar);
    r1.mute();
    r2.mute();
}


// =============================================================================
// BÀI 3: COMPOSITE — CÂY TỔ CHỨC NHÂN SỰ CÔNG TY (TÍNH TỔNG LƯƠNG ĐỆ QUY)
// =============================================================================
class IOrgUnit {
public:
    virtual ~IOrgUnit() = default;
    virtual int total_salary() const = 0;
};

class IndividualWorker : public IOrgUnit {
    int salary;
public:
    explicit IndividualWorker(int s) : salary(s) {}
    int total_salary() const override { return salary; }
};

class DepartmentComposite : public IOrgUnit {
    std::vector<std::unique_ptr<IOrgUnit>> units;
public:
    void add(std::unique_ptr<IOrgUnit> u) { units.push_back(std::move(u)); }
    int total_salary() const override {
        int sum = 0;
        for (const auto &u : units) sum += u->total_salary();
        return sum;
    }
};

void exercise_3_composite() {
    auto company = std::make_unique<DepartmentComposite>();
    company->add(std::make_unique<IndividualWorker>(3000));

    auto eng_dept = std::make_unique<DepartmentComposite>();
    eng_dept->add(std::make_unique<IndividualWorker>(2000));
    eng_dept->add(std::make_unique<IndividualWorker>(2500));
    company->add(std::move(eng_dept));

    std::cout << "\n[Bai 3 - Composite] Tong quy luong toan cong ty (de quy) = $"
              << company->total_salary() << "\n";
}


// =============================================================================
// BÀI 4: DECORATOR — TÍNH GIÁ ĐỒ UỐNG & TOPPING QUÁN CAFE
// =============================================================================
class IBeverage {
public:
    virtual ~IBeverage() = default;
    virtual std::string description() const = 0;
    virtual double cost() const = 0;
};

class Espresso : public IBeverage {
public:
    std::string description() const override { return "Espresso"; }
    double cost() const override { return 2.50; }
};

class MilkDecorator : public IBeverage {
    std::unique_ptr<IBeverage> base;
public:
    explicit MilkDecorator(std::unique_ptr<IBeverage> b) : base(std::move(b)) {}
    std::string description() const override { return base->description() + " + Sua tuoi"; }
    double cost() const override { return base->cost() + 0.75; }
};

class CaramelDecorator : public IBeverage {
    std::unique_ptr<IBeverage> base;
public:
    explicit CaramelDecorator(std::unique_ptr<IBeverage> b) : base(std::move(b)) {}
    std::string description() const override { return base->description() + " + Caramel"; }
    double cost() const override { return base->cost() + 1.00; }
};

void exercise_4_decorator() {
    std::unique_ptr<IBeverage> drink = std::make_unique<Espresso>();
    drink = std::make_unique<MilkDecorator>(std::move(drink));
    drink = std::make_unique<CaramelDecorator>(std::move(drink));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 4 - Decorator] Do uong: " << drink->description()
              << " | Tong gia = $" << drink->cost() << "\n";
}


// =============================================================================
// BÀI 5: PROXY & CACHE — PROTECTION + CACHING PROXY CHO DỊCH VỤ DỮ LIỆU
// =============================================================================
class IDataService {
public:
    virtual ~IDataService() = default;
    virtual std::string query_report(const std::string &role, const std::string &key) = 0;
};

class RealDatabaseService : public IDataService {
public:
    std::string query_report(const std::string &, const std::string &key) override {
        std::cout << "    [RealDB] Truy van nang tren Database cho key '" << key << "'...\n";
        return "Report_Data_For_" + key;
    }
};

class SecurityCachingProxy : public IDataService {
    RealDatabaseService real_db;
    std::unordered_map<std::string, std::string> cache;
public:
    std::string query_report(const std::string &role, const std::string &key) override {
        if (role != "ADMIN") {
            return "[403 Forbidden] Chi ADMIN moi duoc truy cap bao cao!";
        }
        if (auto it = cache.find(key); it != cache.end()) {
            return "[Cache Hit] " + it->second;
        }
        std::string res = real_db.query_report(role, key);
        cache[key] = res;
        return res;
    }
};

void exercise_5_proxy() {
    std::cout << "\n[Bai 5 - Protection & Caching Proxy]:\n";
    SecurityCachingProxy proxy;
    std::cout << "  Guest goi : " << proxy.query_report("GUEST", "Q4_Revenue") << "\n";
    std::cout << "  Admin L1  : " << proxy.query_report("ADMIN", "Q4_Revenue") << "\n";
    std::cout << "  Admin L2  : " << proxy.query_report("ADMIN", "Q4_Revenue") << "\n";
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP STRUCTURAL PATTERNS
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 7.3 (STRUCTURAL PATTERNS) ==========\n";
        std::cout << "1. Adapter   : Tich hop cong thanh toan VND -> Stripe USD\n";
        std::cout << "2. Bridge    : Tach RemoteControl va Thiet bi (TV / Soundbar)\n";
        std::cout << "3. Composite : Cay to chuc nhan su cong ty (Tinh tong luong)\n";
        std::cout << "4. Decorator : Tinh gia Do uong & Topping quan Cafe\n";
        std::cout << "5. Proxy     : Protection & Caching Proxy cho Database\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_adapter(); break;
            case 2: exercise_2_bridge(); break;
            case 3: exercise_3_composite(); break;
            case 4: exercise_4_decorator(); break;
            case 5: exercise_5_proxy(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

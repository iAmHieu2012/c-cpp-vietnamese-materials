#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// =============================================================================
// BÀI 1: ABSTRACT FACTORY — HỌ TRÌNH ĐIỀU KHIỂN CƠ SỞ DỮ LIỆU (POSTGRES VS MYSQL)
// =============================================================================
class IDbConnection { public: virtual ~IDbConnection() = default; virtual void connect() const = 0; };
class IDbCommand    { public: virtual ~IDbCommand() = default;    virtual void execute(const std::string &q) const = 0; };

class PgConnection : public IDbConnection { public: void connect() const override { std::cout << "  [PostgreSQL] Connected port 5432.\n"; } };
class PgCommand    : public IDbCommand    { public: void execute(const std::string &q) const override { std::cout << "  [PostgreSQL] Exec: " << q << "\n"; } };

class IDbDriverFactory {
public:
    virtual ~IDbDriverFactory() = default;
    virtual std::unique_ptr<IDbConnection> create_connection() const = 0;
    virtual std::unique_ptr<IDbCommand>    create_command() const = 0;
};

class PostgresFactory : public IDbDriverFactory {
public:
    std::unique_ptr<IDbConnection> create_connection() const override { return std::make_unique<PgConnection>(); }
    std::unique_ptr<IDbCommand>    create_command()    const override { return std::make_unique<PgCommand>(); }
};

void exercise_1_abstract_factory_db() {
    std::cout << "\n[Bai 1 - Abstract Factory] Tao dong bo Connection + Command:\n";
    PostgresFactory pg_factory;
    auto conn = pg_factory.create_connection();
    auto cmd  = pg_factory.create_command();
    conn->connect();
    cmd->execute("SELECT * FROM users WHERE active = true;");
}


// =============================================================================
// BÀI 2: BUILDER — BỘ XÂY DỰNG CÂU TRUY VẤN SQL (FLUENT SQL QUERY BUILDER)
// =============================================================================
class SqlQueryBuilder {
private:
    std::string table_;
    std::string columns_ = "*";
    std::string where_;
    int limit_ = -1;

public:
    SqlQueryBuilder& select(std::string cols) { columns_ = std::move(cols); return *this; }
    SqlQueryBuilder& from(std::string tbl)    { table_   = std::move(tbl);  return *this; }
    SqlQueryBuilder& where(std::string cond)  { where_   = std::move(cond); return *this; }
    SqlQueryBuilder& limit(int n)             { limit_   = n;               return *this; }

    std::string build() const {
        std::string sql = "SELECT " + columns_ + " FROM " + table_;
        if (!where_.empty()) sql += " WHERE " + where_;
        if (limit_ > 0)      sql += " LIMIT " + std::to_string(limit_);
        sql += ";";
        return sql;
    }
};

void exercise_2_sql_query_builder() {
    std::string query = SqlQueryBuilder{}
                            .select("id, full_name, gpa")
                            .from("students")
                            .where("gpa >= 8.5")
                            .limit(10)
                            .build();
    std::cout << "\n[Bai 2 - Builder] Cau truy van sinh boi Fluent SqlQueryBuilder:\n  " << query << "\n";
}


// =============================================================================
// BÀI 3: FACTORY METHOD — HỆ THỐNG CỔNG THANH TOÁN (PAYMENT GATEWAY FACTORY)
// =============================================================================
class IPaymentProcessor {
public:
    virtual ~IPaymentProcessor() = default;
    virtual void pay(double amount) const = 0;
};

class MomoProcessor : public IPaymentProcessor {
public: void pay(double amount) const override { std::cout << "  [MoMo Wallet] Thanh toan QR: $" << amount << "\n"; }
};

class VisaProcessor : public IPaymentProcessor {
public: void pay(double amount) const override { std::cout << "  [Visa Card]   Tru the quoc te: $" << amount << "\n"; }
};

std::unique_ptr<IPaymentProcessor> create_payment_processor(const std::string &type) {
    if (type == "MOMO") return std::make_unique<MomoProcessor>();
    return std::make_unique<VisaProcessor>();
}

void exercise_3_factory_method_payment() {
    std::cout << "\n[Bai 3 - Factory Method] Khoi tao cong thanh toan dong:\n";
    auto p1 = create_payment_processor("MOMO");
    auto p2 = create_payment_processor("VISA");
    p1->pay(49.99);
    p2->pay(199.00);
}


// =============================================================================
// BÀI 4: PROTOTYPE — KHO MẪU QUÁI VẬT TRONG GAME (PROTOTYPE REGISTRY)
// =============================================================================
class Monster {
public:
    virtual ~Monster() = default;
    virtual std::unique_ptr<Monster> clone() const = 0;
    virtual void spawn_at(int x, int y) const = 0;
};

class Dragon : public Monster {
    std::string element;
    int hp;
public:
    Dragon(std::string el, int h) : element(std::move(el)), hp(h) {}
    std::unique_ptr<Monster> clone() const override {
        return std::make_unique<Dragon>(*this);
    }
    void spawn_at(int x, int y) const override {
        std::cout << "  [Spawn Dragon - " << element << " | HP=" << hp
                  << "] tai toa do (" << x << ", " << y << ")\n";
    }
};

void exercise_4_prototype_registry() {
    std::unordered_map<std::string, std::unique_ptr<Monster>> registry;
    registry["FireDragon"] = std::make_unique<Dragon>("Lua", 5000);
    registry["IceDragon"]  = std::make_unique<Dragon>("Bang", 4200);

    std::cout << "\n[Bai 4 - Prototype] Nhan ban nhanh quai vat tu Prototype Registry:\n";
    auto d1 = registry["FireDragon"]->clone();
    auto d2 = registry["IceDragon"]->clone();
    d1->spawn_at(10, 25);
    d2->spawn_at(80, 90);
}


// =============================================================================
// BÀI 5: SINGLETON — HỆ THỐNG GHI LOG ĐA LUỒNG (THREAD-SAFE LOGGER)
// =============================================================================
class AppLogger {
private:
    std::mutex mtx;
    int log_count = 0;
    AppLogger() = default;

public:
    AppLogger(const AppLogger&) = delete;
    AppLogger& operator=(const AppLogger&) = delete;

    static AppLogger& instance() {
        static AppLogger logger;
        return logger;
    }

    void info(const std::string &msg) {
        std::lock_guard<std::mutex> lk(mtx);
        std::cout << "  [LOG #" << ++log_count << "] " << msg << "\n";
    }
};

void exercise_5_singleton_logger() {
    std::cout << "\n[Bai 5 - Singleton] Goi AppLogger::instance() tu nhieu noi:\n";
    AppLogger::instance().info("Khoi dong ung dung thanh cong.");
    AppLogger::instance().info("Ket noi co so du lieu hoan tat.");
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP CREATIONAL PATTERNS
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 7.2 (CREATIONAL PATTERNS) ==========\n";
        std::cout << "1. Abstract Factory : Ho Driver Co so du lieu dong bo\n";
        std::cout << "2. Builder          : Bo xay dung truy van SqlQueryBuilder\n";
        std::cout << "3. Factory Method   : He thong cong thanh toan dong\n";
        std::cout << "4. Prototype        : Nhan ban quai vat tu Prototype Registry\n";
        std::cout << "5. Singleton        : Thread-Safe AppLogger\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_abstract_factory_db(); break;
            case 2: exercise_2_sql_query_builder(); break;
            case 3: exercise_3_factory_method_payment(); break;
            case 4: exercise_4_prototype_registry(); break;
            case 5: exercise_5_singleton_logger(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

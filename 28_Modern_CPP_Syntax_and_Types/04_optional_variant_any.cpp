#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <vector>

// =============================================================================
// 1. STD::OPTIONAL<T>: BIỂU DIỄN KẾT QUẢ CÓ THỂ TỒN TẠI HOẶC RỖNG (NULLOPT)
// =============================================================================
std::optional<double> safe_divide(double a, double b) {
    if (b == 0.0) return std::nullopt; // Không chia được cho 0 -> trả về rỗng an toàn
    return a / b;
}

// =============================================================================
// 2. STD::VARIANT<T1, T2...>: TYPE-SAFE UNION TRÊN STACK
// =============================================================================
using ConfigValue = std::variant<int, double, std::string>;

void print_config(const std::string &key, const ConfigValue &val) {
    std::cout << "  " << key << " = ";
    // std::visit tự động gọi nhánh Lambda tương ứng với kiểu thực tế đang lưu trong variant
    std::visit([](const auto &arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>)         std::cout << arg << " (int)\n";
        else if constexpr (std::is_same_v<T, double>) std::cout << arg << " (double)\n";
        else                                          std::cout << "\"" << arg << "\" (string)\n";
    }, val);
}

int main() {
    std::cout << "=== 1. STD::OPTIONAL (THAY THE SO MA THUAT -1 / NULL) ===\n";
    auto res1 = safe_divide(10.0, 4.0);
    auto res2 = safe_divide(10.0, 0.0);

    if (res1.has_value()) {
        std::cout << "  10.0 / 4.0 = " << *res1 << "\n";
    }
    // value_or(default): lấy giá trị nếu có, nếu là nullopt thì trả về giá trị mặc định
    std::cout << "  10.0 / 0.0 (voi value_or(0.0)) = " << res2.value_or(0.0) << "\n\n";

    std::cout << "=== 2. STD::VARIANT (TYPE-SAFE UNION + STD::VISIT) ===\n";
    ConfigValue v1 = 8080;
    ConfigValue v2 = 3.14159;
    ConfigValue v3 = std::string("localhost");

    print_config("port", v1);
    print_config("timeout", v2);
    print_config("host", v3);

    // Kiểm tra và lấy giá trị an toàn không ném ngoại lệ bằng std::get_if
    if (const auto *str_ptr = std::get_if<std::string>(&v3)) {
        std::cout << "  [get_if<string>] v3 dang giu chuoi: " << *str_ptr << "\n\n";
    }

    std::cout << "=== 3. STD::ANY (HOP CHUA BAT KY KIEU NAO CO KIEM TRA KIEU) ===\n";
    std::any box = 2026;
    std::cout << "  Kieu hien tai: " << box.type().name()
              << " | Gia tri = " << std::any_cast<int>(box) << "\n";

    box = std::string("Modern C++17 Any");
    try {
        std::cout << "  Sau khi doi sang string: " << std::any_cast<std::string>(box) << "\n";
        // Cố tình ép sai kiểu sang int sẽ ném ngoại lệ std::bad_any_cast (an toàn hơn void*!)
        std::cout << std::any_cast<int>(box) << "\n";
    } catch (const std::bad_any_cast &e) {
        std::cout << "  [Bat ngoai le an toan] Loi ep sai kieu: " << e.what() << "\n";
    }

    return 0;
}

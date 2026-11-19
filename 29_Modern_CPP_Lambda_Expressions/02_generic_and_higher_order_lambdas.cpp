#include <iostream>
#include <string>
#include <vector>
#include <functional>

// =============================================================================
// HIGHER-ORDER FUNCTION (CLOSURE FACTORY): HÀM TRẢ VỀ MỘT LAMBDA
// Lưu ý sống còn: BẮT BUỘC capture theo giá trị [tax_rate, fee] vì biến cục bộ sẽ hủy khi return!
// =============================================================================
auto make_price_calculator(double tax_rate, double shipping_fee) {
    return [tax_rate, shipping_fee](double raw_price) {
        return raw_price * (1.0 + tax_rate) + shipping_fee;
    };
}

int main() {
    std::cout << "=== 1. GENERIC LAMBDA (C++14: THAM SO AUTO DUNG CHO MOI KIEU) ===\n";
    // Một Lambda duy nhất cộng được cả int, double lẫn nối chuỗi std::string!
    auto generic_add = [](const auto &a, const auto &b) {
        return a + b;
    };

    std::cout << "  generic_add(10, 25)              = " << generic_add(10, 25) << "\n";
    std::cout << "  generic_add(3.14, 2.86)          = " << generic_add(3.14, 2.86) << "\n";
    std::cout << "  generic_add(string, string)      = "
              << generic_add(std::string("Modern "), std::string("Lambda")) << "\n\n";

    std::cout << "=== 2. CLOSURE FACTORY (HAM SINH RA HAM CO NHO TRANG THAI) ===\n";
    auto vn_vat_calc = make_price_calculator(0.08, 2.0);  // Thuế 8%, ship $2
    auto us_tax_calc = make_price_calculator(0.10, 15.0); // Thuế 10%, ship $15

    std::cout << "  Gia san pham $100 tai VN (8% + $2)   = $" << vn_vat_calc(100.0) << "\n";
    std::cout << "  Gia san pham $100 tai US (10% + $15) = $" << us_tax_calc(100.0) << "\n\n";

    std::cout << "=== 3. LUU NHIEU LAMBDA VAO VECTOR BANG STD::FUNCTION ===\n";
    std::vector<std::function<int(int, int)>> operations = {
        [](int a, int b) { return a + b; },
        [](int a, int b) { return a - b; },
        [](int a, int b) { return a * b; }
    };

    const char *names[] = {"Cong (+)", "Tru  (-)", "Nhan (*)"};
    for (std::size_t i = 0; i < operations.size(); ++i) {
        std::cout << "  " << names[i] << " voi (12, 4) -> " << operations[i](12, 4) << "\n";
    }

    return 0;
}

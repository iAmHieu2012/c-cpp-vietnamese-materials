#include <iostream>
#include <string>
#include <utility> // std::move, std::forward

void target_handler(const std::string &s) {
    std::cout << "    [target_handler] Nhan LVALUE (Copy/Read-only): \"" << s << "\"\n";
}

void target_handler(std::string &&s) {
    std::cout << "    [target_handler] Nhan RVALUE (Move-ready!)   : \"" << s << "\"\n";
}

// =============================================================================
// 1. WRAPPER SAI: KHÔNG DÙNG STD::FORWARD
// Vì biến `arg` bên trong hàm đã có tên nên nó luôn bị coi là Lvalue!
// =============================================================================
template <typename T>
void bad_Relay(T &&arg) {
    target_handler(arg); // Luôn gọi bản Lvalue ngay cả khi người gọi truyền Rvalue!
}

// =============================================================================
// 2. WRAPPER CHUẨN: UNIVERSAL REFERENCE (T&&) + STD::FORWARD<T>
// Bảo toàn 100% bản chất Lvalue hoặc Rvalue của đối số gốc!
// =============================================================================
template <typename T>
void perfect_relay(T &&arg) {
    target_handler(std::forward<T>(arg));
}

int main() {
    std::string my_str = "Du_Lieu_Goc";

    std::cout << "=== 1. CAM BAY KHI QUEN DUNG STD::FORWARD (BIEN CO TEN LUON LA LVALUE) ===\n";
    std::cout << "  Truyen Lvalue vao bad_Relay:\n";
    bad_Relay(my_str);
    std::cout << "  Truyen Rvalue tam vao bad_Relay (Bi mat tinh Rvalue!):\n";
    bad_Relay(std::string("Chuoi_Tam_Rvalue"));

    std::cout << "\n=== 2. PERFECT FORWARDING VOI STD::FORWARD<T>(ARG) ===\n";
    std::cout << "  Truyen Lvalue vao perfect_relay:\n";
    perfect_relay(my_str);
    std::cout << "  Truyen Rvalue tam vao perfect_relay (Giu nguyen tinh Rvalue!):\n";
    perfect_relay(std::string("Chuoi_Tam_Rvalue"));
    std::cout << "  Truyen std::move(my_str) vao perfect_relay:\n";
    perfect_relay(std::move(my_str));

    return 0;
}

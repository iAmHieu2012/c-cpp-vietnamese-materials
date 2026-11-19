#include <iostream>
#include <string>
#include <utility> // std::move

// Nạp chồng 3 phiên bản để xem trình biên dịch chọn hàm nào cho Lvalue, Const Lvalue và Rvalue
void inspect_reference(std::string &s) {
    std::cout << "  -> Goi ban [1] Lvalue Reference (std::string&): \"" << s << "\"\n";
}

void inspect_reference(const std::string &s) {
    std::cout << "  -> Goi ban [2] Const Lvalue Reference (const std::string&): \"" << s << "\"\n";
}

void inspect_reference(std::string &&s) {
    std::cout << "  -> Goi ban [3] Rvalue Reference (std::string&& - Co the cuop Heap!): \"" << s << "\"\n";
}

int main() {
    std::cout << "=== 1. PHAN BIET LVALUE VS RVALUE QUA OVERLOAD RESOLUTION ===\n";
    std::string lval = "Bien_Lvalue";
    const std::string const_lval = "Bien_Const_Lvalue";

    inspect_reference(lval);                        // Gọi [1] vì lval là biến có tên, sửa được
    inspect_reference(const_lval);                  // Gọi [2] vì const_lval là hằng
    inspect_reference(std::string("Tam_Rvalue"));   // Gọi [3] vì đối tượng tạm là Rvalue
    inspect_reference(lval + "_Ghep_Tam");          // Gọi [3] vì kết quả phép '+' là Rvalue tạm thời

    std::cout << "\n=== 2. BAN CHAT THAT SU CUA STD::MOVE (STATIC_CAST<T&&>) ===\n";
    std::cout << "  Goi inspect_reference(std::move(lval)):\n";
    // std::move chỉ ép kiểu lval thành std::string&& để kích hoạt hàm [3], bản thân inspect_reference chưa cướp dữ liệu
    inspect_reference(std::move(lval));
    std::cout << "  Sau khi chi goi std::move ma chua gan Move, lval van con nguyen: \"" << lval << "\"\n\n";

    std::cout << "  Bay gio thuc hien khoi tao Move: std::string stolen = std::move(lval);\n";
    std::string stolen = std::move(lval); // Lúc này Move Constructor của std::string mới thực sự rút ruột lval!
    std::cout << "  -> stolen = \"" << stolen << "\"\n";
    std::cout << "  -> lval (sau khi bi Move Constructor rut ruot) = \"" << lval << "\" (size=" << lval.size() << ")\n";

    return 0;
}

#include <iostream>
#include <string>

// =============================================================================
// 1. KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES) & ĐỐI SỐ MẶC ĐỊNH
// =============================================================================
// Quy tắc vàng: Các tham số có giá trị mặc định PHẢI nằm dồn về phía bên phải cùng!
// Và chỉ khai báo giá trị mặc định ở Prototype (hoặc ở Definition nếu không có Prototype).
double calculate_bill(double price, int quantity = 1, double tax_rate = 0.08);

// =============================================================================
// 2. NẠP CHỒNG HÀM (FUNCTION OVERLOADING) - CÙNG TÊN 'add', KHÁC THAM SỐ
// =============================================================================
// Phiên bản 1: Cộng 2 số nguyên
int add(int a, int b) {
    std::cout << "[Goi add(int, int)]         -> ";
    return a + b;
}

// Phiên bản 2: Cộng 3 số nguyên (Khác số lượng tham số)
int add(int a, int b, int c) {
    std::cout << "[Goi add(int, int, int)]    -> ";
    return a + b + c;
}

// Phiên bản 3: Cộng 2 số thực (Khác kiểu dữ liệu tham số)
double add(double a, double b) {
    std::cout << "[Goi add(double, double)]   -> ";
    return a + b;
}

// Phiên bản 4: Nối 2 chuỗi
std::string add(const std::string &a, const std::string &b) {
    std::cout << "[Goi add(string, string)]   -> ";
    return a + " " + b;
}

// Hàm inline: Gợi ý trình biên dịch chèn thẳng code vào nơi gọi để tăng tốc cho hàm cực ngắn
inline int square(int x) {
    return x * x;
}

int main() {
    std::cout << "=== 1. DEMO FUNCTION OVERLOADING ===\n";
    std::cout << add(3, 5) << "\n";
    std::cout << add(3, 5, 10) << "\n";
    std::cout << add(2.5, 4.3) << "\n";
    std::cout << add(std::string("Hello"), std::string("C++")) << "\n\n";

    std::cout << "=== 2. DEMO DEFAULT ARGUMENTS ===\n";
    // Gọi chỉ với 1 đối số (quantity tự nhận 1, tax_rate tự nhận 0.08)
    std::cout << "Mua 1 mon 100k (mac dinh): " << calculate_bill(100.0) << "\n";
    // Gọi với 2 đối số (quantity = 3, tax_rate tự nhận 0.08)
    std::cout << "Mua 3 mon 100k (thue 8%):  " << calculate_bill(100.0, 3) << "\n";
    // Gọi truyền đủ cả 3 đối số (ghi đè toàn bộ mặc định)
    std::cout << "Mua 3 mon 100k (thue 10%): " << calculate_bill(100.0, 3, 0.10) << "\n\n";

    std::cout << "=== 3. DEMO INLINE FUNCTION ===\n";
    std::cout << "Binh phuong cua 9 la: " << square(9) << "\n";

    return 0;
}

// Định nghĩa chi tiết hàm ở bên dưới main() (Không viết lại giá trị mặc định ở đây!)
double calculate_bill(double price, int quantity, double tax_rate) {
    double subtotal = price * quantity;
    return subtotal + (subtotal * tax_rate);
}
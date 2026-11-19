#include <iostream>

int main() {
    int x = 42;
    int y = 100;

    // 1. Khai báo con trỏ p trỏ tới địa chỉ của biến x
    int *p = &x;

    std::cout << "=== 1. BAN CHAT CON TRO (& VA *) ===\n";
    std::cout << "Gia tri cua x         (x)  : " << x << "\n";
    std::cout << "Dia chi cua x         (&x) : " << &x << "\n";
    std::cout << "Gia chi p dang luu    (p)  : " << p << " (Chinh la &x!)\n";
    std::cout << "Dia chi rieng cua p   (&p) : " << &p << " (O nho 8 bytes cua rieng p tren Stack)\n";
    std::cout << "Gia tri tai noi p tro (*p) : " << *p << " (Dereference - Giai tham chieu)\n\n";

    // Thay đổi giá trị của x thông qua con trỏ *p
    *p = 999;
    std::cout << "Sau khi gan *p = 999 -> Bien goc x = " << x << "\n\n";


    std::cout << "=== 2. KICH THUOC CON TRO LUON CO DINH (8 BYTES TREN 64-BIT) ===\n";
    std::cout << "sizeof(char*)      = " << sizeof(char*)      << " bytes\n";
    std::cout << "sizeof(int*)       = " << sizeof(int*)       << " bytes\n";
    std::cout << "sizeof(long long*) = " << sizeof(long long*) << " bytes\n";
    std::cout << "sizeof(double*)    = " << sizeof(double*)    << " bytes\n\n";


    std::cout << "=== 3. BA DANG CONST POINTER KINH DIEN ===\n";
    // Dạng 1: Con trỏ trỏ tới hằng (Pointer to const) -> Cấm sửa *p1, cho phép đổi p1 = &y
    const int *p1 = &x;
    // *p1 = 10; // LỖI BIÊN DỊCH!
    p1 = &y;     // HỢP LỆ: Đổi hướng sang trỏ vào y
    std::cout << "p1 (Pointer to const) dang tro toi y = " << *p1 << "\n";

    // Dạng 2: Con trỏ hằng (Const pointer) -> Cho phép sửa *p2, CẤM đổi hướng p2 = &y
    int * const p2 = &x;
    *p2 = 777;   // HỢP LỆ: Sửa giá trị của x thành 777
    // p2 = &y;  // LỖI BIÊN DỊCH!
    std::cout << "p2 (Const pointer) da sua x thanh    = " << x << "\n";

    // 4. Luôn khởi tạo con trỏ bằng nullptr (C++11) nếu chưa có địa chỉ cụ thể
    int *safe_ptr = nullptr;
    if (safe_ptr == nullptr) {
        std::cout << "\nsafe_ptr dang la nullptr (An toan, khong tro lung tung)!\n";
    }

    return 0;
}
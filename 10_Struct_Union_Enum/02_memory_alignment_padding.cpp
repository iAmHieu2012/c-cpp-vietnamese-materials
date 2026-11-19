#include <iostream>
#include <cstddef> // Dùng cho macro offsetof(type, member)

// 1. Thiết kế KÉM TỐI ƯU: Đặt xen kẽ biến 1 byte và biến 4 bytes
struct BadLayout {
    char a;   // 1 byte  (Tại offset 0) + [Chèn 3 bytes Padding ở offset 1,2,3 để b nằm ở offset 4!]
    int b;    // 4 bytes (Tại offset 4..7)
    char c;   // 1 byte  (Tại offset 8) + [Chèn 3 bytes Padding ở offset 9,10,11 để tổng chia hết cho 4!]
}; // -> Tổng cộng: 12 bytes (Lãng phí mất 6 bytes Padding!)

// 2. Thiết kế TỐI ƯU: Gom các biến theo thứ tự kích thước giảm dần (hoặc tăng dần)
struct GoodLayout {
    int b;    // 4 bytes (Tại offset 0..3)
    char a;   // 1 byte  (Tại offset 4)
    char c;   // 1 byte  (Tại offset 5) + [Chỉ cần 2 bytes Padding ở offset 6,7 để chia hết cho 4]
}; // -> Tổng cộng: 8 bytes (Tiết kiệm ngay 33% RAM cho cùng một lượng thông tin!)

// 3. Ép trình biên dịch bỏ hoàn toàn Padding bằng #pragma pack(1)
// (Chỉ dùng khi đóng gói dữ liệu truyền qua mạng hoặc ghi header file nhị phân)
#pragma pack(push, 1)
struct PackedLayout {
    char a;   // 1 byte (offset 0)
    int b;    // 4 bytes (offset 1..4) -> Không căn chỉnh!
    char c;   // 1 byte (offset 5)
}; // -> Tổng cộng: Đúng 6 bytes khít rịt!
#pragma pack(pop)

int main() {
    std::cout << "=== 1. SO SANH KICH THUOC DO STRUCT PADDING ===\n";
    std::cout << "sizeof(BadLayout)    = " << sizeof(BadLayout)    << " bytes (Lang phi 6 bytes padding!)\n";
    std::cout << "sizeof(GoodLayout)   = " << sizeof(GoodLayout)   << " bytes (Chi doi cho bien, giam 4 bytes!)\n";
    std::cout << "sizeof(PackedLayout) = " << sizeof(PackedLayout) << " bytes (Dung #pragma pack(1))\n\n";

    std::cout << "=== 2. SOI VI TRI OFFSET CUA TUNG BIEN TRONG BADLAYOUT ===\n";
    std::cout << "  Offset cua a (1 byte) : " << offsetof(BadLayout, a) << "\n";
    std::cout << "  Offset cua b (4 bytes): " << offsetof(BadLayout, b) << " (Nhay coc qua 3 bytes padding 1,2,3!)\n";
    std::cout << "  Offset cua c (1 byte) : " << offsetof(BadLayout, c) << "\n\n";

    std::cout << "=== 3. SOI VI TRI OFFSET CUA TUNG BIEN TRONG GOODLAYOUT ===\n";
    std::cout << "  Offset cua b (4 bytes): " << offsetof(GoodLayout, b) << "\n";
    std::cout << "  Offset cua a (1 byte) : " << offsetof(GoodLayout, a) << "\n";
    std::cout << "  Offset cua c (1 byte) : " << offsetof(GoodLayout, c) << " (Nam sat ngay sau a!)\n";

    return 0;
}
#include <iostream>
#include <cstdint> // uint32_t, uint8_t

// 1. Ứng dụng thực tế cực hay của UNION: Tách 1 địa chỉ IPv4 (32-bit) thành 4 cụm 8-bit!
union IPv4Converter {
    uint32_t full_address;  // 4 bytes nguyên khối
    uint8_t  octets[4];     // Mảng 4 phần tử 1 byte dùng chung chính xác 4 bytes của full_address!
};

// 2. Scoped Enum (enum class - Chuẩn C++ hiện đại)
enum class OrderStatus {
    Pending = 1,
    Shipping = 2,
    Delivered = 3,
    Cancelled = 4
};

const char* status_to_string(OrderStatus st) {
    switch (st) {
        case OrderStatus::Pending:   return "Dang cho xu ly";
        case OrderStatus::Shipping:  return "Dang giao hang";
        case OrderStatus::Delivered: return "Giao thanh cong";
        case OrderStatus::Cancelled: return "Da huy don";
        default:                     return "Khong xac dinh";
    }
}

int main() {
    std::cout << "=== 1. BAN CHAT DUNG CHUNG VUNG NHO CUA UNION ===\n";
    IPv4Converter ip;
    std::cout << "sizeof(IPv4Converter) = " << sizeof(ip) << " bytes (Bang kich thuoc thanh vien lon nhat)\n";
    std::cout << "Dia chi &ip.full_address: " << &ip.full_address << "\n";
    std::cout << "Dia chi &ip.octets[0]   : " << static_cast<void*>(&ip.octets[0]) << " (TRUNG KHIT 100%!)\n\n";

    // Gán từng byte của địa chỉ IP: 192.168.1.100
    ip.octets[3] = 192;
    ip.octets[2] = 168;
    ip.octets[1] = 1;
    ip.octets[0] = 100;

    std::cout << "Bieu dien 4 octets : " 
              << (int)ip.octets[3] << "." << (int)ip.octets[2] << "." 
              << (int)ip.octets[1] << "." << (int)ip.octets[0] << "\n";
    std::cout << "Gia tri 32-bit Hex : 0x" << std::hex << std::uppercase << ip.full_address << std::dec << "\n\n";


    std::cout << "=== 2. SCOPED ENUM (ENUM CLASS TRONG C++) ===\n";
    OrderStatus current_order = OrderStatus::Shipping;
    std::cout << "Trang thai don hang: " << status_to_string(current_order) 
              << " (Ma so: " << static_cast<int>(current_order) << ")\n";

    return 0;
}
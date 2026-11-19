#include <iostream>
#include <bitset> // Thư viện giúp in số dưới dạng nhị phân trực quan

int main() {
    unsigned char a = 5;  // Nhị phân: 0000 0101
    unsigned char b = 9;  // Nhị phân: 0000 1001

    std::cout << "a = " << std::bitset<8>(a) << " (" << (int)a << ")\n";
    std::cout << "b = " << std::bitset<8>(b) << " (" << (int)b << ")\n\n";

    std::cout << "a & b (AND): " << std::bitset<8>(a & b) << " (" << (a & b) << ")\n";
    std::cout << "a | b (OR) : " << std::bitset<8>(a | b) << " (" << (a | b) << ")\n";
    std::cout << "a ^ b (XOR): " << std::bitset<8>(a ^ b) << " (" << (a ^ b) << ")\n";
    std::cout << "~a    (NOT): " << std::bitset<8>(~a)    << "\n";
    std::cout << "a << 1 (Nhan 2): " << std::bitset<8>(a << 1) << " (" << (a << 1) << ")\n";
    std::cout << "b >> 1 (Chia 2): " << std::bitset<8>(b >> 1) << " (" << (b >> 1) << ")\n\n";

    // === 4 TUYỆT CHIÊU BITWISE KINH ĐIỂN CẦN NHỚ ===
    int n = 13; // 0000 1101
    // 1. Kiểm tra chẵn lẻ cực nhanh (nhanh hơn n % 2)
    bool is_odd = (n & 1); 
    std::cout << n << " la so: " << (is_odd ? "Le" : "Chan") << "\n";

    // 2. Tính 2^k bằng dịch bit: (1 << k)
    int k = 4;
    std::cout << "2 mu " << k << " = " << (1 << k) << "\n";

    // 3. Kiểm tra bit thứ i của n đang là 0 hay 1
    int i = 2;
    int bit_i = (n >> i) & 1;
    std::cout << "Bit thu " << i << " cua " << n << " la: " << bit_i << "\n";

    // 4. Kiểm tra một số có phải lũy thừa của 2 không (2, 4, 8, 16...)
    int x = 16;
    bool is_power_of_2 = (x > 0) && ((x & (x - 1)) == 0);
    std::cout << x << " co phai luy thua cua 2? -> " << is_power_of_2 << "\n";

    return 0;
}
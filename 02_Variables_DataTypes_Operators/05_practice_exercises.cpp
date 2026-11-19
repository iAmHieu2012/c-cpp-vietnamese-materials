#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <bitset>

int main()
{
    unsigned int N = 0;
    std::cout << "Bai 1: Chuyen doi giay sang hh:mm:ss" << std::endl;
    std::cout << "Nhap vao so giay N: ";
    std::cin >> N;
    unsigned int h = N / 3600;
    unsigned int m = (N % 3600) / 60;
    unsigned int s = N % 60;

    printf("%02u:%02u:%02u\n", h, m, s);
    std::cout << std::setfill('0') << std::setw(2) << h << ":"
              << std::setfill('0') << std::setw(2) << m << ":"
              << std::setfill('0') << std::setw(2) << s << std::endl;
    std::cout << std::setfill(' ') << std::endl;

    std::cout << "Bai 2: Hoan doi gia tri 2 bien" << std::endl;
    int a, b;
    std::cout << "Nhap a: ";
    std::cin >> a;
    std::cout << "Nhap b: ";
    std::cin >> b;
    std::cout << "Truoc khi hoan doi: a = " << a << ", b = " << b << std::endl;
    a = a ^ b;
    b = b ^ a;
    a = a ^ b;
    std::cout << "Sau khi hoan doi: a = " << a << ", b = " << b << std::endl
              << std::endl;

    std::cout << "Bai 3: Tim max cua 3 so" << std::endl;
    int x, y, z;
    std::cout << "Nhap 3 so x, y, z: ";
    std::cin >> x >> y >> z;
    std::cout << "Max cua 3 so la: " << (x > y ? (x > z ? x : z) : (y > z ? y : z)) << std::endl << std::endl;

    std::cout << "Bai 4: Thao tac bit: bat, tat, dao bit" << std::endl;
    int n, k;
    std::cout << "Nhap so n: "; std::cin >> n;
    std::cout << "Nhap so k: "; std::cin >> k;
    std::cout << "Bieu dien nhi phan cua n: " << std::bitset<8>(n) << std::endl;
    std::cout << "Bieu dien nhi phan cua khi bat bit k: " << std::bitset<8>(n | (1 << k)) << std::endl;
    std::cout << "Bieu dien nhi phan cua khi tat bit k: " << std::bitset<8>(n & ~(1 << k)) << std::endl;
    std::cout << "Bieu dien nhi phan cua khi dao bit k: " << std::bitset<8>(n ^ (1 << k)) << std::endl;
    std::cout << std::endl;

    std::cout << "Bai 5: Chuyen doi chu hoa va thuong" << std::endl;
    char ch;
    std::cout << "Nhap 1 ky tu chu cai (A-Z hoac a-z): ";
    std::cin >> ch;

    // TUYỆT CHIÊU BITWISE: XOR với ký tự dấu cách ' ' (có mã ASCII là 32 = 0010 0000)
    // Tự động đổi Hoa -> Thường VÀ Thường -> Hoa chỉ bằng 1 phép toán!
    char toggled_ch = ch ^ ' '; 
    
    // Bonus: Ép về chữ thường bằng OR (' ') và ép về chữ hoa bằng AND ('_')
    char forced_lower = ch | ' '; // Luôn ra chữ thường dù nhập Hoa hay Thường
    char forced_upper = ch & '_'; // '_' có ASCII là 95 (0101 1111) -> Luôn ra chữ Hoa

    std::cout << "Ky tu goc: '" << ch << "' (ASCII: " << (int)ch 
              << " | Binary: " << std::bitset<8>(ch) << ")\n";
    std::cout << "1. Dao Hoa <-> Thuong (ch ^ ' '): '" << toggled_ch << "' (ASCII: " << (int)toggled_ch 
              << " | Binary: " << std::bitset<8>(toggled_ch) << ")\n";
    std::cout << "2. Ep ve chu thuong   (ch | ' '): '" << forced_lower << "'\n";
    std::cout << "3. Ep ve chu HOA      (ch & '_'): '" << forced_upper << "'\n";
    return 0;
}
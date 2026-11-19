#include <iostream>

int main()
{
    // =========================================================================
    // 1. KIỂM TRA SỐ NGUYÊN TỐ TỐI ƯU O(sqrt(N))
    // =========================================================================
    int p;
    std::cout << "Nhap so nguyen p de kiem tra nguyen to: ";
    std::cin >> p;

    bool is_prime = (p >= 2); // Số nguyên tố phải >= 2
    // Thay vì dùng i <= sqrt(p) (tốn chi phí gọi hàm số thực), hãy dùng i * i <= p
    for (int i = 2; 1LL * i * i <= p; ++i)
    {
        if (p % i == 0)
        {
            is_prime = false;
            break; // Tìm thấy 1 ước số là kết luận hợp số ngay, không cần chạy tiếp!
        }
    }
    std::cout << p << (is_prime ? " LA so nguyen to!\n\n" : " KHONG phai so nguyen to!\n\n");

    // =========================================================================
    // 2. VÒNG LẶP LỒNG NHAU (NESTED LOOPS): VẼ HÌNH CHỮ NHẬT RỖNG & TAM GIÁC CÂN
    // =========================================================================
    int h = 5; // Chiều cao
    std::cout << "=== Hinh 1: Hinh vuong rong kich thuoc " << h << "x" << h << " ===\n";
    for (int row = 1; row <= h; ++row)
    {
        for (int col = 1; col <= h; ++col)
        {
            // Chỉ in dấu '*' ở viền ngoài (hàng đầu, hàng cuối, cột đầu, cột cuối)
            if (row == 1 || row == h || col == 1 || col == h)
            {
                std::cout << "* ";
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n=== Hinh 2: Tam giac can (Kim tu thap) chieu cao " << h << " ===\n";
    for (int row = 1; row <= h; ++row)
    {
        // 1. In (h - row) khoảng trắng để căn giữa
        for (int space = 1; space <= h - row; ++space)
        {
            std::cout << "  ";
        }
        // 2. In (2 * row - 1) ngôi sao
        for (int star = 1; star <= 2 * row - 1; ++star)
        {
            std::cout << "* ";
        }
        std::cout << "\n";
    }

    return 0;
}
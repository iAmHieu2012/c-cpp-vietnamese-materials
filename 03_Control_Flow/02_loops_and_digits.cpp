#include <iostream>

int main()
{
    // =========================================================================
    // 1. DO-WHILE: BẮT BUỘC NHẬP DỮ LIỆU ĐÚNG (INPUT VALIDATION)
    // =========================================================================
    int n;
    do
    {
        std::cout << "Nhap mot so nguyen duong N (N > 0): ";
        std::cin >> n;
        if (n <= 0)
        {
            std::cout << "Nhap sai! Vui long nhap lai.\n";
        }
    } while (n <= 0); // Lặp lại nếu người dùng cố tình nhập số <= 0

    // =========================================================================
    // 2. FOR + CONTINUE / BREAK: DUYỆT SỐ CÓ ĐIỀU KIỆN
    // =========================================================================
    std::cout << "\nCac so le tu 1 den " << n << " (bo qua so chia het cho 5, dung neu > 20):\n";
    for (int i = 1; i <= n; ++i)
    {
        if (i > 20)
        {
            std::cout << "[Dung som do i > 20!]";
            break; // Thoát hoàn toàn khỏi vòng lặp for ngay lập tức
        }
        if (i % 2 == 0 || i % 5 == 0)
        {
            continue; // Bỏ qua phần code bên dưới, nhảy sang bước lặp (++i) tiếp theo
        }
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    // =========================================================================
    // 3. WHILE: KỸ THUẬT TÁCH CHỮ SỐ & ĐẢO NGƯỢC SỐ (CỰC KỲ QUAN TRỌNG)
    // =========================================================================
    // Công thức vàng:
    // - Lấy chữ số cuối cùng: last_digit = temp % 10
    // - Bỏ chữ số cuối cùng:  temp = temp / 10
    int temp = n;
    int sum_digits = 0;
    int reversed_n = 0;

    while (temp > 0)
    {
        int last_digit = temp % 10;                // Lấy chữ số hàng đơn vị
        sum_digits += last_digit;                  // Cộng dồn vào tổng
        reversed_n = reversed_n * 10 + last_digit; // Đẩy chữ số vào số đảo ngược
        temp /= 10;                                // Loại bỏ chữ số hàng đơn vị
    }

    std::cout << "=== Phan tich so N = " << n << " ===\n";
    std::cout << "- Tong cac chu so: " << sum_digits << "\n";
    std::cout << "- So dao nguoc:    " << reversed_n << "\n";
    std::cout << "- Co phai so Thuan Nghich (Palindrome)? -> "
              << (n == reversed_n ? "CO" : "KHONG") << "\n";

    return 0;
}
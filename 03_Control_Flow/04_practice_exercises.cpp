#include <iostream>
#include <cmath>   // Dùng cho std::sqrt, std::abs
#include <iomanip> // Dùng cho std::fixed, std::setprecision

int main()
{
    int choice;
    const double EPS = 1e-9; // Hằng số sai số cực nhỏ để so sánh số thực với 0

    // =========================================================================
    // BÀI 5 (KHUNG CHÍNH): MENU TƯƠNG TÁC BẰNG DO-WHILE & SWITCH-CASE
    // =========================================================================
    do
    {
        std::cout << "\n================ MENU BAI TAP 1.3 ================\n";
        std::cout << "1. Giai phuong trinh bac 2 (ax^2 + bx + c = 0)\n";
        std::cout << "2. Tim UCLN (GCD) va BCNN (LCM) bang Euclid\n";
        std::cout << "3. In N so Fibonacci dau tien (N <= 90)\n";
        std::cout << "4. Phan tich thua so nguyen to (Prime Factorization)\n";
        std::cout << "5. May tinh bo tui (+, -, *, /)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==================================================\n";
        std::cout << "Nhap lua chon cua ban (0-5): ";
        std::cin >> choice;

        switch (choice)
        {
        // =================================================================
        // BÀI 1: GIẢI PHƯƠNG TRÌNH BẬC 2 (BIỆN LUẬN ĐẦY ĐỦ MỌI TRƯỜNG HỢP)
        // =================================================================
        case 1:
        {
            double a, b, c;
            std::cout << "\n[Bai 1] Nhap 3 he so a, b, c: ";
            std::cin >> a >> b >> c;
            std::cout << std::fixed << std::setprecision(4);

            // Lưu ý: Không dùng (a == 0) cho số thực, dùng std::abs(a) < EPS
            if (std::abs(a) < EPS)
            {
                // Suy biến thành phương trình bậc nhất: bx + c = 0
                if (std::abs(b) < EPS)
                {
                    if (std::abs(c) < EPS)
                    {
                        std::cout << "-> Phuong trinh co VO SO nghiem!\n";
                    }
                    else
                    {
                        std::cout << "-> Phuong trinh VO NGHIEM!\n";
                    }
                }
                else
                {
                    double x = -c / b;
                    // Tránh in ra "-0.0000" khi c = 0
                    if (std::abs(x) < EPS)
                        x = 0.0;
                    std::cout << "-> PT bac nhat co 1 nghiem: x = " << x << "\n";
                }
            }
            else
            {
                // Phương trình bậc 2 thực sự (a != 0)
                double delta = b * b - 4 * a * c;
                if (std::abs(delta) < EPS)
                {
                    double x = -b / (2 * a);
                    if (std::abs(x) < EPS)
                        x = 0.0;
                    std::cout << "-> PT co nghiem kep: x1 = x2 = " << x << "\n";
                }
                else if (delta > 0)
                {
                    double sqrt_d = std::sqrt(delta);
                    double x1 = (-b - sqrt_d) / (2 * a);
                    double x2 = (-b + sqrt_d) / (2 * a);
                    std::cout << "-> PT co 2 nghiem phan biet:\n";
                    std::cout << "   x1 = " << x1 << "\n   x2 = " << x2 << "\n";
                }
                else
                {
                    std::cout << "-> PT VO NGHIEM thuc (Delta < 0)!\n";
                }
            }
            break;
        }

        // =================================================================
        // BÀI 2: TÌM GCD & LCM BẰNG THUẬT TOÁN EUCLID
        // =================================================================
        case 2:
        {
            long long a, b;
            std::cout << "\n[Bai 2] Nhap 2 so nguyen duong a va b: ";
            std::cin >> a >> b;

            if (a <= 0 || b <= 0)
            {
                std::cout << "-> Vui long nhap so nguyen duong (> 0)!\n";
                break;
            }

            long long orig_a = a, orig_b = b;
            // Thuật toán Euclid: GCD(a, b) = GCD(b, a % b)
            while (b != 0)
            {
                long long r = a % b;
                a = b;
                b = r;
            }
            long long gcd = a;
            // QUAN TRỌNG: Chia trước rồi mới nhân để tránh tràn số (Overflow)!
            long long lcm = (orig_a / gcd) * orig_b;

            std::cout << "-> UCLN (GCD) cua " << orig_a << " va " << orig_b << " = " << gcd << "\n";
            std::cout << "-> BCNN (LCM) cua " << orig_a << " va " << orig_b << " = " << lcm << "\n";
            break;
        }

        // =================================================================
        // BÀI 3: IN N SỐ FIBONACCI ĐẦU TIÊN (DÙNG LONG LONG)
        // =================================================================
        case 3:
        {
            int n;
            std::cout << "\n[Bai 3] Nhap so luong so Fibonacci can in (1 -> 90): ";
            std::cin >> n;

            if (n <= 0 || n > 90)
            {
                std::cout << "-> Vui long nhap N trong khoang [1, 90] (qua 92 se tran long long)!\n";
                break;
            }

            std::cout << "-> " << n << " so Fibonacci dau tien:\n   ";
            long long f0 = 0, f1 = 1;
            for (int i = 1; i <= n; ++i)
            {
                if (i == 1)
                {
                    std::cout << f0 << " ";
                    continue;
                }
                if (i == 2)
                {
                    std::cout << f1 << " ";
                    continue;
                }
                long long fn = f0 + f1;
                std::cout << fn << " ";
                // Cập nhật cuốn chiếu 2 số liền trước cho bước tiếp theo
                f0 = f1;
                f1 = fn;
            }
            std::cout << "\n";
            break;
        }

        // =================================================================
        // BÀI 4: PHÂN TÍCH THỪA SỐ NGUYÊN TỐ (PRIME FACTORIZATION)
        // =================================================================
        case 4:
        {
            long long n;
            std::cout << "\n[Bai 4] Nhap so nguyen duong N (N >= 2): ";
            std::cin >> n;

            if (n < 2)
            {
                std::cout << "-> Vui long nhap N >= 2!\n";
                break;
            }

            std::cout << "-> Phan tich: " << n << " = ";
            bool is_first = true; // Cờ đánh dấu để in dấu '*' cho đẹp

            for (long long i = 2; i * i <= n; ++i)
            {
                while (n % i == 0)
                {
                    if (!is_first)
                        std::cout << " * ";
                    std::cout << i;
                    is_first = false;
                    n /= i; // Chia giảm n liên tục cho ước nguyên tố i
                }
            }
            // Nếu sau vòng lặp mà n > 1 thì phần còn lại chính là 1 thừa số nguyên tố lớn
            if (n > 1)
            {
                if (!is_first)
                    std::cout << " * ";
                std::cout << n;
            }
            std::cout << "\n";
            break;
        }

        // =================================================================
        // BÀI 5 (PHẦN TÍNH TOÁN): MÁY TÍNH BỎ TÚI 4 PHÉP TÍNH
        // =================================================================
        case 5:
        {
            double num1, num2;
            char op;
            std::cout << "\n[Bai 5] Nhap bieu thuc (vi du: 12.5 * 4): ";
            std::cin >> num1 >> op >> num2;

            switch (op)
            {
            case '+':
                std::cout << "-> Ket qua: " << (num1 + num2) << "\n";
                break;
            case '-':
                std::cout << "-> Ket qua: " << (num1 - num2) << "\n";
                break;
            case '*':
                std::cout << "-> Ket qua: " << (num1 * num2) << "\n";
                break;
            case '/':
                if (std::abs(num2) < EPS)
                {
                    std::cout << "-> Loi: Khong the chia cho 0!\n";
                }
                else
                {
                    std::cout << "-> Ket qua: " << (num1 / num2) << "\n";
                }
                break;
            default:
                std::cout << "-> Loi: Toan tu '" << op << "' khong hop le!\n";
                break;
            }
            break;
        }

        case 0:
            std::cout << "-> Dang thoat chuong trinh. Tam biet!\n";
            break;

        default:
            std::cout << "-> Lua chon khong hop le! Vui long chon tu 0 den 5.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
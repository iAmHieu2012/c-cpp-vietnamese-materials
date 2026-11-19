#include <iostream>
#include <cmath>   // Dùng cho std::sqrt, std::abs
#include <iomanip> // Dùng cho std::fixed, std::setprecision

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1: Bộ hàm kiểm tra tính chất số
bool is_prime(long long n);
bool is_palindrome(long long n);

// Bài 2: Hàm trả về nhiều kết quả qua Tham chiếu (&)
void analyze_number(long long n, int &count_digits, int &sum_digits, int &max_digit);

// Bài 3: Hàm tính Tổ hợp C(n, k) chống tràn số sớm
long long nCr(int n, int k);

// Bài 4: Hàm hoán đổi và sắp xếp 3 biến bằng Tham chiếu
void swap_ref(int &a, int &b);
void sort_three(int &a, int &b, int &c);

// Bài 5: Nạp chồng hàm (Function Overloading) tính diện tích
double area(double radius);                         // Hình tròn
double area(double length, double width);           // Hình chữ nhật
double area(double a, double b, double c);          // Tam giác (Heron)


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n================ MENU BAI TAP 1.4 ================\n";
        std::cout << "1. Tim so vua Nguyen To vua Thuan Nghich trong doan [L, R]\n";
        std::cout << "2. Phan tich so da ket qua bang Tham chieu (&)\n";
        std::cout << "3. Tinh to hop C(n, k) chong tran so\n";
        std::cout << "4. Sap xep 3 bien tang dan bang Tham chieu\n";
        std::cout << "5. Tinh dien tich (Function Overloading)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                long long L, R;
                std::cout << "\n[Bai 1] Nhap doan [L, R] (vi du: 10 1000): ";
                std::cin >> L >> R;
                std::cout << "-> Cac so vua nguyen to vua thuan nghich:\n   ";
                int count = 0;
                for (long long i = L; i <= R; ++i) {
                    // TỐI ƯU SHORT-CIRCUIT: Kiểm tra thuận nghịch trước vì nó chạy nhanh hơn O(log10(N))
                    // Nếu không thuận nghịch thì máy tính bỏ qua luôn hàm is_prime O(sqrt(N))!
                    if (is_palindrome(i) && is_prime(i)) {
                        std::cout << i << " ";
                        ++count;
                    }
                }
                if (count == 0) std::cout << "(Khong co so nao)";
                std::cout << "\n";
                break;
            }

            case 2: {
                long long n;
                std::cout << "\n[Bai 2] Nhap so nguyen N (vi du: -49285): ";
                std::cin >> n;
                int count_d = 0, sum_d = 0, max_d = 0;
                
                // Gọi hàm: 3 biến count_d, sum_d, max_d sẽ được cập nhật trực tiếp
                analyze_number(n, count_d, sum_d, max_d);
                
                std::cout << "-> Ket qua phan tich so " << n << ":\n";
                std::cout << "   - So luong chu so: " << count_d << "\n";
                std::cout << "   - Tong cac chu so: " << sum_d << "\n";
                std::cout << "   - Chu so lon nhat: " << max_d << "\n";
                break;
            }

            case 3: {
                int n, k;
                std::cout << "\n[Bai 3] Nhap n va k de tinh C(n, k) (vi du: 50 6): ";
                std::cin >> n >> k;
                long long result = nCr(n, k);
                if (result == -1) {
                    std::cout << "-> Loi: Dieu kien hop le la 0 <= k <= n!\n";
                } else {
                    std::cout << "-> C(" << n << ", " << k << ") = " << result << "\n";
                }
                break;
            }

            case 4: {
                int a, b, c;
                std::cout << "\n[Bai 4] Nhap 3 so nguyen a, b, c: ";
                std::cin >> a >> b >> c;
                std::cout << "-> Truoc khi sap xep: a = " << a << ", b = " << b << ", c = " << c << "\n";
                sort_three(a, b, c);
                std::cout << "-> Sau khi sap xep:   a = " << a << ", b = " << b << ", c = " << c << "\n";
                break;
            }

            case 5: {
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "\n[Bai 5] Kiem thu Nap chong ham area():\n";
                std::cout << "1. Hinh tron (r = 5.0):              S = " << area(5.0) << "\n";
                std::cout << "2. Hinh chu nhat (d = 4.0, r = 6.5): S = " << area(4.0, 6.5) << "\n";
                std::cout << "3. Tam giac (3, 4, 5):               S = " << area(3.0, 4.0, 5.0) << "\n";
                break;
            }

            case 0:
                std::cout << "-> Tam biet!\n";
                break;

            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}


// =============================================================================
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM (FUNCTION DEFINITIONS)
// =============================================================================

// --- Bài 1: Kiểm tra số nguyên tố tối ưu (bỏ qua bội của 2 và 3) ---
bool is_prime(long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Mọi số nguyên tố > 3 đều có dạng (6k - 1) hoặc (6k + 1)
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

bool is_palindrome(long long n) {
    if (n < 0) return false; // Số âm không phải số thuận nghịch
    long long original = n, reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + (n % 10);
        n /= 10;
    }
    return original == reversed;
}

// --- Bài 2: Phân tích số trả về 3 kết quả qua Tham chiếu ---
void analyze_number(long long n, int &count_digits, int &sum_digits, int &max_digit) {
    // Khởi tạo lại các biến tham chiếu về 0 để tránh cộng dồn rác từ bên ngoài
    count_digits = 0;
    sum_digits = 0;
    max_digit = 0;

    // Xử lý trường hợp đặc biệt n = 0
    if (n == 0) {
        count_digits = 1;
        return;
    }

    // Nếu n âm, chuyển thành số dương để tách chữ số
    if (n < 0) n = -n;

    while (n > 0) {
        int digit = n % 10;
        ++count_digits;
        sum_digits += digit;
        if (digit > max_digit) {
            max_digit = digit;
        }
        n /= 10;
    }
}

// --- Bài 3: Tính tổ hợp C(n, k) không dùng giai thừa ---
long long nCr(int n, int k) {
    if (k < 0 || k > n) return -1; // Dữ liệu không hợp lệ
    
    // Tính chất đối xứng: C(n, k) = C(n, n - k). Chọn k nhỏ hơn để lặp ít hơn!
    if (k > n - k) {
        k = n - k;
    }

    long long res = 1;
    // Công thức: C(n, k) = [n * (n-1) * ... * (n-k+1)] / [1 * 2 * ... * k]
    for (int i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i; // Nhân đến đâu chia ngay đến đó
    }
    return res;
}

// --- Bài 4: Sắp xếp 3 biến bằng Tham chiếu (3 bước so sánh) ---
void swap_ref(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void sort_three(int &a, int &b, int &c) {
    if (a > b) swap_ref(a, b); // Đảm bảo a <= b
    if (a > c) swap_ref(a, c); // Đảm bảo a là số nhỏ nhất trong 3 số
    if (b > c) swap_ref(b, c); // Đảm bảo b <= c
}

// --- Bài 5: Nạp chồng hàm tính diện tích ---
double area(double radius) {
    const double PI = 3.14159265358979323846;
    return PI * radius * radius;
}

double area(double length, double width) {
    return length * width;
}

double area(double a, double b, double c) {
    // Kiểm tra bất đẳng thức tam giác trước khi tính
    if (a <= 0 || b <= 0 || c <= 0 || (a + b <= c) || (a + c <= b) || (b + c <= a)) {
        std::cout << "[Canh bao: 3 canh khong tao thanh tam giac!] ";
        return 0.0;
    }
    double p = (a + b + c) / 2.0; // Nửa chu vi
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
}
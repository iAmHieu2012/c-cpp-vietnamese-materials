#include <iostream>
#include <iomanip>

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Hàm tiện ích: In mảng thuần bằng con trỏ
void print_array_ptr(const int *arr, int n);

// Bài 1: Tìm Min & Max trả kết quả qua 2 con trỏ đầu ra
void find_min_max_ptr(const int *arr, int n, int *min_out, int *max_out);

// Bài 2: Đảo ngược mảng tại chỗ bằng 2 con trỏ đầu - cuối
void reverse_array_ptr(int *arr, int n);

// Bài 3: Tự viết hàm đo độ dài chuỗi (my_strlen) và nối chuỗi (my_strcat) bằng con trỏ
size_t my_strlen(const char *str);
void my_strcat(char *dest, const char *src);

// Bài 4: Xóa mọi phần tử có giá trị X tại chỗ bằng kỹ thuật 2 con trỏ (Read / Write Pointers)
int remove_all_x_ptr(int *arr, int n, int x);

// Bài 5: Các hàm toán học cho Bảng nhảy Con trỏ hàm (Function Pointer Jump Table)
double op_add(double a, double b) { return a + b; }
double op_sub(double a, double b) { return a - b; }
double op_mul(double a, double b) { return a * b; }
double op_div(double a, double b) { return (b != 0.0) ? (a / b) : 0.0; }


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.2 (THUAN CON TRO) ==========\n";
        std::cout << "1. Tim Min & Max cua mang bang Con tro\n";
        std::cout << "2. Dao nguoc mang bang 2 Con tro (left & right)\n";
        std::cout << "3. Tu cai dat my_strlen va my_strcat bang Con tro\n";
        std::cout << "4. Loc bo phan tu X tai cho (Read / Write Pointers)\n";
        std::cout << "5. May tinh bo tui dung Mang Con Tro Ham (Jump Table)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "======================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int a[] = {42, -15, 99, 8, 0, -73, 64};
                int n = sizeof(a) / sizeof(*a);
                int min_val = 0, max_val = 0;

                std::cout << "\n[Bai 1] Mang ban dau: ";
                print_array_ptr(a, n);

                // Truyền địa chỉ &min_val và &max_val vào để hàm ghi kết quả ra
                find_min_max_ptr(a, n, &min_val, &max_val);
                std::cout << "-> Gia tri nho nhat (Min): " << min_val << "\n";
                std::cout << "-> Gia tri lon nhat (Max): " << max_val << "\n";
                break;
            }

            case 2: {
                int a[] = {10, 20, 30, 40, 50, 60};
                int n = sizeof(a) / sizeof(*a);

                std::cout << "\n[Bai 2] Truoc khi dao nguoc: ";
                print_array_ptr(a, n);

                reverse_array_ptr(a, n);

                std::cout << "-> Sau khi dao nguoc:        ";
                print_array_ptr(a, n);
                break;
            }

            case 3: {
                char buffer[100] = "Hello ";
                const char *addon = "Pointers & Memory!";

                std::cout << "\n[Bai 3] Chuoi goc 1: '" << buffer 
                          << "' (Do dai my_strlen = " << my_strlen(buffer) << ")\n";
                std::cout << "        Chuoi goc 2: '" << addon 
                          << "' (Do dai my_strlen = " << my_strlen(addon) << ")\n";

                my_strcat(buffer, addon);
                std::cout << "-> Sau khi goi my_strcat: '" << buffer 
                          << "' (Do dai moi = " << my_strlen(buffer) << ")\n";
                break;
            }

            case 4: {
                int a[] = {2, 5, 3, 2, 8, 2, 9, 2, 10};
                int n = sizeof(a) / sizeof(*a);
                int x = 2;

                std::cout << "\n[Bai 4] Mang ban dau: ";
                print_array_ptr(a, n);
                std::cout << "Can xoa tat ca phan tu co gia tri X = " << x << "\n";

                int new_n = remove_all_x_ptr(a, n, x);

                std::cout << "-> Mang sau khi loc (kich thuoc moi = " << new_n << "): ";
                print_array_ptr(a, new_n);
                break;
            }

            case 5: {
                // Khởi tạo Mảng gồm 4 Con trỏ hàm (Jump Table)
                double (*ops[4])(double, double) = {op_add, op_sub, op_mul, op_div};
                const char *op_symbols = "+-*/";

                int op_idx;
                double num1, num2;
                std::cout << "\n[Bai 5] Chon phep tinh (0: Cong, 1: Tru, 2: Nhan, 3: Chia): ";
                std::cin >> op_idx;

                if (op_idx < 0 || op_idx > 3) {
                    std::cout << "-> Chi so phep tinh khong hop le!\n";
                    break;
                }

                std::cout << "Nhap 2 so thuc a va b: ";
                std::cin >> num1 >> num2;

                // GỌI HÀM TRỰC TIẾP QUA CON TRỎ MÀ KHÔNG CẦN IF-ELSE HAY SWITCH-CASE!
                // *(ops + op_idx) lấy con trỏ hàm tại vị trí op_idx rồi truyền (num1, num2) vào
                double res = (*(ops + op_idx))(num1, num2);

                std::cout << "-> Ket qua: " << num1 << " " << *(op_symbols + op_idx) << " " 
                          << num2 << " = " << res << "\n";
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
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM (KHÔNG DÙNG NGOẶC VUÔNG [])
// =============================================================================

void print_array_ptr(const int *arr, int n) {
    const int *end_ptr = arr + n; // Địa chỉ ngay sau phần tử cuối cùng
    for (const int *p = arr; p < end_ptr; ++p) {
        std::cout << *p << " ";
    }
    std::cout << "\n";
}

// --- Bài 1: Tìm Min và Max bằng con trỏ chạy ---
void find_min_max_ptr(const int *arr, int n, int *min_out, int *max_out) {
    // Luôn kiểm tra con trỏ khác nullptr để tránh lỗi Segmentation Fault
    if (arr == nullptr || min_out == nullptr || max_out == nullptr || n <= 0) {
        return;
    }

    *min_out = *arr; // Khởi tạo bằng phần tử đầu tiên (*arr tương đương arr[0])
    *max_out = *arr;

    const int *end_ptr = arr + n;
    for (const int *p = arr + 1; p < end_ptr; ++p) {
        if (*p < *min_out) *min_out = *p;
        if (*p > *max_out) *max_out = *p;
    }
}

// --- Bài 2: Đảo ngược mảng bằng 2 con trỏ đầu và cuối ---
void reverse_array_ptr(int *arr, int n) {
    if (arr == nullptr || n <= 1) return;

    int *left  = arr;         // Trỏ vào phần tử đầu tiên
    int *right = arr + n - 1; // Trỏ vào phần tử cuối cùng

    // So sánh trực tiếp địa chỉ của 2 con trỏ!
    while (left < right) {
        int temp = *left;
        *left = *right;
        *right = temp;

        ++left;  // Tiến lên 1 ô nhớ (tăng 4 bytes)
        --right; // Lùi về 1 ô nhớ (giảm 4 bytes)
    }
}

// --- Bài 3: Tự viết hàm đo độ dài chuỗi & nối chuỗi bằng hiệu con trỏ ---
size_t my_strlen(const char *str) {
    if (str == nullptr) return 0;

    const char *p = str;
    // Lặp cho đến khi gặp ký tự kết thúc chuỗi Null-terminator '\0'
    while (*p != '\0') {
        ++p;
    }
    // Hiệu hai con trỏ trả về chính xác số lượng ký tự giữa chúng!
    return static_cast<size_t>(p - str);
}

void my_strcat(char *dest, const char *src) {
    if (dest == nullptr || src == nullptr) return;

    // Bước 1: Đưa con trỏ dest chạy tới đúng vị trí ký tự '\0' ở cuối chuỗi đích
    while (*dest != '\0') {
        ++dest;
    }

    // Bước 2: Sao chép từng ký tự từ src sang dest cho đến khi hết src
    while (*src != '\0') {
        *dest = *src;
        ++dest;
        ++src;
    }
    // Bước 3: Bắt buộc đóng dấu ký tự kết thúc chuỗi '\0' ở cuối cùng!
    *dest = '\0';
}

// --- Bài 4: Kỹ thuật 2 con trỏ Đọc/Ghi (Read/Write Pointers) lọc mảng trong O(N) ---
int remove_all_x_ptr(int *arr, int n, int x) {
    if (arr == nullptr || n <= 0) return 0;

    int *write_ptr = arr;           // Con trỏ chỉ vị trí sẽ ghi phần tử hợp lệ tiếp theo
    const int *end_ptr = arr + n;

    for (int *read_ptr = arr; read_ptr < end_ptr; ++read_ptr) {
        if (*read_ptr != x) {
            *write_ptr = *read_ptr; // Giữ lại phần tử khác x
            ++write_ptr;            // Nhích con trỏ ghi lên 1 ô
        }
    }
    // Khoảng cách từ đầu mảng (arr) đến write_ptr chính là số lượng phần tử còn lại!
    return static_cast<int>(write_ptr - arr);
}
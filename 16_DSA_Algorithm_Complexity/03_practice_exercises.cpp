#include <iostream>
#include <algorithm>

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM
// =============================================================================

// Bài 1: Kiểm tra số nguyên tố trong O(sqrt(N)) Time, O(1) Space
bool is_prime_sqrt(long long n);

// Bài 2: Tìm số bị thiếu trong mảng [0..N] bằng XOR -> O(N) Time, O(1) Space (chống tràn số)
int find_missing_number_xor(const int a[], int n);

// Bài 3: Two Sum trên mảng đã sắp xếp -> O(N) Time, O(1) Space
bool two_sum_sorted(const int a[], int n, int target, int &idx1, int &idx2);

// Bài 4: Tổng dãy con liên tiếp lớn nhất (Kadane's Algorithm) -> O(N) Time, O(1) Space
long long max_subarray_kadane(const int a[], int n);

// Bài 5: Tính Fibonacci thứ N -> O(N) Time, O(1) Space (thay cho O(2^N))
unsigned long long fib_iterative(int n);


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.1 (BIG-O OPTIMIZATION) ==========\n";
        std::cout << "1. Kiem tra so nguyen to toi uu O(sqrt(N))\n";
        std::cout << "2. Tim so bi thieu trong [0..N] bang XOR - O(N) Time, O(1) Space\n";
        std::cout << "3. Two Sum tren mang da sap xep - O(N) Time, O(1) Space\n";
        std::cout << "4. Tong day con lien tiep lon nhat (Kadane) - O(N) Time, O(1) Space\n";
        std::cout << "5. Tinh Fibonacci thu N - O(N) Time, O(1) Space\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                long long n;
                std::cout << "\n[Bai 1] Nhap so nguyen n (VD: 1000000007): ";
                std::cin >> n;
                std::cout << "-> " << n << (is_prime_sqrt(n) ? " LA so nguyen to.\n" : " KHONG phai so nguyen to.\n");
                break;
            }

            case 2: {
                // Mảng có n = 5 phần tử thuộc [0..5], thiếu số 3
                int a[] = {4, 0, 2, 5, 1};
                int n = sizeof(a) / sizeof(a[0]);
                std::cout << "\n[Bai 2] Mang {4, 0, 2, 5, 1} (N = 5):\n";
                std::cout << "-> So bi thieu = " << find_missing_number_xor(a, n) << "\n";
                break;
            }

            case 3: {
                int a[] = {1, 3, 5, 7, 11, 15, 20};
                int n = sizeof(a) / sizeof(a[0]);
                int target = 18;
                int i1 = -1, i2 = -1;
                std::cout << "\n[Bai 3] Mang da sap xep: {1, 3, 5, 7, 11, 15, 20} | Target = " << target << "\n";
                if (two_sum_sorted(a, n, target, i1, i2)) {
                    std::cout << "-> Tim thay: a[" << i1 << "] (" << a[i1]
                              << ") + a[" << i2 << "] (" << a[i2] << ") = " << target << "\n";
                } else {
                    std::cout << "-> Khong ton tai cap nao!\n";
                }
                break;
            }

            case 4: {
                int a[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
                int n = sizeof(a) / sizeof(a[0]);
                std::cout << "\n[Bai 4] Mang: {-2, 1, -3, 4, -1, 2, 1, -5, 4}\n";
                std::cout << "-> Tong day con lien tiep lon nhat (Kadane) = "
                          << max_subarray_kadane(a, n) << "\n";
                break;
            }

            case 5: {
                int n;
                std::cout << "\n[Bai 5] Nhap N (0 -> 93): ";
                std::cin >> n;
                if (n < 0 || n > 93) {
                    std::cout << "-> Nhap N trong doan [0, 93] de tranh tran unsigned long long!\n";
                } else {
                    std::cout << "-> Fibonacci(" << n << ") = " << fib_iterative(n) << "\n";
                }
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
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM
// =============================================================================

// --- Bài 1: Kiểm tra số nguyên tố tối ưu bước nhảy 6k +/- 1 ---
bool is_prime_sqrt(long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Mọi số nguyên tố > 3 đều có dạng 6k - 1 hoặc 6k + 1
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// --- Bài 2: Tìm số bị thiếu bằng XOR (a ^ a = 0, a ^ 0 = a) ---
int find_missing_number_xor(const int a[], int n) {
    int xor_val = n; // Bắt đầu với N
    for (int i = 0; i < n; ++i) {
        xor_val ^= i ^ a[i]; // Triệt tiêu mọi số xuất hiện ở cả chỉ số [0..N] lẫn mảng a[]
    }
    return xor_val;
}

// --- Bài 3: Two Sum trên mảng đã sắp xếp bằng kỹ thuật Hai con trỏ ---
bool two_sum_sorted(const int a[], int n, int target, int &idx1, int &idx2) {
    int left = 0, right = n - 1;
    while (left < right) {
        int sum = a[left] + a[right];
        if (sum == target) {
            idx1 = left;
            idx2 = right;
            return true;
        } else if (sum < target) {
            ++left;  // Tổng nhỏ hơn target -> tăng đầu trái
        } else {
            --right; // Tổng lớn hơn target -> giảm đầu phải
        }
    }
    return false;
}

// --- Bài 4: Kadane's Algorithm (O(N) Time, O(1) Space) ---
long long max_subarray_kadane(const int a[], int n) {
    if (n <= 0) return 0;
    long long cur_sum = a[0];
    long long max_sum = a[0];

    for (int i = 1; i < n; ++i) {
        // Hoặc bắt đầu dãy con mới tại a[i], hoặc nối tiếp dãy con cũ
        cur_sum = std::max(static_cast<long long>(a[i]), cur_sum + a[i]);
        if (cur_sum > max_sum) max_sum = cur_sum;
    }
    return max_sum;
}

// --- Bài 5: Fibonacci khử đệ quy (O(N) Time, O(1) Space) ---
unsigned long long fib_iterative(int n) {
    if (n <= 1) return static_cast<unsigned long long>(n);
    unsigned long long prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; ++i) {
        unsigned long long cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
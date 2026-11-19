#include <iostream>
#include <iomanip>
#include <climits> // Dùng cho INT_MIN, INT_MAX

const int MAXN = 1005;
const int MAX_MAT = 55;

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1: Tìm phần tử lớn thứ hai trong 1 vòng lặp O(N)
bool find_second_largest(const int a[], int n, int &max1, int &max2);

// Bài 2: Mảng cộng dồn 1 chiều (Prefix Sum) truy vấn tổng đoạn [L, R] trong O(1)
void build_prefix_sum(const int a[], long long pref[], int n);
long long query_sum(const long long pref[], int L, int R);

// Bài 3: Xoay mảng sang phải K bước bằng thuật toán đảo mảng 3 lần O(N) Time, O(1) Space
void reverse_subarray(int a[], int left, int right);
void rotate_right(int a[], int n, int k);

// Bài 4: Tìm tất cả Điểm Yên Ngựa (Nhỏ nhất trên hàng, Lớn nhất trên cột)
void find_saddle_points(const int mat[][MAX_MAT], int rows, int cols);

// Bài 5: Sinh ma trận xoắn ốc N x N (Spiral Matrix)
void generate_spiral_matrix(int mat[][MAX_MAT], int n);
void print_matrix(const int mat[][MAX_MAT], int rows, int cols);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n================ MENU BAI TAP 1.5 ================\n";
        std::cout << "1. Tim phan tu lon thu hai (Second Largest) trong O(N)\n";
        std::cout << "2. Mang cong don (Prefix Sum) - Truy van tong doan O(1)\n";
        std::cout << "3. Xoay mang sang phai K buoc voi O(1) Space\n";
        std::cout << "4. Tim Diem Yen Ngua (Saddle Point) tren ma tran\n";
        std::cout << "5. In Ma tran xoan oc (Spiral Matrix N x N)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int n, a[MAXN];
                std::cout << "\n[Bai 1] Nhap so phan tu n: ";
                std::cin >> n;
                std::cout << "Nhap " << n << " so nguyen: ";
                for (int i = 0; i < n; ++i) std::cin >> a[i];

                int max1, max2;
                if (find_second_largest(a, n, max1, max2)) {
                    std::cout << "-> Lon nhat (max1) = " << max1 
                              << " | Lon thu hai (max2) = " << max2 << "\n";
                } else {
                    std::cout << "-> Khong ton tai so lon thu hai (mang < 2 phan tu hoac cac so bang nhau)!\n";
                }
                break;
            }

            case 2: {
                int n, q, a[MAXN];
                long long pref[MAXN];
                std::cout << "\n[Bai 2] Nhap so phan tu n: ";
                std::cin >> n;
                std::cout << "Nhap " << n << " phan tu (chi so tu 0 den " << n - 1 << "): ";
                for (int i = 0; i < n; ++i) std::cin >> a[i];

                // Xây dựng mảng cộng dồn trong O(N)
                build_prefix_sum(a, pref, n);

                std::cout << "Nhap so luong truy van Q: ";
                std::cin >> q;
                while (q--) {
                    int L, R;
                    std::cout << "  Nhap doan [L, R] (0 <= L <= R < " << n << "): ";
                    std::cin >> L >> R;
                    if (L < 0 || R >= n || L > R) {
                        std::cout << "  -> Chi so [L, R] khong hop le!\n";
                    } else {
                        std::cout << "  -> Tong doan [" << L << ", " << R << "] = " 
                                  << query_sum(pref, L, R) << "\n";
                    }
                }
                break;
            }

            case 3: {
                int n = 7, k = 3;
                int a[MAXN] = {1, 2, 3, 4, 5, 6, 7};
                std::cout << "\n[Bai 3] Mang mau ban dau: 1 2 3 4 5 6 7 (n = 7)\n";
                std::cout << "Nhap so buoc can xoay phai K (vi du 3): ";
                std::cin >> k;

                rotate_right(a, n, k);

                std::cout << "-> Mang sau khi xoay phai " << k << " buoc: ";
                for (int i = 0; i < n; ++i) std::cout << a[i] << " ";
                std::cout << "\n";
                break;
            }

            case 4: {
                // Ma trận mẫu có điểm yên ngựa là số 7 tại hàng 2, cột 0 (nhỏ nhất hàng 2: {7,8,9}, lớn nhất cột 0: {1,4,7})
                int rows = 3, cols = 3;
                int mat[MAX_MAT][MAX_MAT] = {
                    {1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9}
                };
                std::cout << "\n[Bai 4] Xet ma tran mau 3x3:\n";
                print_matrix(mat, rows, cols);
                find_saddle_points(mat, rows, cols);
                break;
            }

            case 5: {
                int n;
                int spiral[MAX_MAT][MAX_MAT];
                std::cout << "\n[Bai 5] Nhap kich thuoc ma tran vuong N (1 -> 20): ";
                std::cin >> n;
                if (n <= 0 || n > 20) {
                    std::cout << "-> Kich thuoc khong hop le!\n";
                    break;
                }
                generate_spiral_matrix(spiral, n);
                std::cout << "-> Ma tran xoan oc " << n << "x" << n << ":\n";
                print_matrix(spiral, n, n);
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

// --- Bài 1: Tìm phần tử lớn thứ hai trong 1 lần duyệt ---
bool find_second_largest(const int a[], int n, int &max1, int &max2) {
    if (n < 2) return false;

    max1 = INT_MIN;
    max2 = INT_MIN;
    bool found_max2 = false;

    for (int i = 0; i < n; ++i) {
        if (a[i] > max1) {
            // Khi tìm thấy kỷ lục mới (a[i] > max1):
            // Nếu max1 trước đó đã từng được cập nhật từ mảng, đẩy max1 cũ xuống làm max2!
            if (i > 0) {
                max2 = max1;
                found_max2 = true;
            }
            max1 = a[i];
        } else if (a[i] < max1 && (!found_max2 || a[i] > max2)) {
            // Nếu a[i] nhỏ hơn max1 nhưng lớn hơn max2 hiện tại -> cập nhật max2
            max2 = a[i];
            found_max2 = true;
        }
    }
    return found_max2;
}

// --- Bài 2: Xây dựng & Truy vấn Mảng Cộng Dồn (Prefix Sum) ---
void build_prefix_sum(const int a[], long long pref[], int n) {
    if (n <= 0) return;
    pref[0] = a[0];
    for (int i = 1; i < n; ++i) {
        pref[i] = pref[i - 1] + a[i]; // Tổng từ đầu đến i = Tổng đến (i-1) + a[i]
    }
}

long long query_sum(const long long pref[], int L, int R) {
    if (L == 0) return pref[R];
    return pref[R] - pref[L - 1];
}

// --- Bài 3: Xoay mảng sang phải K bước bằng Reversal Algorithm ---
void reverse_subarray(int a[], int left, int right) {
    while (left < right) {
        int temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        ++left;
        --right;
    }
}

void rotate_right(int a[], int n, int k) {
    if (n <= 1) return;
    k %= n; // Nếu k >= n (ví dụ xoay 10 bước trên mảng 7 phần tử) thì tương đương xoay (10 % 7 = 3) bước
    if (k < 0) k += n; // Phòng trường hợp nhập k âm
    if (k == 0) return;

    // 3 bước thần thánh:
    reverse_subarray(a, 0, n - 1); // Bước 1: Đảo toàn bộ mảng:       [7, 6, 5, 4, 3, 2, 1]
    reverse_subarray(a, 0, k - 1); // Bước 2: Đảo K phần tử đầu:      [5, 6, 7, 4, 3, 2, 1]
    reverse_subarray(a, k, n - 1); // Bước 3: Đảo (N - K) phần tử sau:[5, 6, 7, 1, 2, 3, 4]
}

// --- Bài 4: Tìm Điểm Yên Ngựa (Nhỏ nhất hàng i, Lớn nhất cột j) ---
void find_saddle_points(const int mat[][MAX_MAT], int rows, int cols) {
    int row_min[MAX_MAT];
    int col_max[MAX_MAT];

    // Bước 1: Tìm giá trị nhỏ nhất của từng hàng
    for (int i = 0; i < rows; ++i) {
        row_min[i] = mat[i][0];
        for (int j = 1; j < cols; ++j) {
            if (mat[i][j] < row_min[i]) row_min[i] = mat[i][j];
        }
    }

    // Bước 2: Tìm giá trị lớn nhất của từng cột
    for (int j = 0; j < cols; ++j) {
        col_max[j] = mat[0][j];
        for (int i = 1; i < rows; ++i) {
            if (mat[i][j] > col_max[j]) col_max[j] = mat[i][j];
        }
    }

    // Bước 3: Đối chiếu giao điểm
    bool found = false;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (mat[i][j] == row_min[i] && mat[i][j] == col_max[j]) {
                std::cout << "-> Diem yen ngua tai [" << i << "][" << j << "] = " << mat[i][j] << "\n";
                found = true;
            }
        }
    }
    if (!found) {
        std::cout << "-> Ma tran khong co diem yen ngua nao!\n";
    }
}

// --- Bài 5: Điền Ma trận xoắn ốc bằng kỹ thuật 4 đường biên ---
void generate_spiral_matrix(int mat[][MAX_MAT], int n) {
    int top = 0, bottom = n - 1;
    int left = 0, right = n - 1;
    int val = 1;

    while (top <= bottom && left <= right) {
        // 1. Điền cạnh TRÊN (từ trái sang phải), xong hạ biên top xuống 1 hàng
        for (int col = left; col <= right; ++col) {
            mat[top][col] = val++;
        }
        ++top;

        // 2. Điền cạnh PHẢI (từ trên xuống dưới), xong thu biên right sang trái 1 cột
        for (int row = top; row <= bottom; ++row) {
            mat[row][right] = val++;
        }
        --right;

        // 3. Điền cạnh DƯỚI (từ phải sang trái), xong nâng biên bottom lên 1 hàng
        if (top <= bottom) {
            for (int col = right; col >= left; --col) {
                mat[bottom][col] = val++;
            }
            --bottom;
        }

        // 4. Điền cạnh TRÁI (từ dưới lên trên), xong đẩy biên left sang phải 1 cột
        if (left <= right) {
            for (int row = bottom; row >= top; --row) {
                mat[row][left] = val++;
            }
            ++left;
        }
    }
}

void print_matrix(const int mat[][MAX_MAT], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << mat[i][j];
        }
        std::cout << "\n";
    }
}
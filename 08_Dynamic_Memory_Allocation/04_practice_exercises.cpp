#include <iostream>
#include <iomanip>

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

void print_array(const int *arr, int n);

// Bài 1: Lọc các số dương (> 0) sang một mảng động mới có kích thước vừa khít
int* filter_positives(const int *arr, int n, int &out_size);

// Bài 2: Trộn 2 mảng đã sắp xếp tăng dần thành 1 mảng động mới trong O(N + M)
int* merge_sorted_arrays(const int *a, int n, const int *b, int m);

// Bài 3: Tự động nhân đôi sức chứa (Capacity) khi nhập mảng không biết trước số phần tử
void push_back_dynamic(int *&arr, int &size, int &capacity, int val);

// Bài 4: Cấp phát, Chuyển vị (Transpose R x C -> C x R) và Giải phóng Ma trận 2D động
int** allocate_matrix(int rows, int cols);
int** transpose_matrix(const int * const *mat, int rows, int cols);
void print_matrix_2d(const int * const *mat, int rows, int cols);
void free_matrix(int **&mat, int rows);

// Bài 5: Mảng răng cưa (Jagged Array) - Tạo và in Tam giác Pascal N dòng trên Heap
int** create_pascal_triangle(int n);
void print_and_free_pascal(int **&pascal, int n);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.3 (CAP PHAT DONG HEAP) ==========\n";
        std::cout << "1. Loc so duong sang mang dong moi vua khit tren Heap\n";
        std::cout << "2. Tron 2 mang tang dan thanh mang dong moi O(N + M)\n";
        std::cout << "3. Nhap mang tu co gian (Auto-Resize nhu std::vector)\n";
        std::cout << "4. Chuyen vi Ma tran 2D dong (R x C -> C x R)\n";
        std::cout << "5. Mang rang cua (Jagged Array) - Tam giac Pascal\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int raw[] = {-5, 12, 0, -3, 8, 25, -9, 4};
                int n = sizeof(raw) / sizeof(raw[0]);
                int pos_count = 0;

                std::cout << "\n[Bai 1] Mang goc: ";
                print_array(raw, n);

                // Nhận con trỏ trỏ tới mảng mới trên Heap từ hàm
                int *pos_arr = filter_positives(raw, n, pos_count);

                if (pos_arr != nullptr) {
                    std::cout << "-> Mang dong chua " << pos_count << " so duong: ";
                    print_array(pos_arr, pos_count);
                    
                    // QUAN TRỌNG: Người gọi hàm nhận vùng nhớ Heap phải có trách nhiệm delete[]!
                    delete[] pos_arr;
                    pos_arr = nullptr;
                } else {
                    std::cout << "-> Mang goc khong co so duong nao!\n";
                }
                break;
            }

            case 2: {
                int a[] = {1, 4, 9, 15};
                int b[] = {2, 3, 6, 10, 12, 20};
                int n = sizeof(a) / sizeof(a[0]);
                int m = sizeof(b) / sizeof(b[0]);

                std::cout << "\n[Bai 2] Mang A: "; print_array(a, n);
                std::cout << "        Mang B: "; print_array(b, m);

                int *merged = merge_sorted_arrays(a, n, b, m);
                std::cout << "-> Mang dong sau khi tron (" << (n + m) << " phan tu): ";
                print_array(merged, n + m);

                delete[] merged;
                merged = nullptr;
                break;
            }

            case 3: {
                int capacity = 2; // Khởi đầu chỉ xin 2 ô nhớ trên Heap
                int size = 0;
                int *dyn_arr = new int[capacity];

                std::cout << "\n[Bai 3] Nhap lien tiep cac so nguyen (nhap -1 de dung):\n";
                while (true) {
                    int val;
                    std::cout << "  Nhap so (Size=" << size << ", Cap=" << capacity << "): ";
                    std::cin >> val;
                    if (val == -1) break;

                    push_back_dynamic(dyn_arr, size, capacity, val);
                }

                std::cout << "-> Cac so da luu tren Heap: ";
                print_array(dyn_arr, size);
                std::cout << "   (Thong so cuoi: Size = " << size << ", Capacity = " << capacity << ")\n";

                delete[] dyn_arr;
                dyn_arr = nullptr;
                break;
            }

            case 4: {
                int rows = 2, cols = 3;
                int **mat = allocate_matrix(rows, cols);

                // Điền dữ liệu mẫu: Hàng 0: {1, 2, 3}, Hàng 1: {4, 5, 6}
                int val = 1;
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) mat[i][j] = val++;
                }

                std::cout << "\n[Bai 4] Ma tran goc (" << rows << "x" << cols << "):\n";
                print_matrix_2d(mat, rows, cols);

                // Tạo ma trận chuyển vị (cols x rows: 3x2) trên Heap
                int **trans = transpose_matrix(mat, rows, cols);
                std::cout << "-> Ma tran chuyen vi (" << cols << "x" << rows << "):\n";
                print_matrix_2d(trans, cols, rows);

                // Giải phóng sạch sẽ cả 2 ma trận
                free_matrix(mat, rows);
                free_matrix(trans, cols);
                break;
            }

            case 5: {
                int n;
                std::cout << "\n[Bai 5] Nhap so dong N cua Tam giac Pascal (1 -> 15): ";
                std::cin >> n;
                if (n <= 0 || n > 15) {
                    std::cout << "-> Vui long nhap N trong khoang [1, 15]!\n";
                    break;
                }

                int **pascal = create_pascal_triangle(n);
                std::cout << "-> Tam giac Pascal " << n << " dong (Jagged Array tiet kiem 50% RAM):\n";
                print_and_free_pascal(pascal, n);
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

void print_array(const int *arr, int n) {
    for (int i = 0; i < n; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";
}

// --- Bài 1: Đếm trước -> Cấp phát vừa khít -> Sao chép ---
int* filter_positives(const int *arr, int n, int &out_size) {
    out_size = 0;
    if (arr == nullptr || n <= 0) return nullptr;

    // Bước 1: Đếm chính xác số lượng số dương
    for (int i = 0; i < n; ++i) {
        if (arr[i] > 0) ++out_size;
    }

    // Nếu không có số dương nào, không cấp phát bộ nhớ vô ích!
    if (out_size == 0) return nullptr;

    // Bước 2: Cấp phát mảng trên Heap vừa khít với số lượng đã đếm
    int *result = new int[out_size];
    int idx = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] > 0) {
            result[idx++] = arr[i];
        }
    }
    return result; // Trả địa chỉ vùng nhớ Heap về cho main()
}

// --- Bài 2: Kỹ thuật 2 con trỏ trộn 2 mảng tăng dần trong O(N + M) ---
int* merge_sorted_arrays(const int *a, int n, const int *b, int m) {
    int *res = new int[n + m];
    int i = 0, j = 0, k = 0;

    // So sánh từng cặp ở đầu 2 mảng, số nào nhỏ hơn thì đưa vào mảng kết quả trước
    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            res[k++] = a[i++];
        } else {
            res[k++] = b[j++];
        }
    }

    // Chép nốt các phần tử còn dư của mảng A (nếu có)
    while (i < n) res[k++] = a[i++];

    // Chép nốt các phần tử còn dư của mảng B (nếu có)
    while (j < m) res[k++] = b[j++];

    return res;
}

// --- Bài 3: Tự động nhân đôi mảng khi đầy (Tham chiếu tới con trỏ: int *&arr) ---
void push_back_dynamic(int *&arr, int &size, int &capacity, int val) {
    if (size == capacity) {
        int new_cap = capacity * 2;
        int *new_arr = new int[new_cap];

        for (int i = 0; i < size; ++i) {
            new_arr[i] = arr[i];
        }

        delete[] arr; // Xóa vùng nhớ cũ trước khi trỏ sang vùng nhớ mới
        arr = new_arr;
        capacity = new_cap;
        std::cout << "    [Heap Resize] Da nhan doi Capacity len " << capacity << "!\n";
    }
    arr[size++] = val;
}

// --- Bài 4: Cấp phát, Chuyển vị và Giải phóng Ma trận 2D động ---
int** allocate_matrix(int rows, int cols) {
    int **mat = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        mat[i] = new int[cols](); // Khởi tạo bằng 0
    }
    return mat;
}

int** transpose_matrix(const int * const *mat, int rows, int cols) {
    // Ma trận chuyển vị có số hàng = cols và số cột = rows
    int **trans = allocate_matrix(cols, rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            trans[j][i] = mat[i][j]; // Đảo chỉ số hàng thành cột
        }
    }
    return trans;
}

void print_matrix_2d(const int * const *mat, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << mat[i][j];
        }
        std::cout << "\n";
    }
}

void free_matrix(int **&mat, int rows) {
    if (mat == nullptr) return;
    for (int i = 0; i < rows; ++i) {
        delete[] mat[i];
    }
    delete[] mat;
    mat = nullptr; // Nhờ truyền tham chiếu int **&mat nên con trỏ ở main() tự về nullptr!
}

// --- Bài 5: Mảng răng cưa (Jagged Array) cho Tam giác Pascal ---
int** create_pascal_triangle(int n) {
    int **pascal = new int*[n];
    for (int i = 0; i < n; ++i) {
        // ĐẶC SẢN JAGGED ARRAY: Dòng thứ i chỉ cấp phát đúng (i + 1) ô nhớ!
        pascal[i] = new int[i + 1];

        // Phần tử đầu tiên và cuối cùng của mỗi dòng luôn bằng 1
        pascal[i][0] = 1;
        pascal[i][i] = 1;

        // Các phần tử ở giữa bằng tổng 2 phần tử ngay phía trên nó ở dòng (i - 1)
        for (int j = 1; j < i; ++j) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
    return pascal;
}

void print_and_free_pascal(int **&pascal, int n) {
    if (pascal == nullptr) return;
    for (int i = 0; i < n; ++i) {
        // Căn lề tạo hình tam giác cân đẹp mắt
        std::cout << std::setw((n - i) * 2) << "";
        for (int j = 0; j <= i; ++j) {
            std::cout << std::setw(4) << pascal[i][j];
        }
        std::cout << "\n";
        // In xong dòng nào thì giải phóng luôn dòng đó!
        delete[] pascal[i];
    }
    delete[] pascal;
    pascal = nullptr;
}
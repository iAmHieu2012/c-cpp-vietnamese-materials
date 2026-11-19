#include <iostream>
#include <iomanip>

const int MAX = 50;

// BẮT BUỘC phải chỉ định kích thước cột tối đa 'MAX' trong tham số mảng 2 chiều!
void print_matrix(const int mat[][MAX], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << mat[i][j];
        }
        std::cout << "\n";
    }
}

int main() {
    // 1. Ma trận vuông N x N: Đường chéo chính & Đường chéo phụ
    int n = 3;
    int square[MAX][MAX] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "=== 1. MA TRAN VUONG 3x3 ===\n";
    print_matrix(square, n, n);

    int sum_main_diag = 0; // Đường chéo chính: các phần tử có chỉ số hàng == cột (i == j)
    int sum_anti_diag = 0; // Đường chéo phụ: các phần tử có (i + j == n - 1)

    for (int i = 0; i < n; ++i) {
        sum_main_diag += square[i][i];
        sum_anti_diag += square[i][n - 1 - i];
    }
    std::cout << "-> Tong duong cheo chinh (1 + 5 + 9): " << sum_main_diag << "\n";
    std::cout << "-> Tong duong cheo phu   (3 + 5 + 7): " << sum_anti_diag << "\n\n";


    // 2. Nhân hai ma trận: A (m x n) nhân B (n x p) -> C (m x p)
    std::cout << "=== 2. NHAN HAI MA TRAN A(2x3) * B(3x2) = C(2x2) ===\n";
    int A[MAX][MAX] = { {1, 2, 3}, {4, 5, 6} };       // 2 hàng, 3 cột
    int B[MAX][MAX] = { {7, 8}, {9, 1}, {2, 3} };    // 3 hàng, 2 cột
    int C[MAX][MAX] = {0};                           // Kết quả: 2 hàng, 2 cột

    int m = 2, k_dim = 3, p = 2;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < k_dim; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    std::cout << "Ma tran tich C(2x2):\n";
    print_matrix(C, m, p);

    return 0;
}
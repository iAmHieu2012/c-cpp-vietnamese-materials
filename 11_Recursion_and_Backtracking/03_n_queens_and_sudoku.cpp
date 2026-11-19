#include <iostream>

// =============================================================================
// PHẦN 1: BÀI TOÁN N-QUEENS (N QUÂN HẬU) VỚI KIỂM TRA O(1)
// =============================================================================
const int MAX_Q = 20;
int queen_col[MAX_Q];       // queen_col[row] = cột đặt hậu ở hàng row
bool col_used[MAX_Q];       // Cột đã có hậu
bool diag1_used[2 * MAX_Q]; // Đường chéo chính (row - col + n) đã có hậu
bool diag2_used[2 * MAX_Q]; // Đường chéo phụ (row + col) đã có hậu
int n_queens_solutions = 0;

void solve_n_queens(int row, int n) {
    if (row == n) {
        ++n_queens_solutions;
        if (n_queens_solutions == 1) { // In ra nghiệm đầu tiên làm mẫu
            std::cout << "  [Nghiem mau dau tien cho " << n << "-Queens]:\n";
            for (int i = 0; i < n; ++i) {
                std::cout << "    ";
                for (int j = 0; j < n; ++j) {
                    std::cout << (queen_col[i] == j ? "Q " : ". ");
                }
                std::cout << "\n";
            }
        }
        return;
    }

    for (int col = 0; col < n; ++col) {
        int d1 = row - col + n; // Cộng thêm n để chỉ số luôn dương
        int d2 = row + col;

        if (!col_used[col] && !diag1_used[d1] && !diag2_used[d2]) {
            // 1. CHỌN
            queen_col[row] = col;
            col_used[col] = diag1_used[d1] = diag2_used[d2] = true;

            // 2. ĐI SÂU sang hàng tiếp theo
            solve_n_queens(row + 1, n);

            // 3. QUAY LUI (Gỡ quân hậu khỏi bàn cờ)
            col_used[col] = diag1_used[d1] = diag2_used[d2] = false;
        }
    }
}

// =============================================================================
// PHẦN 2: GIẢI SUDOKU 9x9 BẰNG QUAY LUI
// =============================================================================
bool is_valid_sudoku(int board[9][9], int row, int col, int num) {
    // Kiểm tra trùng hàng và trùng cột
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) return false;
    }
    // Kiểm tra trùng trong khối vuông nhỏ 3x3
    int start_r = (row / 3) * 3;
    int start_c = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[start_r + i][start_c + j] == num) return false;
        }
    }
    return true;
}

bool solve_sudoku(int board[9][9]) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) { // Tìm thấy ô trống
                for (int num = 1; num <= 9; ++num) {
                    if (is_valid_sudoku(board, row, col, num)) {
                        board[row][col] = num;      // 1. CHỌN
                        if (solve_sudoku(board)) {  // 2. ĐI SÂU
                            return true;            // Đã giải xong toàn bộ bàn cờ!
                        }
                        board[row][col] = 0;        // 3. QUAY LUI (Ngõ cụt -> Xóa số vừa điền)
                    }
                }
                return false; // Thử cả 1..9 đều thất bại -> Báo lùi về ô trước đó
            }
        }
    }
    return true; // Không còn ô trống nào -> Thành công!
}

void print_sudoku(int board[9][9]) {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i != 0) std::cout << "  ------+-------+------\n";
        std::cout << "  ";
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0 && j != 0) std::cout << "| ";
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "=== 1. BAI TOAN 8-QUEENS ===\n";
    solve_n_queens(0, 8);
    std::cout << "-> Tong so cach dat 8 quan hau khong an nhau: " << n_queens_solutions << " cach!\n\n";

    std::cout << "=== 2. GIAI SUDOKU 9x9 BANG QUAY LUI ===\n";
    int sudoku[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    if (solve_sudoku(sudoku)) {
        std::cout << "-> Da tim ra loi giai Sudoku:\n";
        print_sudoku(sudoku);
    } else {
        std::cout << "-> Bai toan Sudoku vo nghiem!\n";
    }

    return 0;
}
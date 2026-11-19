#include <iostream>
#include <vector>
#include <string>

// =============================================================================
// 1. SINH TẤT CẢ TẬP CON (SUBSETS - O(N * 2^N))
// =============================================================================
void generate_subsets(int idx, const std::vector<int> &nums,
                      std::vector<int> &curr, std::vector<std::vector<int>> &res) {
    res.push_back(curr);
    for (int i = idx; i < static_cast<int>(nums.size()); ++i) {
        curr.push_back(nums[i]);                  // Choose
        generate_subsets(i + 1, nums, curr, res); // Explore
        curr.pop_back();                          // Unchoose (Backtrack)
    }
}

// =============================================================================
// 2. BÀI TOÁN N-QUEENS (XẾP N QUÂN HẬU KHÔNG ĂN NHAU - O(N!))
// Tối ưu kiểm tra O(1) bằng 3 mảng đánh dấu: cột, đường chéo chính, đường chéo phụ
// =============================================================================
void solve_n_queens_rec(int row, int n, std::vector<std::string> &board,
                        std::vector<bool> &col_used,
                        std::vector<bool> &diag1_used, // row - col + (n - 1)
                        std::vector<bool> &diag2_used, // row + col
                        int &solution_count) {
    if (row == n) {
        ++solution_count;
        if (solution_count == 1) { // In nghiệm đầu tiên làm mẫu
            std::cout << "  Nghiem mau dau tien cho " << n << "-Queens:\n";
            for (const auto &line : board) std::cout << "    " << line << "\n";
        }
        return;
    }

    for (int col = 0; col < n; ++col) {
        int d1 = row - col + (n - 1);
        int d2 = row + col;
        if (col_used[col] || diag1_used[d1] || diag2_used[d2]) continue;

        // Choose
        board[row][col] = 'Q';
        col_used[col] = diag1_used[d1] = diag2_used[d2] = true;

        // Explore
        solve_n_queens_rec(row + 1, n, board, col_used, diag1_used, diag2_used, solution_count);

        // Unchoose (Backtrack)
        board[row][col] = '.';
        col_used[col] = diag1_used[d1] = diag2_used[d2] = false;
    }
}

int main() {
    std::cout << "=== 1. SINH TAP CON (SUBSETS) ===\n";
    std::vector<int> nums = {1, 2, 3};
    std::vector<int> curr;
    std::vector<std::vector<int>> subsets;
    generate_subsets(0, nums, curr, subsets);

    std::cout << "  Tong so tap con: " << subsets.size() << " -> ";
    for (const auto &s : subsets) {
        std::cout << "{";
        for (size_t i = 0; i < s.size(); ++i) std::cout << s[i] << (i + 1 < s.size() ? "," : "");
        std::cout << "} ";
    }
    std::cout << "\n\n";

    std::cout << "=== 2. BAI TOAN N-QUEENS (N = 8) ===\n";
    int n = 8, count = 0;
    std::vector<std::string> board(n, std::string(n, '.'));
    std::vector<bool> col_used(n, false), diag1(2 * n, false), diag2(2 * n, false);
    solve_n_queens_rec(0, n, board, col_used, diag1, diag2, count);
    std::cout << "  -> Tong so cach xep hop le voi N = 8: " << count << "\n";

    return 0;
}
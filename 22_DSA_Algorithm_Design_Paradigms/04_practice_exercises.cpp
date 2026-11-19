#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// =============================================================================
// BÀI 1: COMBINATION SUM (BACKTRACKING - ĐƯỢC CHỌN LẠI PHẦN TỬ)
// =============================================================================
void comb_sum_rec(int idx, int rem, const std::vector<int> &cand,
                  std::vector<int> &curr, std::vector<std::vector<int>> &res) {
    if (rem == 0) {
        res.push_back(curr);
        return;
    }
    for (int i = idx; i < static_cast<int>(cand.size()); ++i) {
        if (cand[i] > rem) break; // Cắt tỉa nhờ mảng đã sắp xếp tăng dần
        curr.push_back(cand[i]);
        // Truyền lại 'i' (không phải i + 1) vì được phép dùng lại cand[i] nhiều lần
        comb_sum_rec(i, rem - cand[i], cand, curr, res);
        curr.pop_back();
    }
}

std::vector<std::vector<int>> combination_sum(std::vector<int> cand, int target) {
    std::sort(cand.begin(), cand.end());
    std::vector<int> curr;
    std::vector<std::vector<int>> res;
    comb_sum_rec(0, target, cand, curr, res);
    return res;
}


// =============================================================================
// BÀI 2: JUMP GAME (GREEDY - O(N) TIME, O(1) SPACE)
// =============================================================================
bool can_jump(const std::vector<int> &nums) {
    int max_reach = 0;
    int n = static_cast<int>(nums.size());
    for (int i = 0; i < n; ++i) {
        if (i > max_reach) return false; // Không thể nhảy tới vị trí i
        max_reach = std::max(max_reach, i + nums[i]);
        if (max_reach >= n - 1) return true;
    }
    return true;
}


// =============================================================================
// BÀI 3: COIN CHANGE (UNBOUNDED KNAPSACK DP - O(N * AMOUNT) TIME, O(AMOUNT) SPACE)
// =============================================================================
int coin_change_min(const std::vector<int> &coins, int amount) {
    const int INF = amount + 1;
    std::vector<int> dp(amount + 1, INF);
    dp[0] = 0;

    for (int coin : coins) {
        // Duyệt xuôi từ coin -> amount vì mỗi mệnh giá có vô hạn đồng
        for (int a = coin; a <= amount; ++a) {
            dp[a] = std::min(dp[a], dp[a - coin] + 1);
        }
    }
    return (dp[amount] > amount) ? -1 : dp[amount];
}


// =============================================================================
// BÀI 4: LONGEST COMMON SUBSEQUENCE (LCS - 2D DP O(N * M))
// =============================================================================
int lcs_length(const std::string &s1, const std::string &s2) {
    int n = static_cast<int>(s1.size());
    int m = static_cast<int>(s2.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
}


// =============================================================================
// BÀI 5: EDIT DISTANCE / LEVENSHTEIN DISTANCE (2D DP O(N * M))
// =============================================================================
int edit_distance(const std::string &w1, const std::string &w2) {
    int n = static_cast<int>(w1.size());
    int m = static_cast<int>(w2.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i) dp[i][0] = i; // Xóa i ký tự
    for (int j = 0; j <= m; ++j) dp[0][j] = j; // Chèn j ký tự

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (w1[i - 1] == w2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // Khớp ký tự -> Không tốn phép biến đổi
            } else {
                dp[i][j] = 1 + std::min({
                    dp[i - 1][j],     // Xóa ký tự w1[i-1]
                    dp[i][j - 1],     // Chèn ký tự w2[j-1]
                    dp[i - 1][j - 1]  // Thay thế w1[i-1] -> w2[j-1]
                });
            }
        }
    }
    return dp[n][m];
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.7 (ALGORITHM DESIGN) ==========\n";
        std::cout << "1. Combination Sum (Backtracking)\n";
        std::cout << "2. Jump Game (Greedy O(N))\n";
        std::cout << "3. Coin Change - Doi tien it to nhat (1D DP)\n";
        std::cout << "4. Longest Common Subsequence - LCS (2D DP)\n";
        std::cout << "5. Edit Distance - Levenshtein Distance (2D DP)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::vector<int> cand = {2, 3, 6, 7};
                int target = 7;
                auto res = combination_sum(cand, target);
                std::cout << "\n[Bai 1] Candidates = {2, 3, 6, 7} | Target = " << target << "\n";
                for (const auto &v : res) {
                    std::cout << "  -> { ";
                    for (int x : v) std::cout << x << " ";
                    std::cout << "}\n";
                }
                break;
            }

            case 2: {
                std::vector<int> a1 = {2, 3, 1, 1, 4};
                std::vector<int> a2 = {3, 2, 1, 0, 4};
                std::cout << "\n[Bai 2]\n";
                std::cout << "  {2, 3, 1, 1, 4} -> " << (can_jump(a1) ? "Co the toi dich" : "Khong the") << "\n";
                std::cout << "  {3, 2, 1, 0, 4} -> " << (can_jump(a2) ? "Co the toi dich" : "Khong the") << "\n";
                break;
            }

            case 3: {
                std::vector<int> coins = {1, 2, 5};
                int amount = 11;
                std::cout << "\n[Bai 3] Menh gia = {1, 2, 5} | Amount = " << amount << "\n";
                std::cout << "  -> So dong xu it nhat = " << coin_change_min(coins, amount) << "\n";
                break;
            }

            case 4: {
                std::string s1 = "abcde", s2 = "ace";
                std::cout << "\n[Bai 4] s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"\n";
                std::cout << "  -> Do dai LCS = " << lcs_length(s1, s2) << "\n";
                break;
            }

            case 5: {
                std::string w1 = "horse", w2 = "ros";
                std::cout << "\n[Bai 5] \"" << w1 << "\" -> \"" << w2 << "\"\n";
                std::cout << "  -> So phep bien doi it nhat (Edit Distance) = "
                          << edit_distance(w1, w2) << "\n";
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
#include <iostream>
#include <vector>
#include <algorithm>

// =============================================================================
// 1. BÀI TOÁN 0/1 KNAPSACK (MỖI ĐỒ VẬT CHỌN 0 HOẶC 1 LẦN)
// =============================================================================

// Cách A: Top-Down (Đệ quy có nhớ - Memoization) -> O(N * W) Time, O(N * W) Space
int knapsack_memo(int idx, int rem_w, const std::vector<int> &w, const std::vector<int> &v,
                  std::vector<std::vector<int>> &memo) {
    if (idx == 0 || rem_w == 0) return 0;
    if (memo[idx][rem_w] != -1) return memo[idx][rem_w];

    int not_take = knapsack_memo(idx - 1, rem_w, w, v, memo);
    int take = 0;
    if (w[idx - 1] <= rem_w) {
        take = v[idx - 1] + knapsack_memo(idx - 1, rem_w - w[idx - 1], w, v, memo);
    }
    return memo[idx][rem_w] = std::max(not_take, take);
}

// Cách B: Bottom-Up tối ưu không gian 1D -> O(N * W) Time, O(W) Space
int knapsack_1d(const std::vector<int> &w, const std::vector<int> &v, int W) {
    std::vector<int> dp(W + 1, 0);
    for (size_t i = 0; i < w.size(); ++i) {
        // BẮT BUỘC duyệt ngược từ W về w[i] để mỗi đồ vật chỉ được dùng tối đa 1 lần!
        for (int cap = W; cap >= w[i]; --cap) {
            dp[cap] = std::max(dp[cap], dp[cap - w[i]] + v[i]);
        }
    }
    return dp[W];
}

// =============================================================================
// 2. DÃY CON TĂNG DÀI NHẤT (LONGEST INCREASING SUBSEQUENCE - LIS)
// Tối ưu bằng Binary Search (std::lower_bound) -> O(N log N)
// =============================================================================
int lis_nlogn(const std::vector<int> &nums) {
    std::vector<int> tails; // tails[i]: phần tử kết thúc nhỏ nhất của dãy tăng độ dài i + 1
    for (int x : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else                   *it = x;
    }
    return static_cast<int>(tails.size());
}

int main() {
    std::cout << "=== 1. 0/1 KNAPSACK (MEMOIZATION VS 1D TABULATION) ===\n";
    std::vector<int> weights = {2, 3, 4, 5};
    std::vector<int> values  = {3, 4, 5, 6};
    int W = 5;
    int n = static_cast<int>(weights.size());

    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(W + 1, -1));
    std::cout << "  Top-Down Memoization : " << knapsack_memo(n, W, weights, values, memo) << "\n";
    std::cout << "  Bottom-Up 1D Space   : " << knapsack_1d(weights, values, W) << "\n\n";

    std::cout << "=== 2. LONGEST INCREASING SUBSEQUENCE (O(N log N)) ===\n";
    std::vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    std::cout << "  Mang: {10, 9, 2, 5, 3, 7, 101, 18}\n";
    std::cout << "  -> Do dai LIS = " << lis_nlogn(arr) << "\n";

    return 0;
}
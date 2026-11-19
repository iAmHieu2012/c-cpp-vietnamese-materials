#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// =============================================================================
// 1. TWO POINTERS & VARIABLE-SIZE SLIDING WINDOW - O(N) TIME, O(1) SPACE
// Bài toán: Tìm độ dài đoạn con liên tiếp ngắn nhất có tổng >= target
// =============================================================================
int min_subarray_len_sliding_window(int target, const std::vector<int> &nums) {
    int n = static_cast<int>(nums.size());
    int left = 0;
    long long window_sum = 0;
    int min_len = n + 1;

    for (int right = 0; right < n; ++right) {
        window_sum += nums[right]; // 1. Mở rộng cửa sổ sang phải
        while (window_sum >= target) { // 2. Thu hẹp cửa sổ từ bên trái khi đã thỏa mãn
            min_len = std::min(min_len, right - left + 1);
            window_sum -= nums[left];
            ++left;
        }
    }
    return (min_len > n) ? 0 : min_len;
}

// =============================================================================
// 2. ACTIVITY SELECTION (CHỌN TỐI ĐA HOẠT ĐỘNG KHÔNG GIAO NHAU - O(N log N))
// Chiến lược tham lam: Luôn chọn hoạt động KẾT THÚC SỚM NHẤT
// =============================================================================
struct Activity {
    int id, start, end;
};

void activity_selection(std::vector<Activity> acts) {
    std::sort(acts.begin(), acts.end(), [](const Activity &a, const Activity &b) {
        return a.end < b.end;
    });

    std::cout << "=== 2. ACTIVITY SELECTION (GREEDY - O(N log N)) ===\n  Cac hoat dong duoc chon: ";
    int last_end = -1;
    int count = 0;
    for (const auto &a : acts) {
        if (a.start >= last_end) {
            std::cout << "[#" << a.id << ": " << a.start << "-" << a.end << "] ";
            last_end = a.end;
            ++count;
        }
    }
    std::cout << "\n  -> Tong so hoat dong toi da = " << count << "\n\n";
}

// =============================================================================
// 3. FRACTIONAL KNAPSACK (CÁI TÚI PHÂN SỐ - ĐƯỢC CẮT NHỎ ĐỒ VẬT - O(N log N))
// Chiến lược tham lam: Sắp xếp giảm dần theo đơn giá (value / weight)
// =============================================================================
struct Item {
    int weight, value;
};

double fractional_knapsack(std::vector<Item> items, int capacity) {
    std::sort(items.begin(), items.end(), [](const Item &a, const Item &b) {
        return static_cast<double>(a.value) / a.weight >
               static_cast<double>(b.value) / b.weight;
    });

    double total_value = 0.0;
    int rem_cap = capacity;

    for (const auto &it : items) {
        if (rem_cap == 0) break;
        if (it.weight <= rem_cap) {
            total_value += it.value;
            rem_cap -= it.weight;
        } else {
            total_value += static_cast<double>(it.value) * rem_cap / it.weight;
            rem_cap = 0;
        }
    }
    return total_value;
}

int main() {
    std::cout << "=== 1. SLIDING WINDOW & TWO POINTERS (O(N)) ===\n";
    std::vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;
    std::cout << "  Mang: {2, 3, 1, 2, 4, 3} | Doan con ngan nhat co tong >= " << target
              << " co do dai = " << min_subarray_len_sliding_window(target, nums) << " ({4, 3})\n\n";

    std::vector<Activity> acts = {
        {1, 1, 4}, {2, 3, 5}, {3, 0, 6}, {4, 5, 7}, {5, 3, 9},
        {6, 5, 9}, {7, 6, 10}, {8, 8, 11}, {9, 8, 12}, {10, 2, 14}, {11, 12, 16}
    };
    activity_selection(acts);

    std::cout << "=== 3. FRACTIONAL KNAPSACK (GREEDY - O(N log N)) ===\n";
    std::vector<Item> items = {{10, 60}, {20, 100}, {30, 120}};
    int W = 50;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Suc chua tui W = " << W << " -> Gia tri toi da = "
              << fractional_knapsack(items, W) << "\n";

    return 0;
}

#include <iostream>
#include <algorithm>

// 1. Linear Search - O(N)
int linear_search(const int a[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (a[i] == target) return i;
    }
    return -1;
}

// 2. Binary Search cơ bản trên mảng đã sắp xếp - O(log N)
int binary_search_exact(const int a[], int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2; // Chống tràn số
        if (a[mid] == target) return mid;
        else if (a[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// 3. Lower Bound tự viết: Vị trí ĐẦU TIÊN có a[i] >= target - O(log N)
int lower_bound_idx(const int a[], int n, int target) {
    int left = 0, right = n; // right = n để xử lý trường hợp mọi phần tử đều < target
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (a[mid] < target) left = mid + 1;
        else right = mid;
    }
    return left;
}

// 4. Upper Bound tự viết: Vị trí ĐẦU TIÊN có a[i] > target - O(log N)
int upper_bound_idx(const int a[], int n, int target) {
    int left = 0, right = n;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (a[mid] <= target) left = mid + 1;
        else right = mid;
    }
    return left;
}

// 5. Binary Search on Answer (Bài toán cắt gỗ - EKO):
// Cho n cây có chiều cao trees[i]. Tìm độ cao lưỡi cưa H lớn nhất sao cho tổng gỗ cắt được >= M.
bool can_cut_enough(const int trees[], int n, long long h, long long m) {
    long long collected = 0;
    for (int i = 0; i < n; ++i) {
        if (trees[i] > h) collected += (trees[i] - h);
    }
    return collected >= m;
}

long long max_saw_height(const int trees[], int n, long long m) {
    long long left = 0, right = 0;
    for (int i = 0; i < n; ++i) {
        if (trees[i] > right) right = trees[i];
    }

    long long ans = 0;
    while (left <= right) {
        long long mid = left + (right - left) / 2;
        if (can_cut_enough(trees, n, mid, m)) {
            ans = mid;       // Lưu nghiệm khả dĩ và thử nâng lưỡi cưa cao hơn
            left = mid + 1;
        } else {
            right = mid - 1; // Thiếu gỗ -> hạ lưỡi cưa xuống
        }
    }
    return ans;
}

int main() {
    int a[] = {2, 5, 5, 5, 8, 12, 16, 23};
    int n = sizeof(a) / sizeof(a[0]);

    std::cout << "=== 1. LINEAR, BINARY SEARCH, LOWER_BOUND & UPPER_BOUND ===\n";
    std::cout << "  Mang da sap xep: {2, 5, 5, 5, 8, 12, 16, 23}\n";
    std::cout << "  linear_search(12)       -> idx = " << linear_search(a, n, 12) << "\n";
    std::cout << "  binary_search_exact(12) -> idx = " << binary_search_exact(a, n, 12) << "\n";

    int lb = lower_bound_idx(a, n, 5);
    int ub = upper_bound_idx(a, n, 5);
    std::cout << "  lower_bound_idx(5)      -> idx = " << lb << " (vi tri dau tien >= 5)\n";
    std::cout << "  upper_bound_idx(5)      -> idx = " << ub << " (vi tri dau tien > 5)\n";
    std::cout << "  -> So lan xuat hien cua 5 (ub - lb) = " << (ub - lb) << " lan (O(log N))\n\n";

    std::cout << "=== 2. BINARY SEARCH ON ANSWER (BAI TOAN CAT GO) ===\n";
    int trees[] = {20, 15, 10, 17};
    long long required_wood = 7;
    std::cout << "  Cac cay: {20, 15, 10, 17} | Can it nhat M = " << required_wood << "m go\n";
    std::cout << "  -> Do cao luoi cua H lon nhat = " << max_saw_height(trees, 4, required_wood) << "m\n";

    return 0;
}

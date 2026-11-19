// ============================================================================
// File: 03_merge_sort.cpp
// Bài 4.2: Merge Sort (Top-Down, Bottom-Up, Tính ổn định Stable & Inversion Count)
// ============================================================================

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void printArray(const std::string &label, const std::vector<int> &a) {
    std::cout << "  " << label << ": ";
    for (int x : a) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================================
// 1. TOP-DOWN MERGE SORT (ĐỆ QUY) & ĐẾM CẶP NGHỊCH THẾ (INVERSION COUNT)
// ============================================================================
long long mergeHalves(std::vector<int> &a, std::vector<int> &temp,
                      int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    long long inversions = 0;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) { // Dùng <= để bảo toàn tính ổn định (Stable)
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
            inversions += (mid - i + 1); // Mọi phần tử a[i..mid] đều > a[j]
        }
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];
    for (int p = left; p <= right; ++p) a[p] = temp[p];

    return inversions;
}

long long mergeSortTopDown(std::vector<int> &a, std::vector<int> &temp,
                           int left, int right) {
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    long long inv = 0;
    inv += mergeSortTopDown(a, temp, left, mid);
    inv += mergeSortTopDown(a, temp, mid + 1, right);
    inv += mergeHalves(a, temp, left, mid, right);
    return inv;
}

// ============================================================================
// 2. BOTTOM-UP MERGE SORT (KHỬ ĐỆ QUY - DÙNG VÒNG LẶP KÍCH THƯỚC 1, 2, 4, 8...)
// ============================================================================
void mergeSortBottomUp(std::vector<int> &a) {
    int n = static_cast<int>(a.size());
    std::vector<int> temp(n);

    for (int width = 1; width < n; width *= 2) {
        for (int left = 0; left < n - width; left += 2 * width) {
            int mid = left + width - 1;
            int right = std::min(left + 2 * width - 1, n - 1);
            mergeHalves(a, temp, left, mid, right);
        }
    }
}

int main() {
    std::cout << "=== MERGE SORT: O(N log N) MOI TRUONG HOP & STABLE ===\n\n";

    std::vector<int> a1 = {38, 27, 43, 3, 9, 82, 10};
    printArray("Mang ban dau        ", a1);

    std::vector<int> temp(a1.size());
    long long inv = mergeSortTopDown(a1, temp, 0, static_cast<int>(a1.size()) - 1);
    printArray("Top-Down Merge Sort ", a1);
    std::cout << "  -> So cap nghich the (Inversion Count) = " << inv << "\n\n";

    std::vector<int> a2 = {64, 25, 12, 22, 11, 90, 1};
    printArray("Mang ban dau        ", a2);
    mergeSortBottomUp(a2);
    printArray("Bottom-Up Merge Sort", a2);

    return 0;
}

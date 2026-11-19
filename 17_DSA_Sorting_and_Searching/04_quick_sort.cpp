// ============================================================================
// File: 04_quick_sort.cpp
// Bài 4.2: Quick Sort (Lomuto vs Hoare Partition, Median-of-Three & 3-Way Partition)
// ============================================================================

#include <iostream>
#include <string>
#include <utility>
#include <vector>

void printArray(const std::string &label, const std::vector<int> &a) {
    std::cout << "  " << label << ": ";
    for (int x : a) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================================
// 1. CHỌN CHỐT MEDIAN-OF-THREE (CHỐNG SUY BIẾN O(N^2) KHI MẢNG ĐÃ SẮP XẾP)
// ============================================================================
int chooseMedianOfThreePivot(std::vector<int> &a, int low, int high) {
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) std::swap(a[low], a[mid]);
    if (a[low] > a[high]) std::swap(a[low], a[high]);
    if (a[mid] > a[high]) std::swap(a[mid], a[high]);
    // Đưa trung vị về vị trí high để làm pivot cho Lomuto
    std::swap(a[mid], a[high]);
    return a[high];
}

// ============================================================================
// 2. PHÂN HOẠCH LOMUTO (LOMUTO PARTITION)
// ============================================================================
int partitionLomuto(std::vector<int> &a, int low, int high) {
    int pivot = chooseMedianOfThreePivot(a, low, high);
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (a[j] <= pivot) {
            std::swap(a[++i], a[j]);
        }
    }
    std::swap(a[i + 1], a[high]);
    return i + 1;
}

void quickSortLomuto(std::vector<int> &a, int low, int high) {
    if (low < high) {
        int pi = partitionLomuto(a, low, high);
        quickSortLomuto(a, low, pi - 1);
        quickSortLomuto(a, pi + 1, high);
    }
}

// ============================================================================
// 3. PHÂN HOẠCH HOARE (HOARE PARTITION - ÍT PHÉP SWAP HƠN LOMUTO 3 LẦN)
// ============================================================================
int partitionHoare(std::vector<int> &a, int low, int high) {
    int pivot = a[low + (high - low) / 2];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do { ++i; } while (a[i] < pivot);
        do { --j; } while (a[j] > pivot);
        if (i >= j) return j;
        std::swap(a[i], a[j]);
    }
}

void quickSortHoare(std::vector<int> &a, int low, int high) {
    if (low < high) {
        int pi = partitionHoare(a, low, high);
        quickSortHoare(a, low, pi);
        quickSortHoare(a, pi + 1, high);
    }
}

// ============================================================================
// 4. 3-WAY QUICK SORT (DUTCH NATIONAL FLAG - TỐI ƯU O(N) KHI NHIỀU PHẦN TỬ TRÙNG)
// ============================================================================
void quickSort3Way(std::vector<int> &a, int low, int high) {
    if (low >= high) return;
    int pivot = a[low];
    int lt = low;      // a[low..lt-1] < pivot
    int gt = high;     // a[gt+1..high] > pivot
    int i = low + 1;   // a[lt..i-1] == pivot

    while (i <= gt) {
        if (a[i] < pivot) {
            std::swap(a[lt++], a[i++]);
        } else if (a[i] > pivot) {
            std::swap(a[i], a[gt--]);
        } else {
            ++i;
        }
    }
    quickSort3Way(a, low, lt - 1);
    quickSort3Way(a, gt + 1, high);
}

int main() {
    std::cout << "=== QUICK SORT: LOMUTO, HOARE & 3-WAY PARTITION ===\n\n";

    std::vector<int> a1 = {38, 27, 43, 3, 9, 82, 10};
    quickSortLomuto(a1, 0, static_cast<int>(a1.size()) - 1);
    printArray("QuickSort (Lomuto + Median-of-3)", a1);

    std::vector<int> a2 = {99, 15, 42, 4, 16, 8, 23, 1};
    quickSortHoare(a2, 0, static_cast<int>(a2.size()) - 1);
    printArray("QuickSort (Hoare Partition)     ", a2);

    std::vector<int> a3 = {4, 9, 4, 4, 1, 9, 4, 4, 9, 4, 4, 1, 4};
    quickSort3Way(a3, 0, static_cast<int>(a3.size()) - 1);
    printArray("QuickSort 3-Way (Many Duplicates)", a3);

    return 0;
}

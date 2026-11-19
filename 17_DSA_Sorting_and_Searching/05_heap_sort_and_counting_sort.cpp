// ============================================================================
// File: 05_heap_sort_and_counting_sort.cpp
// Bài 4.2: Heap Sort O(1) Space & Nhóm Sắp xếp Tuyến tính (Counting Sort, Radix Sort)
// ============================================================================

#include <iostream>
#include <string>
#include <utility>
#include <vector>

void printArray(const std::string &label, const int a[], int n) {
    std::cout << "  " << label << ": ";
    for (int i = 0; i < n; ++i) std::cout << a[i] << " ";
    std::cout << "\n";
}

// ============================================================================
// 1. HEAP SORT - O(N log N) mọi trường hợp | O(1) Auxiliary Space | Unstable
// ============================================================================
void heapify(int a[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        std::swap(a[i], a[largest]);
        heapify(a, n, largest);
    }
}

void heapSort(int a[], int n) {
    // Bước 1: Xây dựng Max-Heap từ dưới lên trong O(N)
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(a, n, i);
    }
    // Bước 2: Đưa phần tử lớn nhất ở đỉnh a[0] về cuối mảng rồi vun lại đống
    for (int i = n - 1; i > 0; --i) {
        std::swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

// ============================================================================
// 2. COUNTING SORT (Số nguyên không âm) - O(N + K) | Stable
// ============================================================================
void countingSort(int a[], int n) {
    if (n <= 1) return;
    int maxVal = a[0];
    for (int i = 1; i < n; ++i) {
        if (a[i] > maxVal) maxVal = a[i];
    }

    std::vector<int> cnt(maxVal + 1, 0);
    std::vector<int> out(n);

    for (int i = 0; i < n; ++i) ++cnt[a[i]];
    for (int i = 1; i <= maxVal; ++i) cnt[i] += cnt[i - 1];

    // Duyệt ngược từ cuối về đầu để giữ tính ổn định (Stable)
    for (int i = n - 1; i >= 0; --i) {
        out[--cnt[a[i]]] = a[i];
    }
    for (int i = 0; i < n; ++i) a[i] = out[i];
}

// ============================================================================
// 3. RADIX SORT (LSD) - O(d * (N + 10)) | Stable
// ============================================================================
void countingSortByDigit(int a[], int n, int exp) {
    std::vector<int> out(n);
    int cnt[10] = {0};

    for (int i = 0; i < n; ++i) ++cnt[(a[i] / exp) % 10];
    for (int i = 1; i < 10; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i) {
        int digit = (a[i] / exp) % 10;
        out[--cnt[digit]] = a[i];
    }
    for (int i = 0; i < n; ++i) a[i] = out[i];
}

void radixSort(int a[], int n) {
    if (n <= 1) return;
    int maxVal = a[0];
    for (int i = 1; i < n; ++i) {
        if (a[i] > maxVal) maxVal = a[i];
    }
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortByDigit(a, n, exp);
    }
}

int main() {
    std::cout << "=== HEAP SORT, COUNTING SORT & RADIX SORT ===\n\n";

    int a1[] = {38, 27, 43, 3, 9, 82, 10};
    int a2[] = {4, 2, 2, 8, 3, 3, 1};
    int a3[] = {170, 45, 75, 90, 802, 24, 2, 66};

    heapSort(a1, 7);
    countingSort(a2, 7);
    radixSort(a3, 8);

    printArray("Heap Sort (In-place O(1) RAM)", a1, 7);
    printArray("Counting Sort (O(N+K) Stable)", a2, 7);
    printArray("Radix Sort (LSD Stable)      ", a3, 8);

    return 0;
}

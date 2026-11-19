#include <iostream>
#include <utility>

void print_arr(const char *label, const int a[], int n) {
    std::cout << "  " << label << ": ";
    for (int i = 0; i < n; ++i) std::cout << a[i] << " ";
    std::cout << "\n";
}

// 1. Selection Sort - O(N^2), Không ổn định (Unstable), tối đa N - 1 lần swap
void selection_sort(int a[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[min_idx]) min_idx = j;
        }
        if (min_idx != i) std::swap(a[i], a[min_idx]);
    }
}

// 2. Insertion Sort - O(N^2) worst, O(N) best, Ổn định (Stable)
void insertion_sort(int a[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

// 3. Bubble Sort (có cờ tối ưu dừng sớm) - O(N^2) worst, O(N) best, Ổn định (Stable)
void bubble_sort(int a[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Mảng đã sắp xếp -> Dừng ngay lập tức
    }
}

int main() {
    int a1[] = {64, 25, 12, 22, 11};
    int a2[] = {64, 25, 12, 22, 11};
    int a3[] = {64, 25, 12, 22, 11};

    selection_sort(a1, 5);
    insertion_sort(a2, 5);
    bubble_sort(a3, 5);

    std::cout << "=== 3 THUAT TOAN SAP XEP CO BAN O(N^2) ===\n";
    print_arr("Selection Sort", a1, 5);
    print_arr("Insertion Sort", a2, 5);
    print_arr("Bubble Sort   ", a3, 5);

    return 0;
}
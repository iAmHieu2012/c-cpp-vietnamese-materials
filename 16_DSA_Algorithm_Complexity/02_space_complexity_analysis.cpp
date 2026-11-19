#include <iostream>

// Cách 1: Đảo ngược mảng dùng mảng phụ -> Time: O(N), Space: O(N)
void reverse_extra_space(int a[], int n) {
    int *temp = new int[n]; // Tốn thêm N * sizeof(int) bytes trên Heap
    for (int i = 0; i < n; ++i) {
        temp[i] = a[n - 1 - i];
    }
    for (int i = 0; i < n; ++i) {
        a[i] = temp[i];
    }
    delete[] temp;
}

// Cách 2: Đảo ngược mảng tại chỗ (In-place) -> Time: O(N), Space: O(1)
void reverse_in_place(int a[], int n) {
    int left = 0, right = n - 1;
    while (left < right) {
        int tmp = a[left];
        a[left] = a[right];
        a[right] = tmp;
        ++left;
        --right;
    }
}

// Cách 3: Tính tổng 1..N bằng Đệ quy -> Time: O(N), Space: O(N) do N Stack Frames!
long long sum_recursive(int n) {
    if (n <= 0) return 0;
    return n + sum_recursive(n - 1);
}

// Cách 4: Tính tổng 1..N bằng Vòng lặp -> Time: O(N), Space: O(1)
long long sum_iterative(int n) {
    long long s = 0;
    for (int i = 1; i <= n; ++i) s += i;
    return s;
}

void print_arr(const char *label, const int a[], int n) {
    std::cout << "  " << label << ": ";
    for (int i = 0; i < n; ++i) std::cout << a[i] << " ";
    std::cout << "\n";
}

int main() {
    int a1[] = {1, 2, 3, 4, 5};
    int a2[] = {1, 2, 3, 4, 5};

    reverse_extra_space(a1, 5);
    reverse_in_place(a2, 5);

    std::cout << "=== SO SANH SPACE COMPLEXITY ===\n";
    print_arr("reverse_extra_space O(N) space", a1, 5);
    print_arr("reverse_in_place    O(1) space", a2, 5);

    std::cout << "\n  sum_recursive(100) [O(N) Stack Space] = " << sum_recursive(100) << "\n";
    std::cout << "  sum_iterative(100) [O(1) Space]       = " << sum_iterative(100) << "\n";

    return 0;
}
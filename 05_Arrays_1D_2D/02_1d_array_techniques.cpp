#include <iostream>

const int MAXN = 100;

void print_arr(const int a[], int n) {
    for (int i = 0; i < n; ++i) std::cout << a[i] << " ";
    std::cout << "(n = " << n << ")\n";
}

// 1. Đảo ngược mảng bằng kỹ thuật 2 chỉ số (left & right)
void reverse_array(int a[], int n) {
    int left = 0, right = n - 1;
    while (left < right) {
        int temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        ++left;
        --right;
    }
}

// 2. Chèn giá trị 'val' vào vị trí 'pos' (Truyền tham chiếu '&n' vì kích thước mảng tăng lên 1)
bool insert_element(int a[], int &n, int pos, int val) {
    if (n >= MAXN || pos < 0 || pos > n) return false;
    
    // QUAN TRỌNG: Phải dịch từ cuối (n) LÙI VỀ (pos + 1) để tránh ghi đè dữ liệu!
    for (int i = n; i > pos; --i) {
        a[i] = a[i - 1];
    }
    a[pos] = val;
    ++n; // Tăng số lượng phần tử thực tế
    return true;
}

// 3. Xóa phần tử tại vị trí 'pos' (Truyền tham chiếu '&n' vì kích thước mảng giảm đi 1)
bool delete_element(int a[], int &n, int pos) {
    if (n <= 0 || pos < 0 || pos >= n) return false;

    // Dịch từ vị trí (pos) TIẾN VỀ cuối mảng để lấp chỗ trống
    for (int i = pos; i < n - 1; ++i) {
        a[i] = a[i + 1];
    }
    --n; // Giảm số lượng phần tử thực tế
    return true;
}

int main() {
    int a[MAXN] = {10, 20, 30, 40, 50};
    int n = 5;

    std::cout << "Mang ban dau:             ";
    print_arr(a, n);

    reverse_array(a, n);
    std::cout << "1. Sau khi dao nguoc:     ";
    print_arr(a, n);

    insert_element(a, n, 2, 99); // Chèn số 99 vào chỉ số 2
    std::cout << "2. Sau khi chen 99 vao 2: ";
    print_arr(a, n);

    delete_element(a, n, 0);     // Xóa phần tử đầu tiên (chỉ số 0)
    std::cout << "3. Sau khi xoa chi so 0:  ";
    print_arr(a, n);

    // 4. Kỹ thuật Mảng Đếm Tần Suất (Frequency Array / Counting Array)
    std::cout << "\n=== 4. KY THUAT DEM TAN SUAT O(N) ===\n";
    int data[] = {4, 2, 4, 5, 2, 3, 4, 1, 2, 4};
    int size = sizeof(data) / sizeof(data[0]);
    int freq[10] = {0}; // Giả sử các số nằm trong đoạn [0, 9]

    for (int i = 0; i < size; ++i) {
        freq[data[i]]++; // Dùng chính giá trị của phần tử làm chỉ số mảng đếm!
    }

    for (int val = 0; val < 10; ++val) {
        if (freq[val] > 0) {
            std::cout << "  So " << val << " xuat hien: " << freq[val] << " lan\n";
        }
    }

    return 0;
}
#include <iostream>

// Hai hàm tính toán có cùng nguyên mẫu: nhận (int, int) và trả về int
int add(int a, int b)      { return a + b; }
int multiply(int a, int b) { return a * b; }

// Hai hàm tiêu chí so sánh (Comparators) để truyền làm Callback cho hàm sắp xếp
bool ascending(int a, int b)  { return a > b; } // Đổi chỗ nếu phần tử trước > phần tử sau
bool descending(int a, int b) { return a < b; } // Đổi chỗ nếu phần tử trước < phần tử sau

// Hàm sắp xếp tổng quát nhận một Con trỏ hàm 'cmp' làm tham số Callback:
void custom_sort(int arr[], int n, bool (*cmp)(int, int)) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            // Gọi hàm thông qua con trỏ hàm cmp để quyết định có đổi chỗ hay không
            if (cmp(arr[j], arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print_array(const int arr[], int n) {
    for (int i = 0; i < n; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== 1. CU PHAP CON TRO HAM CO BAN ===\n";
    // Khai báo con trỏ hàm 'op_ptr' trỏ tới hàm nhận (int, int) và trả về int
    int (*op_ptr)(int, int) = nullptr;

    op_ptr = &add; // Gán địa chỉ hàm add (có thể viết gọn là: op_ptr = add;)
    std::cout << "Goi qua op_ptr (add):      10 + 5 = " << op_ptr(10, 5) << "\n";

    op_ptr = multiply;
    std::cout << "Goi qua op_ptr (multiply): 10 * 5 = " << op_ptr(10, 5) << "\n\n";


    std::cout << "=== 2. UNG DUNG CON TRO HAM LAM CALLBACK TRONG SAP XEP ===\n";
    int a[] = {42, 15, 8, 99, 23, 4};
    int n = sizeof(a) / sizeof(a[0]);

    // Truyền hàm 'ascending' vào để sắp xếp tăng dần
    custom_sort(a, n, ascending);
    std::cout << "Sap xep TANG DAN (truyen ham ascending) : ";
    print_array(a, n);

    // Truyền hàm 'descending' vào cùng hàm custom_sort đó để sắp xếp giảm dần!
    custom_sort(a, n, descending);
    std::cout << "Sap xep GIAM DAN (truyen ham descending): ";
    print_array(a, n);

    return 0;
}
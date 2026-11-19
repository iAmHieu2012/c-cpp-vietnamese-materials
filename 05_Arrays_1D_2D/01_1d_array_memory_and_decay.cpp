#include <iostream>

// KHI TRUYỀN MẢNG VÀO HÀM: 'int arr[]' thực chất bị biên dịch thành con trỏ 'int *arr'
// Do đó, BẮT BUỘC phải truyền kèm kích thước 'int n' vào hàm!
void modify_and_check_size(int arr[], int n) {
    // CẢNH BÁO: sizeof(arr) ở đây chỉ bằng 8 bytes (trên máy 64-bit), KHÔNG phải 20 bytes!
    std::cout << "  [Trong ham] sizeof(arr) chi con: " << sizeof(arr) << " bytes (Array Decay!)\n";
    
    // Sửa phần tử đầu tiên -> Mảng gốc ở main() sẽ bị đổi theo!
    if (n > 0) {
        arr[0] = 999; 
    }
}

// Nếu chỉ muốn đọc mảng mà cấm hàm sửa đổi mảng gốc, hãy thêm từ khóa 'const'
void print_array(const int arr[], int n) {
    std::cout << "  Cac phan tu: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    // 1. Khởi tạo mảng 5 phần tử kiểu int (Mỗi int = 4 bytes -> Tổng = 20 bytes)
    int a[5] = {10, 20, 30, 40, 50};
    
    // Công thức chuẩn để tính số lượng phần tử của mảng tĩnh trong cùng phạm vi:
    size_t n = sizeof(a) / sizeof(a[0]);
    std::cout << "=== 1. KIEM CHUNG O NHO LIEN TIEP TREN RAM ===\n";
    std::cout << "[Trong main] Tong kich thuoc sizeof(a) = " << sizeof(a) 
              << " bytes | So phan tu n = " << n << "\n";

    // Quan sát địa chỉ hệ Hex: Mỗi phần tử cách nhau đúng 4 bytes!
    for (size_t i = 0; i < n; ++i) {
        std::cout << "  a[" << i << "] = " << a[i] << " \t| Dia chi: " << &a[i] << "\n";
    }

    // Mẹo: Khởi tạo toàn bộ mảng bằng số 0 cực nhanh bằng {0} hoặc {}
    int zero_arr[100] = {0}; 
    (void)zero_arr; // Tránh warning biến chưa dùng

    std::cout << "\n=== 2. HIEN TUONG ARRAY DECAY KHI TRUYEN VAO HAM ===\n";
    std::cout << "Truoc khi goi ham: ";
    print_array(a, n);

    modify_and_check_size(a, n);

    std::cout << "Sau khi goi ham:   ";
    print_array(a, n); // a[0] đã bị đổi thành 999!

    return 0;
}
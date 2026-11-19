#include <iostream>

// Hàm mô phỏng cách std::vector tự động nhân đôi sức chứa (Capacity) khi mảng đầy:
// Chú ý tham số 'int *&arr': Tham chiếu tới con trỏ (để đổi hướng con trỏ gốc bên ngoài)
void resize_dynamic_array(int *&arr, int &capacity, int new_capacity) {
    // Bước 1: Cấp phát vùng nhớ mới lớn hơn trên Heap và khởi tạo = 0 bằng cặp ngoặc ()
    int *new_arr = new int[new_capacity](); 

    // Bước 2: Sao chép các phần tử từ mảng cũ sang mảng mới
    int elements_to_copy = (capacity < new_capacity) ? capacity : new_capacity;
    for (int i = 0; i < elements_to_copy; ++i) {
        new_arr[i] = arr[i];
    }

    // Bước 3: Giải phóng vùng nhớ cũ trên Heap (Chống Memory Leak!)
    delete[] arr;

    // Bước 4: Cho con trỏ gốc trỏ sang vùng nhớ mới và cập nhật capacity
    arr = new_arr;
    capacity = new_capacity;
}

int main() {
    std::cout << "=== 1. CAP PHAT BIEN DON LE VA MANG TRONG C++ ===\n";
    // Cấp phát 1 biến int đơn lẻ và khởi tạo = 2026
    int *single_ptr = new int(2026);
    std::cout << "Bien don le tren Heap: " << *single_ptr << "\n";

    // Cấp phát mảng 5 phần tử khởi tạo sẵn giá trị (C++11)
    int *arr_init = new int[5]{10, 20, 30}; // 2 phần tử cuối tự động bằng 0
    std::cout << "Mang khoi tao nhanh:   ";
    for (int i = 0; i < 5; ++i) std::cout << arr_init[i] << " ";
    std::cout << "\n\n";

    // QUY TẮC BẮT BUỘC: 'new' đi với 'delete', còn 'new[]' BẮT BUỘC đi với 'delete[]'!
    delete single_ptr;
    single_ptr = nullptr;

    delete[] arr_init;
    arr_init = nullptr;


    std::cout << "=== 2. TU DONG NHAN DOI KICH THUOC MANG DONG KHI DAY ===\n";
    int capacity = 2; // Sức chứa tối đa ban đầu chỉ có 2 ô
    int size = 0;     // Số phần tử thực tế đang có
    int *dyn_arr = new int[capacity];

    // Thử thêm liên tiếp 6 phần tử vào mảng có sức chứa ban đầu chỉ là 2:
    for (int val = 10; val <= 60; val += 10) {
        if (size == capacity) {
            int old_cap = capacity;
            resize_dynamic_array(dyn_arr, capacity, capacity * 2);
            std::cout << "  [Auto-Resize] Mang day! Da mo rong Capacity tu " 
                      << old_cap << " -> " << capacity << " (Dia chi moi: " << dyn_arr << ")\n";
        }
        dyn_arr[size++] = val;
    }

    std::cout << "Mang sau khi them 6 phan tu: ";
    for (int i = 0; i < size; ++i) std::cout << dyn_arr[i] << " ";
    std::cout << "(Size = " << size << ", Capacity = " << capacity << ")\n";

    delete[] dyn_arr;
    dyn_arr = nullptr;

    return 0;
}
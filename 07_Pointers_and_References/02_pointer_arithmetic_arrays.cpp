#include <iostream>

int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    int *ptr = arr; // Tên mảng 'arr' tự động phân rã thành địa chỉ phần tử đầu tiên (&arr[0])

    std::cout << "=== 1. SO HOC CON TRO (POINTER ARITHMETIC: ptr + i) ===\n";
    for (int i = 0; i < 5; ++i) {
        // *(ptr + i) hoàn toàn tương đương với arr[i]
        std::cout << "i = " << i 
                  << " | Dia chi (ptr + " << i << ") = " << (ptr + i) 
                  << " | Gia tri *(ptr + " << i << ") = " << *(ptr + i) << "\n";
    }

    std::cout << "\n=== 2. DUYET MANG KHONG DUNG CHI SO (DUNG CON TRO CHAY) ===\n";
    std::cout << "Cac phan tu: ";
    // Cho con trỏ p chạy từ đầu mảng (arr) đến trước địa chỉ kết thúc (arr + 5)
    for (int *p = arr; p < arr + 5; ++p) {
        std::cout << *p << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== 3. PHEP TRU HAI CON TRO CUNG MANG (TINH KHOANG CACH PHAN TU) ===\n";
    int *p_start = &arr[1]; // Trỏ vào 20
    int *p_end   = &arr[4]; // Trỏ vào 50
    // Hiệu 2 con trỏ trả về kiểu ptrdiff_t: số lượng phần tử nằm giữa chúng!
    std::cout << "Khoang cach (p_end - p_start) = " << (p_end - p_start) << " phan tu\n";

    return 0;
}
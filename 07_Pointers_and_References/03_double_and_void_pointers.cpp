#include <iostream>

// Hàm đổi hướng trỏ của 2 con trỏ bằng Con trỏ cấp 2 (Chuẩn ngôn ngữ C)
void swap_pointers_c_style(const char **p1, const char **p2) {
    const char *temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int main() {
    std::cout << "=== 1. CON TRO CAP 2 (DOUBLE POINTER: int **pp) ===\n";
    int val = 88;
    int *p = &val;   // Con trỏ cấp 1: Lưu địa chỉ của biến val
    int **pp = &p;   // Con trỏ cấp 2: Lưu địa chỉ của con trỏ p

    std::cout << "Gia tri val   = " << val   << " | &val = " << &val << "\n";
    std::cout << "Con tro p     = " << p     << " | *p   = " << *p   << " | &p = " << &p << "\n";
    std::cout << "Con tro **pp  = " << pp    << " | *pp  = " << *pp  << " | **pp = " << **pp << "\n\n";

    // Sửa biến val thông qua 2 tầng giải tham chiếu (**pp)
    **pp = 999;
    std::cout << "Sau khi gan **pp = 999 -> val = " << val << "\n\n";

    // Ứng dụng con trỏ cấp 2: Hoán đổi 2 con trỏ chuỗi
    const char *str1 = "C Programming";
    const char *str2 = "Modern C++";
    swap_pointers_c_style(&str1, &str2);
    std::cout << "Sau khi hoan doi con tro: str1 = '" << str1 << "', str2 = '" << str2 << "'\n\n";


    std::cout << "=== 2. CON TRO VOID* (CON TRO DA NANG / GENERIC POINTER) ===\n";
    int a = 10;
    double d = 3.14159;
    void *v_ptr; // Có thể trỏ tới bất kỳ kiểu dữ liệu nào

    v_ptr = &a;
    // Bắt buộc phải ép kiểu (static_cast<int*>) trước khi lấy giá trị *
    std::cout << "v_ptr dang tro toi int    : " << *static_cast<int*>(v_ptr) << "\n";

    v_ptr = &d;
    std::cout << "v_ptr dang tro toi double : " << *static_cast<double*>(v_ptr) << "\n";

    return 0;
}
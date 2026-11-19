#include <iostream>

// 1. TRUYỀN THAM TRỊ (Pass by Value - Dùng cho cả C và C++)
// Máy tính copy giá trị sang 2 ô nhớ MỚI. Thay đổi bên trong KHÔNG ảnh hưởng bên ngoài.
void swap_by_value(int a, int b) {
    std::cout << "  [Trong swap_by_value] Dia chi a: " << &a << " | b: " << &b << " (O nho MOI!)\n";
    int temp = a;
    a = b;
    b = temp;
}

// 2. TRUYỀN THAM CHIẾU BẰNG CON TRỎ (Pass by Pointer - Kiểu của ngôn ngữ C)
// Truyền địa chỉ của biến gốc vào, phải dùng dấu * để giải tham chiếu.
void swap_by_pointer(int *a, int *b) {
    std::cout << "  [Trong swap_by_pointer] Tro toi: " << a << " | " << b << " (Cung dia chi goc!)\n";
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 3. TRUYỀN THAM CHIẾU (Pass by Reference - Vũ khí của C++)
// Biến a và b ở đây chính là "tên gọi khác" của biến gốc (cùng chung 1 địa chỉ RAM).
void swap_by_reference(int &a, int &b) {
    std::cout << "  [Trong swap_by_ref]   Dia chi a: " << &a << " | b: " << &b << " (Chinh la bien goc!)\n";
    int temp = a;
    a = b;
    b = temp;
}

// 4. THAM CHIẾU HẰNG (Pass by Const Reference - Chuẩn công nghiệp khi truyền dữ liệu lớn)
// Không tốn bộ nhớ copy như Tham trị, nhưng cấm hàm sửa đổi dữ liệu gốc!
void print_large_data(const long long &data) {
    // data += 10; // Nếu bỏ comment dòng này sẽ bị LỖI BIÊN DỊCH ngay vì có 'const'!
    std::cout << "Gia tri doc an toan (khong copy): " << data << "\n";
}

int main() {
    int x = 10, y = 20;
    std::cout << "[Trong main]            Dia chi x: " << &x << " | y: " << &y << "\n";
    std::cout << "Gia tri ban dau: x = " << x << ", y = " << y << "\n\n";

    // Thử nghiệm 1: Tham trị (Thất bại!)
    swap_by_value(x, y);
    std::cout << "-> Sau swap_by_value:     x = " << x << ", y = " << y << " (Khong doi!)\n\n";

    // Thử nghiệm 2: Con trỏ kiểu C (Thành công, nhưng phải truyền &x, &y)
    swap_by_pointer(&x, &y);
    std::cout << "-> Sau swap_by_pointer:   x = " << x << ", y = " << y << " (Da doi!)\n\n";

    // Thử nghiệm 3: Tham chiếu kiểu C++ (Thành công, cú pháp gọn đẹp tự nhiên)
    swap_by_reference(x, y);
    std::cout << "-> Sau swap_by_reference: x = " << x << ", y = " << y << " (Da doi lai!)\n\n";

    long long big_num = 999999999999LL;
    print_large_data(big_num);

    return 0;
}
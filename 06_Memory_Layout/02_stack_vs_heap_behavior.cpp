#include <iostream>

// Hàm kiểm chứng Stack Frame phát triển từ địa chỉ CAO xuống địa chỉ THẤP
void check_stack_growth(int depth) {
    int local_in_func = depth;
    std::cout << "  [Stack Frame tang " << depth << "] Dia chi bien local: " << &local_in_func << "\n";
    
    if (depth < 3) {
        check_stack_growth(depth + 1); // Gọi đệ quy tạo thêm Stack Frame mới
    }
}

int main() {
    std::cout << "=== 1. KIEM CHUNG STACK PHAT TRIEN TU CAO XUONG THAP ===\n";
    check_stack_growth(1);

    std::cout << "\n=== 2. KIEM CHUNG HEAP PHAT TRIEN TU THAP LEN CAO ===\n";
    int *p1 = new int(1);
    int *p2 = new int(2);
    int *p3 = new int(3);
    std::cout << "  [Heap lan 1] Dia chi p1: " << p1 << "\n";
    std::cout << "  [Heap lan 2] Dia chi p2: " << p2 << "\n";
    std::cout << "  [Heap lan 3] Dia chi p3: " << p3 << "\n";

    // Thu hồi bộ nhớ Heap tránh Memory Leak
    delete p1;
    delete p2;
    delete p3;

    std::cout << "\n=== 3. CAP PHAT MANG LON TREN HEAP (TRANH STACK OVERFLOW) ===\n";
    // Nếu viết: int big_stack[2000000]; (8MB) -> Dễ bị sập chương trình (Stack Overflow)!
    // Nhưng cấp phát trên Heap thì thoải mái (miễn là RAM máy còn trống):
    size_t n = 5000000; // 5 triệu phần tử (~20 MB)
    int *big_heap = new int[n];
    big_heap[0] = 100;
    big_heap[n - 1] = 999;
    std::cout << "  Da cap phat thanh cong " << (n * sizeof(int)) / (1024 * 1024) 
              << " MB tren Heap tai dia chi: " << big_heap << "\n";
    
    // Xóa mảng trên Heap dùng delete[]
    delete[] big_heap;

    return 0;
}
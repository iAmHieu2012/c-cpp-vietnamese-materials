#include <iostream>
#include <cstdlib> // Dùng cho malloc/free

// =============================================================================
// KHAI BÁO BIẾN TOÀN CỤC (NẰM Ở DATA & BSS SEGMENT)
// =============================================================================
int global_init_var = 42;       // Đã khởi tạo (!= 0) -> Nằm ở DATA SEGMENT
int global_uninit_var;          // Chưa khởi tạo      -> Nằm ở BSS SEGMENT (Tự động = 0)
const int global_const = 100;   // Hằng toàn cục      -> Nằm ở Read-Only DATA (.rodata)

void dummy_function() {
    // Hàm này nằm ở TEXT / CODE SEGMENT
}

int main() {
    // 1. Biến tĩnh cục bộ (Static Local Variables) - Vẫn nằm ở DATA / BSS chứ KHÔNG nằm ở Stack!
    static int static_init_var = 99; // Nằm ở DATA SEGMENT
    static int static_uninit_var;    // Nằm ở BSS SEGMENT (Tự động = 0)

    // 2. Hằng chuỗi (String Literal) - Nằm ở Read-Only DATA (.rodata)
    const char *str_literal = "Hello Memory Layout";

    // 3. Cấp phát động trên HEAP
    int *heap_var1 = new int(111);
    int *heap_var2 = new int(222);

    // 4. Biến cục bộ trên STACK
    int stack_var1 = 10;
    int stack_var2 = 20;

    std::cout << "=== SO DO DIA CHI THUC TE TREN RAM (TU THAP DEN CAO) ===\n\n";

    std::cout << "1. [TEXT / CODE SEGMENT] (Dia chi thap nhat - Ma may cua ham):\n";
    std::cout << "   - Dia chi ham main()          : " << (void*)&main << "\n";
    std::cout << "   - Dia chi ham dummy_function(): " << (void*)&dummy_function << "\n\n";

    std::cout << "2. [DATA SEGMENT & .rodata] (Bien Global/Static da khoi tao & Hang):\n";
    std::cout << "   - Hang toan cuc (global_const): " << (void*)&global_const << "\n";
    std::cout << "   - Hang chuoi (str_literal)    : " << (void*)str_literal << "\n";
    std::cout << "   - global_init_var (= 42)      : " << (void*)&global_init_var << "\n";
    std::cout << "   - static_init_var (= 99)      : " << (void*)&static_init_var << "\n\n";

    std::cout << "3. [BSS SEGMENT] (Bien Global/Static chua khoi tao - Tu dong = 0):\n";
    std::cout << "   - global_uninit_var (val = " << global_uninit_var << ") : " << (void*)&global_uninit_var << "\n";
    std::cout << "   - static_uninit_var (val = " << static_uninit_var << ") : " << (void*)&static_uninit_var << "\n\n";

    std::cout << "4. [HEAP SEGMENT] (Cap phat dong bang 'new' - Phinh tu duoi len):\n";
    std::cout << "   - Ban than con tro &heap_var1 (nam tren Stack): " << (void*)&heap_var1 << "\n";
    std::cout << "   - Vung nho dong heap_var1 tro toi (tren Heap) : " << (void*)heap_var1 << "\n";
    std::cout << "   - Vung nho dong heap_var2 tro toi (tren Heap) : " << (void*)heap_var2 << "\n\n";

    std::cout << "5. [STACK SEGMENT] (Dia chi cao nhat - Bien cuc bo trong main):\n";
    std::cout << "   - stack_var1 (= 10)           : " << (void*)&stack_var1 << "\n";
    std::cout << "   - stack_var2 (= 20)           : " << (void*)&stack_var2 << "\n";

    // QUAN TRỌNG: Vùng nhớ trên Heap bắt buộc phải tự giải phóng bằng delete!
    delete heap_var1;
    delete heap_var2;

    return 0;
}   
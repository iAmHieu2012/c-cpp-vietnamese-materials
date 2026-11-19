#include <iostream>
#include <cstring>

// =============================================================================
// BÀI 1: PHÂN LOẠI 5 PHÂN VÙNG BỘ NHỚ (TEXT, DATA, .RODATA, BSS, STACK, HEAP)
// =============================================================================
int g_initialized = 42;            // -> DATA Segment (Biến toàn cục đã khởi tạo != 0)
int g_uninitialized;               // -> BSS Segment  (Biến toàn cục chưa khởi tạo -> tự động = 0)
const int g_const = 2026;          // -> Read-Only Data (.rodata)

void exercise_1_memory_segments() {
    static int s_init = 100;       // -> DATA Segment (Biến static đã khởi tạo != 0)
    static int s_zero;             // -> BSS Segment  (Biến static chưa khởi tạo -> tự động = 0)
    int local_var = 10;            // -> STACK Segment
    int *heap_var = new int(99);   // -> Bản thân `heap_var` trên STACK, vùng nhớ `99` trên HEAP

    std::cout << "\n[Bai 1] Kiem chung dia chi va gia tri mac dinh cua 5 phan vung:\n";
    std::cout << "  1. Text Segment   (&exercise_1)   : " << reinterpret_cast<void*>(&exercise_1_memory_segments) << "\n";
    std::cout << "  2. .rodata        (&g_const)      : " << &g_const << " (val = " << g_const << ")\n";
    std::cout << "  3. Data Segment   (&g_initialized): " << &g_initialized << " | (&s_init): " << &s_init << "\n";
    std::cout << "  4. BSS Segment    (&g_uninit)     : " << &g_uninitialized << " (val = " << g_uninitialized
              << ") | (&s_zero): " << &s_zero << " (val = " << s_zero << ")\n";
    std::cout << "  5. Heap Segment   (heap_var)      : " << heap_var << " (val = " << *heap_var << ")\n";
    std::cout << "  6. Stack Segment  (&local_var)    : " << &local_var << " | (&heap_var): " << &heap_var << "\n";

    delete heap_var;
}


// =============================================================================
// BÀI 2: PHÂN BIỆT CHUỖI TRÊN STACK (`char s[]`) VS CHUỖI `.RODATA` (`const char *p`)
// =============================================================================
void exercise_2_stack_vs_rodata_string() {
    // Con trỏ p (trên Stack) trỏ thẳng vào hằng chuỗi nằm ở vùng .rodata (Read-Only)
    const char *ro_str = "Hello C++";
    // ro_str[0] = 'h'; // LỖI: Cố tình ghi vào vùng .rodata sẽ gây Segmentation Fault!

    // Mảng ký tự trên Stack: Sao chép 10 bytes {'H','e','l','l','o',' ','C','+','+','\0'} lên Stack
    char stack_str[] = "Hello C++";
    stack_str[0] = 'h'; // HỢP LỆ 100%: Sửa dữ liệu trên Stack an toàn tuyệt đối!

    std::cout << "\n[Bai 2] So sanh chuoi tren .rodata vs Stack:\n";
    std::cout << "  ro_str (.rodata)  : \"" << ro_str << "\" | Dia chi: " << static_cast<const void*>(ro_str)
              << " | sizeof(ro_str)    = " << sizeof(ro_str) << " bytes (kich thuoc con tro)\n";
    std::cout << "  stack_str (Stack) : \"" << stack_str << "\" | Dia chi: " << static_cast<const void*>(stack_str)
              << " | sizeof(stack_str) = " << sizeof(stack_str) << " bytes (kich thuoc mang)\n";
}


// =============================================================================
// BÀI 3: SỬA LỖI TRẢ VỀ ĐỊA CHỈ BIẾN LOCAL TRÊN STACK (DANGLING POINTER)
// =============================================================================
/* HÀM LỖI KINH ĐIỂN:
int* create_array_wrong(int n) {
    int arr[100]; // Nằm trên Stack Frame của hàm, sẽ bị hủy ngay khi return!
    return arr;   // LỖI DANGLING POINTER!
}
*/

int* create_sequence_on_heap(int n) {
    if (n <= 0) return nullptr;
    int *arr = new int[n]; // Cấp phát trên Heap -> Sống sót qua khỏi phạm vi hàm!
    for (int i = 0; i < n; ++i) arr[i] = (i + 1) * 10;
    return arr;
}

void exercise_3_fix_dangling_pointer() {
    int n = 5;
    int *seq = create_sequence_on_heap(n);
    std::cout << "\n[Bai 3] Mang song sot sau khi ham ket thuc nho cap phat Heap:\n  ";
    for (int i = 0; i < n; ++i) std::cout << seq[i] << " ";
    std::cout << " (Dia chi Heap: " << seq << ")\n";

    delete[] seq;
    seq = nullptr; // Chống Dangling Pointer sau khi delete
}


// =============================================================================
// BÀI 4: ĐO ĐỘ SÂU CALL STACK & KÍCH THƯỚC MỖI STACK FRAME KHI ĐỆ QUY
// =============================================================================
void trace_stack_frames(int depth, const char *first_frame_addr) {
    char frame_marker = 'X';
    const char *curr_addr = &frame_marker;
    long long bytes_used = first_frame_addr - curr_addr; // Stack phát triển từ cao xuống thấp

    std::cout << "  Depth #" << depth << " | Dia chi bien local: "
              << static_cast<const void*>(curr_addr)
              << " | Do lech so voi Depth #1: " << bytes_used << " bytes\n";

    if (depth < 4) {
        trace_stack_frames(depth + 1, first_frame_addr);
    }
}

void exercise_4_call_stack_growth() {
    std::cout << "\n[Bai 4] Kiem chung Stack Frame mo rong tu dia chi CAO xuong THAP:\n";
    char root_marker = 'R';
    trace_stack_frames(1, &root_marker);
}


// =============================================================================
// BÀI 5: PHÒNG TRÁNH STACK OVERFLOW & MEMORY LEAK VỚI MẢNG LỚN
// =============================================================================
void exercise_5_large_array_and_leak_prevention() {
    std::cout << "\n[Bai 5] Xu ly mang lon (1,000,000 phan tu ~ 4MB) an toan:\n";
    // Nếu khai báo cục bộ `int big[1000000];` trên Stack có thể gây Stack Overflow.
    // Giải pháp 1: Đưa sang BSS Segment bằng `static` (hoặc cấp phát trên Heap bằng `new[]`)
    static int bss_large_array[1000000];
    bss_large_array[0] = 111;
    bss_large_array[999999] = 999;
    std::cout << "  1. Mang static 4MB tren BSS:Dia chi = " << bss_large_array
              << " | Phan tu cuoi = " << bss_large_array[999999] << "\n";

    // Giải pháp 2: Cấp phát và thu hồi trên Heap trong vòng lặp (chống Memory Leak)
    for (int step = 1; step <= 3; ++step) {
        int *heap_chunk = new int[250000]; // ~1MB mỗi lượt
        heap_chunk[0] = step * 100;
        std::cout << "  2. Vong lap " << step << ": Cap phat 1MB Heap tai " << heap_chunk;
        delete[] heap_chunk; // Bắt buộc giải phóng ngay cuối mỗi vòng lặp!
        std::cout << " -> Da delete[] an toan!\n";
    }
}


int main() {
    exercise_1_memory_segments();
    exercise_2_stack_vs_rodata_string();
    exercise_3_fix_dangling_pointer();
    exercise_4_call_stack_growth();
    exercise_5_large_array_and_leak_prevention();
    return 0;
}

#include <iostream>
#include <string>

class DynamicBuffer {
private:
    std::string label;
    int *data;
    int size;
    static int total_buffers; // Biến static nằm ở Data/BSS, KHÔNG làm tăng sizeof(DynamicBuffer)

public:
    // 1. Parameterized Constructor (kèm từ khóa 'explicit' khi có thể gọi với 1 tham số)
    explicit DynamicBuffer(const std::string &lbl, int n = 0)
        : label(lbl), data(n > 0 ? new int[n]() : nullptr), size(n) {
        ++total_buffers;
        std::cout << "  [Parameterized Constructor] Tao '" << label << "' (Heap: " << n << " ints)\n";
    }

    // 2. Delegating Constructor (C++11): Default Constructor ủy quyền gọi Parameterized Constructor
    DynamicBuffer() : DynamicBuffer("Default", 0) {
        std::cout << "  [Delegating Default Constructor] Hoan tat khoi tao '" << label << "'\n";
    }

    // 3. Copy Constructor (Sao chép sâu - Deep Copy để tránh 2 object dùng chung 1 con trỏ Heap)
    DynamicBuffer(const DynamicBuffer &other)
        : label(other.label + "_copy"), data(nullptr), size(other.size) {
        ++total_buffers;
        if (other.data != nullptr && size > 0) {
            data = new int[size];
            for (int i = 0; i < size; ++i) data[i] = other.data[i];
        }
        std::cout << "  [Copy Constructor] Sao chep tu '" << other.label << "' -> '" << label << "'\n";
    }

    // 4. Destructor (~ClassName): Tự động chạy để giải phóng Heap (RAII)
    ~DynamicBuffer() {
        delete[] data;
        data = nullptr;
        --total_buffers;
        std::cout << "  [~Destructor] Da huy '" << label << "' va thu hoi Heap!\n";
    }

    void set_value(int idx, int val) {
        if (idx >= 0 && idx < size) data[idx] = val;
    }

    void print() const {
        std::cout << "  Buffer '" << label << "': ";
        for (int i = 0; i < size; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};

int DynamicBuffer::total_buffers = 0;

int main() {
    std::cout << "=== 1. KIEM CHUNG SIZEOF(OBJECT) & EXPLICIT CONSTRUCTOR ===\n";
    std::cout << "  sizeof(DynamicBuffer) = " << sizeof(DynamicBuffer)
              << " bytes (Chi chua string + int* + int, KHONG chua bien static va ham!)\n\n";

    std::cout << "=== 2. VONG DOI OBJECT TRONG SCOPE {} (LIFO DESTRUCTION) ===\n";
    {
        DynamicBuffer b1;              // Gọi Delegating Default Constructor
        // DynamicBuffer err = "Test"; // LỖI BIÊN DỊCH nhờ từ khóa 'explicit' chặn ép kiểu ngầm!
        DynamicBuffer b2("BufA", 3);   // Gọi Parameterized Constructor
        b2.set_value(0, 10);
        b2.set_value(1, 20);
        b2.set_value(2, 30);

        DynamicBuffer b3 = b2;         // Gọi Copy Constructor
        b3.set_value(0, 999);          // Sửa b3 KHÔNG ảnh hưởng b2 nhờ Deep Copy

        b2.print();
        b3.print();
        std::cout << "--- Chuan bi thoat khoi scope {} (Destructor chay theo thu tu nguoc: b3 -> b2 -> b1) ---\n";
    }

    std::cout << "\n=== 3. OBJECT CAP PHAT DONG TREN HEAP ===\n";
    DynamicBuffer *heap_buf = new DynamicBuffer("HeapBuf", 2);
    delete heap_buf; // Bắt buộc delete để kích hoạt ~Destructor!

    return 0;
}

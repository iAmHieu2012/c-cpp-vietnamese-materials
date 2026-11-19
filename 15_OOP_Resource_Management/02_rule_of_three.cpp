#include <iostream>

class DynamicArray {
private:
    int *data;
    size_t size;

public:
    DynamicArray(size_t n = 0) : data(n > 0 ? new int[n]() : nullptr), size(n) {}

    // 1. Destructor
    ~DynamicArray() {
        delete[] data;
    }

    // 2. Copy Constructor (Khởi tạo object mới từ object cũ: DynamicArray b = a;)
    DynamicArray(const DynamicArray &other) : data(nullptr), size(other.size) {
        if (size > 0) {
            data = new int[size];
            for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        }
        std::cout << "  [Copy Constructor] Deep copy " << size << " phan tu\n";
    }

    // 3. Copy Assignment Operator (Gán object đã tồn tại: b = a;)
    DynamicArray& operator=(const DynamicArray &other) {
        std::cout << "  [Copy Assignment] Gan " << other.size << " phan tu\n";
        // Kiểm tra Self-Assignment (a = a)
        if (this == &other) return *this;

        // Bước 1: Giải phóng tài nguyên cũ của đối tượng hiện tại
        delete[] data;

        // Bước 2: Cấp phát mới và copy dữ liệu từ other
        size = other.size;
        data = (size > 0) ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];

        return *this; // Trả về *this để hỗ trợ gán liên tiếp (a = b = c)
    }

    int& operator[](size_t idx) { return data[idx]; }

    void print(const char *name) const {
        std::cout << "  " << name << ": ";
        for (size_t i = 0; i < size; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};

int main() {
    DynamicArray a(3);
    a[0] = 10; a[1] = 20; a[2] = 30;

    DynamicArray b = a; // Gọi Copy Constructor
    DynamicArray c(2);
    c = a;              // Gọi Copy Assignment Operator
    c = c;              // Test Self-Assignment (an toàn nhờ if (this == &other))

    b[0] = 99;
    c[2] = 77;

    a.print("a");
    b.print("b");
    c.print("c");

    return 0;
}
#include <iostream>
#include <utility> // std::move

class Buffer {
private:
    int *data;
    size_t size;

public:
    Buffer(size_t n = 0) : data(n > 0 ? new int[n]() : nullptr), size(n) {
        std::cout << "  [Constructor] Cap phat " << size << " ints\n";
    }

    // 1. Destructor
    ~Buffer() {
        delete[] data;
    }

    // 2. Copy Constructor (O(N))
    Buffer(const Buffer &other) : data(other.size > 0 ? new int[other.size] : nullptr), size(other.size) {
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        std::cout << "  [Copy Constructor] O(N) - Copy " << size << " ints\n";
    }

    // 3. Copy Assignment Operator (O(N))
    Buffer& operator=(const Buffer &other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = (size > 0) ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        std::cout << "  [Copy Assignment] O(N)\n";
        return *this;
    }

    // 4. Move Constructor (O(1)) - Luôn đánh dấu noexcept để STL (như std::vector) tin dùng
    Buffer(Buffer &&other) noexcept : data(other.data), size(other.size) {
        // Cướp con trỏ của other, sau đó gán other về trạng thái rỗng an toàn
        other.data = nullptr;
        other.size = 0;
        std::cout << "  [Move Constructor] O(1) - Cuop quyen so huu Heap!\n";
    }

    // 5. Move Assignment Operator (O(1))
    Buffer& operator=(Buffer &&other) noexcept {
        if (this == &other) return *this;
        delete[] data;        // Giải phóng tài nguyên cũ của mình

        data = other.data;    // Cướp con trỏ của other
        size = other.size;

        other.data = nullptr; // Đưa other về rỗng để ~other không xóa mất vùng nhớ vừa cướp
        other.size = 0;
        std::cout << "  [Move Assignment] O(1) - Cuop quyen so huu Heap!\n";
        return *this;
    }

    size_t get_size() const { return size; }
};

int main() {
    Buffer b1(1000000);

    std::cout << "\n--- Chuyen quyen so huu bang std::move (Move Constructor) ---\n";
    Buffer b2 = std::move(b1); // Không tốn 1 micro-giây copy nào!
    std::cout << "  b1.size = " << b1.get_size() << " | b2.size = " << b2.get_size() << "\n";

    std::cout << "\n--- Gan di chuyen (Move Assignment) ---\n";
    Buffer b3(5);
    b3 = std::move(b2);
    std::cout << "  b2.size = " << b2.get_size() << " | b3.size = " << b3.get_size() << "\n";

    return 0;
}
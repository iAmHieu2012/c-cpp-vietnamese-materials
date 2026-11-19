#include <iostream>
#include <vector>
#include <utility>

class HeavyPacket {
private:
    int *payload;
    std::size_t size;

public:
    explicit HeavyPacket(std::size_t n = 0)
        : payload(n > 0 ? new int[n]() : nullptr), size(n) {
        std::cout << "  [Constructor] Cap phat Heap (" << size << " ints)\n";
    }

    ~HeavyPacket() {
        delete[] payload;
    }

    // Copy Constructor - O(N)
    HeavyPacket(const HeavyPacket &other)
        : payload(other.size > 0 ? new int[other.size] : nullptr), size(other.size) {
        for (std::size_t i = 0; i < size; ++i) payload[i] = other.payload[i];
        std::cout << "  [COPY Constructor - O(N)] Sao chep " << size << " ints!\n";
    }

    // Copy Assignment - O(N)
    HeavyPacket& operator=(const HeavyPacket &other) {
        if (this == &other) return *this;
        delete[] payload;
        size = other.size;
        payload = (size > 0) ? new int[size] : nullptr;
        for (std::size_t i = 0; i < size; ++i) payload[i] = other.payload[i];
        std::cout << "  [COPY Assignment - O(N)]\n";
        return *this;
    }

    // Move Constructor - O(1) (BẮT BUỘC có noexcept để std::vector dám dùng khi resize!)
    HeavyPacket(HeavyPacket &&other) noexcept
        : payload(other.payload), size(other.size) {
        other.payload = nullptr;
        other.size = 0;
        std::cout << "  [MOVE Constructor - O(1) noexcept] Cuop con tro Heap (0 allocation)!\n";
    }

    // Move Assignment - O(1) noexcept
    HeavyPacket& operator=(HeavyPacket &&other) noexcept {
        if (this == &other) return *this;
        delete[] payload;
        payload = other.payload;
        size = other.size;
        other.payload = nullptr;
        other.size = 0;
        std::cout << "  [MOVE Assignment - O(1) noexcept] Cuop con tro Heap!\n";
        return *this;
    }
};

int main() {
    std::cout << "=== 1. MOVE CONSTRUCTOR & MOVE ASSIGNMENT TRUC TIEP ===\n";
    HeavyPacket p1(100000);
    HeavyPacket p2 = std::move(p1); // Gọi Move Constructor O(1)

    HeavyPacket p3(10);
    p3 = std::move(p2);             // Gọi Move Assignment O(1)

    std::cout << "\n=== 2. STD::VECTOR TU DONG DUNG MOVE KHI REALLOCATE (NHO NOEXCEPT) ===\n";
    std::vector<HeavyPacket> vec;
    std::cout << "  -> Day phan tu #1 vao vector:\n";
    vec.push_back(HeavyPacket(50000));
    std::cout << "  -> Day phan tu #2 (Vector day capacity -> tu dong Move phan tu #1 sang vung nho moi!):\n";
    vec.push_back(HeavyPacket(50000));

    return 0;
}

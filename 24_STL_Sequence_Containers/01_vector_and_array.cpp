#include <iostream>
#include <array>
#include <vector>
#include <string>

struct Student {
    int id;
    std::string name;

    Student(int i, std::string n) : id(i), name(std::move(n)) {
        std::cout << "    [Constructor] " << name << "\n";
    }
    Student(const Student &o) : id(o.id), name(o.name) {
        std::cout << "    [Copy] " << name << "\n";
    }
    Student(Student &&o) noexcept : id(o.id), name(std::move(o.name)) {
        std::cout << "    [Move] " << name << "\n";
    }
};

int main() {
    std::cout << "=== 1. STD::ARRAY (STACK, FIXED SIZE, NO ARRAY DECAY) ===\n";
    std::array<int, 4> arr = {10, 20, 30, 40};
    std::cout << "  Size = " << arr.size() << " | front = " << arr.front()
              << " | back = " << arr.back() << " | at(2) = " << arr.at(2) << "\n\n";

    std::cout << "=== 2. STD::VECTOR: RESERVE & EMPLACE_BACK VS PUSH_BACK ===\n";
    std::vector<Student> vec;
    vec.reserve(3); // Cấp phát trước 3 chỗ -> KHÔNG bị reallocation & move lại phần tử cũ!
    std::cout << "  Sau reserve(3): size = " << vec.size() << ", capacity = " << vec.capacity() << "\n";

    std::cout << "  -> push_back (tao object tam roi Move vao vector):\n";
    vec.push_back(Student(1, "An"));

    std::cout << "  -> emplace_back (khoi tao truc tiep tai cho, 0 Copy, 0 Move):\n";
    vec.emplace_back(2, "Binh");
    vec.emplace_back(3, "Chau");

    std::cout << "\n  Danh sach trong vector:\n";
    for (const auto &s : vec) {
        std::cout << "    #" << s.id << " - " << s.name << "\n";
    }

    return 0;
}

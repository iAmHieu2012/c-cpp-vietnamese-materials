// ============================================================================
// File: 02_map_and_multimap.cpp
// Bài 5.3: std::map & std::multimap (operator[] vs find vs try_emplace, equal_range)
// ============================================================================

#include <iostream>
#include <map>
#include <string>

int main() {
    std::cout << "=== 1. STD::MAP (OPERATOR[] VS FIND VS TRY_EMPLACE C++17) ===\n";
    std::map<std::string, int> scores = {{"An", 85}, {"Binh", 92}};

    // try_emplace (C++17): Chỉ chèn nếu key chưa tồn tại, không ghi đè key cũ
    auto [it1, inserted1] = scores.try_emplace("Chau", 78);
    auto [it2, inserted2] = scores.try_emplace("An", 100); // Bị bỏ qua vì "An" đã có
    std::cout << "  try_emplace(\"Chau\", 78) thanh cong? " << (inserted1 ? "Co" : "Khong") << "\n";
    std::cout << "  try_emplace(\"An\", 100)  thanh cong? " << (inserted2 ? "Co" : "Khong (giu nguyen 85)") << "\n";

    // Tra cứu an toàn bằng find() (tránh tạo key rác như scores["Dung"])
    if (auto found = scores.find("Dung"); found != scores.end()) {
        std::cout << "  Dung: " << found->second << "\n";
    } else {
        std::cout << "  Tra cuu find(\"Dung\"): Khong ton tai (size van la "
                  << scores.size() << ")\n";
    }

    std::cout << "\n  Danh sach diem theo thu tu bang chu cai Key:\n";
    for (const auto &[name, sc] : scores) {
        std::cout << "    * " << name << " : " << sc << "\n";
    }

    std::cout << "\n=== 2. STD::MULTIMAP & TRUY VAN EQUAL_RANGE ===\n";
    std::multimap<std::string, std::string> courses = {
        {"CS101", "An"},
        {"CS102", "Binh"},
        {"CS101", "Chau"},
        {"CS101", "Dung"}
    };

    auto [first, last] = courses.equal_range("CS101");
    std::cout << "  Danh sach sinh vien dang ky lop CS101: ";
    for (auto it = first; it != last; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << "\n";

    return 0;
}

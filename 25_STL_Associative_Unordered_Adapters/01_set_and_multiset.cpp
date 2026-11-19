// ============================================================================
// File: 01_set_and_multiset.cpp
// Bài 5.3: std::set & std::multiset (Red-Black Tree, Range Query, Xóa 1 bản sao)
// ============================================================================

#include <iostream>
#include <set>
#include <string>

struct Candidate {
    std::string name;
    int score;
};

struct CandidateCompare {
    bool operator()(const Candidate &a, const Candidate &b) const {
        if (a.score != b.score) return a.score > b.score; // Điểm cao đứng trước
        return a.name < b.name;
    }
};

int main() {
    std::cout << "=== 1. STD::SET & RANGE QUERY (LOWER_BOUND / UPPER_BOUND) ===\n";
    std::set<int> s = {30, 10, 50, 20, 40, 20}; // Tự động sắp xếp & loại trùng
    std::cout << "  Phan tu trong set: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    // Tìm các phần tử trong đoạn [20, 40] trong O(log N) bằng phương thức thành viên
    auto itLow = s.lower_bound(20);  // Phần tử đầu tiên >= 20
    auto itHigh = s.upper_bound(40); // Phần tử đầu tiên > 40
    std::cout << "  Doan [20, 40]: ";
    for (auto it = itLow; it != itHigh; ++it) std::cout << *it << " ";
    std::cout << "\n\n";

    std::cout << "=== 2. STD::MULTISET (XOA DUNG 1 BAN SAO BANG ITERATOR) ===\n";
    std::multiset<int> ms = {10, 20, 20, 20, 30};
    if (auto it = ms.find(20); it != ms.end()) {
        ms.erase(it); // Chỉ xóa ĐÚNG 1 số 20 (nếu gọi ms.erase(20) sẽ xóa cả 3!)
    }
    std::cout << "  Multiset sau khi xoa 1 ban sao cua 20: ";
    for (int x : ms) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== 3. STD::SET VOI CUSTOM FUNCTOR COMPARATOR ===\n";
    std::set<Candidate, CandidateCompare> leaderboard = {
        {"Binh", 90}, {"An", 95}, {"Chau", 90}
    };
    std::cout << "  Leaderboard: ";
    for (const auto &c : leaderboard) {
        std::cout << "[" << c.name << ":" << c.score << "] ";
    }
    std::cout << "\n";

    return 0;
}

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

struct Point {
    int x, y;
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

// Custom Hash Functor cho struct Point (Công thức trộn bit hash_combine chuẩn Boost)
struct PointHash {
    std::size_t operator()(const Point &p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

int main() {
    std::cout << "=== 1. STD::UNORDERED_MAP (DEM TAN SUAT O(1) AVG) ===\n";
    std::unordered_map<std::string, int> freq;
    freq.reserve(16); // Cấp phát trước số bucket để tránh rehash nhiều lần

    for (const char *w : {"cpp", "stl", "cpp", "dsa", "stl", "cpp"}) {
        ++freq[w]; // Ở đây operator[] rất tiện vì tự khởi tạo = 0 rồi tăng lên 1
    }

    for (const auto &[word, count] : freq) {
        std::cout << "  " << word << " -> " << count << " lan\n";
    }
    std::cout << "  Load factor: " << freq.load_factor() << " | Buckets: " << freq.bucket_count() << "\n\n";

    std::cout << "=== 2. CUSTOM STRUCT LAM KEY TRONG UNORDERED_SET ===\n";
    std::unordered_set<Point, PointHash> visited;
    visited.insert({0, 0});
    visited.insert({1, 2});
    visited.insert({2, 1}); // Không đụng độ mã băm với (1, 2) nhờ công thức hash_combine bất đối xứng
    visited.insert({0, 0}); // Trùng -> tự động bỏ qua

    std::cout << "  So diem duy nhat: " << visited.size() << "\n";
    std::cout << "  Co chua (1, 2)? " << (visited.count({1, 2}) ? "Co" : "Khong") << "\n";

    return 0;
}

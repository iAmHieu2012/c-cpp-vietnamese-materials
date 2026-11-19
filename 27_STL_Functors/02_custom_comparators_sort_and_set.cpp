#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>

struct Product {
    int id;
    std::string name;
    double price;
    int rating; // 1..5 sao
};

// =============================================================================
// CUSTOM COMPARATOR FUNCTOR CHO STD::SORT & STD::SET
// Quy tắc: Trả về true nếu `a` ĐỨNG TRƯỚC `b` (Bắt buộc có `const` ở cuối!)
// =============================================================================
struct ProductRankComparator {
    bool operator()(const Product &a, const Product &b) const {
        // 1. Ưu tiên sản phẩm có Rating cao hơn đứng trước
        if (a.rating != b.rating) return a.rating > b.rating;
        // 2. Cùng Rating thì sản phẩm có Giá rẻ hơn đứng trước
        if (a.price != b.price)   return a.price < b.price;
        // 3. Cùng cả Rating và Giá thì phân biệt theo ID (Bắt buộc cho std::set để không bị coi là trùng!)
        return a.id < b.id;
    }
};

// Functor so sánh chuỗi theo độ dài tăng dần (nếu cùng độ dài thì theo từ điển A-Z)
struct StringLengthComparator {
    bool operator()(const std::string &a, const std::string &b) const {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    }
};

int main() {
    std::vector<Product> catalog = {
        {101, "Ban phim Co",   1200.0, 5},
        {102, "Chuot Gaming",   800.0, 4},
        {103, "Tai nghe Studio",950.0, 5},
        {104, "Lot chuot XXL",  250.0, 4},
        {105, "Webcam 4K",     1200.0, 5}
    };

    std::cout << "=== 1. DUNG FUNCTOR OBJECT TRONG STD::SORT (TRUYEN INSTANCE {}) ===\n";
    // Lưu ý: std::sort là hàm nên truyền ĐỐI TƯỢNG Functor: ProductRankComparator{}
    std::sort(catalog.begin(), catalog.end(), ProductRankComparator{});

    std::cout << std::fixed << std::setprecision(0);
    for (const auto &p : catalog) {
        std::cout << "  [" << p.rating << "* | " << std::setw(5) << p.price << "k] #"
                  << p.id << " - " << p.name << "\n";
    }

    std::cout << "\n=== 2. DUNG FUNCTOR TYPE TRONG STD::SET (TRUYEN KIEU TRONG <>) ===\n";
    // Lưu ý: std::set là Class Template nên truyền KIỂU Functor: std::set<Product, ProductRankComparator>
    std::set<Product, ProductRankComparator> best_sellers(catalog.begin(), catalog.end());
    std::cout << "  San pham dau bang trong std::set: " << best_sellers.begin()->name
              << " (" << best_sellers.begin()->price << "k)\n";

    std::cout << "\n=== 3. STD::MAP VOI CUSTOM KEY COMPARATOR (SAP THEO DO DAI TEN) ===\n";
    std::map<std::string, int, StringLengthComparator> word_rank = {
        {"algorithm", 1}, {"c", 2}, {"stl", 3}, {"cpp", 4}, {"functor", 5}
    };

    std::cout << "  Thu tu Key trong map (ngan -> dai, cung do dai -> A-Z):\n  ";
    for (const auto &[word, id] : word_rank) {
        std::cout << word << "(" << word.size() << ")  ";
    }
    std::cout << "\n";

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional> // std::greater, std::multiplies, std::bit_xor

// =============================================================================
// 1. FUNCTOR CƠ BẢN & STATEFUL FUNCTOR (LƯU TRẠNG THÁI NỘI BỘ)
// =============================================================================
class RangeFilter {
private:
    int min_val;
    int max_val;

public:
    RangeFilter(int low, int high) : min_val(low), max_val(high) {}

    // Nạp chồng toán tử () -> Đối tượng RangeFilter được gọi như một hàm Predicate!
    bool operator()(int x) const noexcept {
        return x >= min_val && x <= max_val;
    }
};

// Stateful Functor dùng với std::for_each để thống kê Min, Max, Sum trong 1 lượt duyệt
struct StatsCollector {
    int count = 0;
    long long sum = 0;

    void operator()(int x) noexcept {
        ++count;
        sum += x;
    }

    double average() const noexcept {
        return count > 0 ? static_cast<double>(sum) / count : 0.0;
    }
};

int main() {
    std::cout << "=== 1. STATEFUL FUNCTOR (BO LOC KHOANG DONG & THONG KE) ===\n";
    std::vector<int> scores = {45, 72, 88, 60, 95, 30, 81, 50};

    RangeFilter pass_filter(50, 85);      // Lọc điểm trong đoạn [50, 85]
    RangeFilter excellent_filter(80, 100);// Lọc điểm trong đoạn [80, 100]

    std::cout << "  So diem trong [50, 85]  : "
              << std::count_if(scores.begin(), scores.end(), pass_filter) << "\n";
    std::cout << "  So diem trong [80, 100] : "
              << std::count_if(scores.begin(), scores.end(), excellent_filter) << "\n";

    // std::for_each trả về bản sao của Functor sau khi đã tích lũy trạng thái qua toàn bộ mảng
    StatsCollector stats = std::for_each(scores.begin(), scores.end(), StatsCollector{});
    std::cout << "  Thong ke qua Functor    : Count = " << stats.count
              << " | Sum = " << stats.sum << " | Avg = " << stats.average() << "\n\n";

    std::cout << "=== 2. CAC FUNCTOR DUNG SAN TRONG <FUNCTIONAL> ===\n";
    // Sắp xếp giảm dần bằng std::greater<int>{}
    std::sort(scores.begin(), scores.end(), std::greater<int>{});
    std::cout << "  Sap xep giam dan (std::greater): ";
    for (int x : scores) std::cout << x << " ";
    std::cout << "\n";

    std::vector<int> small = {1, 2, 3, 4, 5};
    long long factorial = std::accumulate(small.begin(), small.end(), 1LL, std::multiplies<long long>{});
    std::cout << "  Tich 1..5 (std::multiplies)    : " << factorial << "\n";

    std::vector<int> pairs = {4, 7, 4, 9, 7};
    int unique_num = std::accumulate(pairs.begin(), pairs.end(), 0, std::bit_xor<int>{});
    std::cout << "  So duy nhat (std::bit_xor)     : " << unique_num << "\n";

    return 0;
}

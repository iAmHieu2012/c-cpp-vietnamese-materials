#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Player {
    std::string name;
    int score;
};

int main() {
    std::cout << "=== 1. STD::SORT & STD::STABLE_SORT VOI LAMBDA ===\n";
    std::vector<Player> players = {
        {"An", 80}, {"Binh", 95}, {"Chau", 80}, {"Dung", 95}, {"Em", 70}
    };

    // Sắp xếp giảm dần theo điểm; nếu bằng điểm giữ nguyên thứ tự xuất hiện ban đầu (Stable)
    // LƯU Ý SỐNG CÒN: Luôn dùng '>' hoặc '<' (Strict Weak Ordering), TUYỆT ĐỐI KHÔNG dùng '>='!
    std::stable_sort(players.begin(), players.end(), [](const Player &a, const Player &b) {
        return a.score > b.score;
    });

    for (const auto &p : players) {
        std::cout << "  " << p.name << ": " << p.score << "\n";
    }

    std::cout << "\n=== 2. STD::PARTIAL_SORT (LAY TOP K PHAN TU TRONG O(N log K)) ===\n";
    std::vector<int> scores = {75, 99, 60, 88, 92, 54, 81};
    // Chỉ sắp xếp đúng 3 phần tử lớn nhất lên đầu mảng, phần còn lại không tốn công sắp xếp
    std::partial_sort(scores.begin(), scores.begin() + 3, scores.end(), std::greater<int>());
    std::cout << "  Top 3 diem cao nhat: " << scores[0] << ", " << scores[1] << ", " << scores[2] << "\n";

    std::cout << "\n=== 3. STD::NTH_ELEMENT (TIM PHAN TU LON THU K TRONG O(N)) ===\n";
    std::vector<int> nums = {30, 10, 50, 20, 40, 60, 25};
    int k = 3; // Tìm phần tử lớn thứ 3 (chỉ số k - 1 nếu sắp giảm dần)
    std::nth_element(nums.begin(), nums.begin() + (k - 1), nums.end(), std::greater<int>());
    std::cout << "  Phan tu lon thu " << k << " = " << nums[k - 1] << "\n";

    auto [min_it, max_it] = std::minmax_element(nums.begin(), nums.end());
    std::cout << "  Min = " << *min_it << " | Max = " << *max_it << "\n\n";

    std::cout << "=== 4. LOWER_BOUND / UPPER_BOUND / EQUAL_RANGE (O(log N)) ===\n";
    std::vector<int> sorted = {1, 2, 5, 5, 5, 8, 10};
    auto [lo, hi] = std::equal_range(sorted.begin(), sorted.end(), 5);
    std::cout << "  So 5 xuat hien tu chi so " << (lo - sorted.begin())
              << " den " << (hi - sorted.begin() - 1)
              << " (tong " << (hi - lo) << " lan)\n";

    return 0;
}

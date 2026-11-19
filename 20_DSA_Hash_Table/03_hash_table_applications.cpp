#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

// =============================================================================
// 1. ĐẾM TẦN SUẤT TỪ TRONG VĂN BẢN (WORD FREQUENCY COUNTER - O(N))
// =============================================================================
void word_frequency_counter(const std::string &text) {
    std::unordered_map<std::string, int> freq;
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        ++freq[word];
    }

    std::cout << "=== 1. THONG KE TAN SUAT TU BANG HASH MAP ===\n  ";
    for (const auto &[w, count] : freq) {
        std::cout << "[" << w << ": " << count << "] ";
    }
    std::cout << "\n\n";
}

// =============================================================================
// 2. KIỂM TRA TRÙNG LẶP TRONG CỬA SỔ TRƯỢT K (SLIDING WINDOW HASH SET - O(N))
// Kiểm tra có tồn tại a[i] == a[j] sao cho |i - j| <= k hay không
// =============================================================================
bool contains_nearby_duplicate(const std::vector<int> &nums, int k) {
    std::unordered_set<int> window;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (i > k) {
            window.erase(nums[i - k - 1]); // Loại phần tử trượt ra khỏi cửa sổ kích thước k
        }
        auto [it, inserted] = window.insert(nums[i]);
        if (!inserted) return true; // Đã tồn tại trong cửa sổ k phần tử gần nhất!
    }
    return false;
}

int main() {
    word_frequency_counter("hash table fast lookup hash table collision chaining fast");

    std::cout << "=== 2. KIEM TRA TRUNG LAP TRONG KHOANG CACH <= K (HASH SET) ===\n";
    std::vector<int> a1 = {1, 2, 3, 1};
    std::vector<int> a2 = {1, 2, 3, 4, 1};
    std::cout << "  Mang {1, 2, 3, 1}    voi k = 3: "
              << (contains_nearby_duplicate(a1, 3) ? "Co trung" : "Khong") << "\n";
    std::cout << "  Mang {1, 2, 3, 4, 1} voi k = 3: "
              << (contains_nearby_duplicate(a2, 3) ? "Co trung" : "Khong") << "\n";

    return 0;
}

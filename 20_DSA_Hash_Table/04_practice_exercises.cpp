#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// =============================================================================
// BÀI 1: TWO SUM TRÊN MẢNG CHƯA SẮP XẾP BẰNG HASH MAP - O(N) TIME
// =============================================================================
bool two_sum_hash(const std::vector<int> &nums, int target, int &idx1, int &idx2) {
    std::unordered_map<int, int> seen;
    seen.reserve(nums.size());
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int comp = target - nums[i];
        if (auto it = seen.find(comp); it != seen.end()) {
            idx1 = it->second;
            idx2 = i;
            return true;
        }
        seen[nums[i]] = i;
    }
    return false;
}


// =============================================================================
// BÀI 2: KÝ TỰ KHÔNG LẶP LẠI ĐẦU TIÊN TRONG CHUỖI (FIRST UNIQUE CHAR - O(N))
// =============================================================================
int first_unique_char_index(const std::string &s) {
    int freq[256] = {0}; // Bảng băm trực tiếp cho bảng mã ASCII 256 ký tự
    for (unsigned char c : s) ++freq[c];
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (freq[static_cast<unsigned char>(s[i])] == 1) return i;
    }
    return -1;
}


// =============================================================================
// BÀI 3: ĐẾM SỐ ĐOẠN CON LIÊN TIẾP CÓ TỔNG BẰNG K (PREFIX SUM + HASH MAP - O(N))
// =============================================================================
int subarray_sum_equals_k(const std::vector<int> &nums, int k) {
    std::unordered_map<long long, int> prefix_count;
    prefix_count[0] = 1; // Trường hợp đoạn con bắt đầu ngay từ chỉ số 0

    long long curr_sum = 0;
    int total_subarrays = 0;
    for (int x : nums) {
        curr_sum += x;
        // Nếu từng xuất hiện prefix_sum = curr_sum - k trước đó -> đoạn ở giữa có tổng đúng bằng k!
        if (auto it = prefix_count.find(curr_sum - k); it != prefix_count.end()) {
            total_subarrays += it->second;
        }
        ++prefix_count[curr_sum];
    }
    return total_subarrays;
}


// =============================================================================
// BÀI 4: DÃY SỐ LIÊN TIẾP DÀI NHẤT (LONGEST CONSECUTIVE SEQUENCE - O(N) TIME)
// =============================================================================
int longest_consecutive_sequence(const std::vector<int> &nums) {
    std::unordered_set<int> num_set(nums.begin(), nums.end());
    int best_len = 0;

    for (int x : num_set) {
        // Chỉ bắt đầu đếm từ đầu chuỗi (khi x - 1 KHÔNG có trong tập hợp) -> Đảm bảo đúng O(N)!
        if (num_set.find(x - 1) == num_set.end()) {
            int curr = x;
            int len = 1;
            while (num_set.find(curr + 1) != num_set.end()) {
                ++curr;
                ++len;
            }
            best_len = std::max(best_len, len);
        }
    }
    return best_len;
}


// =============================================================================
// BÀI 5: KIỂM TRA HAI CHUỖI ĐẲNG CẤU (ISOMORPHIC STRINGS - SONG ÁNH 2 BẢNG BĂM)
// =============================================================================
bool is_isomorphic(const std::string &s, const std::string &t) {
    if (s.size() != t.size()) return false;
    int map_s_to_t[256] = {0};
    int map_t_to_s[256] = {0};

    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char cs = static_cast<unsigned char>(s[i]);
        unsigned char ct = static_cast<unsigned char>(t[i]);
        if (map_s_to_t[cs] != map_t_to_s[ct]) return false;
        map_s_to_t[cs] = static_cast<int>(i + 1);
        map_t_to_s[ct] = static_cast<int>(i + 1);
    }
    return true;
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP BẢNG BĂM
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.5 (HASH TABLE) ==========\n";
        std::cout << "1. Two Sum tren mang chua sap xep - O(N)\n";
        std::cout << "2. Tim ky tu khong lap lai dau tien (First Unique Char)\n";
        std::cout << "3. Dem so doan con lien tiep co tong bang K (Prefix + Hash)\n";
        std::cout << "4. Day so lien tiep dai nhat (Longest Consecutive - O(N))\n";
        std::cout << "5. Kiem tra hai chuoi dang cau (Isomorphic Strings)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::vector<int> a = {7, 11, -2, 15, 2, 8};
                int i1 = -1, i2 = -1;
                if (two_sum_hash(a, 9, i1, i2)) {
                    std::cout << "\n[Bai 1] Tim thay: a[" << i1 << "] (" << a[i1]
                              << ") + a[" << i2 << "] (" << a[i2] << ") = 9\n";
                }
                break;
            }

            case 2: {
                std::string s = "leetcode";
                int idx = first_unique_char_index(s);
                std::cout << "\n[Bai 2] Chuoi \"" << s << "\" -> Ky tu duy nhat dau tien tai idx = "
                          << idx << " ('" << (idx != -1 ? s[idx] : '?') << "')\n";
                break;
            }

            case 3: {
                std::vector<int> nums = {1, 2, 3, -1, 1, 2};
                int k = 3;
                std::cout << "\n[Bai 3] Mang {1, 2, 3, -1, 1, 2} | So doan con co tong = " << k
                          << " la: " << subarray_sum_equals_k(nums, k) << " doan\n";
                break;
            }

            case 4: {
                std::vector<int> nums = {100, 4, 200, 1, 3, 2};
                std::cout << "\n[Bai 4] Mang {100, 4, 200, 1, 3, 2} -> Do dai day lien tiep dai nhat = "
                          << longest_consecutive_sequence(nums) << " ({1, 2, 3, 4})\n";
                break;
            }

            case 5: {
                std::cout << "\n[Bai 5]\n";
                std::cout << "  (\"paper\", \"title\") -> " << (is_isomorphic("paper", "title") ? "Dang cau" : "Khong") << "\n";
                std::cout << "  (\"foo\",   \"bar\")   -> " << (is_isomorphic("foo", "bar") ? "Dang cau" : "Khong") << "\n";
                break;
            }

            case 0:
                std::cout << "-> Tam biet!\n";
                break;

            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

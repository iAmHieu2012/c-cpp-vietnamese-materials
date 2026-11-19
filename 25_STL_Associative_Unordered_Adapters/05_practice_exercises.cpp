#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iomanip>

// =============================================================================
// BÀI 1: GROUP ANAGRAMS (NHÓM CÁC TỪ ĐẢO CHỮ - UNORDERED_MAP)
// =============================================================================
std::vector<std::vector<std::string>> group_anagrams(const std::vector<std::string> &words) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    for (const auto &w : words) {
        std::string key = w;
        std::sort(key.begin(), key.end()); // Chuỗi đã sắp xếp làm chữ ký (signature)
        groups[key].push_back(w);
    }

    std::vector<std::vector<std::string>> result;
    result.reserve(groups.size());
    for (auto &[key, vec] : groups) {
        result.push_back(std::move(vec));
    }
    return result;
}


// =============================================================================
// BÀI 2: TOP K FREQUENT ELEMENTS (UNORDERED_MAP + MIN-HEAP - O(N log K))
// =============================================================================
std::vector<int> top_k_frequent(const std::vector<int> &nums, int k) {
    std::unordered_map<int, int> freq;
    for (int x : nums) ++freq[x];

    // Min-Heap lưu {tần suất, giá trị} -> phần tử ít xuất hiện nhất nằm ở đỉnh để dễ loại
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> min_pq;

    for (const auto &[val, cnt] : freq) {
        min_pq.push({cnt, val});
        if (static_cast<int>(min_pq.size()) > k) {
            min_pq.pop();
        }
    }

    std::vector<int> res;
    res.reserve(k);
    while (!min_pq.empty()) {
        res.push_back(min_pq.top().second);
        min_pq.pop();
    }
    std::reverse(res.begin(), res.end()); // Sắp lại từ phổ biến nhất -> ít hơn
    return res;
}


// =============================================================================
// BÀI 3: LRU CACHE CHUẨN CÔNG NGHIỆP (STD::LIST + UNORDERED_MAP - O(1))
// =============================================================================
class LRUCache {
private:
    int capacity;
    // list lưu {key, value}: đầu list = mới dùng gần nhất (MRU), cuối list = lâu nhất (LRU)
    std::list<std::pair<int, int>> cache;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> pos;

public:
    explicit LRUCache(int cap) : capacity(cap) {
        pos.reserve(cap);
    }

    int get(int key) {
        auto it = pos.find(key);
        if (it == pos.end()) return -1;
        // Đưa node vừa truy cập lên đầu list trong O(1) bằng splice (0 copy!)
        cache.splice(cache.begin(), cache, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = value;
            cache.splice(cache.begin(), cache, it->second);
            return;
        }
        if (static_cast<int>(cache.size()) == capacity) {
            int lru_key = cache.back().first;
            pos.erase(lru_key);
            cache.pop_back();
        }
        cache.emplace_front(key, value);
        pos[key] = cache.begin();
    }

    void print_state() const {
        std::cout << "  Cache (MRU -> LRU): ";
        for (const auto &[k, v] : cache) std::cout << "[" << k << ":" << v << "] ";
        std::cout << "\n";
    }
};


// =============================================================================
// BÀI 4: TÌM TRUNG VỊ LUỒNG DỮ LIỆU ĐỘNG (2 HEAPS - ADD O(log N), MEDIAN O(1))
// =============================================================================
class MedianFinder {
private:
    std::priority_queue<int> lower_half;                                     // Max-Heap (nửa nhỏ)
    std::priority_queue<int, std::vector<int>, std::greater<int>> upper_half; // Min-Heap (nửa lớn)

public:
    void add_num(int num) {
        lower_half.push(num);
        // Đảm bảo mọi phần tử của lower_half <= mọi phần tử của upper_half
        upper_half.push(lower_half.top());
        lower_half.pop();

        // Duy trì kích thước: lower_half.size() >= upper_half.size() (chênh tối đa 1)
        if (lower_half.size() < upper_half.size()) {
            lower_half.push(upper_half.top());
            upper_half.pop();
        }
    }

    double find_median() const {
        if (lower_half.size() > upper_half.size()) {
            return static_cast<double>(lower_half.top());
        }
        return (lower_half.top() + upper_half.top()) / 2.0;
    }
};


// =============================================================================
// BÀI 5: TRUY VẤN ĐOẠN [L, R] & TÌM PHẦN TỬ GẦN X NHẤT BẰNG STD::SET - O(log N)
// =============================================================================
int find_closest_in_set(const std::set<int> &s, int x) {
    auto it = s.lower_bound(x); // Phần tử đầu tiên >= x
    if (it == s.begin()) return *it;
    if (it == s.end())   return *std::prev(it);

    int right_val = *it;
    int left_val  = *std::prev(it);
    return (std::abs(x - left_val) <= std::abs(right_val - x)) ? left_val : right_val;
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 5.3 (ASSOCIATIVE & ADAPTERS) ==========\n";
        std::cout << "1. Group Anagrams (unordered_map)\n";
        std::cout << "2. Top K Frequent Elements (unordered_map + Min-Heap)\n";
        std::cout << "3. LRU Cache O(1) (std::list + unordered_map + splice)\n";
        std::cout << "4. Find Median from Data Stream (2 Heaps)\n";
        std::cout << "5. Tim phan tu gan X nhat & dem doan [L, R] (std::set)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::vector<std::string> words = {"eat", "tea", "tan", "ate", "nat", "bat"};
                auto groups = group_anagrams(words);
                std::cout << "\n[Bai 1] Cac nhom Anagrams:\n";
                for (const auto &g : groups) {
                    std::cout << "  -> { ";
                    for (const auto &w : g) std::cout << w << " ";
                    std::cout << "}\n";
                }
                break;
            }

            case 2: {
                std::vector<int> nums = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4};
                int k = 2;
                auto topk = top_k_frequent(nums, k);
                std::cout << "\n[Bai 2] Top " << k << " phan tu xuat hien nhieu nhat: ";
                for (int x : topk) std::cout << x << " ";
                std::cout << "\n";
                break;
            }

            case 3: {
                std::cout << "\n[Bai 3] Kiem thu LRU Cache (capacity = 2):\n";
                LRUCache lru(2);
                lru.put(1, 100);
                lru.put(2, 200);
                lru.print_state();
                std::cout << "  get(1) = " << lru.get(1) << " (1 duoc day len MRU)\n";
                lru.put(3, 300); // Đẩy key 2 (LRU) ra khỏi cache
                lru.print_state();
                std::cout << "  get(2) = " << lru.get(2) << " (da bi loai)\n";
                break;
            }

            case 4: {
                std::cout << "\n[Bai 4] Luong du lieu dong: 5, 15, 1, 3\n";
                MedianFinder mf;
                std::cout << std::fixed << std::setprecision(1);
                for (int x : {5, 15, 1, 3}) {
                    mf.add_num(x);
                    std::cout << "  + Them " << std::setw(2) << x
                              << " -> Trung vi hien tai = " << mf.find_median() << "\n";
                }
                break;
            }

            case 5: {
                std::set<int> s = {10, 22, 35, 48, 60, 75};
                int x = 40;
                auto it_l = s.lower_bound(20);
                auto it_r = s.upper_bound(60);
                std::cout << "\n[Bai 5] Set: {10, 22, 35, 48, 60, 75}\n";
                std::cout << "  -> Phan tu gan " << x << " nhat: " << find_closest_in_set(s, x) << "\n";
                std::cout << "  -> So phan tu trong doan [20, 60]: " << std::distance(it_l, it_r) << "\n";
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

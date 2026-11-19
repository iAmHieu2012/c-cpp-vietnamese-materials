#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <algorithm>

// =============================================================================
// BÀI 1: SLIDING WINDOW MAXIMUM BẰNG MONOTONIC DEQUE - O(N) TIME, O(K) SPACE
// =============================================================================
std::vector<int> sliding_window_max(const std::vector<int> &nums, int k) {
    std::vector<int> result;
    if (nums.empty() || k <= 0) return result;
    result.reserve(nums.size() - k + 1);

    // Deque lưu CHỈ SỐ (index) của các phần tử sao cho giá trị tương ứng GIẢM DẦN
    std::deque<int> dq;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // 1. Loại bỏ chỉ số đã trượt ra khỏi cửa sổ [i - k + 1 .. i]
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // 2. Loại bỏ các phần tử ở cuối nhỏ hơn hoặc bằng nums[i] (không bao giờ là max nữa)
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        // 3. Khi cửa sổ đã đủ k phần tử, phần tử đầu deque chính là max của cửa sổ hiện tại
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}


// =============================================================================
// BÀI 2: LOẠI BỎ TRÙNG LẶP & LỌC SỐ ÂM BẰNG ERASE-REMOVE IDIOM - O(N log N)
// =============================================================================
void deduplicate_and_filter(std::vector<int> &v) {
    // 1. Xóa toàn bộ số âm trong O(N)
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x < 0; }), v.end());
    // 2. Sắp xếp tăng dần để gom các số trùng lặp nằm cạnh nhau
    std::sort(v.begin(), v.end());
    // 3. std::unique dồn các phần tử duy nhất lên đầu, erase cắt bỏ phần đuôi thừa
    v.erase(std::unique(v.begin(), v.end()), v.end());
}


// =============================================================================
// BÀI 3: TRÌNH SOẠN THẢO VĂN BẢN (TEXT EDITOR CURSOR) BẰNG STD::LIST - O(1)
// =============================================================================
class TextEditor {
private:
    std::list<char> text;
    std::list<char>::iterator cursor;

public:
    TextEditor() : cursor(text.begin()) {}

    void type(const std::string &s) {
        for (char c : s) {
            text.insert(cursor, c); // Chèn ngay trước cursor, cursor giữ nguyên vị trí
        }
    }

    void backspace(int k) {
        while (k-- > 0 && cursor != text.begin()) {
            auto to_erase = std::prev(cursor);
            text.erase(to_erase); // Với std::list, xóa node khác không làm hỏng cursor!
        }
    }

    void move_left(int k) {
        while (k-- > 0 && cursor != text.begin()) --cursor;
    }

    void move_right(int k) {
        while (k-- > 0 && cursor != text.end()) ++cursor;
    }

    void print() const {
        std::cout << "  Buffer: \"";
        for (auto it = text.begin(); it != text.end(); ++it) {
            if (it == cursor) std::cout << '|';
            std::cout << *it;
        }
        if (cursor == text.end()) std::cout << '|';
        std::cout << "\"\n";
    }
};


// =============================================================================
// BÀI 4: LỊCH SỬ TRÌNH DUYỆT (BROWSER HISTORY) BẰNG STD::DEQUE - O(1)
// =============================================================================
class BrowserHistory {
private:
    std::deque<std::string> history;
    int curr_idx;
    int max_cap;

public:
    BrowserHistory(const std::string &homepage, int cap = 5) : curr_idx(0), max_cap(cap) {
        history.push_back(homepage);
    }

    void visit(const std::string &url) {
        // Cắt bỏ toàn bộ lịch sử forward phía sau trang hiện tại
        while (static_cast<int>(history.size()) - 1 > curr_idx) {
            history.pop_back();
        }
        history.push_back(url);
        ++curr_idx;

        // Nếu vượt quá giới hạn dung lượng -> Xóa trang cũ nhất ở đầu trong O(1)
        if (static_cast<int>(history.size()) > max_cap) {
            history.pop_front();
            --curr_idx;
        }
    }

    std::string back(int steps) {
        curr_idx = std::max(0, curr_idx - steps);
        return history[curr_idx];
    }

    std::string forward(int steps) {
        curr_idx = std::min(static_cast<int>(history.size()) - 1, curr_idx + steps);
        return history[curr_idx];
    }

    std::string current() const { return history[curr_idx]; }
};


// =============================================================================
// BÀI 5: GỘP NHIỀU STD::LIST ĐÃ SẮP XẾP KHÔNG CẤP PHÁT MỚI (LIST::MERGE)
// =============================================================================
std::list<int> merge_k_sorted_lists(std::vector<std::list<int>> &lists) {
    std::list<int> merged;
    for (auto &lst : lists) {
        // list::merge() chuyển trực tiếp các node của lst vào merged trong O(N + M), 0 allocation!
        merged.merge(lst);
    }
    return merged;
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 5.2 (STL SEQUENCE CONTAINERS) ==========\n";
        std::cout << "1. Sliding Window Maximum bang Monotonic Deque - O(N)\n";
        std::cout << "2. Loc so am & Xoa trung lap bang Erase-Remove Idiom\n";
        std::cout << "3. Trinh soan thao van ban (TextEditor Cursor) bang std::list\n";
        std::cout << "4. Lich su trinh duyet (BrowserHistory) bang std::deque\n";
        std::cout << "5. Gop nhieu std::list da sap xep bang list::merge() (0 alloc)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "================================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
                int k = 3;
                auto res = sliding_window_max(nums, k);
                std::cout << "\n[Bai 1] Mang: {1, 3, -1, -3, 5, 3, 6, 7} | k = " << k << "\n  -> Max tung cua so: ";
                for (int x : res) std::cout << x << " ";
                std::cout << "\n";
                break;
            }

            case 2: {
                std::vector<int> v = {5, -2, 3, 5, -9, 1, 3, 8, 1};
                deduplicate_and_filter(v);
                std::cout << "\n[Bai 2] Sau khi loc so am & xoa trung lap: ";
                for (int x : v) std::cout << x << " ";
                std::cout << "\n";
                break;
            }

            case 3: {
                std::cout << "\n[Bai 3] Mo phong TextEditor (dau '|' la vi tri con tro):\n";
                TextEditor ed;
                ed.type("Hello World");
                ed.print();
                ed.move_left(5);
                ed.type("C++ ");
                ed.print();
                ed.backspace(4);
                ed.print();
                break;
            }

            case 4: {
                std::cout << "\n[Bai 4] Mo phong BrowserHistory (max_cap = 3):\n";
                BrowserHistory bh("google.com", 3);
                bh.visit("github.com");
                bh.visit("cppreference.com");
                bh.visit("leetcode.com"); // Đẩy google.com ra ngoài (cap = 3)
                std::cout << "  Trang hien tai : " << bh.current() << "\n";
                std::cout << "  Back 1 buoc    : " << bh.back(1) << "\n";
                std::cout << "  Back 5 buoc    : " << bh.back(5) << " (cham day: github.com)\n";
                std::cout << "  Forward 1 buoc : " << bh.forward(1) << "\n";
                break;
            }

            case 5: {
                std::vector<std::list<int>> lists = {
                    {1, 4, 7},
                    {2, 5, 8},
                    {0, 3, 6, 9}
                };
                auto merged = merge_k_sorted_lists(lists);
                std::cout << "\n[Bai 5] Ket qua gop 3 std::list: ";
                for (int x : merged) std::cout << x << " ";
                std::cout << "\n";
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

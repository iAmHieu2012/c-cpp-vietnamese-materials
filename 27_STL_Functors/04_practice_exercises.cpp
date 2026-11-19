#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <functional>
#include <iomanip>

// =============================================================================
// BÀI 1: HỆ THỐNG LỌC & CHIẾT KHẤU SẢN PHẨM BẰNG STATEFUL FUNCTOR
// =============================================================================
struct Item {
    std::string name;
    double price;
};

class PriceBracketFilter {
private:
    double min_p, max_p;
public:
    PriceBracketFilter(double lo, double hi) : min_p(lo), max_p(hi) {}
    bool operator()(const Item &it) const noexcept {
        return it.price >= min_p && it.price <= max_p;
    }
};

class DiscountTransformer {
private:
    double discount_rate; // VD: 0.15 = giảm 15%
public:
    explicit DiscountTransformer(double rate) : discount_rate(rate) {}
    Item operator()(Item it) const noexcept {
        it.price *= (1.0 - discount_rate);
        return it;
    }
};

void exercise_1_stateful_functors() {
    std::vector<Item> items = {
        {"Laptop", 2200.0}, {"Mouse", 45.0}, {"Monitor", 450.0},
        {"Keyboard", 120.0}, {"Headset", 95.0}
    };

    // 1. Lọc các sản phẩm tầm trung [80$, 500$] bằng Stateful Functor
    std::vector<Item> mid_range;
    std::copy_if(items.begin(), items.end(), std::back_inserter(mid_range),
                 PriceBracketFilter(80.0, 500.0));

    // 2. Áp dụng giảm giá 20% cho toàn bộ sản phẩm đã lọc bằng DiscountTransformer
    std::transform(mid_range.begin(), mid_range.end(), mid_range.begin(),
                   DiscountTransformer(0.20));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 1] San pham tam trung [80$, 500$] sau khi giam gia 20%:\n";
    for (const auto &it : mid_range) {
        std::cout << "  - " << std::left << std::setw(12) << it.name << ": $" << it.price << "\n";
    }
}


// =============================================================================
// BÀI 2: XẾP HẠNG HỒ SƠ ỨNG VIÊN ĐA TIÊU CHÍ (CUSTOM FUNCTOR CHO STD::SORT)
// =============================================================================
struct Applicant {
    std::string name;
    int exp_years;
    int test_score;
    int expected_salary;
};

struct ApplicantSortFunctor {
    bool operator()(const Applicant &a, const Applicant &b) const {
        if (a.exp_years != b.exp_years)       return a.exp_years > b.exp_years;       // Kinh nghiệm giảm dần
        if (a.test_score != b.test_score)     return a.test_score > b.test_score;     // Điểm test giảm dần
        if (a.expected_salary != b.expected_salary) return a.expected_salary < b.expected_salary; // Lương tăng dần
        return a.name < b.name;
    }
};

void exercise_2_applicant_ranking() {
    std::vector<Applicant> list = {
        {"Nguyen Van An",  3, 90, 1500},
        {"Tran Thi Binh",  5, 85, 2200},
        {"Le Hoang Chau",  3, 90, 1300}, // Cùng 3 năm, 90 điểm nhưng lương kỳ vọng thấp hơn An
        {"Pham Minh Dung", 5, 95, 2500}
    };

    std::sort(list.begin(), list.end(), ApplicantSortFunctor{});

    std::cout << "\n[Bai 2] Bang xep hang Ung vien (Exp giam -> Score giam -> Salary tang):\n";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << "  #" << (i + 1) << " " << std::left << std::setw(16) << list[i].name
                  << " | Exp: " << list[i].exp_years << "y | Score: " << list[i].test_score
                  << " | Salary: $" << list[i].expected_salary << "\n";
    }
}


// =============================================================================
// BÀI 3: HÀNG ĐỢI PHÂN LOẠI CẤP CỨU BỆNH VIỆN (PRIORITY_QUEUE + FUNCTOR)
// =============================================================================
struct Patient {
    std::string name;
    int severity;     // Mức độ nguy kịch: 10 (Nguy kịch nhất) -> 1 (Nhẹ)
    int arrival_order;// Thứ tự đến khám: Số nhỏ hơn là đến trước (FIFO khi cùng mức độ)
};

struct EmergencyTriageFunctor {
    bool operator()(const Patient &a, const Patient &b) const {
        // Trong priority_queue: trả về true nếu `a` ưu tiên THẤP HƠN `b`
        if (a.severity != b.severity) return a.severity < b.severity; // Bệnh nặng hơn lên đỉnh
        return a.arrival_order > b.arrival_order;                     // Cùng mức độ -> Đến sớm hơn lên đỉnh
    }
};

void exercise_3_emergency_triage() {
    std::priority_queue<Patient, std::vector<Patient>, EmergencyTriageFunctor> er_queue;
    er_queue.push({"Benh nhan A (Cam cum)",     2, 1});
    er_queue.push({"Benh nhan B (Nhoi mau co)", 9, 2});
    er_queue.push({"Benh nhan C (Gay xuong)",   6, 3});
    er_queue.push({"Benh nhan D (Dot quy)",     9, 4}); // Cùng mức 9 với B nhưng đến sau B (4 > 2)

    std::cout << "\n[Bai 3] Thu tu bac si goi cap cuu (Severity cao -> Den truoc):\n";
    while (!er_queue.empty()) {
        const auto &p = er_queue.top();
        std::cout << "  -> [Muc " << p.severity << " | STT #" << p.arrival_order << "] " << p.name << "\n";
        er_queue.pop();
    }
}


// =============================================================================
// BÀI 4: SỔ LỆNH CHỨNG KHOÁN (ORDER BOOK: BID GIẢM DẦN, ASK TĂNG DẦN)
// =============================================================================
void exercise_4_stock_order_book() {
    // Bên Mua (Bids): Giá mua CAO NHẤT phải nằm ở đầu -> dùng std::greater<double>
    std::map<double, int, std::greater<double>> bids = {
        {25.10, 500}, {25.30, 200}, {25.20, 800}
    };

    // Bên Bán (Asks): Giá bán THẤP NHẤT phải nằm ở đầu -> dùng std::less<double>
    std::map<double, int, std::less<double>> asks = {
        {25.60, 400}, {25.40, 300}, {25.50, 600}
    };

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 4] So lenh Chung khoan (Order Book):\n";
    std::cout << "  Best Bid (Gia MUA cao nhat dau bang - std::greater): $"
              << bids.begin()->first << " (KL: " << bids.begin()->second << ")\n";
    std::cout << "  Best Ask (Gia BAN thap nhat dau bang - std::less)  : $"
              << asks.begin()->first << " (KL: " << asks.begin()->second << ")\n";
    std::cout << "  -> Spread (Do lech Ask - Bid) = $"
              << (asks.begin()->first - bids.begin()->first) << "\n";
}


// =============================================================================
// BÀI 5: CUSTOM HASH FUNCTOR & EQUALITY FUNCTOR CHO UNORDERED_MAP
// =============================================================================
struct DeptEmployeeKey {
    std::string dept_code;
    int emp_id;
};

struct KeyHashFunctor {
    std::size_t operator()(const DeptEmployeeKey &k) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(k.dept_code);
        std::size_t h2 = std::hash<int>{}(k.emp_id);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

struct KeyEqualFunctor {
    bool operator()(const DeptEmployeeKey &a, const DeptEmployeeKey &b) const noexcept {
        return a.emp_id == b.emp_id && a.dept_code == b.dept_code;
    }
};

void exercise_5_custom_hash_and_equal_functors() {
    std::unordered_map<DeptEmployeeKey, std::string, KeyHashFunctor, KeyEqualFunctor> directory;
    directory[{"ENG", 101}] = "Nguyen Van An (Senior Engineer)";
    directory[{"HR",  205}] = "Tran Thi Binh (HR Manager)";

    std::cout << "\n[Bai 5] Tra cuu unordered_map voi Custom Hash & Equal Functors:\n";
    DeptEmployeeKey query{"ENG", 101};
    if (auto it = directory.find(query); it != directory.end()) {
        std::cout << "  Tim thay [" << query.dept_code << "-" << query.emp_id << "]: " << it->second << "\n";
    }
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 5.5 (FUNCTORS & COMPARATORS) ==========\n";
        std::cout << "1. Bo loc & Chiet khau san pham bang Stateful Functor\n";
        std::cout << "2. Xep hang Ho so ung vien da tieu chi (std::sort + Functor)\n";
        std::cout << "3. Hang doi dieu phoi cap cuu Benh vien (priority_queue + Functor)\n";
        std::cout << "4. So lenh chung khoan Bid/Ask (std::map + greater/less Functor)\n";
        std::cout << "5. Custom Hash & Equality Functor cho unordered_map\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_stateful_functors(); break;
            case 2: exercise_2_applicant_ranking(); break;
            case 3: exercise_3_emergency_triage(); break;
            case 4: exercise_4_stock_order_book(); break;
            case 5: exercise_5_custom_hash_and_equal_functors(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

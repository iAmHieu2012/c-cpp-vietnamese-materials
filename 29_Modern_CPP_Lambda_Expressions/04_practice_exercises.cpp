#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>

// =============================================================================
// BÀI 1: HỆ THỐNG ĐĂNG KÝ SỰ KIỆN (EVENT BUS / OBSERVER CALLBACKS) BẰNG LAMBDA
// =============================================================================
class Button {
private:
    std::vector<std::function<void(int)>> click_listeners;

public:
    void on_click(std::function<void(int)> callback) {
        click_listeners.push_back(std::move(callback));
    }

    void simulate_click(int click_count) const {
        for (const auto &listener : click_listeners) {
            listener(click_count);
        }
    }
};

void exercise_1_event_callbacks() {
    Button submit_btn;
    int total_saved = 0;

    // Đăng ký 2 Lambda listener có capture trạng thái khác nhau
    submit_btn.on_click([](int cnt) {
        std::cout << "  [UI Logger] Nut Submit duoc bam lan thu #" << cnt << "\n";
    });
    submit_btn.on_click([&total_saved](int cnt) {
        total_saved += cnt * 10;
        std::cout << "  [Database] Da luu du lieu! Tong ban ghi tich luy = " << total_saved << "\n";
    });

    std::cout << "\n[Bai 1] Mo phong kich hoat su kien onClick qua Lambda Callbacks:\n";
    submit_btn.simulate_click(1);
    submit_btn.simulate_click(2);
}


// =============================================================================
// BÀI 2: BỘ SINH DÃY SỐ (STATEFUL GENERATOR) BẰNG MUTABLE LAMBDA + STD::GENERATE
// =============================================================================
void exercise_2_mutable_fibonacci_generator() {
    std::vector<long long> fib(10);
    // Dùng Init-Capture khởi tạo 2 biến trạng thái a = 0, b = 1 ngay bên trong Closure
    std::generate(fib.begin(), fib.end(), [a = 0LL, b = 1LL]() mutable {
        long long current = a;
        a = b;
        b = current + b;
        return current;
    });

    std::cout << "\n[Bai 2] 10 so Fibonacci dau tien sinh boi mutable Lambda:\n  ";
    for (long long x : fib) std::cout << x << " ";
    std::cout << "\n";
}


// =============================================================================
// BÀI 3: PIPELINE XỬ LÝ DỮ LIỆU NHÂN SỰ (FILTER -> TRANSFORM -> ACCUMULATE)
// =============================================================================
struct Staff {
    std::string name;
    std::string dept;
    double salary;
};

void exercise_3_data_pipeline_lambdas() {
    std::vector<Staff> company = {
        {"An",   "AI",    3000.0},
        {"Binh", "Sales", 1800.0},
        {"Chau", "AI",    3500.0},
        {"Dung", "AI",    2500.0},
        {"Em",   "HR",    1500.0}
    };

    std::string target_dept = "AI";
    double bonus_rate = 0.15;

    // Tính tổng quỹ lương (đã cộng 15% thưởng) của riêng phòng "AI" chỉ trong 1 lượt std::accumulate!
    double ai_budget = std::accumulate(company.begin(), company.end(), 0.0,
        [&target_dept, bonus_rate](double acc, const Staff &s) {
            if (s.dept == target_dept) {
                return acc + s.salary * (1.0 + bonus_rate);
            }
            return acc;
        }
    );

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 3] Tong quy luong phong '" << target_dept
              << "' (sau khi +15% thuong) = $" << ai_budget << "\n";
}


// =============================================================================
// BÀI 4: ĐỆ QUY LAMBDA KHÔNG DÙNG STD::FUNCTION (KỸ THUẬT AUTO&& SELF - C++14/17)
// =============================================================================
void exercise_4_recursive_lambda_self() {
    // Truyền chính Lambda vào tham số đầu tiên `auto &&self` để đệ quy Zero-overhead (không tốn std::function)
    auto gcd_lambda = [](auto &&self, long long a, long long b) -> long long {
        return (b == 0) ? a : self(self, b, a % b);
    };

    std::cout << "\n[Bai 4] De quy Lambda Zero-overhead (auto&& self):\n";
    std::cout << "  GCD(252, 105) = " << gcd_lambda(gcd_lambda, 252, 105) << "\n";
}


// =============================================================================
// BÀI 5: TẠO BỘ KIỂM TRA HỢP LỆ TỔ HỢP (COMPOSITE VALIDATOR FACTORY)
// =============================================================================
void exercise_5_validator_composition() {
    auto min_len = [](std::size_t n) {
        return [n](const std::string &s) { return s.size() >= n; };
    };
    auto must_contain = [](char ch) {
        return [ch](const std::string &s) { return s.find(ch) != std::string::npos; };
    };
    // Kết hợp 2 Lambda kiểm tra thành 1 Lambda duy nhất
    auto combine_and = [](auto pred1, auto pred2) {
        return [pred1, pred2](const std::string &s) { return pred1(s) && pred2(s); };
    };

    auto valid_email = combine_and(min_len(6), must_contain('@'));

    std::cout << "\n[Bai 5] Kiem tra Email bang Composite Lambda Validators:\n";
    for (const char *mail : {"a@b", "admin@company.com", "invalid_email.com"}) {
        std::cout << "  \"" << mail << "\" -> "
                  << (valid_email(mail) ? "HOP LE" : "KHONG HOP LE") << "\n";
    }
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 6.2 (LAMBDA EXPRESSIONS) ==========\n";
        std::cout << "1. He thong Event Callbacks (std::function + Lambda)\n";
        std::cout << "2. Bo sinh day Fibonacci bang mutable Lambda + Init-Capture\n";
        std::cout << "3. Pipeline tinh quy luong phong ban (std::accumulate + Lambda)\n";
        std::cout << "4. De quy Lambda Zero-overhead (auto&& self)\n";
        std::cout << "5. Ket hop nhieu bo kiem tra (Higher-Order Validator Factory)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_event_callbacks(); break;
            case 2: exercise_2_mutable_fibonacci_generator(); break;
            case 3: exercise_3_data_pipeline_lambdas(); break;
            case 4: exercise_4_recursive_lambda_self(); break;
            case 5: exercise_5_validator_composition(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

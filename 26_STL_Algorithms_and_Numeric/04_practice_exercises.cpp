#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <limits>

// =============================================================================
// BÀI 1: BẢNG TỔNG SẮP HUY CHƯƠNG OLYMPIC ĐA TIÊU CHÍ (STD::SORT + LAMBDA)
// =============================================================================
struct CountryMedal {
    std::string country;
    int gold, silver, bronze;
};

void exercise_1_medal_leaderboard() {
    std::vector<CountryMedal> table = {
        {"VietNam",   15, 12, 20},
        {"Thailand",  15, 18, 10},
        {"Indonesia", 15, 12, 25},
        {"Japan",     28, 21, 19},
        {"Singapore", 10, 15, 12}
    };

    // Sắp xếp giảm dần theo Vàng -> Bạc -> Đồng; nếu bằng cả 3 thì xếp tên A-Z
    std::sort(table.begin(), table.end(), [](const CountryMedal &a, const CountryMedal &b) {
        if (a.gold != b.gold)     return a.gold > b.gold;
        if (a.silver != b.silver) return a.silver > b.silver;
        if (a.bronze != b.bronze) return a.bronze > b.bronze;
        return a.country < b.country;
    });

    std::cout << "\n[Bai 1] Bang Tong Sap Huy Chuong (Da tieu chi):\n";
    std::cout << "  " << std::left << std::setw(6) << "Hang" << std::setw(14) << "Quoc Gia"
              << "Vang  Bac   Dong\n";
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << "  #" << std::left << std::setw(5) << (i + 1)
                  << std::setw(14) << table[i].country
                  << std::setw(6) << table[i].gold
                  << std::setw(6) << table[i].silver
                  << table[i].bronze << "\n";
    }
}


// =============================================================================
// BÀI 2: CHUẨN HÓA ĐIỂM SỐ BẰNG TRANSFORM + CLAMP & TRUY VẤN TỔNG ĐOẠN BẰNG PARTIAL_SUM
// =============================================================================
void exercise_2_normalize_and_prefix_query() {
    std::vector<double> raw_scores = {8.5, 11.2, -1.5, 7.0, 9.8, 10.5, 6.4};
    std::vector<double> clean_scores(raw_scores.size());

    // 1. Kẹp toàn bộ điểm lỗi về đoạn hợp lệ [0.0, 10.0] và cộng 0.5 điểm thưởng (tối đa 10.0)
    std::transform(raw_scores.begin(), raw_scores.end(), clean_scores.begin(), [](double s) {
        return std::clamp(s, 0.0, 10.0);
    });

    // 2. Tạo mảng cộng dồn Prefix Sum bằng std::partial_sum
    std::vector<double> pref(clean_scores.size());
    std::partial_sum(clean_scores.begin(), clean_scores.end(), pref.begin());

    auto range_sum = [&](int L, int R) {
        return pref[R] - (L > 0 ? pref[L - 1] : 0.0);
    };

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 2] Diem sau khi chuan hoa [0.0, 10.0] bang std::transform + std::clamp:\n  ";
    for (double s : clean_scores) std::cout << s << "  ";
    std::cout << "\n  -> Tong diem doan [1..4] (O(1) query) = " << range_sum(1, 4) << "\n";
}


// =============================================================================
// BÀI 3: GIẢI BÀI TOÁN NGƯỜI DU LỊCH (TSP) BẰNG STD::NEXT_PERMUTATION
// Tìm hành trình đi qua tất cả N thành phố (0 -> ... -> 0) có tổng chi phí nhỏ nhất
// =============================================================================
void exercise_3_tsp_next_permutation() {
    const int N = 4;
    const int cost[N][N] = {
        {0,  10, 15, 20},
        {10, 0,  35, 25},
        {15, 35, 0,  30},
        {20, 25, 30, 0}
    };

    // Cố định đỉnh xuất phát là thành phố 0, chỉ hoán vị các thành phố {1, 2, 3}
    std::vector<int> cities = {1, 2, 3};
    int best_cost = std::numeric_limits<int>::max();
    std::vector<int> best_tour;

    do {
        int curr_cost = 0;
        int prev = 0;
        for (int c : cities) {
            curr_cost += cost[prev][c];
            prev = c;
        }
        curr_cost += cost[prev][0]; // Quay về thành phố 0

        if (curr_cost < best_cost) {
            best_cost = curr_cost;
            best_tour = cities;
        }
    } while (std::next_permutation(cities.begin(), cities.end()));

    std::cout << "\n[Bai 3] Lo trinh TSP toi uu qua 4 thanh pho (next_permutation):\n  0 -> ";
    for (int c : best_tour) std::cout << c << " -> ";
    std::cout << "0 | Tong chi phi nho nhat = " << best_cost << "\n";
}


// =============================================================================
// BÀI 4: PHÂN TÍCH TẬP KHÁCH HÀNG 2 CHI NHÁNH TRONG O(N + M)
// (SORT + UNIQUE + SET_INTERSECTION + SET_DIFFERENCE + SET_UNION)
// =============================================================================
void exercise_4_customer_set_analytics() {
    std::vector<int> branch_a = {105, 101, 103, 101, 108, 102};
    std::vector<int> branch_b = {102, 109, 105, 104, 102, 110};

    // Bước chuẩn bị bắt buộc: Sort + Khử trùng lặp bằng Erase-Unique Idiom
    auto normalize = [](std::vector<int> &v) {
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
    };
    normalize(branch_a);
    normalize(branch_b);

    std::vector<int> common, only_a, all_customers;
    std::set_intersection(branch_a.begin(), branch_a.end(),
                          branch_b.begin(), branch_b.end(), std::back_inserter(common));
    std::set_difference(branch_a.begin(), branch_a.end(),
                        branch_b.begin(), branch_b.end(), std::back_inserter(only_a));
    std::set_union(branch_a.begin(), branch_a.end(),
                   branch_b.begin(), branch_b.end(), std::back_inserter(all_customers));

    auto print_vec = [](const char *label, const std::vector<int> &v) {
        std::cout << "  " << label << ": ";
        for (int x : v) std::cout << x << " ";
        std::cout << "\n";
    };

    std::cout << "\n[Bai 4] Phan tich tap khach hang 2 chi nhanh A va B:\n";
    print_vec("Chi nhanh A (da khu trung) ", branch_a);
    print_vec("Chi nhanh B (da khu trung) ", branch_b);
    print_vec("Khach mua o CA 2 chi nhanh ", common);
    print_vec("Khach CHI mua o chi nhanh A", only_a);
    print_vec("Tong hop toan bo khach hang", all_customers);
}


// =============================================================================
// BÀI 5: TÌM TRUNG VỊ (MEDIAN) TRONG O(N) BẰNG STD::NTH_ELEMENT & TRIMMED MEAN
// =============================================================================
void exercise_5_median_and_trimmed_mean() {
    std::vector<int> latencies = {25, 18, 22, 950, 20, 24, 1, 21, 19}; // 1 và 950 là nhiễu (Outliers)
    int n = static_cast<int>(latencies.size());

    // Tìm trung vị (Median) tại vị trí n / 2 chỉ trong O(N) mà không cần sort toàn bộ mảng!
    int mid = n / 2;
    std::nth_element(latencies.begin(), latencies.begin() + mid, latencies.end());
    int median = latencies[mid];

    // Tính trung bình sau khi loại bỏ giá trị nhỏ nhất và lớn nhất (Trimmed Mean)
    auto [min_it, max_it] = std::minmax_element(latencies.begin(), latencies.end());
    long long total = std::accumulate(latencies.begin(), latencies.end(), 0LL);
    double trimmed_mean = static_cast<double>(total - *min_it - *max_it) / (n - 2);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 5] Phan tich do tre mang (chong nhieu Outliers):\n";
    std::cout << "  -> Trung vi (Median - O(N) nth_element) = " << median << " ms\n";
    std::cout << "  -> Trung binh da cat nhieu (Trimmed Mean) = " << trimmed_mean << " ms\n";
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 5.4 (STL ALGORITHMS & NUMERIC) ==========\n";
        std::cout << "1. Bang tong sap Huy chuong da tieu chi (std::sort + Lambda)\n";
        std::cout << "2. Chuan hoa diem (transform + clamp) & Prefix Sum (partial_sum)\n";
        std::cout << "3. Bai toan Nguoi du lich TSP (std::next_permutation)\n";
        std::cout << "4. Phan tich tap khach hang 2 chi nhanh O(N+M) (unique + set_ops)\n";
        std::cout << "5. Tim Trung vi O(N) (nth_element) & Trimmed Mean (accumulate)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=================================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_medal_leaderboard(); break;
            case 2: exercise_2_normalize_and_prefix_query(); break;
            case 3: exercise_3_tsp_next_permutation(); break;
            case 4: exercise_4_customer_set_analytics(); break;
            case 5: exercise_5_median_and_trimmed_mean(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

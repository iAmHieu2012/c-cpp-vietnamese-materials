#include <iostream>
#include <ranges>
#include <vector>
#include <array>
#include <span>
#include <string>
#include <algorithm>

struct Developer {
    std::string name;
    int salary;
};

// =============================================================================
// STD::SPAN<T>: NHẬN MỌI LOẠI MẢNG LIÊN TIẾP (C-ARRAY, STD::ARRAY, STD::VECTOR)
// Không tốn chi phí copy, không bị Array Decay mất kích thước!
// =============================================================================
void double_elements_in_place(std::span<int> slice) {
    for (int &x : slice) {
        x *= 2;
    }
}

int main() {
    std::cout << "=== 1. C++20 RANGES PIPELINE (LAZY EVALUATION - 0 MANG TAM!) ===\n";
    // Bài toán: Từ dãy số ảo 1..1000, lọc các số chia hết cho 3, bình phương lên, và chỉ lấy 5 số đầu tiên!
    auto pipeline = std::views::iota(1, 1000)
                  | std::views::filter([](int x) { return x % 3 == 0; })
                  | std::views::transform([](int x) { return x * x; })
                  | std::views::take(5);

    std::cout << "  5 ket qua dau tien tu Pipeline: ";
    for (int val : pipeline) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    std::cout << "=== 2. STD::RANGES::SORT VOI PROJECTION (KHONG CAN VIET LAMBDA!) ===\n";
    std::vector<Developer> team = {
        {"An", 2500}, {"Binh", 4200}, {"Chau", 3100}
    };

    // Tham số thứ 2 là bộ so sánh (std::ranges::greater{}), tham số thứ 3 là Phép chiếu (&Developer::salary)
    std::ranges::sort(team, std::ranges::greater{}, &Developer::salary);

    for (const auto &dev : team) {
        std::cout << "  - " << dev.name << ": $" << dev.salary << "\n";
    }

    std::cout << "\n=== 3. STD::SPAN<T> (CUA SO MANG LIEN TIEP ZERO-COPY) ===\n";
    int c_arr[] = {1, 2, 3, 4, 5, 6};
    std::vector<int> vec = {10, 20, 30, 40};

    // Chỉ nhân đôi 3 phần tử ở giữa của c_arr (từ chỉ số 1, độ dài 3) bằng subspan
    std::span<int> full_span(c_arr);
    double_elements_in_place(full_span.subspan(1, 3));
    double_elements_in_place(vec);

    std::cout << "  c_arr sau khi nhan doi doan giua [1..3]: ";
    for (int x : c_arr) std::cout << x << " ";
    std::cout << "\n  vec sau khi nhan doi toan bo qua span  : ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

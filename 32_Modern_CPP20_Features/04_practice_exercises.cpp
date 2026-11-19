#include <iostream>
#include <concepts>
#include <compare>
#include <ranges>
#include <span>
#include <coroutine>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>

// =============================================================================
// BÀI 1: RÀNG BUỘC CONTAINER & SỐ HỌC BẰNG C++20 CONCEPTS
// =============================================================================
template <typename C>
concept IterableContainer = requires(C c) {
    std::ranges::begin(c);
    std::ranges::end(c);
    { c.size() } -> std::convertible_to<std::size_t>;
};

template <IterableContainer C>
void print_container_stats(const char *label, const C &container) {
    std::cout << "  " << label << " (size=" << container.size() << "): ";
    for (const auto &item : container) std::cout << item << " ";
    std::cout << "\n";
}

void exercise_1_custom_concepts() {
    std::cout << "\n[Bai 1] Kiem thu IterableContainer Concept:\n";
    std::vector<int> nums = {10, 20, 30, 40};
    std::string word = "Concepts";
    print_container_stats("std::vector<int>", nums);
    print_container_stats("std::string     ", word);
}


// =============================================================================
// BÀI 2: SẮP XẾP & TÌM KIẾM VỚI C++20 SPACESHIP (<=>) & RANGES PROJECTIONS
// =============================================================================
struct Student {
    int id;
    std::string name;
    double gpa;

    auto operator<=>(const Student&) const = default;
};

void exercise_2_projections_and_spaceship() {
    std::vector<Student> cls = {
        {103, "Chau", 8.2},
        {101, "An",   9.5},
        {104, "Dung", 7.8},
        {102, "Binh", 9.1}
    };

    // 1. Sắp xếp giảm dần theo GPA bằng Ranges Projection (&Student::gpa) không cần viết Lambda!
    std::ranges::sort(cls, std::ranges::greater{}, &Student::gpa);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n[Bai 2] Sap xep giam dan theo GPA bang C++20 Ranges Projection:\n";
    for (const auto &s : cls) {
        std::cout << "  - #" << s.id << " " << std::left << std::setw(8) << s.name
                  << " | GPA: " << s.gpa << "\n";
    }

    // 2. Tìm sinh viên có GPA cao nhất bằng std::ranges::max_element + Projection
    auto top_it = std::ranges::max_element(cls, {}, &Student::gpa);
    std::cout << "  -> Thu khoa (max_element projection): " << top_it->name << "\n";
}


// =============================================================================
// BÀI 3: ĐƯỜNG ỐNG PHÂN TÍCH DỮ LIỆU LƯỜI (RANGES VIEWS PIPELINE)
// =============================================================================
void exercise_3_ranges_pipeline_analytics() {
    std::vector<int> transactions = {-50, 120, -10, 300, 80, 450, -90, 200, 600};

    // Lọc giao dịch dương (>= 100), cộng 10% thuế, đảo ngược thứ tự và lấy 3 giao dịch cuối
    auto view = transactions
              | std::views::filter([](int t) { return t >= 100; })
              | std::views::transform([](int t) { return static_cast<int>(t * 1.1); })
              | std::views::reverse
              | std::views::take(3);

    std::cout << "\n[Bai 3] Ket qua duong ong Views Pipeline (0 mang tam tren Heap):\n  ";
    for (int val : view) std::cout << "$" << val << "  ";
    std::cout << "\n";
}


// =============================================================================
// BÀI 4: XỬ LÝ CỬA SỔ TRƯỢT ZERO-COPY TRÊN MỌI MẢNG BẰNG STD::SPAN<T>
// =============================================================================
double compute_span_average(std::span<const int> window) {
    if (window.empty()) return 0.0;
    long long sum = std::accumulate(window.begin(), window.end(), 0LL);
    return static_cast<double>(sum) / window.size();
}

void exercise_4_span_sliding_average() {
    int raw_sensor[] = {10, 20, 30, 40, 50, 60};
    std::span<const int> full(raw_sensor);
    std::size_t k = 3;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n[Bai 4] Trung binh truot (Moving Average, k=3) Zero-Copy bang std::span:\n  ";
    for (std::size_t i = 0; i + k <= full.size(); ++i) {
        std::cout << compute_span_average(full.subspan(i, k)) << "  ";
    }
    std::cout << "\n";
}


// =============================================================================
// BÀI 5: BỘ SINH SỐ NGUYÊN TỐ LƯỜI (LAZY PRIME GENERATOR) BẰNG COROUTINE
// =============================================================================
template <typename T>
class SimpleGenerator {
public:
    struct promise_type {
        T val{};
        SimpleGenerator get_return_object() {
            return SimpleGenerator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) noexcept { val = v; return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit SimpleGenerator(Handle h) : h_(h) {}
    ~SimpleGenerator() { if (h_) h_.destroy(); }
    SimpleGenerator(const SimpleGenerator&) = delete;
    SimpleGenerator& operator=(const SimpleGenerator&) = delete;

    bool next() {
        if (!h_ || h_.done()) return false;
        h_.resume();
        return !h_.done();
    }
    T value() const noexcept { return h_.promise().val; }

private:
    Handle h_;
};

SimpleGenerator<int> prime_stream(int max_count) {
    int found = 0;
    for (int n = 2; found < max_count; ++n) {
        bool is_p = true;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) { is_p = false; break; }
        }
        if (is_p) {
            ++found;
            co_yield n; // Tạm dừng Coroutine và trả số nguyên tố n ra ngoài
        }
    }
}

void exercise_5_coroutine_prime_generator() {
    std::cout << "\n[Bai 5] 10 so nguyen to dau tien sinh boi C++20 Coroutine (co_yield):\n  ";
    auto gen = prime_stream(10);
    while (gen.next()) {
        std::cout << gen.value() << " ";
    }
    std::cout << "\n";
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP C++20
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 6.5 (C++20 FEATURES) ==========\n";
        std::cout << "1. Rang buoc Container & So hoc bang C++20 Concepts\n";
        std::cout << "2. Sap xep & Tim kiem voi Ranges Projections & Spaceship <=>\n";
        std::cout << "3. Duong ong phan tich giao dich (Ranges Views Pipeline |)\n";
        std::cout << "4. Trung binh truot Zero-Copy tren mang bang std::span\n";
        std::cout << "5. Bo sinh so nguyen to luoi bang C++20 Coroutine (co_yield)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=======================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_custom_concepts(); break;
            case 2: exercise_2_projections_and_spaceship(); break;
            case 3: exercise_3_ranges_pipeline_analytics(); break;
            case 4: exercise_4_span_sliding_average(); break;
            case 5: exercise_5_coroutine_prime_generator(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

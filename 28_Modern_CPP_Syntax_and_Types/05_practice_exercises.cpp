#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <variant>
#include <tuple>
#include <cmath>
#include <charconv>

// =============================================================================
// BÀI 1: TẠO BẢNG TRA CỨU LŨY THỪA 2 NGAY LÚC BIÊN DỊCH BẰNG CONSTEXPR
// =============================================================================
constexpr std::array<unsigned long long, 16> build_power_of_two_table() {
    std::array<unsigned long long, 16> table{};
    for (std::size_t i = 0; i < 16; ++i) {
        table[i] = (1ULL << i);
    }
    return table;
}

void exercise_1_constexpr_lookup_table() {
    // Toàn bộ mảng 16 phần tử được tính sẵn ở vùng .rodata ngay lúc biên dịch!
    constexpr auto pow2 = build_power_of_two_table();
    static_assert(pow2[10] == 1024ULL, "2^10 phai bang 1024");

    std::cout << "\n[Bai 1] Bang luy thua 2 tinh san luc bien dich (constexpr std::array):\n  ";
    for (std::size_t i = 0; i <= 10; ++i) {
        std::cout << "2^" << i << "=" << pow2[i] << "  ";
    }
    std::cout << "\n";
}


// =============================================================================
// BÀI 2: BỘ TÁCH DÒNG CSV SIÊU TỐC ZERO-COPY BẰNG STD::STRING_VIEW
// =============================================================================
std::vector<std::string_view> split_csv_zero_copy(std::string_view line, char delim = ',') {
    std::vector<std::string_view> tokens;
    std::size_t start = 0;
    while (start <= line.size()) {
        std::size_t pos = line.find(delim, start);
        if (pos == std::string_view::npos) {
            tokens.push_back(line.substr(start));
            break;
        }
        tokens.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    return tokens;
}

void exercise_2_string_view_csv_parser() {
    std::string raw_csv = "1001,Nguyen Van An,Senior C++ Engineer,3500.50";
    auto cols = split_csv_zero_copy(raw_csv, ',');

    std::cout << "\n[Bai 2] Tach " << cols.size() << " cot CSV voi 0 lan cap phat chuoi tren Heap:\n";
    for (std::size_t i = 0; i < cols.size(); ++i) {
        std::cout << "  Cot [" << i << "]: \"" << cols[i] << "\" (Addr: "
                  << static_cast<const void*>(cols[i].data()) << ")\n";
    }
}


// =============================================================================
// BÀI 3: CHUYỂN CHUỖI SANG SỐ AN TOÀN BẰNG STD::OPTIONAL + STD::FROM_CHARS
// =============================================================================
std::optional<int> parse_int_safe(std::string_view sv) {
    // Bỏ khoảng trắng đầu/cuối
    while (!sv.empty() && sv.front() == ' ') sv.remove_prefix(1);
    while (!sv.empty() && sv.back() == ' ')  sv.remove_suffix(1);
    if (sv.empty()) return std::nullopt;

    int value = 0;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc{} && ptr == sv.data() + sv.size()) {
        return value;
    }
    return std::nullopt;
}

void exercise_3_optional_safe_parser() {
    std::cout << "\n[Bai 3] Parse so nguyen an toan khong nem Exception (std::optional):\n";
    for (std::string_view input : {"  2026 ", "123abc", "", "-404"}) {
        auto parsed = parse_int_safe(input);
        std::cout << "  Input: \"" << input << "\"\t-> ";
        if (parsed) std::cout << "Hop le: " << *parsed << "\n";
        else        std::cout << "Loi (std::nullopt) -> Dung mac dinh: " << parsed.value_or(0) << "\n";
    }
}


// =============================================================================
// BÀI 4: MÔ HÌNH HÓA PHẢN HỒI HTTP API BẰNG STD::VARIANT + STD::VISIT
// =============================================================================
struct HttpSuccess { int status_code; std::string json_body; };
struct HttpRedirect { std::string target_url; };
struct HttpError { int error_code; std::string reason; };

using ApiResponse = std::variant<HttpSuccess, HttpRedirect, HttpError>;

void handle_response(const ApiResponse &resp) {
    std::visit([](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, HttpSuccess>) {
            std::cout << "  [200 OK] Data: " << r.json_body << "\n";
        } else if constexpr (std::is_same_v<T, HttpRedirect>) {
            std::cout << "  [301 Redirect] Chuyen huong toi: " << r.target_url << "\n";
        } else if constexpr (std::is_same_v<T, HttpError>) {
            std::cout << "  [Error " << r.error_code << "] Nguyen nhan: " << r.reason << "\n";
        }
    }, resp);
}

void exercise_4_variant_api_response() {
    std::cout << "\n[Bai 4] Xu ly ket qua API da trang thai bang std::variant:\n";
    handle_response(HttpSuccess{200, "{\"user\":\"An\",\"role\":\"admin\"}"});
    handle_response(HttpRedirect{"https://new-domain.com/login"});
    handle_response(HttpError{503, "Database connection timeout"});
}


// =============================================================================
// BÀI 5: GIẢI PHƯƠNG TRÌNH BẬC 2 TRẢ VỀ STRUCTURED BINDINGS + OPTIONAL
// =============================================================================
struct QuadraticRoots {
    int count; // 0, 1, hoặc 2 nghiệm thực
    double x1;
    double x2;
};

QuadraticRoots solve_quadratic(double a, double b, double c) {
    double delta = b * b - 4.0 * a * c;
    if (delta < -1e-9) return {0, 0.0, 0.0};
    if (std::abs(delta) <= 1e-9) {
        double x = -b / (2.0 * a);
        return {1, x, x};
    }
    double sqrt_d = std::sqrt(delta);
    return {2, (-b - sqrt_d) / (2.0 * a), (-b + sqrt_d) / (2.0 * a)};
}

void exercise_5_structured_bindings_roots() {
    std::cout << "\n[Bai 5] Giai phuong trinh x^2 - 5x + 6 = 0 bang Structured Bindings:\n";
    auto [num_roots, r1, r2] = solve_quadratic(1.0, -5.0, 6.0);
    std::cout << "  So nghiem = " << num_roots << " -> x1 = " << r1 << ", x2 = " << r2 << "\n";
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 6.1 (MODERN C++ SYNTAX & TYPES) ==========\n";
        std::cout << "1. Bang tra cuu Luy thua 2 luc bien dich (constexpr std::array)\n";
        std::cout << "2. Bo tach CSV Zero-Copy sieu toc (std::string_view)\n";
        std::cout << "3. Parse chuoi sang so an toan (std::optional + std::from_chars)\n";
        std::cout << "4. Xu ly phan hoi API da trang thai (std::variant + std::visit)\n";
        std::cout << "5. Giai phuong trinh tra ve da ket qua (Structured Bindings)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==================================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_constexpr_lookup_table(); break;
            case 2: exercise_2_string_view_csv_parser(); break;
            case 3: exercise_3_optional_safe_parser(); break;
            case 4: exercise_4_variant_api_response(); break;
            case 5: exercise_5_structured_bindings_roots(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

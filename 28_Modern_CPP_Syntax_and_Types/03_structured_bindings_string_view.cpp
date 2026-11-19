#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <tuple>

struct EmployeeRecord {
    int id;
    std::string name;
    double salary;
};

// Trả về std::tuple chứa nhiều giá trị khác kiểu
std::tuple<int, int, double> analyze_scores(const int a[], int n) {
    int min_v = a[0], max_v = a[0];
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] < min_v) min_v = a[i];
        if (a[i] > max_v) max_v = a[i];
        sum += a[i];
    }
    return {min_v, max_v, static_cast<double>(sum) / n};
}

// =============================================================================
// STD::STRING_VIEW: TRUYỀN & CẮT CHUỖI ZERO-COPY (0 HEAP ALLOCATION!)
// =============================================================================
std::string_view extract_domain(std::string_view email) {
    std::size_t at_pos = email.find('@');
    if (at_pos == std::string_view::npos) return {};
    // substr của string_view chỉ dịch con trỏ và giảm size -> Độ phức tạp O(1), không cấp phát Heap!
    return email.substr(at_pos + 1);
}

int main() {
    std::cout << "=== 1. STRUCTURED BINDINGS (C++17: AUTO [A, B, C] = ...) ===\n";
    // 1a. Giải nén struct trực tiếp
    EmployeeRecord emp{1001, "Nguyen Van An", 2500.0};
    const auto &[id, full_name, pay] = emp;
    std::cout << "  Unpack Struct : ID=" << id << " | Name=" << full_name << " | Pay=$" << pay << "\n";

    // 1b. Giải nén kết quả trả về từ std::tuple
    int scores[] = {70, 85, 90, 95, 60};
    auto [min_s, max_s, avg_s] = analyze_scores(scores, 5);
    std::cout << "  Unpack Tuple  : Min=" << min_s << " | Max=" << max_s << " | Avg=" << avg_s << "\n";

    // 1c. Duyệt std::map cực kỳ tường minh
    std::map<std::string, int> ports = {{"HTTP", 80}, {"HTTPS", 443}, {"SSH", 22}};
    std::cout << "  Duyet std::map: ";
    for (const auto &[protocol, port] : ports) {
        std::cout << "[" << protocol << ":" << port << "] ";
    }
    std::cout << "\n\n";

    std::cout << "=== 2. STD::STRING_VIEW (ZERO-COPY SUBSTRING O(1)) ===\n";
    std::string user_email = "developer@google.com";
    std::string_view domain_sv = extract_domain(user_email);

    std::cout << "  sizeof(std::string_view) = " << sizeof(std::string_view) << " bytes (1 con tro + 1 bien size)\n";
    std::cout << "  Email goc  : " << user_email << " (Addr: "
              << static_cast<const void*>(user_email.data()) << ")\n";
    std::cout << "  Domain view: " << domain_sv  << " (Addr: "
              << static_cast<const void*>(domain_sv.data()) << " -> Cung vung RAM, 0 Copy!)\n";

    return 0;
}

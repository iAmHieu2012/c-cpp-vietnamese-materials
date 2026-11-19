#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <iomanip>

// =============================================================================
// BÀI 1: TỰ CÀI ĐẶT GENERICVECTOR<T> TRÊN HEAP (CLASS TEMPLATE + RULE OF FIVE)
// =============================================================================
template <typename T>
class GenericVector {
private:
    T *data;
    std::size_t sz;
    std::size_t cap;

    void reallocate(std::size_t new_cap) {
        T *new_data = new T[new_cap];
        for (std::size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_cap;
    }

public:
    GenericVector() : data(nullptr), sz(0), cap(0) {}
    ~GenericVector() { delete[] data; }

    GenericVector(const GenericVector &other)
        : data(other.cap > 0 ? new T[other.cap] : nullptr), sz(other.sz), cap(other.cap) {
        for (std::size_t i = 0; i < sz; ++i) data[i] = other.data[i];
    }

    GenericVector(GenericVector &&other) noexcept
        : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = other.cap = 0;
    }

    GenericVector& operator=(GenericVector other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
        return *this;
    }

    void push_back(const T &val) {
        if (sz == cap) reallocate(cap == 0 ? 2 : cap * 2);
        data[sz++] = val;
    }

    std::size_t size() const noexcept { return sz; }
    std::size_t capacity() const noexcept { return cap; }
    T& operator[](std::size_t i) { return data[i]; }
    const T& operator[](std::size_t i) const { return data[i]; }

    const T* begin() const noexcept { return data; }
    const T* end()   const noexcept { return data + sz; }
};


// =============================================================================
// BÀI 2: LỚP KEYVALUEPAIR<K, V> KÈM PARTIAL SPECIALIZATION KHI K VÀ V CÙNG KIỂU
// =============================================================================
template <typename K, typename V>
class KeyValuePair {
private:
    K key;
    V val;
public:
    KeyValuePair(const K &k, const V &v) : key(k), val(v) {}
    void print() const {
        std::cout << "  [General <K, V>] Key = " << key << " | Value = " << val << "\n";
    }
};

// Partial Specialization khi cả Key và Value có CÙNG KIỂU T (KeyValuePair<T, T>)
template <typename T>
class KeyValuePair<T, T> {
private:
    T first, second;
public:
    KeyValuePair(const T &a, const T &b) : first(a), second(b) {}
    T sum() const { return first + second; }
    void print() const {
        std::cout << "  [Partial Spec <T, T>] (" << first << ", " << second
                  << ") -> Tong/Gop = " << sum() << "\n";
    }
};


// =============================================================================
// BÀI 3: HÀM VARIADIC MIN_ALL & PRINT_CSV_ROW (FOLD EXPRESSIONS)
// =============================================================================
template <typename First, typename... Rest>
auto min_all(const First &first, const Rest&... rest) {
    auto result = first;
    ((result = (rest < result ? rest : result)), ...);
    return result;
}

template <typename First, typename... Rest>
void print_csv_row(const First &first, const Rest&... rest) {
    std::cout << "  " << first;
    ((std::cout << "," << rest), ...);
    std::cout << "\n";
}


// =============================================================================
// BÀI 4: GENERIC FILTER & REDUCE (KẾT HỢP TEMPLATE + LAMBDA PREDICATE)
// =============================================================================
template <typename T, typename Predicate>
std::vector<T> generic_filter(const std::vector<T> &items, Predicate pred) {
    std::vector<T> result;
    for (const auto &item : items) {
        if (pred(item)) result.push_back(item);
    }
    return result;
}


// =============================================================================
// BÀI 5: BỘ TUẦN TỰ HÓA JSON COMPILE-TIME BẰNG IF CONSTEXPR & TYPE_TRAITS
// =============================================================================
template <typename T>
std::string to_json_value(const T &val) {
    if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
        return std::to_string(val);
    } else {
        // Chuỗi hoặc const char* -> bọc trong cặp dấu nháy kép "..."
        return "\"" + std::string(val) + "\"";
    }
}

template <typename V>
void print_json_field(const std::string &key, const V &val, bool is_last = false) {
    std::cout << "    \"" << key << "\": " << to_json_value(val) << (is_last ? "\n" : ",\n");
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 5.1 (TEMPLATES & GENERIC) ==========\n";
        std::cout << "1. Tu cai dat GenericVector<T> tren Heap (int & std::string)\n";
        std::cout << "2. KeyValuePair<K, V> & Partial Specialization <T, T>\n";
        std::cout << "3. Variadic min_all(...) & print_csv_row(...) (Fold Expr)\n";
        std::cout << "4. Generic Filter ket hop Template & Lambda Predicate\n";
        std::cout << "5. Bo chuyen doi JSON Compile-Time (if constexpr)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                GenericVector<std::string> langs;
                langs.push_back("C");
                langs.push_back("C++17");
                langs.push_back("Rust");
                std::cout << "\n[Bai 1] GenericVector<string> (size=" << langs.size()
                          << ", cap=" << langs.capacity() << "): ";
                for (const auto &s : langs) std::cout << s << " ";
                std::cout << "\n";
                break;
            }

            case 2: {
                std::cout << "\n[Bai 2] Kiem thu Partial Specialization:\n";
                KeyValuePair<std::string, int> p1("Age", 21);
                KeyValuePair<int, int> p2(100, 250);
                KeyValuePair<std::string, std::string> p3("Hello ", "Templates!");
                p1.print();
                p2.print();
                p3.print();
                break;
            }

            case 3: {
                std::cout << "\n[Bai 3] Variadic Fold Expressions:\n";
                std::cout << "  min_all(42, 15, 89, 7, 23) = " << min_all(42, 15, 89, 7, 23) << "\n";
                std::cout << "  CSV Output:\n";
                print_csv_row("ID", "FullName", "GPA", "Passed");
                print_csv_row(1001, "Nguyen Van An", 8.75, "true");
                break;
            }

            case 4: {
                std::vector<int> nums = {12, 5, 18, 3, 25, 8, 30};
                auto gt10 = generic_filter(nums, [](int x) { return x > 10; });

                std::vector<std::string> words = {"cat", "elephant", "dog", "algorithm"};
                auto long_words = generic_filter(words, [](const std::string &s) { return s.size() >= 5; });

                std::cout << "\n[Bai 4] Ket qua generic_filter:\n  So > 10       : ";
                for (int x : gt10) std::cout << x << " ";
                std::cout << "\n  Tu >= 5 ky tu : ";
                for (const auto &w : long_words) std::cout << w << " ";
                std::cout << "\n";
                break;
            }

            case 5: {
                std::cout << "\n[Bai 5] JSON Serialization bang if constexpr:\n  {\n";
                print_json_field("id", 1001);
                print_json_field("name", "Tran Thi Binh");
                print_json_field("gpa", 9.25);
                print_json_field("graduated", true, true);
                std::cout << "  }\n";
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

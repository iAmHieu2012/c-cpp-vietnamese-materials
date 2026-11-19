#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

// =============================================================================
// 1. FUNCTION TEMPLATES (KHUÔN MẪU HÀM ĐỘC LẬP KIỂU DỮ LIỆU)
// =============================================================================
template <typename T>
void my_swap(T &a, T &b) noexcept {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// Hỗ trợ truyền 2 kiểu dữ liệu khác nhau (VD: int và double) với kiểu trả về tự suy luận (auto)
template <typename T1, typename T2>
auto my_max(const T1 &a, const T2 &b) {
    return (a < b) ? b : a;
}

// =============================================================================
// 2. CLASS TEMPLATE VỚI NON-TYPE TEMPLATE PARAMETER (BẢN CHẤT CỦA STD::ARRAY)
// =============================================================================
template <typename T, std::size_t N>
class StaticArray {
private:
    T data[N]; // Cấp phát trực tiếp trên Stack với kích thước N biết lúc biên dịch

public:
    constexpr std::size_t size() const noexcept { return N; }

    T& operator[](std::size_t idx) { return data[idx]; }
    const T& operator[](std::size_t idx) const { return data[idx]; }

    T& at(std::size_t idx) {
        if (idx >= N) throw std::out_of_range("StaticArray: Chi so vuot bien!");
        return data[idx];
    }

    void fill(const T &val) {
        for (std::size_t i = 0; i < N; ++i) data[i] = val;
    }

    void print(const char *label) const {
        std::cout << "  " << label << " [" << N << "]: ";
        for (std::size_t i = 0; i < N; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};

int main() {
    std::cout << "=== 1. FUNCTION TEMPLATES (MY_SWAP & MY_MAX) ===\n";
    int x = 10, y = 99;
    my_swap(x, y);
    std::cout << "  Sau my_swap<int>    : x = " << x << ", y = " << y << "\n";

    std::string s1 = "Alpha", s2 = "Omega";
    my_swap(s1, s2);
    std::cout << "  Sau my_swap<string> : s1 = " << s1 << ", s2 = " << s2 << "\n";

    // Kết hợp int (15) và double (19.75) -> tự động trả về double (19.75)
    std::cout << "  my_max(15, 19.75)   = " << my_max(15, 19.75) << "\n\n";

    std::cout << "=== 2. CLASS TEMPLATE + NON-TYPE PARAMETER (STATICARRAY<T, N>) ===\n";
    StaticArray<int, 5> int_arr{};
    int_arr.fill(7);
    int_arr[2] = 42;
    int_arr.print("StaticArray<int, 5>   ");

    StaticArray<std::string, 3> str_arr{};
    str_arr[0] = "C++11";
    str_arr[1] = "C++17";
    str_arr[2] = "C++20";
    str_arr.print("StaticArray<string, 3>");

    return 0;
}

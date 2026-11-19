#include <iostream>
#include <cstring>
#include <string>

// =============================================================================
// 1. FULL FUNCTION TEMPLATE SPECIALIZATION (CHUYÊN BIỆT HÓA CHO CONST CHAR*)
// =============================================================================
template <typename T>
bool is_equal(const T &a, const T &b) {
    return a == b;
}

// Nếu không có bản chuyên biệt hóa này, is_equal(const char*, const char*) sẽ so sánh
// 2 ĐỊA CHỈ CON TRỎ thay vì so sánh nội dung chuỗi ký tự!
template <>
bool is_equal<const char*>(const char* const &a, const char* const &b) {
    return std::strcmp(a, b) == 0;
}

// =============================================================================
// 2. CLASS TEMPLATE: FULL SPECIALIZATION & PARTIAL SPECIALIZATION (CHO CON TRỎ T*)
// =============================================================================
// 2a. Khuôn mẫu gốc (Primary Template)
template <typename T>
class DataHolder {
private:
    T value;
public:
    explicit DataHolder(const T &v) : value(v) {}
    void print() const {
        std::cout << "  [Primary Template] Gia tri = " << value << "\n";
    }
};

// 2b. Chuyên biệt hóa một phần cho MỌI KIỂU CON TRỎ T* (Partial Specialization)
template <typename T>
class DataHolder<T*> {
private:
    T *ptr;
public:
    explicit DataHolder(T *p) : ptr(p) {}
    void print() const {
        if (ptr != nullptr) {
            std::cout << "  [Partial Spec T*] Dia chi = " << ptr << " -> Gia tri = " << *ptr << "\n";
        } else {
            std::cout << "  [Partial Spec T*] nullptr\n";
        }
    }
};

// 2c. Chuyên biệt hóa toàn phần cho kiểu bool (Full Specialization)
template <>
class DataHolder<bool> {
private:
    bool flag;
public:
    explicit DataHolder(bool f) : flag(f) {}
    void print() const {
        std::cout << "  [Full Spec bool] Trang thai = " << (flag ? "TRUE (1)" : "FALSE (0)") << "\n";
    }
};

int main() {
    std::cout << "=== 1. FULL SPECIALIZATION CHO CONST CHAR* ===\n";
    char buf1[] = "Hello";
    char buf2[] = "Hello"; // Khác địa chỉ RAM với buf1 nhưng cùng nội dung chữ!
    const char *p1 = buf1;
    const char *p2 = buf2;

    std::cout << "  So sanh dia chi (p1 == p2)       : " << (p1 == p2 ? "Bang" : "Khac (2 vung Stack)") << "\n";
    std::cout << "  So sanh qua is_equal(p1, p2)     : " << (is_equal(p1, p2) ? "Bang (nho strcmp)" : "Khac") << "\n\n";

    std::cout << "=== 2. PRIMARY vs PARTIAL (T*) vs FULL SPECIALIZATION (BOOL) ===\n";
    int number = 2026;
    DataHolder<int>  h_int(number);
    DataHolder<int*> h_ptr(&number);
    DataHolder<bool> h_bool(true);

    h_int.print();
    h_ptr.print();
    h_bool.print();

    return 0;
}

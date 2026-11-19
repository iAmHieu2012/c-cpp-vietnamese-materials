#include <iostream>
#include <cmath>

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 1. Nạp chồng toán tử cộng (+) và trừ (-) bằng phương thức thành viên
    Complex operator+(const Complex &other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex &other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // 2. Nạp chồng toán tử so sánh bằng (==)
    bool operator==(const Complex &other) const {
        return std::abs(real - other.real) < 1e-9 && std::abs(imag - other.imag) < 1e-9;
    }

    // 3. Nạp chồng toán tử truy cập chỉ số [] (0: phần thực, 1: phần ảo)
    double& operator[](int idx) {
        return (idx == 0) ? real : imag;
    }

    // 4. Nạp chồng toán tử xuất (<<) và nhập (>>) BẮT BUỘC dùng friend function
    // vì vế trái là std::ostream / std::istream chứ không phải Complex!
    friend std::ostream& operator<<(std::ostream &os, const Complex &c) {
        os << c.real << (c.imag >= 0 ? " + " : " - ") << std::abs(c.imag) << "i";
        return os;
    }

    friend std::istream& operator>>(std::istream &is, Complex &c) {
        is >> c.real >> c.imag;
        return is;
    }
};

int main() {
    Complex c1(3.0, 4.0);
    Complex c2(1.5, -2.0);

    Complex sum = c1 + c2; // Gọi c1.operator+(c2)
    Complex diff = c1 - c2;

    std::cout << "c1      = " << c1 << "\n";
    std::cout << "c2      = " << c2 << "\n";
    std::cout << "c1 + c2 = " << sum << "\n";
    std::cout << "c1 - c2 = " << diff << "\n";
    std::cout << "c1 == c2? " << (c1 == c2 ? "Bang nhau" : "Khac nhau") << "\n";

    c1[0] = 10.0; // Gọi operator[]
    std::cout << "Sau khi doi c1[0] = 10: " << c1 << "\n";

    return 0;
}
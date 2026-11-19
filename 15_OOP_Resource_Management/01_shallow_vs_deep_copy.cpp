#include <iostream>
#include <cstring>

class SafeString {
private:
    char *str;
    size_t len;

public:
    SafeString(const char *s = "") {
        len = std::strlen(s);
        str = new char[len + 1];
        std::strcpy(str, s);
    }

    // DEEP COPY CONSTRUCTOR: Cấp phát vùng nhớ mới hoàn toàn độc lập
    SafeString(const SafeString &other) {
        len = other.len;
        str = new char[len + 1];
        std::strcpy(str, other.str);
        std::cout << "  [Deep Copy] Cap phat vung nho moi tai: " << static_cast<const void*>(str) << "\n";
    }

    ~SafeString() {
        delete[] str;
    }

    void set_char(size_t idx, char c) {
        if (idx < len) str[idx] = c;
    }

    void print(const char *label) const {
        std::cout << "  " << label << " = \"" << str << "\" (Addr: "
                  << static_cast<const void*>(str) << ")\n";
    }
};

int main() {
    SafeString s1("Hello");
    SafeString s2 = s1; // Gọi Deep Copy Constructor

    s2.set_char(0, 'X'); // Sửa s2 không làm ảnh hưởng s1

    s1.print("s1 (goc) ");
    s2.print("s2 (copy)");

    // Khi thoát main(), s2 và s1 giải phóng 2 vùng nhớ khác nhau -> An toàn tuyệt đối!
    return 0;
}
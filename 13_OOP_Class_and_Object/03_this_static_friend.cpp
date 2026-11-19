#include <iostream>
#include <string>

class Box {
private:
    double width;
    double height;
    // Biến static: Dùng chung cho mọi object Box (chỉ khai báo ở đây, định nghĩa ngoài class)
    static int total_boxes;

public:
    Box(double width = 1.0, double height = 1.0) {
        // Dùng con trỏ 'this' phân biệt thuộc tính và tham số trùng tên
        this->width = width;
        this->height = height;
        ++total_boxes;
    }

    ~Box() {
        --total_boxes;
    }

    // Method Chaining: Trả về tham chiếu '*this' để gọi nối tiếp nhau
    Box& set_width(double width) {
        this->width = width;
        return *this;
    }

    Box& set_height(double height) {
        this->height = height;
        return *this;
    }

    // Hàm static: Gọi trực tiếp qua tên Class (Box::get_total_boxes()) không cần tạo Object
    static int get_total_boxes() {
        return total_boxes;
    }

    // Khai báo Hàm bạn (friend function) & Lớp bạn (friend class)
    friend double calculate_area(const Box &b);
    friend class BoxInspector;
};

// Định nghĩa và khởi tạo biến static ở phạm vi toàn cục (Bắt buộc trước C++17)
int Box::total_boxes = 0;

// Hàm bạn: Truy cập thẳng vào b.width và b.height dù chúng là private!
double calculate_area(const Box &b) {
    return b.width * b.height;
}

// Lớp bạn: Mọi phương thức của BoxInspector đều truy cập được private của Box
class BoxInspector {
public:
    void inspect(const Box &b) const {
        std::cout << "  [BoxInspector] W = " << b.width << ", H = " << b.height
                  << ", Area = " << (b.width * b.height) << "\n";
    }
};

int main() {
    std::cout << "So Box ban dau (goi qua Class): " << Box::get_total_boxes() << "\n";

    Box b1(2.0, 3.0);
    // Method Chaining nhờ return *this
    b1.set_width(5.0).set_height(4.0);

    std::cout << "Dien tich b1 (qua friend function): " << calculate_area(b1) << "\n";

    BoxInspector inspector;
    inspector.inspect(b1);

    {
        Box b2, b3;
        std::cout << "So Box trong scope: " << Box::get_total_boxes() << "\n";
    }
    std::cout << "So Box sau khi b2, b3 bi huy: " << Box::get_total_boxes() << "\n";

    return 0;
}
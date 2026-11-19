// ============================================================================
// File: 03_polymorphism_and_vtable.cpp
// Bài 3.2: Đa hình Runtime (virtual, override, final, vtable/vptr, Slicing)
// ============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

const double PI = 3.141592653589793;

// ============================================================================
// 1. ĐA HÌNH RUNTIME VỚI VIRTUAL, OVERRIDE, FINAL & VIRTUAL DESTRUCTOR
// ============================================================================
class Shape {
public:
    virtual double area() const { return 0.0; }
    virtual void draw() const {
        std::cout << "  [Shape] Hinh co ban\n";
    }

    // VIRTUAL DESTRUCTOR: Bắt buộc để khi 'delete Shape*' gọi đúng ~Derived() trước
    virtual ~Shape() {
        std::cout << "  [~Shape] Huy lop co so\n";
    }
};

class Circle final : public Shape { // 'final' cấm các lớp khác kế thừa tiếp từ Circle
    double radius_;

public:
    explicit Circle(double r) : radius_(r) {}

    double area() const override { return PI * radius_ * radius_; }
    void draw() const override {
        std::cout << "  [Circle] Ban kinh = " << radius_
                  << " | Dien tich = " << area() << "\n";
    }
    double getRadius() const { return radius_; }

    ~Circle() override {
        std::cout << "  [~Circle] Huy hinh tron\n";
    }
};

class Rectangle : public Shape {
    double width_, height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double area() const override { return width_ * height_; }
    void draw() const override {
        std::cout << "  [Rectangle] " << width_ << "x" << height_
                  << " | Dien tich = " << area() << "\n";
    }

    ~Rectangle() override {
        std::cout << "  [~Rectangle] Huy hinh chu nhat\n";
    }
};

// ============================================================================
// 2. MINH HỌA CẠM BẪY OBJECT SLICING (CẮT LÁT ĐỐI TƯỢNG)
// ============================================================================
class BaseAnimal {
protected:
    std::string name_;

public:
    explicit BaseAnimal(std::string n) : name_(std::move(n)) {}
    virtual ~BaseAnimal() = default;
    virtual void speak() const {
        std::cout << "  [BaseAnimal] " << name_ << " keu chung chung...\n";
    }
};

class Dog : public BaseAnimal {
    std::string breed_;

public:
    Dog(std::string n, std::string b)
        : BaseAnimal(std::move(n)), breed_(std::move(b)) {}

    void speak() const override {
        std::cout << "  [Dog - " << breed_ << "] " << name_ << " sua: Gau Gau!\n";
    }
};

// SAI: Truyền theo giá trị (Pass by Value) gây cắt bỏ phần Dog (Object Slicing)!
void makeSpeakSliced(BaseAnimal a) {
    a.speak(); // Luôn gọi BaseAnimal::speak()!
}

// ĐÚNG: Truyền qua tham chiếu hằng (const BaseAnimal&) giữ nguyên đa hình Runtime!
void makeSpeakPolymorphic(const BaseAnimal &a) {
    a.speak(); // Tra vtable gọi đúng Dog::speak()!
}

// Chứng minh sự tồn tại của con trỏ ẩn vptr (8 bytes trên 64-bit)
class NoVirtual { int x; };
class HasVirtual { int x; virtual void f() {} };

int main() {
    std::cout << "=== 1. CHUNG MINH CON TRO AN VPTR TREN RAM ===\n";
    std::cout << "  sizeof(NoVirtual)  = " << sizeof(NoVirtual)
              << " bytes (chi co int x)\n";
    std::cout << "  sizeof(HasVirtual) = " << sizeof(HasVirtual)
              << " bytes (int x + 8 bytes vptr + padding)\n\n";

    std::cout << "=== 2. DA HINH RUNTIME & RTTI (DYNAMIC_CAST) ===\n";
    Shape *shapes[2];
    shapes[0] = new Circle(5.0);
    shapes[1] = new Rectangle(4.0, 6.0);

    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < 2; ++i) {
        shapes[i]->draw();
        // Kiểm tra kiểu thực tế lúc chạy an toàn bằng dynamic_cast
        if (auto *c = dynamic_cast<Circle *>(shapes[i])) {
            std::cout << "    -> [dynamic_cast] Phat hien Circle voi ban kinh r = "
                      << c->getRadius() << "\n";
        }
    }

    std::cout << "\n=== 3. CAM BAY OBJECT SLICING (VALUE VS CONST REFERENCE) ===\n";
    Dog myDog("Milu", "Shiba");
    std::cout << "  -> Khi truyen tham tri (Bi Object Slicing):\n";
    makeSpeakSliced(myDog);
    std::cout << "  -> Khi truyen tham chieu const& (Da hinh chuan):\n";
    makeSpeakPolymorphic(myDog);

    std::cout << "\n=== 4. GIAI PHONG QUA CON TRO LOP CHA (VIRTUAL DESTRUCTOR) ===\n";
    for (int i = 0; i < 2; ++i) {
        delete shapes[i];
    }

    return 0;
}

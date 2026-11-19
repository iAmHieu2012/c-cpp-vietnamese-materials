// ============================================================================
// File: 03_liskov_substitution_lsp.cpp
// Bài 7.5: SOLID Principles - [L] Liskov Substitution Principle (Thay thế Liskov)
// "Subtypes must be substitutable for their base types without altering correctness."
// ============================================================================

#include <iostream>
#include <memory>
#include <vector>

// ============================================================================
// 1. VI PHẠM LSP (Cái bẫy kinh điển: Square kế thừa từ Rectangle)
// ============================================================================
namespace BadLSP {

class Rectangle {
protected:
    int width_;
    int height_;

public:
    Rectangle(int w, int h) : width_(w), height_(h) {}
    virtual ~Rectangle() = default;

    virtual void setWidth(int w) { width_ = w; }
    virtual void setHeight(int h) { height_ = h; }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    int getArea() const { return width_ * height_; }
};

// Hình vuông trong toán học là Hình chữ nhật, nhưng trong OOP nếu Square
// âm thầm sửa cả height khi gọi setWidth() thì nó PHÁ VỠ hợp đồng của Rectangle!
class Square : public Rectangle {
public:
    explicit Square(int side) : Rectangle(side, side) {}

    void setWidth(int w) override {
        width_ = height_ = w;
    }

    void setHeight(int h) override {
        width_ = height_ = h;
    }
};

void processRectangle(Rectangle &r) {
    int w = r.getWidth();
    r.setHeight(10); // Client kỳ vọng width giữ nguyên, diện tích = w * 10
    std::cout << "  [BadLSP] Ky vong dien tich = " << (w * 10)
              << " | Thuc te = " << r.getArea()
              << ((r.getArea() == w * 10) ? " (OK)\n" : " (SAI - VI PHAM LSP!)\n");
}

} // namespace BadLSP

// ============================================================================
// 2. TUÂN THỦ LSP (Tách giao diện bất biến IShape hoặc tách cây kế thừa hợp lý)
// ============================================================================
namespace GoodLSP {

class IShape {
public:
    virtual ~IShape() = default;
    virtual int getArea() const = 0;
    virtual void describe() const = 0;
};

class Rectangle : public IShape {
    int width_;
    int height_;

public:
    Rectangle(int w, int h) : width_(w), height_(h) {}

    void setWidth(int w) { width_ = w; }
    void setHeight(int h) { height_ = h; }

    int getArea() const override { return width_ * height_; }
    void describe() const override {
        std::cout << "  [GoodLSP] Rectangle (" << width_ << "x" << height_
                  << ") -> Area = " << getArea() << "\n";
    }
};

class Square : public IShape {
    int side_;

public:
    explicit Square(int side) : side_(side) {}

    void setSide(int s) { side_ = s; }

    int getArea() const override { return side_ * side_; }
    void describe() const override {
        std::cout << "  [GoodLSP] Square (" << side_ << "x" << side_
                  << ") -> Area = " << getArea() << "\n";
    }
};

} // namespace GoodLSP

int main() {
    std::cout << "=== [L] LISKOV SUBSTITUTION PRINCIPLE (LSP) ===\n\n";

    std::cout << "--- 1. Khi Square ke thua sai tu Rectangle (Vi pham LSP) ---\n";
    BadLSP::Rectangle rect(5, 5);
    BadLSP::processRectangle(rect);

    BadLSP::Square sq(5);
    BadLSP::processRectangle(sq); // Truyền lớp con Square vào chỗ Rectangle làm kết quả sai!

    std::cout << "\n--- 2. Thiet ke dung: Rectangle va Square cung thuc thi IShape ---\n";
    std::vector<std::unique_ptr<GoodLSP::IShape>> shapes;
    shapes.push_back(std::make_unique<GoodLSP::Rectangle>(5, 10));
    shapes.push_back(std::make_unique<GoodLSP::Square>(5));

    for (const auto &shape : shapes) {
        shape->describe(); // Mọi lớp con của IShape đều thay thế hoàn hảo cho IShape
    }

    return 0;
}

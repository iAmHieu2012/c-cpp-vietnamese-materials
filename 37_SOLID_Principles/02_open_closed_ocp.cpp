// ============================================================================
// File: 02_open_closed_ocp.cpp
// Bài 7.5: SOLID Principles - [O] Open/Closed Principle (Nguyên lý Đóng/Mở)
// "Software entities should be open for extension, but closed for modification."
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

enum class Color { RED, GREEN, SILVER };
enum class Size { SMALL, MEDIUM, LARGE };

struct Product {
    std::string name;
    Color color;
    Size size;
    double price;
};

// ============================================================================
// 1. VI PHẠM OCP (Mỗi lần thêm tiêu chí lọc mới lại phải sửa đổi class cũ)
// ============================================================================
namespace BadOCP {

class RigidProductFilter {
public:
    static std::vector<Product> filterByColor(const std::vector<Product> &items, Color c) {
        std::vector<Product> result;
        for (const auto &p : items) {
            if (p.color == c) result.push_back(p);
        }
        return result;
    }

    // Khi sếp yêu cầu lọc theo Size, rồi lọc theo (Color + Size), rồi lọc theo Giá...
    // ta phải liên tục mở class này ra sửa và copy-paste vòng lặp!
    static std::vector<Product> filterByColorAndSize(const std::vector<Product> &items,
                                                     Color c, Size s) {
        std::vector<Product> result;
        for (const auto &p : items) {
            if (p.color == c && p.size == s) result.push_back(p);
        }
        return result;
    }
};

} // namespace BadOCP

// ============================================================================
// 2. TUÂN THỦ OCP (Áp dụng Specification Pattern: Mở để mở rộng, Đóng để sửa đổi)
// ============================================================================
namespace GoodOCP {

// Giao diện tiêu chí lọc tổng quát
template <typename T>
class ISpecification {
public:
    virtual ~ISpecification() = default;
    virtual bool isSatisfied(const T &item) const = 0;
};

// Bộ lọc đóng kín hoàn toàn (Closed for modification) - Không bao giờ phải sửa lại!
template <typename T>
class BetterFilter {
public:
    std::vector<const T *> filter(const std::vector<T> &items,
                                  const ISpecification<T> &spec) const {
        std::vector<const T *> result;
        for (const auto &item : items) {
            if (spec.isSatisfied(item)) {
                result.push_back(&item);
            }
        }
        return result;
    }
};

// Kết hợp 2 tiêu chí lọc bất kỳ bằng toán tử logic AND (Combinator)
template <typename T>
class AndSpecification : public ISpecification<T> {
    const ISpecification<T> &first_;
    const ISpecification<T> &second_;

public:
    AndSpecification(const ISpecification<T> &first, const ISpecification<T> &second)
        : first_(first), second_(second) {}

    bool isSatisfied(const T &item) const override {
        return first_.isSatisfied(item) && second_.isSatisfied(item);
    }
};

// Mở rộng các tiêu chí lọc mới bằng cách tạo Class mới (Open for extension)
class ColorSpecification : public ISpecification<Product> {
    Color color_;

public:
    explicit ColorSpecification(Color c) : color_(c) {}
    bool isSatisfied(const Product &item) const override {
        return item.color == color_;
    }
};

class SizeSpecification : public ISpecification<Product> {
    Size size_;

public:
    explicit SizeSpecification(Size s) : size_(s) {}
    bool isSatisfied(const Product &item) const override {
        return item.size == size_;
    }
};

class MaxPriceSpecification : public ISpecification<Product> {
    double maxPrice_;

public:
    explicit MaxPriceSpecification(double maxP) : maxPrice_(maxP) {}
    bool isSatisfied(const Product &item) const override {
        return item.price <= maxPrice_;
    }
};

} // namespace GoodOCP

int main() {
    std::cout << "=== [O] OPEN/CLOSED PRINCIPLE (SPECIFICATION PATTERN) ===\n\n";

    std::vector<Product> catalog = {
        {"Apple Tree", Color::GREEN, Size::LARGE, 150.0},
        {"Matcha Tea", Color::GREEN, Size::SMALL, 15.0},
        {"MacBook Pro", Color::SILVER, Size::MEDIUM, 2200.0},
        {"Gaming Mouse", Color::SILVER, Size::SMALL, 65.0},
        {"Ferrari Model", Color::RED, Size::LARGE, 890.0}
    };

    GoodOCP::BetterFilter<Product> filterEngine;

    // 1. Lọc các sản phẩm màu Xanh lá (GREEN)
    GoodOCP::ColorSpecification greenSpec(Color::GREEN);
    std::cout << "--- 1. San pham mau GREEN ---\n";
    for (const auto *p : filterEngine.filter(catalog, greenSpec)) {
        std::cout << "  * " << p->name << " ($" << p->price << ")\n";
    }

    // 2. Kết hợp tiêu chí: Màu Bạc (SILVER) VÀ Giá <= $1000
    GoodOCP::ColorSpecification silverSpec(Color::SILVER);
    GoodOCP::MaxPriceSpecification affordableSpec(1000.0);
    GoodOCP::AndSpecification<Product> silverAndAffordable(silverSpec, affordableSpec);

    std::cout << "\n--- 2. San pham mau SILVER va Gia <= $1000 ---\n";
    for (const auto *p : filterEngine.filter(catalog, silverAndAffordable)) {
        std::cout << "  * " << p->name << " ($" << p->price << ")\n";
    }

    return 0;
}

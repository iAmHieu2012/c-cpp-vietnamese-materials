#include <iostream>
#include <string>

// =============================================================================
// BRIDGE PATTERN (TÁCH RỜI ABSTRACTION VÀ IMPLEMENTATION)
// =============================================================================

// Nhánh Implementation: API Đồ họa cấp thấp
class IRenderAPI {
public:
    virtual ~IRenderAPI() = default;
    virtual void render_circle(double x, double y, double radius) const = 0;
    virtual void render_box(double w, double h) const = 0;
};

class OpenGLAPI : public IRenderAPI {
public:
    void render_circle(double x, double y, double r) const override {
        std::cout << "  [OpenGL API] Ve Circle tai (" << x << ", " << y << "), R = " << r << "\n";
    }
    void render_box(double w, double h) const override {
        std::cout << "  [OpenGL API] Ve Box kich thuoc " << w << "x" << h << "\n";
    }
};

class VulkanAPI : public IRenderAPI {
public:
    void render_circle(double x, double y, double r) const override {
        std::cout << "  [Vulkan API] Ve Circle tai (" << x << ", " << y << "), R = " << r << "\n";
    }
    void render_box(double w, double h) const override {
        std::cout << "  [Vulkan API] Ve Box kich thuoc " << w << "x" << h << "\n";
    }
};

// Nhánh Abstraction: Hình học cấp cao giữ cầu nối (Bridge) sang IRenderAPI
class Shape {
protected:
    const IRenderAPI &renderer; // Cầu nối (Bridge)
public:
    explicit Shape(const IRenderAPI &api) : renderer(api) {}
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class CircleShape : public Shape {
    double x, y, radius;
public:
    CircleShape(double x_, double y_, double r_, const IRenderAPI &api)
        : Shape(api), x(x_), y(y_), radius(r_) {}
    void draw() const override { renderer.render_circle(x, y, radius); }
};

class BoxShape : public Shape {
    double width, height;
public:
    BoxShape(double w_, double h_, const IRenderAPI &api)
        : Shape(api), width(w_), height(h_) {}
    void draw() const override { renderer.render_box(width, height); }
};

int main() {
    std::cout << "=== BRIDGE PATTERN (TACH SHAPE VA RENDER API) ===\n";
    OpenGLAPI opengl;
    VulkanAPI vulkan;

    CircleShape c1(10.0, 20.0, 5.0, opengl);
    BoxShape    b1(12.0, 8.0, vulkan);

    c1.draw();
    b1.draw();

    return 0;
}

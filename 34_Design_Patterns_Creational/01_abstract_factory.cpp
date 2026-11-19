#include <iostream>
#include <memory>
#include <string>

// =============================================================================
// ABSTRACT FACTORY PATTERN (TẠO CẢ MỘT HỌ ĐỐI TƯỢNG ĐỒNG BỘ ĐA NỀN TẢNG)
// =============================================================================
class IButton {
public:
    virtual ~IButton() = default;
    virtual void render() const = 0;
};

class ICheckbox {
public:
    virtual ~ICheckbox() = default;
    virtual void render() const = 0;
};

// Họ sản phẩm 1: Giao diện Windows
class WinButton : public IButton {
public:
    void render() const override { std::cout << "  [Windows UI] Ve WinButton phong cach Fluent.\n"; }
};

class WinCheckbox : public ICheckbox {
public:
    void render() const override { std::cout << "  [Windows UI] Ve WinCheckbox vuong.\n"; }
};

// Họ sản phẩm 2: Giao diện macOS
class MacButton : public IButton {
public:
    void render() const override { std::cout << "  [macOS UI] Ve MacButton bo tron Aqua.\n"; }
};

class MacCheckbox : public ICheckbox {
public:
    void render() const override { std::cout << "  [macOS UI] Ve MacCheckbox xanh.\n"; }
};

// Giao diện Nhà máy trừu tượng (Abstract Factory)
class IGUIFactory {
public:
    virtual ~IGUIFactory() = default;
    virtual std::unique_ptr<IButton>   create_button() const = 0;
    virtual std::unique_ptr<ICheckbox> create_checkbox() const = 0;
};

class WinFactory : public IGUIFactory {
public:
    std::unique_ptr<IButton>   create_button()   const override { return std::make_unique<WinButton>(); }
    std::unique_ptr<ICheckbox> create_checkbox() const override { return std::make_unique<WinCheckbox>(); }
};

class MacFactory : public IGUIFactory {
public:
    std::unique_ptr<IButton>   create_button()   const override { return std::make_unique<MacButton>(); }
    std::unique_ptr<ICheckbox> create_checkbox() const override { return std::make_unique<MacCheckbox>(); }
};

// Client chỉ làm việc với giao diện IGUIFactory mà không phụ thuộc lớp cụ thể Win/Mac
void render_application_ui(const IGUIFactory &factory) {
    auto btn = factory.create_button();
    auto chk = factory.create_checkbox();
    btn->render();
    chk->render();
}

int main() {
    std::cout << "=== ABSTRACT FACTORY PATTERN (CROSS-PLATFORM UI FAMILIES) ===\n";
    WinFactory win_factory;
    MacFactory mac_factory;

    std::cout << "-> Khoi tao App voi WinFactory:\n";
    render_application_ui(win_factory);

    std::cout << "\n-> Khoi tao App voi MacFactory:\n";
    render_application_ui(mac_factory);

    return 0;
}

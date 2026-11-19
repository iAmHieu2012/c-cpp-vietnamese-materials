#include <iostream>
#include <string>
#include <memory>

// =============================================================================
// PROXY PATTERN (KHỞI TẠO LƯỜI LAZY LOADING & KIỂM SOÁT TRUY CẬP)
// =============================================================================
class IImage {
public:
    virtual ~IImage() = default;
    virtual void display() = 0;
};

class HighResImage : public IImage {
private:
    std::string filename;
public:
    explicit HighResImage(std::string f) : filename(std::move(f)) {
        std::cout << "    [HighResImage] Dang tai anh 8K nang 50MB tu o cung: " << filename << "...\n";
    }
    void display() override {
        std::cout << "    [HighResImage] Hien thi anh: " << filename << "\n";
    }
};

// Virtual Proxy: Trì hoãn việc tải ảnh 50MB cho tới khi hàm display() thực sự được gọi!
class LazyImageProxy : public IImage {
private:
    std::string filename;
    std::unique_ptr<HighResImage> real_image;
public:
    explicit LazyImageProxy(std::string f) : filename(std::move(f)), real_image(nullptr) {
        std::cout << "  [LazyImageProxy] Khoi tao Proxy nhe cho '" << filename << "' (Chua tai anh that!).\n";
    }

    void display() override {
        if (!real_image) {
            real_image = std::make_unique<HighResImage>(filename); // Chỉ nạp lần đầu tiên!
        }
        real_image->display();
    }
};

int main() {
    std::cout << "=== PROXY PATTERN (LAZY LOADING VIRTUAL PROXY) ===\n";
    LazyImageProxy img_proxy("panorama_universe_8k.png");

    std::cout << "  -> Lan 1 goi display() (Bay gio moi nap tu o cung):\n";
    img_proxy.display();

    std::cout << "  -> Lan 2 goi display() (Tai su dung ban da nap tren RAM):\n";
    img_proxy.display();

    return 0;
}

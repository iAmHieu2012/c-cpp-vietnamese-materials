#include <iostream>
#include <memory> // std::shared_ptr, std::weak_ptr
#include <string>

// =============================================================================
// 1. MÔ HÌNH LỖI: HAI NODE TRỎ CHÉO NHAU BẰNG SHARED_PTR -> MEMORY LEAK!
// =============================================================================
struct LeakyNode {
    std::string name;
    std::shared_ptr<LeakyNode> partner; // Cả 2 chiều đều là shared_ptr -> Vòng lặp tham chiếu!

    explicit LeakyNode(std::string n) : name(std::move(n)) {
        std::cout << "  [+LeakyNode] Tao " << name << "\n";
    }
    ~LeakyNode() {
        std::cout << "  [-LeakyNode] Huy " << name << "\n"; // DÒNG NÀY SẼ KHÔNG BAO GIỜ CHẠY!
    }
};

// =============================================================================
// 2. MÔ HÌNH CHUẨN: DÙNG STD::WEAK_PTR PHÁ VỠ VÒNG LẶP THAM CHIẾU
// =============================================================================
struct SafeNode {
    std::string name;
    std::shared_ptr<SafeNode> next; // Chiều xuôi giữ quyền sở hữu (shared_ptr)
    std::weak_ptr<SafeNode>   prev; // Chiều ngược chỉ quan sát (weak_ptr - KHÔNG tăng use_count!)

    explicit SafeNode(std::string n) : name(std::move(n)) {
        std::cout << "  [+SafeNode] Tao " << name << "\n";
    }
    ~SafeNode() {
        std::cout << "  [-SafeNode] Da huy an toan: " << name << "\n";
    }

    void inspect_prev() const {
        // Dùng .lock() để kiểm tra đối tượng còn sống không và lấy ra shared_ptr tạm thời
        if (std::shared_ptr<SafeNode> p = prev.lock()) {
            std::cout << "  [weak_ptr.lock()] Node truoc cua " << name << " la: " << p->name
                      << " (use_count tam thoi = " << p.use_count() << ")\n";
        } else {
            std::cout << "  [weak_ptr.lock()] Node truoc da bi huy (expired)!\n";
        }
    }
};

int main() {
    std::cout << "=== 1. MINH HOA LOI CIRCULAR REFERENCE (KHONG BAO GIO GOI DESTRUCTOR!) ===\n";
    {
        auto bad1 = std::make_shared<LeakyNode>("Bad_A");
        auto bad2 = std::make_shared<LeakyNode>("Bad_B");
        bad1->partner = bad2;
        bad2->partner = bad1;
        std::cout << "  bad1.use_count() = " << bad1.use_count()
                  << " | bad2.use_count() = " << bad2.use_count() << "\n";
        // Phá vòng thủ công để chương trình mẫu không bị leak thật trên máy:
        bad1->partner.reset();
    }

    std::cout << "\n=== 2. GIAI PHAP CHUAN VOI STD::WEAK_PTR & PHUONG THUC .LOCK() ===\n";
    std::weak_ptr<SafeNode> observer;
    {
        auto n1 = std::make_shared<SafeNode>("Safe_1");
        auto n2 = std::make_shared<SafeNode>("Safe_2");

        n1->next = n2; // shared_ptr (n2.use_count = 2)
        n2->prev = n1; // weak_ptr   (n1.use_count VẪN BẰNG 1!)
        observer = n1;

        std::cout << "  n1.use_count() = " << n1.use_count()
                  << " | n2.use_count() = " << n2.use_count() << "\n";
        n2->inspect_prev();
        std::cout << "  --- Thoat khoi scope {} (Ca Safe_1 va Safe_2 deu tu dong duoc giai phong!) ---\n";
    }

    std::cout << "  Sau khi thoat scope, observer.expired()? "
              << (observer.expired() ? "True (Doi tuong da huy)" : "False") << "\n";

    return 0;
}

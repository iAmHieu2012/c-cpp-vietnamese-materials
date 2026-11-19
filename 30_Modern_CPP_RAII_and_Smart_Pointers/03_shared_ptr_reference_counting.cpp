#include <iostream>
#include <memory> // std::shared_ptr, std::make_shared
#include <string>
#include <vector>

class SharedDocument {
private:
    std::string title;
public:
    explicit SharedDocument(std::string t) : title(std::move(t)) {
        std::cout << "  [Constructor] Khoi tao tai lieu: '" << title << "'\n";
    }
    ~SharedDocument() {
        std::cout << "  [~Destructor] Tai lieu '" << title << "' da bi huy (khi use_count ve 0)!\n";
    }
    const std::string& get_title() const { return title; }
};

int main() {
    std::cout << "=== THEO DOI BO DEM THAM CHIEU (REFERENCE COUNTING) CUA STD::SHARED_PTR ===\n";
    // std::make_shared cấp phát gộp cả Control Block và SharedDocument trong 1 lần gọi Heap duy nhất!
    std::shared_ptr<SharedDocument> doc_main = std::make_shared<SharedDocument>("Kien_Truc_He_Thong.pdf");
    std::cout << "  1. Vua tao doc_main          -> use_count() = " << doc_main.use_count() << "\n";

    {
        std::shared_ptr<SharedDocument> editor_view = doc_main; // Copy -> Tăng bộ đếm lên 2
        std::cout << "  2. Copy sang editor_view     -> use_count() = " << doc_main.use_count() << "\n";

        std::vector<std::shared_ptr<SharedDocument>> recent_files;
        recent_files.push_back(doc_main);                       // Copy vào vector -> Tăng bộ đếm lên 3
        std::cout << "  3. Them vao vector           -> use_count() = " << doc_main.use_count() << "\n";
        std::cout << "  --- Chuan bi thoat khoi scope con {} (editor_view & recent_files bi huy) ---\n";
    }

    std::cout << "  4. Sau khi thoat scope con   -> use_count() = " << doc_main.use_count()
              << " (Tai lieu van song vi doc_main con giu!)\n";

    std::cout << "  5. Goi doc_main.reset()      -> ";
    doc_main.reset(); // Giảm bộ đếm từ 1 về 0 -> Kích hoạt ~SharedDocument() ngay lập tức!
    std::cout << "  6. Sau reset(), use_count()  = " << doc_main.use_count() << "\n";

    return 0;
}

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>

// =============================================================================
// BÀI 1: QUẢN LÝ ĐA HÌNH OOP BẰNG VECTOR<UNIQUE_PTR<SHAPE>> (0 DÒNG DELETE!)
// =============================================================================
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void print() const = 0;
};

class Circle : public Shape {
    double r;
public:
    explicit Circle(double radius) : r(radius) {}
    double area() const override { return 3.14159265 * r * r; }
    void print() const override {
        std::cout << "  - Circle (r=" << r << ") -> Area = " << area() << "\n";
    }
};

class Box : public Shape {
    double w, h;
public:
    Box(double width, double height) : w(width), h(height) {}
    double area() const override { return w * h; }
    void print() const override {
        std::cout << "  - Box (" << w << "x" << h << ") -> Area = " << area() << "\n";
    }
};

void exercise_1_polymorphic_unique_ptr() {
    std::vector<std::unique_ptr<Shape>> canvas;
    canvas.push_back(std::make_unique<Circle>(5.0));
    canvas.push_back(std::make_unique<Box>(4.0, 6.0));
    canvas.push_back(std::make_unique<Circle>(2.5));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n[Bai 1] Duyet vector<unique_ptr<Shape>> (Tu dong giai phong 100% khi ket thuc):\n";
    for (const auto &s : canvas) {
        s->print();
    }
}


// =============================================================================
// BÀI 2: CÂY NHỊ PHÂN TÌM KIẾM (BST) HIỆN ĐẠI DÙNG UNIQUE_PTR (RULE OF ZERO)
// Không cần tự viết hàm đệ quy destroy_tree() trong Destructor!
// =============================================================================
class ModernBST {
private:
    struct Node {
        int val;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        explicit Node(int v) : val(v), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    void insert_rec(std::unique_ptr<Node> &node, int val) {
        if (!node) {
            node = std::make_unique<Node>(val);
            return;
        }
        if (val < node->val)      insert_rec(node->left, val);
        else if (val > node->val) insert_rec(node->right, val);
    }

    void inorder_rec(const Node *node) const {
        if (!node) return;
        inorder_rec(node->left.get());
        std::cout << node->val << " ";
        inorder_rec(node->right.get());
    }

public:
    void insert(int val) { insert_rec(root, val); }
    void print_sorted() const {
        std::cout << "  In-order BST: ";
        inorder_rec(root.get());
        std::cout << "\n";
    }
};

void exercise_2_modern_bst_rule_of_zero() {
    ModernBST tree;
    for (int x : {40, 20, 60, 10, 30, 50, 70}) tree.insert(x);
    std::cout << "\n[Bai 2] Modern BST dung std::unique_ptr<Node> (0 Destructor thu cong):\n";
    tree.print_sorted();
}


// =============================================================================
// BÀI 3: MÔ HÌNH PHÒNG BAN - NHÂN VIÊN HAI CHIỀU (SHARED_PTR & WEAK_PTR)
// =============================================================================
struct Department;

struct Worker {
    std::string name;
    std::weak_ptr<Department> dept; // Dùng weak_ptr để không tạo chu trình với Department
    explicit Worker(std::string n) : name(std::move(n)) {}
};

struct Department {
    std::string title;
    std::vector<std::shared_ptr<Worker>> members;
    explicit Department(std::string t) : title(std::move(t)) {}
};

void exercise_3_parent_child_weak_ptr() {
    auto ai_dept = std::make_shared<Department>("Phong Nghien Cuu AI");
    auto w1 = std::make_shared<Worker>("Nguyen Van An");
    auto w2 = std::make_shared<Worker>("Tran Thi Binh");

    w1->dept = ai_dept;
    w2->dept = ai_dept;
    ai_dept->members.push_back(w1);
    ai_dept->members.push_back(w2);

    std::cout << "\n[Bai 3] Quan he 2 chieu Department <-> Worker (khong bi Circular Leak):\n";
    std::cout << "  ai_dept.use_count() = " << ai_dept.use_count() << " (Khong bi tang boi weak_ptr cua Worker)\n";
    if (auto d = w1->dept.lock()) {
        std::cout << "  Nhan vien '" << w1->name << "' thuoc: " << d->title << "\n";
    }
}


// =============================================================================
// BÀI 4: BỘ NHỚ ĐỆM TÀI NGUYÊN TỰ DỌN RÁC (WEAK_PTR RESOURCE CACHE)
// =============================================================================
struct HeavyAsset {
    std::string path;
    explicit HeavyAsset(std::string p) : path(std::move(p)) {
        std::cout << "    [Disk I/O] Nap moi tai nguyen tu o cung: " << path << "\n";
    }
    ~HeavyAsset() {
        std::cout << "    [Free RAM] Giai phong tai nguyen khong con ai dung: " << path << "\n";
    }
};

class AssetCache {
private:
    std::unordered_map<std::string, std::weak_ptr<HeavyAsset>> cache;

public:
    std::shared_ptr<HeavyAsset> load(const std::string &path) {
        if (auto sp = cache[path].lock()) {
            std::cout << "    [Cache Hit] Tai su dung tai nguyen dang co tren RAM: " << path << "\n";
            return sp;
        }
        auto sp = std::make_shared<HeavyAsset>(path);
        cache[path] = sp;
        return sp;
    }
};

void exercise_4_weak_ptr_asset_cache() {
    std::cout << "\n[Bai 4] Kiem thu AssetCache bang std::weak_ptr:\n";
    AssetCache manager;
    {
        auto tex1 = manager.load("hero_skin.png"); // Nạp từ đĩa
        auto tex2 = manager.load("hero_skin.png"); // Cache Hit!
        std::cout << "    So doi tuong dang cung dung hero_skin.png = " << tex1.use_count() << "\n";
    } // Ra khỏi scope -> cả tex1 và tex2 hủy -> HeavyAsset tự giải phóng khỏi RAM!

    std::cout << "  -> Goi load() lai sau khi tat ca user cu da huy:\n";
    auto tex3 = manager.load("hero_skin.png"); // Tự động nạp mới lại từ đĩa!
}


// =============================================================================
// BÀI 5: CHỐNG RÒ RỈ TÀI NGUYÊN KHI CÓ NGOẠI LỆ (EXCEPTION SAFETY VỚI RAII)
// =============================================================================
void exercise_5_exception_safety_raii() {
    std::cout << "\n[Bai 5] Dam bao thu hoi Heap 100% ngay ca khi ham nem Exception:\n";
    try {
        auto safe_buffer = std::make_unique<int[]>(10000);
        safe_buffer[0] = 42;
        std::cout << "  Da cap phat 10,000 ints qua unique_ptr. Gia lap nem Exception...\n";
        throw std::runtime_error("Loi ket noi mang dot ngot!");
    } catch (const std::exception &e) {
        std::cout << "  [Da bat Exception]: " << e.what()
                  << "\n  -> Nho Stack Unwinding, unique_ptr da TU DONG delete[] 10,000 ints an toan!\n";
    }
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 6.3 (RAII & SMART POINTERS) ==========\n";
        std::cout << "1. Quan ly Da hinh OOP voi vector<unique_ptr<Shape>>\n";
        std::cout << "2. Cay BST hien dai dung unique_ptr (Rule of Zero)\n";
        std::cout << "3. Quan he 2 chieu Department <-> Worker (shared_ptr + weak_ptr)\n";
        std::cout << "4. Bo nho dem tai nguyen tu don rac (weak_ptr AssetCache)\n";
        std::cout << "5. An toan bo nho tuyet doi khi nem Exception (Stack Unwinding)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_polymorphic_unique_ptr(); break;
            case 2: exercise_2_modern_bst_rule_of_zero(); break;
            case 3: exercise_3_parent_child_weak_ptr(); break;
            case 4: exercise_4_weak_ptr_asset_cache(); break;
            case 5: exercise_5_exception_safety_raii(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

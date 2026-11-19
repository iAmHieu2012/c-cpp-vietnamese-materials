#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// =============================================================================
// FLYWEIGHT PATTERN (CHIA SẺ DỮ LIỆU NẶNG DÙNG CHUNG ĐỂ TIẾT KIỆM RAM)
// =============================================================================

// Intrinsic State (Dữ liệu nặng bất biến dùng chung: Loài cây, Màu lá, 3D Mesh Texture)
struct TreeTypeFlyweight {
    std::string species;
    std::string color;
    std::string texture_mesh;

    TreeTypeFlyweight(std::string s, std::string c, std::string t)
        : species(std::move(s)), color(std::move(c)), texture_mesh(std::move(t)) {
        std::cout << "  [Flyweight Factory] Nap moi Texture nang vao RAM cho loai: " << species << "\n";
    }
};

class TreeFactory {
private:
    std::unordered_map<std::string, std::shared_ptr<TreeTypeFlyweight>> pool;
public:
    std::shared_ptr<TreeTypeFlyweight> get_tree_type(const std::string &species,
                                                     const std::string &color,
                                                     const std::string &mesh) {
        auto it = pool.find(species);
        if (it != pool.end()) return it->second;
        auto fw = std::make_shared<TreeTypeFlyweight>(species, color, mesh);
        pool[species] = fw;
        return fw;
    }
    std::size_t unique_flyweights() const { return pool.size(); }
};

// Extrinsic State (Dữ liệu nhẹ riêng biệt của từng cây: Tọa độ x, y)
struct TreeInstance {
    int x, y;
    std::shared_ptr<TreeTypeFlyweight> type; // Dùng chung con trỏ tới Flyweight!
};

int main() {
    std::cout << "=== FLYWEIGHT PATTERN (1,000 CAY CHUNG 2 BAN TEXTURE TREN RAM) ===\n";
    TreeFactory factory;
    std::vector<TreeInstance> forest;
    forest.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        auto type = (i % 2 == 0)
            ? factory.get_tree_type("Thong_Da_Lat", "Xanh_Dam", "pine_mesh_4k.obj")
            : factory.get_tree_type("Phong_La_Do",  "Do_Cam",   "maple_mesh_4k.obj");
        forest.push_back({i * 5, i * 10, type});
    }

    std::cout << "  -> Da trong " << forest.size() << " cay trong rung nhung chi ton "
              << factory.unique_flyweights() << " doi tuong Flyweight trong RAM!\n";

    return 0;
}

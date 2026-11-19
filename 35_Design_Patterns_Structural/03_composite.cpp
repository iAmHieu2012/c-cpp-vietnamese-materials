#include <iostream>
#include <vector>
#include <string>
#include <memory>

// =============================================================================
// COMPOSITE PATTERN (CẤU TRÚC CÂY PHÂN CẤP: FILE & FOLDER)
// =============================================================================
class IFileSystemNode {
public:
    virtual ~IFileSystemNode() = default;
    virtual int get_size_kb() const = 0;
    virtual void print(int indent = 0) const = 0;
};

// Leaf (Nút lá đơn lẻ)
class FileItem : public IFileSystemNode {
private:
    std::string name;
    int size_kb;
public:
    FileItem(std::string n, int sz) : name(std::move(n)), size_kb(sz) {}
    int get_size_kb() const override { return size_kb; }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "- [File] " << name << " (" << size_kb << " KB)\n";
    }
};

// Composite (Nút nhánh chứa danh sách các IFileSystemNode con)
class FolderItem : public IFileSystemNode {
private:
    std::string name;
    std::vector<std::unique_ptr<IFileSystemNode>> children;
public:
    explicit FolderItem(std::string n) : name(std::move(n)) {}

    void add(std::unique_ptr<IFileSystemNode> node) {
        children.push_back(std::move(node));
    }

    int get_size_kb() const override {
        int total = 0;
        for (const auto &child : children) total += child->get_size_kb();
        return total;
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "+ [Folder] " << name
                  << " (Tong: " << get_size_kb() << " KB)\n";
        for (const auto &child : children) child->print(indent + 4);
    }
};

int main() {
    std::cout << "=== COMPOSITE PATTERN (CAY THU MUC FILE / FOLDER) ===\n";
    auto root = std::make_unique<FolderItem>("project_root");
    root->add(std::make_unique<FileItem>("README.md", 15));

    auto src = std::make_unique<FolderItem>("src");
    src->add(std::make_unique<FileItem>("main.cpp", 45));
    src->add(std::make_unique<FileItem>("engine.cpp", 120));
    root->add(std::move(src));

    root->print(2);

    return 0;
}

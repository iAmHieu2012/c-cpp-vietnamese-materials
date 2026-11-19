// ============================================================================
// File: 03_iterator.cpp
// Bài 7.4: Behavioral Design Patterns - Iterator Pattern (Bộ lặp)
// Tham khảo: https://refactoring.guru/design-patterns/iterator
// ============================================================================

#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

// ============================================================================
// 1. CẤU TRÚC PHẦN TỬ BÊN TRONG (Node cây sơ đồ tổ chức công ty)
// ============================================================================
struct EmployeeNode {
    std::string name;
    std::string title;
    std::vector<std::shared_ptr<EmployeeNode>> subordinates;

    EmployeeNode(std::string n, std::string t)
        : name(std::move(n)), title(std::move(t)) {}

    void addSubordinate(std::shared_ptr<EmployeeNode> emp) {
        subordinates.push_back(std::move(emp));
    }
};

// ============================================================================
// 2. ITERATOR INTERFACE (Giao diện bộ lặp trừu tượng)
// ============================================================================
class IOrgIterator {
public:
    virtual ~IOrgIterator() = default;
    virtual bool hasNext() const = 0;
    virtual std::shared_ptr<EmployeeNode> next() = 0;
};

// ============================================================================
// 3. CONCRETE ITERATORS (Duyệt theo chiều rộng BFS vs chiều sâu DFS)
// ============================================================================

// Bộ lặp 1: Duyệt theo cấp bậc từ cao xuống thấp (Breadth-First Search - BFS)
class LevelOrderOrgIterator : public IOrgIterator {
    std::queue<std::shared_ptr<EmployeeNode>> queue_;

public:
    explicit LevelOrderOrgIterator(const std::shared_ptr<EmployeeNode> &root) {
        if (root) queue_.push(root);
    }

    bool hasNext() const override {
        return !queue_.empty();
    }

    std::shared_ptr<EmployeeNode> next() override {
        auto current = queue_.front();
        queue_.pop();
        for (const auto &sub : current->subordinates) {
            queue_.push(sub);
        }
        return current;
    }
};

// Bộ lặp 2: Duyệt đi sâu vào từng phòng ban (Depth-First PreOrder - DFS)
class DepthFirstOrgIterator : public IOrgIterator {
    std::stack<std::shared_ptr<EmployeeNode>> stack_;

public:
    explicit DepthFirstOrgIterator(const std::shared_ptr<EmployeeNode> &root) {
        if (root) stack_.push(root);
    }

    bool hasNext() const override {
        return !stack_.empty();
    }

    std::shared_ptr<EmployeeNode> next() override {
        auto current = stack_.top();
        stack_.pop();
        // Đẩy ngược từ phải sang trái để nhánh con bên trái được lấy ra trước
        for (auto it = current->subordinates.rbegin(); it != current->subordinates.rend(); ++it) {
            stack_.push(*it);
        }
        return current;
    }
};

// ============================================================================
// 4. AGGREGATE / COLLECTION (Cây tổ chức cung cấp các Factory tạo Iterator)
// ============================================================================
class OrganizationTree {
    std::shared_ptr<EmployeeNode> ceo_;

public:
    explicit OrganizationTree(std::shared_ptr<EmployeeNode> ceo)
        : ceo_(std::move(ceo)) {}

    std::unique_ptr<IOrgIterator> createLevelOrderIterator() const {
        return std::make_unique<LevelOrderOrgIterator>(ceo_);
    }

    std::unique_ptr<IOrgIterator> createDepthFirstIterator() const {
        return std::make_unique<DepthFirstOrgIterator>(ceo_);
    }
};

// Client sử dụng giao diện IOrgIterator mà không cần biết cây lưu trữ thế nào
void printHierarchy(const std::string &label, IOrgIterator &it) {
    std::cout << "--- " << label << " ---\n";
    int step = 1;
    while (it.hasNext()) {
        auto emp = it.next();
        std::cout << "  " << step++ << ". [" << emp->title << "] " << emp->name << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== ITERATOR PATTERN (DUYET CAY TO CHUC BFS & DFS) ===\n\n";

    auto ceo = std::make_shared<EmployeeNode>("Nguyen Van A", "CEO");
    auto cto = std::make_shared<EmployeeNode>("Tran Thi B", "CTO");
    auto cfo = std::make_shared<EmployeeNode>("Le Van C", "CFO");

    cto->addSubordinate(std::make_shared<EmployeeNode>("Pham Minh D", "Lead Backend Eng"));
    cto->addSubordinate(std::make_shared<EmployeeNode>("Hoang Anh E", "Lead AI Eng"));
    cfo->addSubordinate(std::make_shared<EmployeeNode>("Vu Thi F", "Chief Accountant"));

    ceo->addSubordinate(cto);
    ceo->addSubordinate(cfo);

    OrganizationTree company(ceo);

    auto bfsIt = company.createLevelOrderIterator();
    printHierarchy("Duyet theo Cap bac (BFS Level-Order)", *bfsIt);

    auto dfsIt = company.createDepthFirstIterator();
    printHierarchy("Duyet theo Tung khoi/Phong ban (DFS Pre-Order)", *dfsIt);

    return 0;
}

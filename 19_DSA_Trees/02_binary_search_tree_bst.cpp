// ============================================================================
// File: 02_binary_search_tree_bst.cpp
// Bài 4.4: Cây Nhị Phân Tìm Kiếm (BST: Insert, Search, Delete 3 TH, Validate, LCA)
// ============================================================================

#include <climits>
#include <iostream>
#include <queue>

class BinarySearchTree {
    struct Node {
        int val;
        Node *left;
        Node *right;
        explicit Node(int v) : val(v), left(nullptr), right(nullptr) {}
    };

    Node *root_ = nullptr;

    Node *insertRec(Node *node, int val) {
        if (!node) return new Node(val);
        if (val < node->val) node->left = insertRec(node->left, val);
        else if (val > node->val) node->right = insertRec(node->right, val);
        return node;
    }

    bool searchRec(const Node *node, int val) const {
        if (!node) return false;
        if (val == node->val) return true;
        return (val < node->val) ? searchRec(node->left, val)
                                 : searchRec(node->right, val);
    }

    Node *findMin(Node *node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    Node *removeRec(Node *node, int val) {
        if (!node) return nullptr;
        if (val < node->val) {
            node->left = removeRec(node->left, val);
        } else if (val > node->val) {
            node->right = removeRec(node->right, val);
        } else {
            // TH1 & TH2: Node lá hoặc chỉ có 1 con
            if (!node->left) {
                Node *r = node->right;
                delete node;
                return r;
            }
            if (!node->right) {
                Node *l = node->left;
                delete node;
                return l;
            }
            // TH3: Node có đủ 2 con -> Thay bằng In-order Successor
            Node *succ = findMin(node->right);
            node->val = succ->val;
            node->right = removeRec(node->right, succ->val);
        }
        return node;
    }

    void inorderRec(const Node *node) const {
        if (!node) return;
        inorderRec(node->left);
        std::cout << node->val << " ";
        inorderRec(node->right);
    }

    bool validateRec(const Node *node, long long minB, long long maxB) const {
        if (!node) return true;
        if (node->val <= minB || node->val >= maxB) return false;
        return validateRec(node->left, minB, node->val) &&
               validateRec(node->right, node->val, maxB);
    }

    void destroyRec(Node *node) {
        if (!node) return;
        destroyRec(node->left);
        destroyRec(node->right);
        delete node;
    }

public:
    BinarySearchTree() = default;
    ~BinarySearchTree() { destroyRec(root_); }

    BinarySearchTree(const BinarySearchTree &) = delete;
    BinarySearchTree &operator=(const BinarySearchTree &) = delete;

    void insert(int val) { root_ = insertRec(root_, val); }
    bool search(int val) const { return searchRec(root_, val); }
    void remove(int val) { root_ = removeRec(root_, val); }
    bool isValidBST() const { return validateRec(root_, LLONG_MIN, LLONG_MAX); }

    int lowestCommonAncestor(int p, int q) const {
        const Node *curr = root_;
        while (curr) {
            if (p < curr->val && q < curr->val) curr = curr->left;
            else if (p > curr->val && q > curr->val) curr = curr->right;
            else return curr->val;
        }
        return -1;
    }

    void printInorder() const {
        std::cout << "  In-order (Sorted): ";
        inorderRec(root_);
        std::cout << "\n";
    }
};

int main() {
    std::cout << "=== BINARY SEARCH TREE (BST) ===\n";
    BinarySearchTree bst;
    for (int v : {50, 30, 70, 20, 40, 60, 80}) bst.insert(v);

    bst.printInorder();
    std::cout << "  Validate BST  : " << (bst.isValidBST() ? "Hop le" : "Sai") << "\n";
    std::cout << "  Search(40)    : " << (bst.search(40) ? "Tim thay" : "Khong thay") << "\n";
    std::cout << "  LCA(20, 40)   : " << bst.lowestCommonAncestor(20, 40) << "\n";
    std::cout << "  LCA(20, 80)   : " << bst.lowestCommonAncestor(20, 80) << "\n";

    std::cout << "\n--- Xoa goc 50 (Node co du 2 con) ---\n";
    bst.remove(50);
    bst.printInorder();

    return 0;
}

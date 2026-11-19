// ============================================================================
// File: 04_avl_self_balancing_tree.cpp
// Bài 4.4: Cây Nhị Phân Tự Cân Bằng AVL Tree (4 Phép Xoay LL, RR, LR, RL: Insert & Delete)
// ============================================================================

#include <algorithm>
#include <iostream>

class AVLTree {
    struct Node {
        int val;
        int height;
        Node *left;
        Node *right;
        explicit Node(int v) : val(v), height(1), left(nullptr), right(nullptr) {}
    };

    Node *root_ = nullptr;

    int height(const Node *n) const { return n ? n->height : 0; }
    int balanceFactor(const Node *n) const {
        return n ? height(n->left) - height(n->right) : 0;
    }

    void updateHeight(Node *n) {
        if (n) n->height = 1 + std::max(height(n->left), height(n->right));
    }

    // Phép quay phải đơn (Single Right Rotation - Xử lý lệch Trái-Trái LL)
    Node *rotateRight(Node *y) {
        Node *x = y->left;
        Node *t2 = x->right;
        x->right = y;
        y->left = t2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Phép quay trái đơn (Single Left Rotation - Xử lý lệch Phải-Phải RR)
    Node *rotateLeft(Node *x) {
        Node *y = x->right;
        Node *t2 = y->left;
        y->left = x;
        x->right = t2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node *rebalance(Node *node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        // 1. Lệch Trái (LL hoặc LR)
        if (bf > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left); // LR -> LL
            }
            return rotateRight(node);
        }
        // 2. Lệch Phải (RR hoặc RL)
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right); // RL -> RR
            }
            return rotateLeft(node);
        }
        return node;
    }

    Node *insertRec(Node *node, int val) {
        if (!node) return new Node(val);
        if (val < node->val) node->left = insertRec(node->left, val);
        else if (val > node->val) node->right = insertRec(node->right, val);
        else return node;

        return rebalance(node);
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
            if (!node->left || !node->right) {
                Node *child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            Node *succ = findMin(node->right);
            node->val = succ->val;
            node->right = removeRec(node->right, succ->val);
        }
        return rebalance(node);
    }

    void preorderRec(const Node *n) const {
        if (!n) return;
        std::cout << n->val << "(h=" << n->height << ",bf=" << balanceFactor(n) << ") ";
        preorderRec(n->left);
        preorderRec(n->right);
    }

    void destroyRec(Node *n) {
        if (!n) return;
        destroyRec(n->left);
        destroyRec(n->right);
        delete n;
    }

public:
    AVLTree() = default;
    ~AVLTree() { destroyRec(root_); }

    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;

    void insert(int val) { root_ = insertRec(root_, val); }
    void remove(int val) { root_ = removeRec(root_, val); }
    int getRootHeight() const { return height(root_); }

    void printPreorder() const {
        std::cout << "  Pre-order: ";
        preorderRec(root_);
        std::cout << "\n";
    }
};

int main() {
    std::cout << "=== CAY TU CAN BANG AVL TREE (INSERT & DELETE O(log N)) ===\n\n";

    AVLTree avl;
    for (int x : {10, 20, 30, 40, 50, 60, 70}) {
        avl.insert(x);
    }
    std::cout << "  Chieu cao cay AVL sau khi chen 7 so tang dan = "
              << avl.getRootHeight() << " (BST thuong se la 7!)\n";
    avl.printPreorder();

    std::cout << "\n--- Xoa 40 (Node goc hien tai) va tu can bang lai ---\n";
    avl.remove(40);
    avl.printPreorder();

    return 0;
}

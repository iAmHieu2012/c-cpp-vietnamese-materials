// ============================================================================
// File: 01_binary_tree_traversals.cpp
// Bài 4.4: Cây Nhị Phân (Binary Tree Traversals: Đệ quy, Khử đệ quy Stack, BFS)
// ============================================================================

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    explicit TreeNode(int v, TreeNode *l = nullptr, TreeNode *r = nullptr)
        : val(v), left(l), right(r) {}
};

void destroyTree(TreeNode *root) {
    if (!root) return;
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

// ============================================================================
// 1. BA PHÉP DUYỆT CHIỀU SÂU ĐỆ QUY (PRE-ORDER, IN-ORDER, POST-ORDER)
// ============================================================================
void preorderRec(const TreeNode *node) {
    if (!node) return;
    std::cout << node->val << " ";
    preorderRec(node->left);
    preorderRec(node->right);
}

void inorderRec(const TreeNode *node) {
    if (!node) return;
    inorderRec(node->left);
    std::cout << node->val << " ";
    inorderRec(node->right);
}

void postorderRec(const TreeNode *node) {
    if (!node) return;
    postorderRec(node->left);
    postorderRec(node->right);
    std::cout << node->val << " ";
}

// ============================================================================
// 2. DUYỆT IN-ORDER KHỬ ĐỆ QUY BẰNG STACK (ITERATIVE IN-ORDER)
// ============================================================================
void inorderIterative(const TreeNode *root) {
    std::stack<const TreeNode *> st;
    const TreeNode *curr = root;

    std::cout << "  In-order (Iterative Stack): ";
    while (curr != nullptr || !st.empty()) {
        while (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top();
        st.pop();
        std::cout << curr->val << " ";
        curr = curr->right;
    }
    std::cout << "\n";
}

// ============================================================================
// 3. DUYỆT THEO TỪNG TẦNG (BFS LEVEL-ORDER BY LEVEL)
// ============================================================================
void printLevelOrderByLevel(const TreeNode *root) {
    if (!root) return;
    std::queue<const TreeNode *> q;
    q.push(root);
    int level = 0;

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::cout << "    Tang " << level++ << ": ";
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode *curr = q.front();
            q.pop();
            std::cout << curr->val << " ";
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        std::cout << "\n";
    }
}

// ============================================================================
// 4. CHIỀU CAO & ĐƯỜNG KÍNH CÂY NHỊ PHÂN (DIAMETER OF BINARY TREE - O(N))
// ============================================================================
int heightAndDiameter(const TreeNode *node, int &maxDiameter) {
    if (!node) return 0;
    int leftH = heightAndDiameter(node->left, maxDiameter);
    int rightH = heightAndDiameter(node->right, maxDiameter);
    maxDiameter = std::max(maxDiameter, leftH + rightH);
    return 1 + std::max(leftH, rightH);
}

int main() {
    /*
              1
            /   \
           2     3
          / \     \
         4   5     6
    */
    TreeNode *root = new TreeNode(
        1,
        new TreeNode(2, new TreeNode(4), new TreeNode(5)),
        new TreeNode(3, nullptr, new TreeNode(6)));

    std::cout << "=== 1. DUYET CAY NHI PHAN (DFS & BFS) ===\n";
    std::cout << "  Pre-order  (NLR): "; preorderRec(root);  std::cout << "\n";
    std::cout << "  In-order   (LNR): "; inorderRec(root);   std::cout << "\n";
    std::cout << "  Post-order (LRN): "; postorderRec(root); std::cout << "\n";
    inorderIterative(root);

    std::cout << "\n=== 2. DUYET THEO TUNG TANG (BFS LEVEL-ORDER) ===\n";
    printLevelOrderByLevel(root);

    std::cout << "\n=== 3. CHIEU CAO & DUONG KINH CAY (DIAMETER) ===\n";
    int diameter = 0;
    int height = heightAndDiameter(root, diameter);
    std::cout << "  Chieu cao cay (Max Depth) = " << height << "\n";
    std::cout << "  Duong kinh cay (Diameter) = " << diameter << " canh\n";

    destroyTree(root);
    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <utility>

// =============================================================================
// CẤU TRÚC HỖ TRỢ: TREENODE & MIN-HEAP TỰ CÀI ĐẶT
// =============================================================================
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    explicit TreeNode(int v, TreeNode *l = nullptr, TreeNode *r = nullptr)
        : val(v), left(l), right(r) {}
};

void free_tree(TreeNode *root) {
    if (root == nullptr) return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}

void print_inorder(TreeNode *root) {
    if (root == nullptr) return;
    print_inorder(root->left);
    std::cout << root->val << " ";
    print_inorder(root->right);
}

// Min-Heap kích thước cố định phục vụ Bài 4 (Kth Largest)
class FixedMinHeap {
private:
    int *data;
    int size;
    int capacity;

    void sift_up(int idx) {
        while (idx > 0) {
            int p = (idx - 1) / 2;
            if (data[idx] < data[p]) { std::swap(data[idx], data[p]); idx = p; }
            else break;
        }
    }

    void sift_down(int idx) {
        while (true) {
            int l = 2 * idx + 1, r = 2 * idx + 2, smallest = idx;
            if (l < size && data[l] < data[smallest]) smallest = l;
            if (r < size && data[r] < data[smallest]) smallest = r;
            if (smallest != idx) { std::swap(data[idx], data[smallest]); idx = smallest; }
            else break;
        }
    }

public:
    explicit FixedMinHeap(int cap) : data(new int[cap]), size(0), capacity(cap) {}
    ~FixedMinHeap() { delete[] data; }
    FixedMinHeap(const FixedMinHeap&) = delete;
    FixedMinHeap& operator=(const FixedMinHeap&) = delete;

    int top() const { return data[0]; }

    void push(int val) {
        if (size < capacity) {
            data[size] = val;
            sift_up(size++);
        } else if (val > data[0]) {
            data[0] = val;
            sift_down(0);
        }
    }
};


// =============================================================================
// BÀI 1: TÍNH CHIỀU CAO (MAX DEPTH) & ĐẢO NGƯỢC CÂY NHỊ PHÂN (INVERT TREE)
// =============================================================================
int max_depth(TreeNode *root) {
    if (root == nullptr) return 0;
    return 1 + std::max(max_depth(root->left), max_depth(root->right));
}

TreeNode* invert_tree(TreeNode *root) {
    if (root == nullptr) return nullptr;
    std::swap(root->left, root->right);
    invert_tree(root->left);
    invert_tree(root->right);
    return root;
}


// =============================================================================
// BÀI 2: KIỂM TRA CÂY NHỊ PHÂN CÓ PHẢI BST HỢP LỆ (VALIDATE BST - O(N))
// =============================================================================
bool is_valid_bst_rec(TreeNode *node, long long min_bound, long long max_bound) {
    if (node == nullptr) return true;
    if (node->val <= min_bound || node->val >= max_bound) return false;
    return is_valid_bst_rec(node->left,  min_bound, node->val) &&
           is_valid_bst_rec(node->right, node->val, max_bound);
}

bool is_valid_bst(TreeNode *root) {
    return is_valid_bst_rec(root, LLONG_MIN, LLONG_MAX);
}


// =============================================================================
// BÀI 3: TÌM TỔ TIÊN CHUNG GẦN NHẤT TRONG BST (LCA - O(H) TIME, O(1) SPACE)
// =============================================================================
TreeNode* lowest_common_ancestor_bst(TreeNode *root, int p, int q) {
    TreeNode *curr = root;
    while (curr != nullptr) {
        if (p < curr->val && q < curr->val)      curr = curr->left;
        else if (p > curr->val && q > curr->val) curr = curr->right;
        else                                     return curr;
    }
    return nullptr;
}


// =============================================================================
// BÀI 4: TÌM PHẦN TỬ LỚN THỨ K BẰNG MIN-HEAP KÍCH THƯỚC K - O(N log K)
// =============================================================================
int kth_largest_min_heap(const int a[], int n, int k) {
    FixedMinHeap heap(k);
    for (int i = 0; i < n; ++i) {
        heap.push(a[i]);
    }
    return heap.top();
}


// =============================================================================
// BÀI 5: HỆ THỐNG GỢI Ý TỪ KHÓA TỰ ĐỘNG (AUTOCOMPLETE) BẰNG CÂY TIỀN TỐ TRIE
// =============================================================================
class AutocompleteTrie {
private:
    struct Node {
        Node *children[26];
        bool is_word;
        Node() : is_word(false) {
            for (int i = 0; i < 26; ++i) children[i] = nullptr;
        }
    };

    Node *root;

    void destroy(Node *n) {
        if (!n) return;
        for (int i = 0; i < 26; ++i) destroy(n->children[i]);
        delete n;
    }

    void collect_words(const Node *n, std::string &curr, std::vector<std::string> &out) const {
        if (!n) return;
        if (n->is_word) out.push_back(curr);
        for (int i = 0; i < 26; ++i) {
            if (n->children[i]) {
                curr.push_back(static_cast<char>('a' + i));
                collect_words(n->children[i], curr, out);
                curr.pop_back();
            }
        }
    }

public:
    AutocompleteTrie() : root(new Node()) {}
    ~AutocompleteTrie() { destroy(root); }

    AutocompleteTrie(const AutocompleteTrie&) = delete;
    AutocompleteTrie& operator=(const AutocompleteTrie&) = delete;

    void insert(const std::string &w) {
        Node *curr = root;
        for (char c : w) {
            if (c < 'a' || c > 'z') continue;
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new Node();
            curr = curr->children[idx];
        }
        curr->is_word = true;
    }

    std::vector<std::string> suggest(const std::string &prefix) const {
        const Node *curr = root;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') return {};
            int idx = c - 'a';
            if (!curr->children[idx]) return {};
            curr = curr->children[idx];
        }
        std::vector<std::string> results;
        std::string buf = prefix;
        collect_words(curr, buf, results);
        return results;
    }
};


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP CẤU TRÚC CÂY
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.4 (TREES, HEAP & TRIE) ==========\n";
        std::cout << "1. Tinh chieu cao & Dao nguoc cay nhi phan (Max Depth & Invert)\n";
        std::cout << "2. Kiem tra cay BST hop le (Validate BST - O(N))\n";
        std::cout << "3. Tim To tien chung gan nhat trong BST (LCA - O(H))\n";
        std::cout << "4. Tim phan tu lon thu K bang Min-Heap kich thuoc K - O(N log K)\n";
        std::cout << "5. Goi y tu khoa tu dong (Autocomplete) bang Cay tien to Trie\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                TreeNode *root = new TreeNode(4,
                    new TreeNode(2, new TreeNode(1), new TreeNode(3)),
                    new TreeNode(7, new TreeNode(6), new TreeNode(9))
                );
                std::cout << "\n[Bai 1] Chieu cao cay = " << max_depth(root) << "\n";
                std::cout << "  In-order truoc khi dao: "; print_inorder(root); std::cout << "\n";
                invert_tree(root);
                std::cout << "  In-order sau khi dao  : "; print_inorder(root); std::cout << "\n";
                free_tree(root);
                break;
            }

            case 2: {
                TreeNode *valid = new TreeNode(10,
                    new TreeNode(5),
                    new TreeNode(15, new TreeNode(12), new TreeNode(20))
                );
                TreeNode *invalid = new TreeNode(10,
                    new TreeNode(5),
                    new TreeNode(15, new TreeNode(6), new TreeNode(20))
                );
                std::cout << "\n[Bai 2]\n";
                std::cout << "  Cay 1 (hop le)  -> " << (is_valid_bst(valid)   ? "Valid BST" : "Invalid BST") << "\n";
                std::cout << "  Cay 2 (cay bay) -> " << (is_valid_bst(invalid) ? "Valid BST" : "Invalid BST") << "\n";
                free_tree(valid);
                free_tree(invalid);
                break;
            }

            case 3: {
                TreeNode *root = new TreeNode(20,
                    new TreeNode(10, new TreeNode(5), new TreeNode(15)),
                    new TreeNode(30, new TreeNode(25), new TreeNode(35))
                );
                TreeNode *lca1 = lowest_common_ancestor_bst(root, 5, 15);
                TreeNode *lca2 = lowest_common_ancestor_bst(root, 5, 30);
                std::cout << "\n[Bai 3] BST goc 20:\n";
                std::cout << "  LCA(5, 15) = " << (lca1 ? lca1->val : -1) << "\n";
                std::cout << "  LCA(5, 30) = " << (lca2 ? lca2->val : -1) << "\n";
                free_tree(root);
                break;
            }

            case 4: {
                int a[] = {3, 2, 3, 1, 2, 4, 5, 5, 6};
                int n = sizeof(a) / sizeof(a[0]);
                int k = 4;
                std::cout << "\n[Bai 4] Mang: {3, 2, 3, 1, 2, 4, 5, 5, 6} | K = " << k << "\n";
                std::cout << "  -> Phan tu lon thu " << k << " = " << kth_largest_min_heap(a, n, k) << "\n";
                break;
            }

            case 5: {
                AutocompleteTrie dict;
                for (const char *w : {"cpp", "cmake", "compiler", "class", "const", "algorithm", "array"}) {
                    dict.insert(w);
                }
                std::cout << "\n[Bai 5] Goi y tu khoa bat dau bang tien to \"c\":\n  ";
                for (const auto &s : dict.suggest("c")) std::cout << s << "  ";
                std::cout << "\n  Goi y tu khoa bat dau bang tien to \"co\":\n  ";
                for (const auto &s : dict.suggest("co")) std::cout << s << "  ";
                std::cout << "\n";
                break;
            }

            case 0:
                std::cout << "-> Tam biet!\n";
                break;

            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

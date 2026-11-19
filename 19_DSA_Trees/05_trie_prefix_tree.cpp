// ============================================================================
// File: 05_trie_prefix_tree.cpp
// Bài 4.4: Cây Tiền Tố (Trie / Prefix Tree: Insert, Search, Prefix, Autocomplete, Delete)
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

class Trie {
    struct TrieNode {
        TrieNode *children[26];
        bool isEndOfWord = false;

        TrieNode() {
            for (int i = 0; i < 26; ++i) children[i] = nullptr;
        }
    };

    TrieNode *root_;

    void destroyRec(TrieNode *node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i) destroyRec(node->children[i]);
        delete node;
    }

    const TrieNode *findPrefixNode(const std::string &prefix) const {
        const TrieNode *curr = root_;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') return nullptr;
            int idx = c - 'a';
            if (!curr->children[idx]) return nullptr;
            curr = curr->children[idx];
        }
        return curr;
    }

    void collectWords(const TrieNode *node, std::string &currentWord,
                      std::vector<std::string> &results) const {
        if (!node) return;
        if (node->isEndOfWord) results.push_back(currentWord);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                currentWord.push_back(static_cast<char>('a' + i));
                collectWords(node->children[i], currentWord, results);
                currentWord.pop_back();
            }
        }
    }

    bool hasNoChildren(const TrieNode *node) const {
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) return false;
        }
        return true;
    }

    bool removeRec(TrieNode *node, const std::string &word, size_t depth) {
        if (!node) return false;
        if (depth == word.size()) {
            if (!node->isEndOfWord) return false;
            node->isEndOfWord = false;
            return hasNoChildren(node); // Xóa node vật lý nếu không còn nhánh con nào
        }
        int idx = word[depth] - 'a';
        if (removeRec(node->children[idx], word, depth + 1)) {
            delete node->children[idx];
            node->children[idx] = nullptr;
            return !node->isEndOfWord && hasNoChildren(node);
        }
        return false;
    }

public:
    Trie() : root_(new TrieNode()) {}
    ~Trie() { destroyRec(root_); }

    Trie(const Trie &) = delete;
    Trie &operator=(const Trie &) = delete;

    void insert(const std::string &word) {
        TrieNode *curr = root_;
        for (char c : word) {
            if (c < 'a' || c > 'z') continue;
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->isEndOfWord = true;
    }

    bool search(const std::string &word) const {
        const TrieNode *node = findPrefixNode(word);
        return node && node->isEndOfWord;
    }

    bool startsWith(const std::string &prefix) const {
        return findPrefixNode(prefix) != nullptr;
    }

    void removeWord(const std::string &word) {
        removeRec(root_, word, 0);
    }

    std::vector<std::string> autocomplete(const std::string &prefix) const {
        const TrieNode *node = findPrefixNode(prefix);
        if (!node) return {};
        std::vector<std::string> results;
        std::string buffer = prefix;
        collectWords(node, buffer, results);
        return results;
    }
};

int main() {
    std::cout << "=== CAY TIEN TO (TRIE / PREFIX TREE - O(L)) ===\n\n";

    Trie trie;
    for (const char *w : {"algo", "algorithm", "alignment", "array", "avl"}) {
        trie.insert(w);
    }

    std::cout << "  search(\"algo\")      : " << (trie.search("algo") ? "Co" : "Khong") << "\n";
    std::cout << "  search(\"alg\")       : " << (trie.search("alg") ? "Co" : "Khong") << "\n";
    std::cout << "  startsWith(\"alg\")   : " << (trie.startsWith("alg") ? "Co" : "Khong") << "\n\n";

    std::cout << "--- Goi y tu dong (Autocomplete) cho tien to \"al\" ---\n  ";
    for (const auto &word : trie.autocomplete("al")) {
        std::cout << word << "  ";
    }
    std::cout << "\n\n";

    std::cout << "--- Xoa tu \"algorithm\" khoi Trie (giu nguyen \"algo\") ---\n";
    trie.removeWord("algorithm");
    std::cout << "  search(\"algorithm\") : " << (trie.search("algorithm") ? "Co" : "Khong") << "\n";
    std::cout << "  search(\"algo\")      : " << (trie.search("algo") ? "Co" : "Khong") << "\n";

    return 0;
}

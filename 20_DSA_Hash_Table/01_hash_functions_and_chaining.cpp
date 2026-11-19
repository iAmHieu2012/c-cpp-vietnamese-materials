#include <iostream>
#include <string>

// =============================================================================
// 1. CÁC HÀM BĂM CHUỖI KINH ĐIỂN (DJB2 & FNV-1A 64-BIT)
// =============================================================================
unsigned long long hash_djb2(const std::string &str) {
    unsigned long long h = 5381;
    for (unsigned char c : str) {
        h = ((h << 5) + h) + c; // h * 33 + c
    }
    return h;
}

unsigned long long hash_fnv1a(const std::string &str) {
    unsigned long long h = 14695981039346656037ULL;
    for (unsigned char c : str) {
        h ^= c;
        h *= 1099511628211ULL;
    }
    return h;
}

// =============================================================================
// 2. HASH TABLE PHƯƠNG PHÁP DÂY CHUYỀN (SEPARATE CHAINING + DYNAMIC REHASHING)
// =============================================================================
class ChainingHashTable {
private:
    struct Node {
        std::string key;
        int value;
        Node *next;
        Node(const std::string &k, int v, Node *n = nullptr)
            : key(k), value(v), next(n) {}
    };

    Node **buckets;
    int bucket_count;
    int size;

    int get_bucket_index(const std::string &key, int mod) const {
        return static_cast<int>(hash_fnv1a(key) % static_cast<unsigned long long>(mod));
    }

    void rehash(int new_bucket_count) {
        Node **new_buckets = new Node*[new_bucket_count]();
        for (int i = 0; i < bucket_count; ++i) {
            Node *curr = buckets[i];
            while (curr != nullptr) {
                Node *next_node = curr->next;
                int new_idx = get_bucket_index(curr->key, new_bucket_count);
                curr->next = new_buckets[new_idx];
                new_buckets[new_idx] = curr;
                curr = next_node;
            }
        }
        delete[] buckets;
        buckets = new_buckets;
        bucket_count = new_bucket_count;
    }

public:
    explicit ChainingHashTable(int init_buckets = 4)
        : buckets(new Node*[init_buckets]()), bucket_count(init_buckets), size(0) {}

    ~ChainingHashTable() {
        for (int i = 0; i < bucket_count; ++i) {
            Node *curr = buckets[i];
            while (curr != nullptr) {
                Node *tmp = curr;
                curr = curr->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    ChainingHashTable(const ChainingHashTable&) = delete;
    ChainingHashTable& operator=(const ChainingHashTable&) = delete;

    void put(const std::string &key, int value) {
        if (static_cast<double>(size + 1) / bucket_count > 0.75) {
            rehash(bucket_count * 2);
        }
        int idx = get_bucket_index(key, bucket_count);
        for (Node *curr = buckets[idx]; curr != nullptr; curr = curr->next) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
        }
        buckets[idx] = new Node(key, value, buckets[idx]);
        ++size;
    }

    bool get(const std::string &key, int &out_val) const {
        int idx = get_bucket_index(key, bucket_count);
        for (Node *curr = buckets[idx]; curr != nullptr; curr = curr->next) {
            if (curr->key == key) {
                out_val = curr->value;
                return true;
            }
        }
        return false;
    }

    bool remove(const std::string &key) {
        int idx = get_bucket_index(key, bucket_count);
        Node *prev = nullptr;
        Node *curr = buckets[idx];
        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) buckets[idx] = curr->next;
                else                 prev->next = curr->next;
                delete curr;
                --size;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void print_debug() const {
        std::cout << "  [ChainingHashTable: size=" << size << ", buckets=" << bucket_count << "]\n";
        for (int i = 0; i < bucket_count; ++i) {
            std::cout << "    Bucket[" << i << "]: ";
            for (Node *c = buckets[i]; c != nullptr; c = c->next) {
                std::cout << "{" << c->key << ": " << c->value << "} -> ";
            }
            std::cout << "nullptr\n";
        }
    }
};

int main() {
    std::cout << "=== 1. SO SANH MA BAM DJB2 & FNV-1A (AVALANCHE EFFECT) ===\n";
    for (const char *s : {"alg1", "alg2", "cpp17"}) {
        std::cout << "  Key = \"" << s << "\" -> djb2: " << hash_djb2(s)
                  << " | fnv1a: " << hash_fnv1a(s) << "\n";
    }

    std::cout << "\n=== 2. SEPARATE CHAINING + DYNAMIC REHASHING ===\n";
    ChainingHashTable ht(4);
    ht.put("An", 85);
    ht.put("Binh", 92);
    ht.put("Chau", 78);
    ht.put("Dung", 95); // Vượt Load Factor 0.75 -> Tự động Rehash lên 8 buckets!
    ht.print_debug();

    int score = 0;
    if (ht.get("Binh", score)) {
        std::cout << "\n  Tra cuu 'Binh' -> " << score << "\n";
    }
    ht.remove("Chau");
    std::cout << "  Sau khi xoa 'Chau', tra cuu 'Chau': "
              << (ht.get("Chau", score) ? "Tim thay" : "Khong ton tai") << "\n";

    return 0;
}

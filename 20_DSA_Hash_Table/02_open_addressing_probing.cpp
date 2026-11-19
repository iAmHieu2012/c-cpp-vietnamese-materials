#include <iostream>
#include <vector>
#include <string>

// =============================================================================
// BẢNG BĂM THĂM DÒ MỞ (OPEN ADDRESSING: LINEAR PROBING vs QUADRATIC PROBING)
// Hỗ trợ xóa an toàn bằng Tombstone (DELETED)
// =============================================================================
class OpenAddressingHashTable {
public:
    enum class ProbingMode { LINEAR, QUADRATIC };

private:
    enum class SlotState { EMPTY, OCCUPIED, DELETED }; // DELETED = Tombstone

    struct Slot {
        int key = 0;
        int value = 0;
        SlotState state = SlotState::EMPTY;
    };

    std::vector<Slot> table;
    int capacity;
    ProbingMode mode;

    int base_hash(int key) const {
        return ((key % capacity) + capacity) % capacity;
    }

    // Công thức bước nhảy: Linear (+ step) vs Quadratic (+ step * step)
    int probe_index(int start_idx, int step) const {
        long long offset = (mode == ProbingMode::LINEAR)
                           ? static_cast<long long>(step)
                           : 1LL * step * step;
        return static_cast<int>((start_idx + offset) % capacity);
    }

public:
    explicit OpenAddressingHashTable(int cap = 7, ProbingMode m = ProbingMode::LINEAR)
        : table(cap), capacity(cap), mode(m) {}

    bool put(int key, int value) {
        int start_idx = base_hash(key);
        int first_deleted = -1;

        for (int step = 0; step < capacity; ++step) {
            int idx = probe_index(start_idx, step);
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key) {
                table[idx].value = value;
                return true;
            }
            if (table[idx].state == SlotState::DELETED && first_deleted == -1) {
                first_deleted = idx;
            }
            if (table[idx].state == SlotState::EMPTY) {
                int target = (first_deleted != -1) ? first_deleted : idx;
                table[target] = {key, value, SlotState::OCCUPIED};
                return true;
            }
        }
        if (first_deleted != -1) {
            table[first_deleted] = {key, value, SlotState::OCCUPIED};
            return true;
        }
        return false;
    }

    bool get(int key, int &out_val) const {
        int start_idx = base_hash(key);
        for (int step = 0; step < capacity; ++step) {
            int idx = probe_index(start_idx, step);
            if (table[idx].state == SlotState::EMPTY) return false;
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key) {
                out_val = table[idx].value;
                return true;
            }
        }
        return false;
    }

    bool remove(int key) {
        int start_idx = base_hash(key);
        for (int step = 0; step < capacity; ++step) {
            int idx = probe_index(start_idx, step);
            if (table[idx].state == SlotState::EMPTY) return false;
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key) {
                table[idx].state = SlotState::DELETED; // Đánh dấu Tombstone!
                return true;
            }
        }
        return false;
    }

    void print_slots(const char *title) const {
        std::cout << "  " << title << ":\n    ";
        for (int i = 0; i < capacity; ++i) {
            std::cout << "[" << i << ":";
            if (table[i].state == SlotState::EMPTY)         std::cout << "EMPTY";
            else if (table[i].state == SlotState::DELETED)  std::cout << "TOMB";
            else std::cout << table[i].key << "->" << table[i].value;
            std::cout << "] ";
        }
        std::cout << "\n";
    }
};

int main() {
    // Các khóa 10, 17, 24 đều có (key % 7 == 3) -> Đụng độ 100% tại ô số 3!
    std::cout << "=== 1. LINEAR PROBING (+1, +2, +3...) & TOMBSTONE DELETION ===\n";
    OpenAddressingHashTable linear_ht(7, OpenAddressingHashTable::ProbingMode::LINEAR);
    linear_ht.put(10, 100); // Vào ô 3
    linear_ht.put(17, 170); // Đụng ô 3 -> trượt sang ô 4 (+1)
    linear_ht.put(24, 240); // Đụng ô 3, 4 -> trượt sang ô 5 (+2)
    linear_ht.print_slots("Truoc khi xoa 17 (Dinh chum tai 3, 4, 5)");

    linear_ht.remove(17);   // Ô 4 biến thành TOMB (Tombstone)
    linear_ht.print_slots("Sau khi xoa 17 (O 4 la Tombstone)");

    int val = 0;
    std::cout << "  -> Tra cuu 24 (vuot qua Tombstone o 4 de toi o 5): "
              << (linear_ht.get(24, val) ? std::to_string(val) : "Khong thay") << "\n\n";

    std::cout << "=== 2. QUADRATIC PROBING (+1^2, +2^2...) CHONG DINH CHUM ===\n";
    OpenAddressingHashTable quad_ht(7, OpenAddressingHashTable::ProbingMode::QUADRATIC);
    quad_ht.put(10, 100); // Vào ô 3
    quad_ht.put(17, 170); // Đụng ô 3 -> nhảy sang (3 + 1^2) % 7 = ô 4
    quad_ht.put(24, 240); // Đụng ô 3 -> nhảy sang (3 + 2^2) % 7 = ô 0 (Không dính chùm vào ô 5!)
    quad_ht.print_slots("Phan bo Quadratic Probing (O 3, 4 va 0)");

    return 0;
}

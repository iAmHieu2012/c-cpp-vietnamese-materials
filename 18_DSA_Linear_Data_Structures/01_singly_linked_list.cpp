#include <iostream>

class SinglyLinkedList {
private:
    struct Node {
        int data;
        Node *next;
        explicit Node(int val) : data(val), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    int size;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyLinkedList() { clear(); }

    // Cấm Shallow Copy mặc định để tránh lỗi Double Free
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    bool empty() const { return size == 0; }
    int get_size() const { return size; }

    // O(1)
    void push_front(int val) {
        Node *node = new Node(val);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
        ++size;
    }

    // O(1) nhờ duy trì con trỏ tail
    void push_back(int val) {
        Node *node = new Node(val);
        if (tail == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++size;
    }

    // O(1)
    void pop_front() {
        if (head == nullptr) return;
        Node *tmp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete tmp;
        --size;
    }

    // O(N): Xóa node đầu tiên có giá trị bằng val
    bool remove_value(int val) {
        if (head == nullptr) return false;
        if (head->data == val) {
            pop_front();
            return true;
        }

        Node *prev = head;
        Node *curr = head->next;
        while (curr != nullptr && curr->data != val) {
            prev = curr;
            curr = curr->next;
        }
        if (curr == nullptr) return false;

        prev->next = curr->next;
        if (curr == tail) tail = prev;
        delete curr;
        --size;
        return true;
    }

    // O(N) Time, O(1) Space: Đảo ngược danh sách tại chỗ bằng 3 con trỏ
    void reverse() {
        Node *prev = nullptr;
        Node *curr = head;
        tail = head;
        while (curr != nullptr) {
            Node *next_node = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next_node;
        }
        head = prev;
    }

    void clear() {
        while (head != nullptr) pop_front();
    }

    void print(const char *label) const {
        std::cout << "  " << label << " (size=" << size << "): ";
        Node *curr = head;
        while (curr != nullptr) {
            std::cout << curr->data << " -> ";
            curr = curr->next;
        }
        std::cout << "nullptr\n";
    }
};

int main() {
    SinglyLinkedList list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_front(5);

    std::cout << "=== SINGLY LINKED LIST ===\n";
    list.print("Ban dau       ");

    list.remove_value(20);
    list.print("Sau xoa 20    ");

    list.reverse();
    list.print("Sau khi reverse");

    return 0;
}
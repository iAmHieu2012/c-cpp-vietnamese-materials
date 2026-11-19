#include <iostream>

// =============================================================================
// 1. DOUBLY LINKED LIST (O(1) THÊM/XÓA CẢ 2 ĐẦU)
// =============================================================================
class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node *prev;
        Node *next;
        explicit Node(int val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DoublyLinkedList() { while (head != nullptr) pop_front(); }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_front(int val) {
        Node *node = new Node(val);
        node->next = head;
        if (head != nullptr) head->prev = node;
        else                 tail = node;
        head = node;
        ++size;
    }

    void push_back(int val) {
        Node *node = new Node(val);
        node->prev = tail;
        if (tail != nullptr) tail->next = node;
        else                 head = node;
        tail = node;
        ++size;
    }

    void pop_front() {
        if (head == nullptr) return;
        Node *tmp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else                 tail = nullptr;
        delete tmp;
        --size;
    }

    // O(1) nhờ con trỏ prev của tail!
    void pop_back() {
        if (tail == nullptr) return;
        Node *tmp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else                 head = nullptr;
        delete tmp;
        --size;
    }

    void print_forward() const {
        std::cout << "  Xuoi : nullptr <- ";
        for (Node *c = head; c != nullptr; c = c->next) {
            std::cout << c->data << (c->next ? " <-> " : " -> ");
        }
        std::cout << "nullptr\n";
    }

    void print_backward() const {
        std::cout << "  Nguoc: nullptr <- ";
        for (Node *c = tail; c != nullptr; c = c->prev) {
            std::cout << c->data << (c->prev ? " <-> " : " -> ");
        }
        std::cout << "nullptr\n";
    }
};

// =============================================================================
// 2. CIRCULAR LINKED LIST (DANH SÁCH LIÊN KẾT VÒNG)
// =============================================================================
class CircularLinkedList {
private:
    struct Node {
        int data;
        Node *next;
        explicit Node(int val) : data(val), next(nullptr) {}
    };

    Node *tail; // Chỉ cần giữ con trỏ tail: head chính là tail->next!
    int size;

public:
    CircularLinkedList() : tail(nullptr), size(0) {}

    ~CircularLinkedList() {
        if (tail == nullptr) return;
        Node *head = tail->next;
        tail->next = nullptr; // Cắt vòng trước khi giải phóng
        while (head != nullptr) {
            Node *tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    CircularLinkedList(const CircularLinkedList&) = delete;
    CircularLinkedList& operator=(const CircularLinkedList&) = delete;

    void push_back(int val) {
        Node *node = new Node(val);
        if (tail == nullptr) {
            tail = node;
            tail->next = tail; // Tự trỏ vào chính mình
        } else {
            node->next = tail->next; // Trỏ về head cũ
            tail->next = node;
            tail = node;
        }
        ++size;
    }

    void print_rounds(int steps) const {
        if (tail == nullptr) return;
        std::cout << "  Duyet vong (" << steps << " buoc): ";
        Node *curr = tail->next; // Bắt đầu từ head
        for (int i = 0; i < steps; ++i) {
            std::cout << curr->data << " -> ";
            curr = curr->next;
        }
        std::cout << "...\n";
    }
};

int main() {
    std::cout << "=== 1. DOUBLY LINKED LIST ===\n";
    DoublyLinkedList dll;
    dll.push_back(10);
    dll.push_back(20);
    dll.push_back(30);
    dll.push_front(5);
    dll.pop_back(); // Xóa 30 trong O(1)
    dll.print_forward();
    dll.print_backward();

    std::cout << "\n=== 2. CIRCULAR LINKED LIST ===\n";
    CircularLinkedList cll;
    cll.push_back(1);
    cll.push_back(2);
    cll.push_back(3);
    cll.print_rounds(7); // In 1 -> 2 -> 3 -> 1 -> 2 -> 3 -> 1 -> ...

    return 0;
}
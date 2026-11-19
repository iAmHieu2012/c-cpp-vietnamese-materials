#include <iostream>
#include <string>
#include <sstream>

// =============================================================================
// CẤU TRÚC HỖ TRỢ TỰ VIẾT (KHÔNG PHỤ THUỘC STL CONTAINER)
// =============================================================================
template <typename T>
class SimpleStack {
private:
    struct Node {
        T data;
        Node *next;
        Node(const T &val, Node *nxt) : data(val), next(nxt) {}
    };
    Node *top_node = nullptr;

public:
    ~SimpleStack() { while (!empty()) pop(); }
    SimpleStack() = default;
    SimpleStack(const SimpleStack&) = delete;
    SimpleStack& operator=(const SimpleStack&) = delete;

    bool empty() const { return top_node == nullptr; }
    void push(const T &val) { top_node = new Node(val, top_node); }
    void pop() {
        if (empty()) return;
        Node *tmp = top_node;
        top_node = top_node->next;
        delete tmp;
    }
    T top() const { return top_node->data; }
};

struct ListNode {
    int val;
    ListNode *next;
    explicit ListNode(int v, ListNode *n = nullptr) : val(v), next(n) {}
};

void free_list(ListNode *head) {
    while (head != nullptr) {
        ListNode *tmp = head;
        head = head->next;
        delete tmp;
    }
}

void print_list(const char *label, ListNode *head) {
    std::cout << "  " << label << ": ";
    while (head != nullptr) {
        std::cout << head->val << " -> ";
        head = head->next;
    }
    std::cout << "nullptr\n";
}


// =============================================================================
// BÀI 1: KIỂM TRA CHUỖI NGOẶC HỢP LỆ (VALID PARENTHESES - O(N))
// =============================================================================
bool is_valid_parentheses(const std::string &s) {
    SimpleStack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char open = st.top();
            st.pop();
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
                return false;
            }
        }
    }
    return st.empty();
}


// =============================================================================
// BÀI 2: TÍNH GIÁ TRỊ BIỂU THỨC HẬU TỐ (REVERSE POLISH NOTATION - O(N))
// =============================================================================
int eval_postfix(const std::string &expr) {
    SimpleStack<int> st;
    std::stringstream ss(expr);
    std::string token;

    while (ss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            int b = st.top(); st.pop(); // Toán hạng thứ 2 nằm trên đỉnh
            int a = st.top(); st.pop(); // Toán hạng thứ 1 nằm dưới
            if      (token == "+") st.push(a + b);
            else if (token == "-") st.push(a - b);
            else if (token == "*") st.push(a * b);
            else if (token == "/") st.push(a / b);
        } else {
            st.push(std::stoi(token));
        }
    }
    return st.empty() ? 0 : st.top();
}


// =============================================================================
// BÀI 3: FLOYD'S TORTOISE & HARE (PHÁT HIỆN CHU TRÌNH & TÌM NODE GIỮA)
// =============================================================================
bool has_cycle(ListNode *head) {
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

ListNode* find_middle_node(ListNode *head) {
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}


// =============================================================================
// BÀI 4: GỘP 2 SINGLY LINKED LIST ĐÃ SẮP XẾP (DUMMY NODE - O(N+M), O(1) SPACE)
// =============================================================================
ListNode* merge_sorted_lists(ListNode *l1, ListNode *l2) {
    ListNode dummy(0); // Dummy Node nằm trên Stack, tránh xử lý đặc biệt cho head
    ListNode *tail = &dummy;

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = (l1 != nullptr) ? l1 : l2;
    return dummy.next;
}


// =============================================================================
// BÀI 5: CÀI ĐẶT QUEUE BẰNG 2 STACK (AMORTIZED O(1))
// =============================================================================
class QueueUsingTwoStacks {
private:
    SimpleStack<int> in_stack;
    SimpleStack<int> out_stack;

    void shift_stacks() {
        if (out_stack.empty()) {
            while (!in_stack.empty()) {
                out_stack.push(in_stack.top());
                in_stack.pop();
            }
        }
    }

public:
    bool empty() const { return in_stack.empty() && out_stack.empty(); }

    void enqueue(int val) { in_stack.push(val); }

    void dequeue() {
        shift_stacks();
        if (!out_stack.empty()) out_stack.pop();
    }

    int front() {
        shift_stacks();
        return out_stack.top();
    }
};


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.3 (LINEAR DATA STRUCTURES) ==========\n";
        std::cout << "1. Kiem tra chuoi ngoac hop le (Valid Parentheses - Stack)\n";
        std::cout << "2. Tinh gia tri bieu thuc hau to (Postfix / RPN - Stack)\n";
        std::cout << "3. Phat hien chu trinh & Tim node giua (Floyd's Tortoise & Hare)\n";
        std::cout << "4. Gop 2 Linked List da sap xep (Dummy Node - O(1) Space)\n";
        std::cout << "5. Cai dat Queue bang 2 Stack (Amortized O(1))\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string s1 = "({[()]})";
                std::string s2 = "([)]";
                std::cout << "\n[Bai 1]\n";
                std::cout << "  \"" << s1 << "\" -> " << (is_valid_parentheses(s1) ? "Hop le" : "Khong hop le") << "\n";
                std::cout << "  \"" << s2 << "\"     -> " << (is_valid_parentheses(s2) ? "Hop le" : "Khong hop le") << "\n";
                break;
            }

            case 2: {
                std::string expr = "3 4 + 2 * 7 /"; // ((3 + 4) * 2) / 7 = 2
                std::cout << "\n[Bai 2] Bieu thuc hau to: \"" << expr << "\"\n";
                std::cout << "  -> Ket qua = " << eval_postfix(expr) << "\n";
                break;
            }

            case 3: {
                ListNode *n1 = new ListNode(10);
                ListNode *n2 = new ListNode(20);
                ListNode *n3 = new ListNode(30);
                ListNode *n4 = new ListNode(40);
                ListNode *n5 = new ListNode(50);
                n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5;

                std::cout << "\n[Bai 3] Danh sach: 10 -> 20 -> 30 -> 40 -> 50\n";
                std::cout << "  -> Co chu trinh? " << (has_cycle(n1) ? "Co" : "Khong") << "\n";
                std::cout << "  -> Node giua   : " << find_middle_node(n1)->val << "\n";

                // Tạo chu trình n5 -> n2 để kiểm thử
                n5->next = n2;
                std::cout << "  -> Sau khi noi 50 -> 20, co chu trinh? " << (has_cycle(n1) ? "Co" : "Khong") << "\n";

                n5->next = nullptr; // Bắt buộc cắt chu trình trước khi giải phóng bộ nhớ!
                free_list(n1);
                break;
            }

            case 4: {
                ListNode *l1 = new ListNode(1, new ListNode(4, new ListNode(7)));
                ListNode *l2 = new ListNode(2, new ListNode(3, new ListNode(8, new ListNode(10))));
                std::cout << "\n[Bai 4]\n";
                print_list("List 1", l1);
                print_list("List 2", l2);

                ListNode *merged = merge_sorted_lists(l1, l2);
                print_list("Merged", merged);
                free_list(merged);
                break;
            }

            case 5: {
                QueueUsingTwoStacks q;
                q.enqueue(10);
                q.enqueue(20);
                q.enqueue(30);
                std::cout << "\n[Bai 5] Enqueue 10, 20, 30 -> Dequeue: " << q.front() << "\n";
                q.dequeue();
                q.enqueue(40);
                std::cout << "  Enqueue 40 -> Dequeue con lai: ";
                while (!q.empty()) {
                    std::cout << q.front() << " ";
                    q.dequeue();
                }
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

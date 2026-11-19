// ============================================================================
// File: 03_stack_and_applications.cpp
// Bài 4.3: Stack (LIFO - Array Stack & Linked Stack, MinStack O(1), Monotonic Stack)
// ============================================================================

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================================================
// 1. LINKED STACK (LIFO - CÀI ĐẶT BẰNG DANH SÁCH LIÊN KẾT ĐƠN)
// ============================================================================
class LinkedStack {
    struct Node {
        int data;
        Node *next;
        Node(int val, Node *nxt) : data(val), next(nxt) {}
    };

    Node *topNode_ = nullptr;
    int size_ = 0;

public:
    LinkedStack() = default;
    ~LinkedStack() { while (!empty()) pop(); }

    LinkedStack(const LinkedStack &) = delete;
    LinkedStack &operator=(const LinkedStack &) = delete;

    bool empty() const { return topNode_ == nullptr; }
    int size() const { return size_; }

    void push(int val) {
        topNode_ = new Node(val, topNode_);
        ++size_;
    }

    void pop() {
        if (empty()) return;
        Node *tmp = topNode_;
        topNode_ = topNode_->next;
        delete tmp;
        --size_;
    }

    int top() const {
        if (empty()) throw std::underflow_error("Stack rong!");
        return topNode_->data;
    }
};

// ============================================================================
// 2. MIN-STACK: LẤY PHẦN TỬ NHỎ NHẤT TRONG STACK VỚI THỜI GIAN O(1)
// ============================================================================
class MinStack {
    std::vector<std::pair<int, int>> st_; // {value, current_min}

public:
    void push(int val) {
        if (st_.empty()) {
            st_.push_back({val, val});
        } else {
            st_.push_back({val, std::min(val, st_.back().second)});
        }
    }

    void pop() {
        if (!st_.empty()) st_.pop_back();
    }

    int top() const { return st_.back().first; }
    int getMin() const { return st_.back().second; }
};

// ============================================================================
// 3. MONOTONIC STACK (NGĂN XẾP ĐƠN ĐIỆU): TÌM NEXT GREATER ELEMENT TRONG O(N)
// ============================================================================
std::vector<int> nextGreaterElements(const std::vector<int> &arr) {
    int n = static_cast<int>(arr.size());
    std::vector<int> res(n, -1);
    LinkedStack st; // Lưu CHỈ SỐ các phần tử đang chờ tìm phần tử lớn hơn bên phải

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[i] > arr[st.top()]) {
            res[st.top()] = arr[i];
            st.pop();
        }
        st.push(i);
    }
    return res;
}

int main() {
    std::cout << "=== 1. LINKED STACK (LIFO) ===\n";
    LinkedStack st;
    st.push(10);
    st.push(20);
    st.push(30);
    std::cout << "  Pop lan luot: ";
    while (!st.empty()) {
        std::cout << st.top() << " ";
        st.pop();
    }
    std::cout << "\n\n";

    std::cout << "=== 2. MIN-STACK (TRUY VAN MIN TRONG O(1)) ===\n";
    MinStack minSt;
    minSt.push(5);
    minSt.push(2);
    minSt.push(8);
    minSt.push(1);
    std::cout << "  Top = " << minSt.top() << " | Min hien tai = " << minSt.getMin() << "\n";
    minSt.pop(); // Xóa 1
    std::cout << "  Sau khi pop(1) -> Top = " << minSt.top()
              << " | Min hien tai = " << minSt.getMin() << "\n\n";

    std::cout << "=== 3. MONOTONIC STACK (NEXT GREATER ELEMENT - O(N)) ===\n";
    std::vector<int> arr = {2, 1, 5, 6, 2, 3};
    std::vector<int> nge = nextGreaterElements(arr);
    std::cout << "  Mang goc            : ";
    for (int x : arr) std::cout << x << "\t";
    std::cout << "\n  Next Greater Element: ";
    for (int x : nge) std::cout << x << "\t";
    std::cout << "\n";

    return 0;
}

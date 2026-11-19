// ============================================================================
// File: 03_list_and_forward_list.cpp
// Bài 5.2: std::list (Doubly Linked List + splice O(1)) & std::forward_list
// ============================================================================

#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>

int main() {
    std::cout << "=== 1. STD::LIST (DOUBLY LINKED LIST + SPLICE O(1) ZERO-COPY) ===\n";
    std::list<int> l1 = {1, 2, 3};
    std::list<int> l2 = {100, 200, 300};

    // Chuyển toàn bộ node của l2 vào ngay sau phần tử đầu tiên của l1 trong O(1) (0 copy!)
    auto it = std::next(l1.begin());
    l1.splice(it, l2);

    std::cout << "  l1 sau splice: ";
    for (int x : l1) std::cout << x << " ";
    std::cout << "\n  l2.size() sau splice = " << l2.size() << "\n";

    // Các phương thức thành viên đặc quyền của std::list: sort(), unique(), remove_if()
    l1.sort();
    std::cout << "  l1 sau l1.sort(): ";
    for (int x : l1) std::cout << x << " ";
    std::cout << "\n";

    l1.remove_if([](int x) { return x % 2 == 0; });
    std::cout << "  l1 sau remove_if (xoa so chan): ";
    for (int x : l1) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== 2. STD::FORWARD_LIST (SINGLY LINKED LIST SIEU TIET KIEM RAM) ===\n";
    std::forward_list<int> fl = {10, 30, 40};
    fl.push_front(5);

    // Chèn 20 vào ngay phía sau phần tử thứ hai (10) trong O(1) bằng insert_after
    auto pos = std::next(fl.begin()); // Trỏ vào 10
    fl.insert_after(pos, 20);

    std::cout << "  Phan tu trong forward_list: ";
    for (int x : fl) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

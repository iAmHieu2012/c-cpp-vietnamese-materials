#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
    std::cout << "=== 1. STD::NEXT_PERMUTATION (BẮT BUỘC SORT TĂNG DẦN TRƯỚC) ===\n";
    std::vector<int> p = {3, 1, 2};
    std::sort(p.begin(), p.end()); // Phải đưa về {1, 2, 3} trước để sinh đủ 3! = 6 hoán vị
    std::cout << "  Tat ca 6 hoan vi cua {1, 2, 3}:\n";
    do {
        std::cout << "    ";
        for (int x : p) std::cout << x << " ";
        std::cout << "\n";
    } while (std::next_permutation(p.begin(), p.end()));

    std::cout << "\n=== 2. STD::ROTATE (XOAY TRÁI / XOAY PHẢI TẠI CHỖ O(N) TIME, O(1) SPACE) ===\n";
    std::vector<int> r = {1, 2, 3, 4, 5};
    std::rotate(r.begin(), r.begin() + 2, r.end()); // Xoay trái 2 bước -> {3, 4, 5, 1, 2}
    std::cout << "  Xoay trai 2 buoc : ";
    for (int x : r) std::cout << x << " ";
    std::cout << "\n";

    std::rotate(r.rbegin(), r.rbegin() + 2, r.rend()); // Xoay phải 2 bước -> trở về {1, 2, 3, 4, 5}
    std::cout << "  Xoay phai 2 buoc : ";
    for (int x : r) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== 3. PHEP TOAN TAP HOP TREN 2 MANG DA SAP XEP O(N + M) ===\n";
    std::vector<int> a = {1, 2, 4, 5, 7};
    std::vector<int> b = {2, 3, 5, 6, 7};
    std::vector<int> uni, inter, diff, sym_diff;

    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(uni));
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(inter));
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(diff));
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(sym_diff));

    std::cout << "  Hop (A u B)           : ";
    for (int x : uni) std::cout << x << " ";
    std::cout << "\n  Giao (A n B)          : ";
    for (int x : inter) std::cout << x << " ";
    std::cout << "\n  Hieu (A \\ B)          : ";
    for (int x : diff) std::cout << x << " ";
    std::cout << "\n  Hieu doi xung (A ^ B) : ";
    for (int x : sym_diff) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

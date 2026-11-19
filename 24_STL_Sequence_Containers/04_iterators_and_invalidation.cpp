#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

int main() {
    std::cout << "=== 1. ERASE-REMOVE IDIOM TREN STD::VECTOR (O(N)) ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // std::remove_if dồn các phần tử cần giữ lên đầu và trả về iterator đánh dấu đuôi rác
    // v.erase(...) cắt bỏ toàn bộ phần đuôi rác trong 1 lần duy nhất
    v.erase(
        std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }),
        v.end()
    );

    std::cout << "  Vector sau khi xoa so chan: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== 2. XOA AN TOAN TRONG VONG LAP VOI STD::LIST ===\n";
    std::list<int> lst = {10, 25, 30, 45, 50};
    for (auto it = lst.begin(); it != lst.end(); ) {
        if (*it % 2 != 0) {
            it = lst.erase(it); // BẮT BUỘC hứng lại iterator mới! Không viết lst.erase(it); ++it;
        } else {
            ++it;
        }
    }

    std::cout << "  List sau khi xoa so le: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "=== 3. THAO TAC ITERATOR (STD::ADVANCE & STD::DISTANCE) ===\n";
    auto it_list = lst.begin();
    // it_list + 2; // LỖI BIÊN DỊCH vì std::list là Bidirectional Iterator, không hỗ trợ cộng số nguyên!
    std::advance(it_list, 2); // Cách chuẩn dùng được cho mọi loại Iterator
    std::cout << "  Phan tu thu 2 trong list: " << *it_list << "\n";
    std::cout << "  Khoang cach tu begin()  : " << std::distance(lst.begin(), it_list) << "\n";

    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <utility>

int main() {
    std::cout << "=== 1. CAPTURE BY VALUE [x] VS CAPTURE BY REFERENCE [&x] ===\n";
    int base = 10;
    int total_calls = 0;

    // [base] chụp bản sao base = 10 ngay lúc tạo; [&total_calls] tham chiếu thẳng tới biến gốc
    auto compute = [base, &total_calls](int x) {
        ++total_calls;
        return x + base;
    };

    base = 999; // Đổi base bên ngoài sau khi đã tạo Lambda -> KHÔNG ảnh hưởng tới bản sao [base] (= 10) bên trong!
    std::cout << "  compute(5) khi base ngoai da doi thanh 999 = " << compute(5)
              << " (base trong Lambda van la 10!)\n";
    std::cout << "  compute(20) = " << compute(20) << " | total_calls (qua &total_calls) = " << total_calls << "\n\n";

    std::cout << "=== 2. TU KHOA MUTABLE (CHO PHEP SUA BAN SAO NOI BO TRONG LAMBDA) ===\n";
    int start_id = 1000;
    // Nhờ 'mutable', mỗi lần gọi next_id() sẽ tăng bản sao nội bộ của start_id mà không làm đổi biến start_id bên ngoài
    auto next_id = [start_id]() mutable {
        return ++start_id;
    };

    std::cout << "  Goi next_id() 3 lan: " << next_id() << ", " << next_id() << ", " << next_id() << "\n";
    std::cout << "  Bien start_id goc o main() van giu nguyen = " << start_id << "\n\n";

    std::cout << "=== 3. C++14 INIT-CAPTURE (CHUYEN QUYEN SO HUU VAO LAMBDA BANG STD::MOVE) ===\n";
    std::vector<int> heavy_data = {10, 20, 30, 40, 50};
    // Cướp thẳng vùng nhớ Heap của heavy_data đưa vào thuộc tính 'owned' của Lambda trong O(1)
    auto print_owned = [owned = std::move(heavy_data)]() {
        std::cout << "  Du lieu da move vao Lambda (size=" << owned.size() << "): ";
        for (int x : owned) std::cout << x << " ";
        std::cout << "\n";
    };

    std::cout << "  heavy_data.size() ben ngoai sau khi move = " << heavy_data.size() << "\n";
    print_owned();

    return 0;
}

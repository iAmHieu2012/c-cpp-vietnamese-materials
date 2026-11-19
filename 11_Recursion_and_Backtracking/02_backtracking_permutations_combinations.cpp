#include <iostream>

const int MAXN = 20;
int x[MAXN];         // Mảng lưu cấu hình hiện tại đang thử
bool used[MAXN];     // Mảng đánh dấu các số đã được dùng (cho Hoán vị)

// =============================================================================
// 1. SINH HOÁN VỊ CỦA N PHẦN TỬ (1..N) -> Tổng cộng N! cấu hình
// =============================================================================
void generate_permutations(int idx, int n) {
    // Điều kiện dừng: Đã điền đủ N vị trí (từ 0 đến n - 1)
    if (idx == n) {
        std::cout << "  ";
        for (int i = 0; i < n; ++i) std::cout << x[i] << " ";
        std::cout << "\n";
        return;
    }

    // Thử đặt từng giá trị val từ 1 đến n vào vị trí idx
    for (int val = 1; val <= n; ++val) {
        if (!used[val]) {
            x[idx] = val;          // 1. CHỌN (Choose)
            used[val] = true;      // Đánh dấu đã dùng

            generate_permutations(idx + 1, n); // 2. ĐI SÂU (Explore)

            used[val] = false;     // 3. QUAY LUI / BỎ CHỌN (Backtrack) để nhánh khác dùng lại
        }
    }
}

// =============================================================================
// 2. SINH TỔ HỢP CHẬP K CỦA N PHẦN TỬ (1..N) -> Tổng cộng C(N, K) cấu hình
// =============================================================================
void generate_combinations(int idx, int start_val, int n, int k) {
    if (idx == k) {
        std::cout << "  { ";
        for (int i = 0; i < k; ++i) std::cout << x[i] << " ";
        std::cout << "}\n";
        return;
    }

    // Chỉ duyệt từ start_val trở đi để đảm bảo tổ hợp luôn tăng dần và không bị trùng lặp
    for (int val = start_val; val <= n - (k - 1 - idx); ++val) {
        x[idx] = val;                                      // 1. CHỌN
        generate_combinations(idx + 1, val + 1, n, k);     // 2. ĐI SÂU (nhánh sau bắt đầu từ val + 1)
        // Không cần unchoose mảng x vì vòng lặp sau sẽ tự ghi đè x[idx]
    }
}

int main() {
    std::cout << "=== 1. HOAN VI CUA N = 3 (3! = 6 cau hinh) ===\n";
    generate_permutations(0, 3);

    std::cout << "\n=== 2. TO HOP CHAP K = 3 CUA N = 5 (C(5,3) = 10 cau hinh) ===\n";
    generate_combinations(0, 1, 5, 3);

    return 0;
}
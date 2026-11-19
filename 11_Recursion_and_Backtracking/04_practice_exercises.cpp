#include <iostream>
#include <string>

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1: Lũy thừa nhị phân O(log n)
long long fast_power(long long base, long long exp);

// Bài 2: Tháp Hà Nội (Tower of Hanoi)
void tower_of_hanoi(int n, char from, char aux, char to, int &step_count);

// Bài 3: Sinh chuỗi nhị phân độ dài N không có 2 số '1' đứng cạnh nhau
void gen_binary_no_adj_ones(int idx, int n, std::string &current);

// Bài 4: Tìm tất cả các tập con có tổng bằng target (Subset Sum Backtracking)
void subset_sum(const int arr[], int n, int idx, int current_sum, int target,
                int chosen[], int chosen_len, int &solution_count);

// Bài 5: Chuột tìm đường thoát Mê cung (Rat in a Maze)
const int MAZE_N = 4;
void solve_maze(int maze[MAZE_N][MAZE_N], bool visited[MAZE_N][MAZE_N],
                int r, int c, std::string &path, int &path_count);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.6 (DE QUY & QUAY LUI) ==========\n";
        std::cout << "1. Luy thua nhi phan a^n trong O(log n)\n";
        std::cout << "2. Thap Ha Noi (Tower of Hanoi)\n";
        std::cout << "3. Chuoi nhi phan do dai N khong co hai so '1' ke nhau\n";
        std::cout << "4. Tim tat ca tap con co tong bang S (Subset Sum)\n";
        std::cout << "5. Chuot tim duong thoat Me cung (Rat in a Maze)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                long long a, n;
                std::cout << "\n[Bai 1] Nhap co so a va so mu n (n >= 0): ";
                std::cin >> a >> n;
                if (n < 0) {
                    std::cout << "-> Vui long nhap so mu n >= 0!\n";
                } else {
                    std::cout << "-> Ket qua " << a << "^" << n << " = " << fast_power(a, n) << "\n";
                }
                break;
            }

            case 2: {
                int n;
                std::cout << "\n[Bai 2] Nhap so dia N (khuyen nghi 1 -> 5): ";
                std::cin >> n;
                if (n <= 0) {
                    std::cout << "-> So dia phai > 0!\n";
                } else {
                    int steps = 0;
                    std::cout << "Cac buoc di chuyen " << n << " dia tu cot A sang cot C:\n";
                    tower_of_hanoi(n, 'A', 'B', 'C', steps);
                    std::cout << "-> Tong so buoc: " << steps << " (Cong thuc 2^N - 1)\n";
                }
                break;
            }

            case 3: {
                int n;
                std::cout << "\n[Bai 3] Nhap do dai N (VD: 4): ";
                std::cin >> n;
                if (n <= 0) {
                    std::cout << "-> N phai > 0!\n";
                } else {
                    std::string current(n, '0');
                    std::cout << "Cac chuoi nhi phan do dai " << n << " khong co '11':\n";
                    gen_binary_no_adj_ones(0, n, current);
                }
                break;
            }

            case 4: {
                int arr[] = {2, 3, 5, 6, 8, 10};
                int n = sizeof(arr) / sizeof(arr[0]);
                int target = 10;
                int chosen[20];
                int sol_count = 0;

                std::cout << "\n[Bai 4] Mang: {2, 3, 5, 6, 8, 10} | Target S = " << target << "\n";
                std::cout << "Cac tap con co tong bang " << target << ":\n";
                subset_sum(arr, n, 0, 0, target, chosen, 0, sol_count);
                if (sol_count == 0) {
                    std::cout << "-> Khong co tap con nao thoa man!\n";
                } else {
                    std::cout << "-> Tong cong: " << sol_count << " tap con.\n";
                }
                break;
            }

            case 5: {
                int maze[MAZE_N][MAZE_N] = {
                    {1, 0, 0, 0},
                    {1, 1, 0, 1},
                    {1, 1, 0, 0},
                    {0, 1, 1, 1}
                };
                bool visited[MAZE_N][MAZE_N] = {false};
                std::string path = "";
                int path_count = 0;

                std::cout << "\n[Bai 5] Me cung 4x4 (1: Duong di, 0: Tuong):\n";
                for (int i = 0; i < MAZE_N; ++i) {
                    std::cout << "  ";
                    for (int j = 0; j < MAZE_N; ++j) std::cout << maze[i][j] << " ";
                    std::cout << "\n";
                }

                std::cout << "Cac duong di tu (0,0) den (3,3):\n";
                if (maze[0][0] == 1) {
                    visited[0][0] = true;
                    solve_maze(maze, visited, 0, 0, path, path_count);
                }
                if (path_count == 0) {
                    std::cout << "-> Khong co loi thoat!\n";
                } else {
                    std::cout << "-> Tong cong: " << path_count << " duong di.\n";
                }
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


// =============================================================================
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM (FUNCTION DEFINITIONS)
// =============================================================================

// --- Bài 1: Lũy thừa nhị phân O(log n) ---
long long fast_power(long long base, long long exp) {
    if (exp == 0) return 1; // Base Case: a^0 = 1

    // Chỉ tính base^(exp / 2) ĐÚNG 1 LẦN rồi bình phương lên
    // (Nếu gọi 2 lần fast_power(base, exp/2) * fast_power(base, exp/2) sẽ bị chậm về O(n)!)
    long long half = fast_power(base, exp / 2);
    if (exp % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}

// --- Bài 2: Tháp Hà Nội ---
void tower_of_hanoi(int n, char from, char aux, char to, int &step_count) {
    if (n == 1) {
        std::cout << "  Buoc " << ++step_count << ": Chuyen dia 1 tu " << from << " -> " << to << "\n";
        return;
    }
    // Bước 1: Chuyển (n - 1) đĩa trên cùng từ 'from' sang cột trung gian 'aux'
    tower_of_hanoi(n - 1, from, to, aux, step_count);

    // Bước 2: Chuyển đĩa lớn nhất (đĩa n) từ 'from' sang đích 'to'
    std::cout << "  Buoc " << ++step_count << ": Chuyen dia " << n << " tu " << from << " -> " << to << "\n";

    // Bước 3: Chuyển (n - 1) đĩa từ cột trung gian 'aux' sang đích 'to'
    tower_of_hanoi(n - 1, aux, from, to, step_count);
}

// --- Bài 3: Chuỗi nhị phân không có hai số '1' kề nhau (Cắt tỉa nhánh) ---
void gen_binary_no_adj_ones(int idx, int n, std::string &current) {
    if (idx == n) {
        std::cout << "  " << current << "\n";
        return;
    }

    // Luôn có thể đặt '0' tại vị trí idx
    current[idx] = '0';
    gen_binary_no_adj_ones(idx + 1, n, current);

    // Chỉ được đặt '1' nếu đây là vị trí đầu tiên HOẶC ký tự liền trước không phải '1'
    if (idx == 0 || current[idx - 1] != '1') {
        current[idx] = '1';
        gen_binary_no_adj_ones(idx + 1, n, current);
    }
}

// --- Bài 4: Subset Sum Backtracking (Có cắt tỉa khi current_sum > target) ---
void subset_sum(const int arr[], int n, int idx, int current_sum, int target,
                int chosen[], int chosen_len, int &solution_count) {
    if (current_sum == target) {
        ++solution_count;
        std::cout << "  { ";
        for (int i = 0; i < chosen_len; ++i) std::cout << chosen[i] << " ";
        std::cout << "}\n";
        return;
    }

    // Cắt tỉa (Pruning): Hết mảng hoặc tổng hiện tại đã vượt quá target (do mảng toàn số dương)
    if (idx == n || current_sum > target) {
        return;
    }

    // Nhánh 1: CHỌN phần tử arr[idx]
    chosen[chosen_len] = arr[idx];
    subset_sum(arr, n, idx + 1, current_sum + arr[idx], target, chosen, chosen_len + 1, solution_count);

    // Nhánh 2: KHÔNG CHỌN phần tử arr[idx] (Backtrack ngầm qua việc giữ nguyên chosen_len)
    subset_sum(arr, n, idx + 1, current_sum, target, chosen, chosen_len, solution_count);
}

// --- Bài 5: Chuột tìm đường thoát Mê cung (Rat in a Maze) ---
void solve_maze(int maze[MAZE_N][MAZE_N], bool visited[MAZE_N][MAZE_N],
                int r, int c, std::string &path, int &path_count) {
    // Điều kiện dừng: Đã tới ô đích (MAZE_N - 1, MAZE_N - 1)
    if (r == MAZE_N - 1 && c == MAZE_N - 1) {
        ++path_count;
        std::cout << "  Duong " << path_count << ": " << path << "\n";
        return;
    }

    // 4 hướng di chuyển: Down (D), Left (L), Right (R), Up (U)
    const int dr[4] = {1, 0, 0, -1};
    const int dc[4] = {0, -1, 1, 0};
    const char dir_char[4] = {'D', 'L', 'R', 'U'};

    for (int d = 0; d < 4; ++d) {
        int nr = r + dr[d];
        int nc = c + dc[d];

        // Kiểm tra: Nằm trong biên + Là đường đi (1) + Chưa từng đi qua
        if (nr >= 0 && nr < MAZE_N && nc >= 0 && nc < MAZE_N &&
            maze[nr][nc] == 1 && !visited[nr][nc]) {
            
            visited[nr][nc] = true;        // 1. CHỌN (Đánh dấu đã đi qua)
            path.push_back(dir_char[d]);

            solve_maze(maze, visited, nr, nc, path, path_count); // 2. ĐI SÂU

            path.pop_back();               // 3. QUAY LUI (Trả lại trạng thái cũ)
            visited[nr][nc] = false;
        }
    }
}
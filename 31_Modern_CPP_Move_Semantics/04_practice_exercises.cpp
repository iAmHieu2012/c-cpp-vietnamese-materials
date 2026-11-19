#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <chrono>
#include <utility>

// =============================================================================
// BÀI 1: HÀM HOÁN ĐỔI ZERO-COPY FAST_SWAP<T> BẰNG STD::MOVE - O(1)
// =============================================================================
template <typename T>
void fast_swap(T &a, T &b) noexcept {
    // Nếu không có std::move, hoán đổi 2 vector 1 triệu phần tử sẽ phải copy 3 triệu phần tử!
    // Nhờ std::move, chỉ hoán đổi 3 cặp con trỏ Heap trong O(1) (3 bước gán địa chỉ)!
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

void exercise_1_fast_swap() {
    std::vector<int> v1(1000000, 7);
    std::vector<int> v2(500000, 9);

    const int *addr1_before = v1.data();
    const int *addr2_before = v2.data();

    fast_swap(v1, v2);

    std::cout << "\n[Bai 1] Hoan doi 2 vector 1,000,000 phan tu trong O(1) bang std::move:\n";
    std::cout << "  Dia chi Heap v1 truoc: " << addr1_before << " -> Sau la dia chi cua v2: " << v2.data() << "\n";
    std::cout << "  Dia chi Heap v2 truoc: " << addr2_before << " -> Sau la dia chi cua v1: " << v1.data() << "\n";
}


// =============================================================================
// BÀI 2: ĐO THỜI GIAN THỰC TẾ GIỮA COPY O(N) VÀ MOVE O(1)
// =============================================================================
void exercise_2_copy_vs_move_benchmark() {
    using Clock = std::chrono::high_resolution_clock;
    std::string giant_str(5000000, 'A'); // Chuỗi nặng 5 MB

    auto t1 = Clock::now();
    std::string copied_str = giant_str; // Copy 5 MB trên Heap
    auto t2 = Clock::now();
    auto copy_us = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    auto t3 = Clock::now();
    std::string moved_str = std::move(giant_str); // Move chỉ chép con trỏ 8 bytes!
    auto t4 = Clock::now();
    auto move_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();

    std::cout << "\n[Bai 2] Benchmark Copy vs Move tren chuoi 5 MB (5,000,000 chars):\n";
    std::cout << "  - Thoi gian Copy O(N) : " << copy_us << " ns (copied.size = " << copied_str.size() << ")\n";
    std::cout << "  - Thoi gian Move O(1) : " << move_ns << " ns (moved.size  = " << moved_str.size() << ")\n";
}


// =============================================================================
// BÀI 3: TỰ CÀI ĐẶT MY_MAKE_UNIQUE<T> BẰNG VARIADIC + PERFECT FORWARDING
// =============================================================================
template <typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct ServerNode {
    int id;
    std::string hostname;
    ServerNode(int i, std::string &&host) : id(i), hostname(std::move(host)) {
        std::cout << "  [ServerNode] Khoi tao truc tiep qua Rvalue Move: #" << id << " - " << hostname << "\n";
    }
};

void exercise_3_custom_make_unique() {
    std::cout << "\n[Bai 3] Tu viet my_make_unique<T>(Args&&...) voi std::forward:\n";
    std::string temp_host = "prod-db-01.internal";
    auto node = my_make_unique<ServerNode>(101, std::move(temp_host));
    std::cout << "  temp_host sau khi forward Move vao ServerNode: \"" << temp_host << "\" (da rong)\n";
}


// =============================================================================
// BÀI 4: HÀM BỌC LOG & THỰC THI ĐA NĂNG (GENERIC EXECUTION WRAPPER)
// =============================================================================
template <typename Func, typename... Args>
decltype(auto) invoke_with_log(const char *task_name, Func &&fn, Args&&... args) {
    std::cout << "  [START] Dang chay tac vu: " << task_name << "\n";
    return std::forward<Func>(fn)(std::forward<Args>(args)...);
}

void exercise_4_perfect_forwarding_wrapper() {
    std::cout << "\n[Bai 4] Goi ham bat ky qua invoke_with_log (Perfect Forwarding):\n";
    int base = 100;
    auto add_to_ref = [](int &target, int delta) {
        target += delta;
        return target;
    };

    // Bảo toàn được tham chiếu Lvalue `int &target` nhờ std::forward!
    int updated = invoke_with_log("Cap nhat bien tham chieu", add_to_ref, base, 55);
    std::cout << "  [DONE] Gia tri bien goc 'base' sau khi goi = " << updated << "\n";
}


// =============================================================================
// BÀI 5: HÀNG ĐỢI TÁC VỤ ĐỘC QUYỀN (MOVE-ONLY TASK QUEUE VỚI UNIQUE_PTR)
// =============================================================================
struct AsyncJob {
    int job_id;
    std::string payload;
    AsyncJob(int id, std::string p) : job_id(id), payload(std::move(p)) {}
};

void exercise_5_move_only_queue() {
    std::queue<std::unique_ptr<AsyncJob>> job_queue;

    // Đẩy unique_ptr vào queue bắt buộc phải dùng std::move (hoặc truyền trực tiếp Rvalue)
    job_queue.push(std::make_unique<AsyncJob>(1, "Compress_Video_4K.mp4"));
    job_queue.push(std::make_unique<AsyncJob>(2, "Train_Neural_Network.bin"));

    std::cout << "\n[Bai 5] Rut va xu ly Move-Only unique_ptr tu std::queue:\n";
    while (!job_queue.empty()) {
        // Phải dùng std::move(job_queue.front()) để cướp quyền sở hữu trước khi pop()!
        std::unique_ptr<AsyncJob> current_job = std::move(job_queue.front());
        job_queue.pop();
        std::cout << "  -> Xu ly Job #" << current_job->job_id << ": " << current_job->payload << "\n";
    }
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 6.4 (MOVE SEMANTICS & FORWARDING) ==========\n";
        std::cout << "1. Hoan doi 2 vector khong lo trong O(1) bang fast_swap (std::move)\n";
        std::cout << "2. Benchmark thoi gian thuc te: Copy O(N) vs Move O(1)\n";
        std::cout << "3. Tu cai dat my_make_unique<T> bang Perfect Forwarding\n";
        std::cout << "4. Ham boc thuc thi da nang invoke_with_log(Func&&, Args&&...)\n";
        std::cout << "5. Hang doi tac vu Move-Only (std::queue<std::unique_ptr<Job>>)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "====================================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_fast_swap(); break;
            case 2: exercise_2_copy_vs_move_benchmark(); break;
            case 3: exercise_3_custom_make_unique(); break;
            case 4: exercise_4_perfect_forwarding_wrapper(); break;
            case 5: exercise_5_move_only_queue(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

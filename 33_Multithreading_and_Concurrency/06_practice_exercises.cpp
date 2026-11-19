#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <numeric>

// =============================================================================
// BÀI 1: TÍNH TỔNG MẢNG LỚN SONG SONG TRÊN K LUỒNG (PARALLEL MAP-REDUCE)
// =============================================================================
long long parallel_array_sum(const std::vector<int> &arr, int num_threads) {
    int n = static_cast<int>(arr.size());
    int chunk_size = (n + num_threads - 1) / num_threads;
    std::vector<std::future<long long>> futures;

    for (int t = 0; t < num_threads; ++t) {
        int start_idx = t * chunk_size;
        int end_idx   = std::min(n, start_idx + chunk_size);
        if (start_idx >= end_idx) break;

        futures.push_back(std::async(std::launch::async, [&arr, start_idx, end_idx]() {
            return std::accumulate(arr.begin() + start_idx, arr.begin() + end_idx, 0LL);
        }));
    }

    long long total = 0;
    for (auto &f : futures) total += f.get();
    return total;
}

void exercise_1_parallel_sum() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1); // 1 .. 1,000,000
    long long res = parallel_array_sum(data, 4);
    std::cout << "\n[Bai 1] Tong 1..1,000,000 tinh song song tren 4 luong async = " << res << "\n";
}


// =============================================================================
// BÀI 2: HÀNG ĐỢI ĐA LUỒNG AN TOÀN (THREAD-SAFE BLOCKING QUEUE)
// =============================================================================
template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool closed = false;

public:
    void push(T val) {
        {
            std::lock_guard<std::mutex> lk(mtx);
            q.push(std::move(val));
        }
        cv.notify_one();
    }

    bool pop(T &out_val) {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]() { return !q.empty() || closed; });
        if (q.empty()) return false;
        out_val = std::move(q.front());
        q.pop();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lk(mtx);
            closed = true;
        }
        cv.notify_all();
    }
};

void exercise_2_thread_safe_queue() {
    std::cout << "\n[Bai 2] Kiem thu ThreadSafeQueue (2 Producers -> 1 Consumer):\n";
    ThreadSafeQueue<std::string> tsq;

    std::thread p1([&]() { tsq.push("Log_P1_A"); tsq.push("Log_P1_B"); });
    std::thread p2([&]() { tsq.push("Log_P2_A"); tsq.push("Log_P2_B"); });

    p1.join();
    p2.join();
    tsq.close();

    std::string msg;
    std::cout << "  Cac goi tin Consumer rut ra an toan: ";
    while (tsq.pop(msg)) {
        std::cout << "[" << msg << "] ";
    }
    std::cout << "\n";
}


// =============================================================================
// BÀI 3: IN CHẵn - LẺ LUÂN PHIÊN GIỮA 2 LUỒNG BẰNG CONDITION_VARIABLE
// =============================================================================
void exercise_3_alternating_odd_even() {
    std::cout << "\n[Bai 3] 2 Luong in so Le va Chan luan phien 1..10:\n  ";
    std::mutex mtx;
    std::condition_variable cv;
    int current = 1;
    const int MAX_N = 10;

    std::thread odd_thread([&]() {
        while (true) {
            std::unique_lock<std::mutex> lk(mtx);
            cv.wait(lk, [&]() { return (current % 2 == 1) || (current > MAX_N); });
            if (current > MAX_N) break;
            std::cout << "Le(" << current++ << ") ";
            cv.notify_all();
        }
    });

    std::thread even_thread([&]() {
        while (true) {
            std::unique_lock<std::mutex> lk(mtx);
            cv.wait(lk, [&]() { return (current % 2 == 0) || (current > MAX_N); });
            if (current > MAX_N) break;
            std::cout << "Chan(" << current++ << ") ";
            cv.notify_all();
        }
    });

    odd_thread.join();
    even_thread.join();
    std::cout << "\n";
}


// =============================================================================
// BÀI 4: BỘ ĐẾM THỐNG KÊ KHÔNG KHÓA (LOCK-FREE ATOMIC CAS / FETCH_ADD)
// =============================================================================
void exercise_4_atomic_lock_free_stats() {
    std::atomic<long long> total_requests{0};
    std::atomic<int> max_latency{0};

    auto update_max_atomic = [&](int new_val) {
        int prev = max_latency.load();
        // Compare-And-Swap (CAS Loop): Cập nhật max_latency hoàn toàn Lock-free!
        while (prev < new_val && !max_latency.compare_exchange_weak(prev, new_val)) {}
    };

    std::vector<std::thread> workers;
    for (int t = 1; t <= 4; ++t) {
        workers.emplace_back([&, t]() {
            total_requests.fetch_add(1000, std::memory_order_relaxed);
            update_max_atomic(t * 125);
        });
    }
    for (auto &w : workers) w.join();

    std::cout << "\n[Bai 4] Thong ke Lock-Free bang std::atomic & Compare-And-Swap (CAS):\n";
    std::cout << "  -> Tong Requests = " << total_requests.load()
              << " | Max Latency = " << max_latency.load() << " ms\n";
}


// =============================================================================
// BÀI 5: THIẾT KẾ THREAD POOL (HỒ CHỨA LUỒNG) CHUẨN DOANH NGHIỆP
// =============================================================================
class SimpleThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;

public:
    explicit SimpleThreadPool(std::size_t num_threads) {
        for (std::size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lk(mtx);
                        cv.wait(lk, [this]() { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~SimpleThreadPool() {
        {
            std::lock_guard<std::mutex> lk(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto &w : workers) {
            if (w.joinable()) w.join();
        }
    }

    void enqueue(std::function<void()> fn) {
        {
            std::lock_guard<std::mutex> lk(mtx);
            tasks.push(std::move(fn));
        }
        cv.notify_one();
    }
};

void exercise_5_thread_pool() {
    std::cout << "\n[Bai 5] Chay 6 tac vu tren ThreadPool gom 2 Worker Threads:\n";
    std::atomic<int> completed{0};
    {
        SimpleThreadPool pool(2);
        for (int i = 1; i <= 6; ++i) {
            pool.enqueue([i, &completed]() {
                ++completed;
                (void)i;
            });
        }
    } // Ra khỏi scope -> Destructor của ThreadPool tự động đợi chạy xong cả 6 tác vụ và join()!
    std::cout << "  -> Da hoan tat an toan " << completed.load() << "/6 tac vu qua ThreadPool!\n";
}


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP ĐA LUỒNG
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 7.1 (MULTITHREADING & CONCURRENCY) ==========\n";
        std::cout << "1. Tinh tong mang 1,000,000 phan tu song song (std::async & future)\n";
        std::cout << "2. Hang doi da luong an toan (Thread-Safe Blocking Queue)\n";
        std::cout << "3. Dong bo 2 luong in Chan - Le luan phien (condition_variable)\n";
        std::cout << "4. Thong ke Lock-Free & Compare-And-Swap (std::atomic CAS)\n";
        std::cout << "5. Thiet ke Thread Pool xu ly tac vu bat dong bo\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=====================================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: exercise_1_parallel_sum(); break;
            case 2: exercise_2_thread_safe_queue(); break;
            case 3: exercise_3_alternating_odd_even(); break;
            case 4: exercise_4_atomic_lock_free_stats(); break;
            case 5: exercise_5_thread_pool(); break;
            case 0: std::cout << "-> Tam biet!\n"; break;
            default: std::cout << "-> Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

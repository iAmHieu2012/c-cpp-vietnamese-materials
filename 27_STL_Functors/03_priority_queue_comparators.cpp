#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

struct Job {
    int id;
    std::string title;
    int priority;     // Số càng LỚN càng khẩn cấp (10 = Cao nhất, 1 = Thấp nhất)
    int duration_ms;  // Thời gian chạy càng NHỎ càng được ưu tiên xử lý trước (Shortest Job First)
};

// =============================================================================
// CUSTOM COMPARATOR CHO STD::PRIORITY_QUEUE (QUY TẮC NGƯỢC CHIỀU!)
// Trả về true nếu `a` có độ ưu tiên THẤP HƠN `b` (`b` sẽ được đẩy lên đỉnh top() trước `a`!)
// =============================================================================
struct JobPriorityQueueCmp {
    bool operator()(const Job &a, const Job &b) const {
        // 1. Nếu priority khác nhau: `a` thua `b` khi a.priority < b.priority (đẩy priority lớn lên đỉnh)
        if (a.priority != b.priority) return a.priority < b.priority;
        // 2. Nếu cùng priority: `a` thua `b` khi a.duration_ms > b.duration_ms (đẩy job ngắn hơn lên đỉnh)
        if (a.duration_ms != b.duration_ms) return a.duration_ms > b.duration_ms;
        // 3. Cùng cả hai: ưu tiên ID nhỏ hơn lên đỉnh
        return a.id > b.id;
    }
};

int main() {
    std::cout << "=== 1. DOI CHIEU TRUC DIEN: STD::LESS vs STD::GREATER ===\n";
    std::vector<int> data = {40, 10, 90, 20, 70};

    // Trong std::sort: std::less -> Tăng dần (Nhỏ nhất ở đầu), std::greater -> Giảm dần (Lớn nhất ở đầu)
    // Trong priority_queue: std::less -> MAX-HEAP (Lớn nhất ở đỉnh), std::greater -> MIN-HEAP (Nhỏ nhất ở đỉnh)!
    std::priority_queue<int, std::vector<int>, std::less<int>>    max_heap(data.begin(), data.end());
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap(data.begin(), data.end());

    std::cout << "  Max-Heap (std::less)    top() = " << max_heap.top() << " (So LON nhat)\n";
    std::cout << "  Min-Heap (std::greater) top() = " << min_heap.top() << " (So NHO nhat)\n\n";

    std::cout << "=== 2. LAP LICH TIEN TRINH CPU VOI CUSTOM FUNCTOR ===\n";
    std::priority_queue<Job, std::vector<Job>, JobPriorityQueueCmp> scheduler;

    scheduler.push({1, "Backup Database",    5, 1200});
    scheduler.push({2, "Hotfix Security",   10,  450});
    scheduler.push({3, "Send Email Report",  5,  150}); // Cùng priority 5 với #1 nhưng chạy nhanh hơn (150ms < 1200ms)
    scheduler.push({4, "Restart Payment",   10,  100}); // Cùng priority 10 với #2 nhưng chạy nhanh hơn (100ms < 450ms)

    std::cout << "  Thu tu CPU lay Job ra xu ly (Priority cao -> Duration ngan):\n";
    while (!scheduler.empty()) {
        const Job &j = scheduler.top();
        std::cout << "    -> [P=" << j.priority << " | " << j.duration_ms << "ms] Job #"
                  << j.id << ": " << j.title << "\n";
        scheduler.pop();
    }

    return 0;
}

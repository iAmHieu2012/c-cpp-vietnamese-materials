// ============================================================================
// File: 04_queue_and_circular_queue.cpp
// Bài 4.3: Queue (FIFO - Linked Queue, Circular Ring Buffer & Monotonic Deque)
// ============================================================================

#include <deque>
#include <iostream>
#include <stdexcept>
#include <vector>

// ============================================================================
// 1. CIRCULAR QUEUE / RING BUFFER (FIFO - MẢNG VÒNG O(1) CHỐNG FALSE OVERFLOW)
// ============================================================================
class CircularQueue {
    int *data_;
    int capacity_;
    int frontIdx_ = 0;
    int rearIdx_ = 0;
    int count_ = 0;

public:
    explicit CircularQueue(int cap)
        : data_(new int[cap]), capacity_(cap) {}

    ~CircularQueue() { delete[] data_; }

    CircularQueue(const CircularQueue &) = delete;
    CircularQueue &operator=(const CircularQueue &) = delete;

    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == capacity_; }
    int size() const { return count_; }

    bool enqueue(int val) {
        if (full()) return false;
        data_[rearIdx_] = val;
        rearIdx_ = (rearIdx_ + 1) % capacity_; // Cuộn tròn chỉ số về 0 khi chạm biên
        ++count_;
        return true;
    }

    bool dequeue() {
        if (empty()) return false;
        frontIdx_ = (frontIdx_ + 1) % capacity_;
        --count_;
        return true;
    }

    int front() const {
        if (empty()) throw std::underflow_error("Queue rong!");
        return data_[frontIdx_];
    }
};

// ============================================================================
// 2. MONOTONIC DEQUE: TÌM MAX TRÊN TỪNG CỬA SỔ TRƯỢT KÍCH THƯỚC K TRONG O(N)
// ============================================================================
std::vector<int> slidingWindowMaximum(const std::vector<int> &nums, int k) {
    std::vector<int> result;
    std::deque<int> dq; // Lưu CHỈ SỐ theo thứ tự giá trị giảm dần

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // 1. Loại bỏ chỉ số ở đầu hàng đợi nếu đã rơi ra ngoài cửa sổ [i - k + 1 .. i]
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // 2. Loại bỏ các phần tử ở cuối nhỏ hơn hoặc bằng nums[i]
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        // 3. Từ vị trí i >= k - 1, phần tử ở front luôn là Max của cửa sổ hiện tại
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}

int main() {
    std::cout << "=== 1. CIRCULAR QUEUE (RING BUFFER - FIFO O(1)) ===\n";
    CircularQueue q(3);
    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);
    std::cout << "  Queue full? " << (q.full() ? "Yes" : "No") << "\n";

    q.dequeue();    // Lấy 100 ra -> ô 0 trống
    q.enqueue(400); // Quay vòng ghi 400 vào ô 0!

    std::cout << "  Dequeue lan luot: ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.dequeue();
    }
    std::cout << "\n\n";

    std::cout << "=== 2. MONOTONIC DEQUE (SLIDING WINDOW MAXIMUM - O(N)) ===\n";
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    std::vector<int> maxPerWindow = slidingWindowMaximum(nums, k);

    std::cout << "  Mang: {1, 3, -1, -3, 5, 3, 6, 7} | K = " << k << "\n";
    std::cout << "  Max tren tung cua so truot: ";
    for (int m : maxPerWindow) std::cout << m << " ";
    std::cout << "\n";

    return 0;
}

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>

struct Task {
    std::string name;
    int priority; // Số càng nhỏ càng ưu tiên cao (VD: 1 = Khẩn cấp, 5 = Thấp)
};

// Comparator cho Min-Heap theo priority: trả về true nếu a ưu tiên THẤP hơn b
struct TaskCompare {
    bool operator()(const Task &a, const Task &b) const {
        return a.priority > b.priority;
    }
};

int main() {
    std::cout << "=== 1. STD::STACK (LIFO) & STD::QUEUE (FIFO) ===\n";
    std::stack<int> st;
    st.push(1); st.push(2); st.push(3);
    std::cout << "  Stack top: " << st.top() << "\n";

    std::queue<int> q;
    q.push(10); q.push(20); q.push(30);
    std::cout << "  Queue front: " << q.front() << " | back: " << q.back() << "\n\n";

    std::cout << "=== 2. PRIORITY_QUEUE: MAX-HEAP VS MIN-HEAP ===\n";
    std::priority_queue<int> max_pq;
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

    for (int x : {40, 10, 50, 20}) {
        max_pq.push(x);
        min_pq.push(x);
    }
    std::cout << "  Max-Heap top: " << max_pq.top() << " | Min-Heap top: " << min_pq.top() << "\n\n";

    std::cout << "=== 3. PRIORITY_QUEUE VOI CUSTOM STRUCT ===\n";
    std::priority_queue<Task, std::vector<Task>, TaskCompare> task_pq;
    task_pq.push({"Viet tai lieu", 3});
    task_pq.push({"Fix loi Production", 1});
    task_pq.push({"Review PR", 2});

    while (!task_pq.empty()) {
        const auto &t = task_pq.top();
        std::cout << "  [P" << t.priority << "] " << t.name << "\n";
        task_pq.pop();
    }

    return 0;
}

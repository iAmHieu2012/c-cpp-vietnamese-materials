#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

struct Task {
    std::string name;
    int priority;
    int hours;
};

int main() {
    std::vector<Task> tasks = {
        {"Viet Unit Test",   2, 4},
        {"Fix Bug Memory",   1, 2},
        {"Toi uu Database",  1, 5},
        {"Cap nhat README",  3, 1}
    };

    std::cout << "=== 1. TRUYEN LAMBDA TRUC TIEP VAO STD::SORT ===\n";
    // Sắp xếp ưu tiên 1 -> 2 -> 3; nếu cùng độ ưu tiên thì việc tốn ít giờ hơn làm trước
    std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.hours < b.hours;
    });

    for (const auto &t : tasks) {
        std::cout << "  [P" << t.priority << " | " << t.hours << "h] " << t.name << "\n";
    }

    std::cout << "\n=== 2. DUNG LAMBDA LAM COMPARATOR CHO STD::PRIORITY_QUEUE ===\n";
    // Dùng decltype(cmp) để truyền kiểu của Lambda vào priority_queue
    auto pq_cmp = [](const Task &a, const Task &b) {
        return a.priority > b.priority; // Min-Heap: priority số nhỏ (P1) nổi lên đỉnh
    };
    std::priority_queue<Task, std::vector<Task>, decltype(pq_cmp)> pq(pq_cmp);
    for (const auto &t : tasks) pq.push(t);

    std::cout << "  Task khan cap nhat o dinh priority_queue: " << pq.top().name << "\n\n";

    std::cout << "=== 3. ERASE-REMOVE VOI LAMBDA CO CAPTURE BIEN NGOAI ===\n";
    int max_allowed_hours = 3;
    tasks.erase(
        std::remove_if(tasks.begin(), tasks.end(), [max_allowed_hours](const Task &t) {
            return t.hours > max_allowed_hours;
        }),
        tasks.end()
    );

    std::cout << "  Cac Task ngan (<= " << max_allowed_hours << "h) con lai:\n";
    for (const auto &t : tasks) {
        std::cout << "    - " << t.name << " (" << t.hours << "h)\n";
    }

    return 0;
}

#include <iostream>
#include <stdexcept>
#include <utility>

class MaxHeap {
private:
    int *data;
    int size;
    int capacity;

    void resize(int new_cap) {
        int *new_data = new int[new_cap];
        for (int i = 0; i < size; ++i) new_data[i] = data[i];
        delete[] data;
        data = new_data;
        capacity = new_cap;
    }

    // Đẩy phần tử ở vị trí idx lên trên nếu lớn hơn cha nó - O(log N)
    void sift_up(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (data[idx] > data[parent]) {
                std::swap(data[idx], data[parent]);
                idx = parent;
            } else {
                break;
            }
        }
    }

    // Đẩy phần tử ở vị trí idx xuống dưới nếu nhỏ hơn con lớn hơn - O(log N)
    void sift_down(int idx) {
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int largest = idx;

            if (left < size && data[left] > data[largest])   largest = left;
            if (right < size && data[right] > data[largest]) largest = right;

            if (largest != idx) {
                std::swap(data[idx], data[largest]);
                idx = largest;
            } else {
                break;
            }
        }
    }

public:
    explicit MaxHeap(int init_cap = 4)
        : data(new int[init_cap]), size(0), capacity(init_cap) {}

    // Xây dựng Heap từ mảng có sẵn bằng Floyd's Algorithm trong O(N)
    MaxHeap(const int arr[], int n)
        : data(new int[n > 0 ? n : 4]), size(n), capacity(n > 0 ? n : 4) {
        for (int i = 0; i < n; ++i) data[i] = arr[i];
        for (int i = size / 2 - 1; i >= 0; --i) {
            sift_down(i);
        }
    }

    ~MaxHeap() { delete[] data; }

    MaxHeap(const MaxHeap&) = delete;
    MaxHeap& operator=(const MaxHeap&) = delete;

    bool empty() const { return size == 0; }
    int get_size() const { return size; }

    void push(int val) {
        if (size == capacity) resize(capacity * 2);
        data[size] = val;
        sift_up(size);
        ++size;
    }

    int top() const {
        if (empty()) throw std::underflow_error("Heap rong!");
        return data[0];
    }

    void pop() {
        if (empty()) return;
        data[0] = data[size - 1];
        --size;
        if (size > 0) sift_down(0);
    }
};

int main() {
    std::cout << "=== 1. MAX-HEAP (PRIORITY QUEUE) ===\n";
    MaxHeap pq;
    pq.push(15);
    pq.push(40);
    pq.push(10);
    pq.push(55);
    pq.push(25);

    std::cout << "  Rut lan luot tu lon den be: ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << "\n\n";

    std::cout << "=== 2. BUILD HEAP TU MANG CO SAN TRONG O(N) ===\n";
    int arr[] = {3, 19, 1, 14, 8, 7};
    MaxHeap pq2(arr, 6);
    std::cout << "  Phan tu lon nhat (top): " << pq2.top() << "\n";

    return 0;
}

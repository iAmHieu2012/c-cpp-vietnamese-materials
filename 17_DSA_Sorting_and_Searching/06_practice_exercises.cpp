#include <iostream>
#include <utility>
#include <algorithm>

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM
// =============================================================================

// Bài 1: Tìm vị trí đầu tiên & cuối cùng của target trong mảng đã sắp xếp - O(log N)
int find_first_pos(const int a[], int n, int target);
int find_last_pos(const int a[], int n, int target);

// Bài 2: Tìm kiếm trong mảng đã sắp xếp bị xoay (Rotated Sorted Array) - O(log N)
int search_rotated_array(const int a[], int n, int target);

// Bài 3: Đếm số cặp nghịch thế (Inversion Count) bằng Merge Sort - O(N log N)
long long count_inversions(int a[], int n);

// Bài 4: Tìm phần tử lớn thứ K bằng Quickselect - O(N) trung bình
int find_kth_largest(int a[], int n, int k);

// Bài 5: Binary Search on Answer - Tải trọng tàu nhỏ nhất để chuyển hàng trong D ngày
int min_ship_capacity(const int weights[], int n, int days);


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 4.2 (SORTING & SEARCHING) ==========\n";
        std::cout << "1. Tim vi tri dau/cuoi & dem so lan xuat hien - O(log N)\n";
        std::cout << "2. Tim kiem trong mang sap xep bi xoay (Rotated Array) - O(log N)\n";
        std::cout << "3. Dem so cap nghich the (Inversion Count) bang Merge Sort\n";
        std::cout << "4. Tim phan tu lon thu K bang Quickselect - O(N) avg\n";
        std::cout << "5. Binary Search on Answer: Tai trong tau nho nhat (D ngay)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int a[] = {1, 2, 5, 5, 5, 5, 8, 10};
                int n = sizeof(a) / sizeof(a[0]);
                int target = 5;
                int first = find_first_pos(a, n, target);
                int last  = find_last_pos(a, n, target);
                std::cout << "\n[Bai 1] Mang: {1, 2, 5, 5, 5, 5, 8, 10} | Target = " << target << "\n";
                if (first == -1) {
                    std::cout << "-> Khong tim thay!\n";
                } else {
                    std::cout << "-> Vi tri dau: " << first << " | Vi tri cuoi: " << last
                              << " | So lan xuat hien: " << (last - first + 1) << "\n";
                }
                break;
            }

            case 2: {
                int a[] = {4, 5, 6, 7, 0, 1, 2};
                int n = sizeof(a) / sizeof(a[0]);
                int target = 0;
                std::cout << "\n[Bai 2] Mang xoay: {4, 5, 6, 7, 0, 1, 2} | Target = " << target << "\n";
                std::cout << "-> Vi tri tim thay = " << search_rotated_array(a, n, target) << "\n";
                break;
            }

            case 3: {
                int a[] = {2, 4, 1, 3, 5};
                int n = sizeof(a) / sizeof(a[0]);
                std::cout << "\n[Bai 3] Mang: {2, 4, 1, 3, 5}\n";
                // Các cặp nghịch thế: (2,1), (4,1), (4,3) -> 3 cặp
                std::cout << "-> Tong so cap nghich the = " << count_inversions(a, n) << "\n";
                break;
            }

            case 4: {
                int a[] = {3, 2, 1, 5, 6, 4};
                int n = sizeof(a) / sizeof(a[0]);
                int k = 2;
                std::cout << "\n[Bai 4] Mang: {3, 2, 1, 5, 6, 4} | K = " << k << "\n";
                std::cout << "-> Phan tu lon thu " << k << " = " << find_kth_largest(a, n, k) << "\n";
                break;
            }

            case 5: {
                int weights[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
                int n = sizeof(weights) / sizeof(weights[0]);
                int days = 5;
                std::cout << "\n[Bai 5] Cac goi hang: {1..10} | So ngay D = " << days << "\n";
                std::cout << "-> Tai trong tau nho nhat = " << min_ship_capacity(weights, n, days) << "\n";
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
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM
// =============================================================================

// --- Bài 1: First & Last Position ---
int find_first_pos(const int a[], int n, int target) {
    int left = 0, right = n - 1, ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (a[mid] == target) {
            ans = mid;
            right = mid - 1; // Tiếp tục ép sang trái để tìm vị trí đầu tiên
        } else if (a[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

int find_last_pos(const int a[], int n, int target) {
    int left = 0, right = n - 1, ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (a[mid] == target) {
            ans = mid;
            left = mid + 1; // Tiếp tục ép sang phải để tìm vị trí cuối cùng
        } else if (a[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

// --- Bài 2: Search in Rotated Sorted Array ---
int search_rotated_array(const int a[], int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (a[mid] == target) return mid;

        // Xác định nửa nào đang có thứ tự tăng dần
        if (a[left] <= a[mid]) {
            // Nửa trái [left..mid] đã sắp xếp
            if (target >= a[left] && target < a[mid]) right = mid - 1;
            else                                      left  = mid + 1;
        } else {
            // Nửa phải [mid..right] đã sắp xếp
            if (target > a[mid] && target <= a[right]) left  = mid + 1;
            else                                       right = mid - 1;
        }
    }
    return -1;
}

// --- Bài 3: Inversion Count bằng Merge Sort ---
long long merge_and_count(int a[], int temp[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    long long inv_count = 0;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
            // Vì nửa trái [i..mid] đã tăng dần, mọi phần tử từ i đến mid đều > a[j]
            inv_count += (mid - i + 1);
        }
    }
    while (i <= mid)   temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];
    for (int p = left; p <= right; ++p) a[p] = temp[p];

    return inv_count;
}

long long merge_sort_count(int a[], int temp[], int left, int right) {
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    long long cnt = 0;
    cnt += merge_sort_count(a, temp, left, mid);
    cnt += merge_sort_count(a, temp, mid + 1, right);
    cnt += merge_and_count(a, temp, left, mid, right);
    return cnt;
}

long long count_inversions(int a[], int n) {
    if (n <= 1) return 0;
    int *temp = new int[n];
    long long total = merge_sort_count(a, temp, 0, n - 1);
    delete[] temp;
    return total;
}

// --- Bài 4: Quickselect tìm phần tử lớn thứ K ---
int partition_qs(int a[], int low, int high) {
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (a[j] <= pivot) std::swap(a[++i], a[j]);
    }
    std::swap(a[i + 1], a[high]);
    return i + 1;
}

int find_kth_largest(int a[], int n, int k) {
    int target_idx = n - k; // Lớn thứ K tương đương vị trí (n - k) nếu sắp tăng dần
    int low = 0, high = n - 1;
    while (low <= high) {
        int pi = partition_qs(a, low, high);
        if (pi == target_idx)     return a[pi];
        else if (pi < target_idx) low  = pi + 1;
        else                      high = pi - 1;
    }
    return -1;
}

// --- Bài 5: Capacity To Ship Packages Within D Days ---
bool can_ship(const int weights[], int n, int days, int cap) {
    int days_used = 1;
    int current_load = 0;
    for (int i = 0; i < n; ++i) {
        if (current_load + weights[i] > cap) {
            ++days_used;
            current_load = weights[i];
        } else {
            current_load += weights[i];
        }
    }
    return days_used <= days;
}

int min_ship_capacity(const int weights[], int n, int days) {
    int left = 0, right = 0;
    for (int i = 0; i < n; ++i) {
        if (weights[i] > left) left = weights[i]; // Tối thiểu phải chở được gói nặng nhất
        right += weights[i];                      // Tối đa là chở toàn bộ trong 1 ngày
    }

    int ans = right;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (can_ship(weights, n, days, mid)) {
            ans = mid;
            right = mid - 1; // Thử giảm tải trọng xuống
        } else {
            left = mid + 1;  // Tải trọng quá nhỏ -> tăng lên
        }
    }
    return ans;
}

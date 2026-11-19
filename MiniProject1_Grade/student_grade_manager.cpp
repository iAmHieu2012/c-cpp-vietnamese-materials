#include <iostream>
#include <iomanip>
#include <cmath>

const int MAX_STUDENTS = 100;
const double EPS = 1e-9;

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Nhóm hàm bổ trợ (Helper functions)
const char* get_academic_rank(double score);
int find_index_by_id(const int ids[], int n, int target_id);
void swap_int(int &a, int &b);
void swap_double(double &a, double &b);

// Nhóm hàm chức năng chính (Core CRUD & Analytics)
void print_student_table(const int ids[], const double scores[], int n);
bool add_student(int ids[], double scores[], int &n, int new_id, double new_score);
bool delete_student_by_id(int ids[], double scores[], int &n, int target_id);
bool update_score_by_id(const int ids[], double scores[], int n, int target_id, double new_score);
void show_statistics(const int ids[], const double scores[], int n);
void sort_by_score_desc(int ids[], double scores[], int n);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN CHƯƠNG TRÌNH
// =============================================================================
int main() {
    // Khởi tạo sẵn 5 sinh viên mẫu bằng Mảng Song Song (Parallel Arrays)
    int ids[MAX_STUDENTS]       = {1001, 1002, 1003, 1004, 1005};
    double scores[MAX_STUDENTS] = {8.50, 9.25, 6.75, 4.50, 7.80};
    int n = 5; // Số lượng sinh viên hiện tại

    int choice;
    do {
        std::cout << "\n========== HE THONG QUAN LY DIEM SINH VIEN (STAGE 1) ==========\n";
        std::cout << "1. In danh sach bang diem sinh vien\n";
        std::cout << "2. Them sinh vien moi (Kiem tra trung ID & diem [0, 10])\n";
        std::cout << "3. Cap nhat diem theo Ma SV (ID)\n";
        std::cout << "4. Xoa sinh vien theo Ma SV (ID)\n";
        std::cout << "5. Tim kiem sinh vien theo Ma SV (ID)\n";
        std::cout << "6. Thong ke lop hoc (Diem TB, Max, Min, Phan bo hoc luc)\n";
        std::cout << "7. Sap xep bang diem GIAM DAN theo diem so\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===============================================================\n";
        std::cout << "Nhap lua chon cua ban (0-7): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                print_student_table(ids, scores, n);
                break;
            }

            case 2: {
                if (n >= MAX_STUDENTS) {
                    std::cout << "-> Loi: Danh sach da day (" << MAX_STUDENTS << " SV)!\n";
                    break;
                }
                int new_id;
                double new_score;
                std::cout << "\n[Them SV] Nhap Ma SV (so nguyen duong, vi du 1006): ";
                std::cin >> new_id;

                if (find_index_by_id(ids, n, new_id) != -1) {
                    std::cout << "-> Loi: Ma SV " << new_id << " da ton tai trong danh sach!\n";
                    break;
                }

                // Bắt buộc nhập điểm hợp lệ trong đoạn [0.0, 10.0] bằng do-while
                do {
                    std::cout << "Nhap diem so (0.0 -> 10.0): ";
                    std::cin >> new_score;
                    if (new_score < 0.0 || new_score > 10.0) {
                        std::cout << "  Diem khong hop le! Vui long nhap lai.\n";
                    }
                } while (new_score < 0.0 || new_score > 10.0);

                if (add_student(ids, scores, n, new_id, new_score)) {
                    std::cout << "-> Them thanh cong SV ma " << new_id << "!\n";
                }
                break;
            }

            case 3: {
                int target_id;
                std::cout << "\n[Cap nhat] Nhap Ma SV can sua diem: ";
                std::cin >> target_id;

                int idx = find_index_by_id(ids, n, target_id);
                if (idx == -1) {
                    std::cout << "-> Khong tim thay sinh vien co Ma SV " << target_id << "!\n";
                    break;
                }

                double new_score;
                do {
                    std::cout << "Diem hien tai la " << scores[idx] << ". Nhap diem moi (0.0 -> 10.0): ";
                    std::cin >> new_score;
                } while (new_score < 0.0 || new_score > 10.0);

                update_score_by_id(ids, scores, n, target_id, new_score);
                std::cout << "-> Da cap nhat diem thanh cong!\n";
                break;
            }

            case 4: {
                int target_id;
                std::cout << "\n[Xoa SV] Nhap Ma SV can xoa: ";
                std::cin >> target_id;

                if (delete_student_by_id(ids, scores, n, target_id)) {
                    std::cout << "-> Da xoa sinh vien Ma SV " << target_id << " khoi danh sach!\n";
                } else {
                    std::cout << "-> Khong tim thay sinh vien co Ma SV " << target_id << "!\n";
                }
                break;
            }

            case 5: {
                int target_id;
                std::cout << "\n[Tim kiem] Nhap Ma SV can tra cuu: ";
                std::cin >> target_id;

                int idx = find_index_by_id(ids, n, target_id);
                if (idx == -1) {
                    std::cout << "-> Khong tim thay sinh vien co Ma SV " << target_id << "!\n";
                } else {
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "-> Tim thay tai vi tri #" << (idx + 1) 
                              << " | Ma SV: " << ids[idx] 
                              << " | Diem: " << scores[idx] 
                              << " | Xep loai: " << get_academic_rank(scores[idx]) << "\n";
                }
                break;
            }

            case 6: {
                show_statistics(ids, scores, n);
                break;
            }

            case 7: {
                sort_by_score_desc(ids, scores, n);
                std::cout << "-> Da sap xep bang diem GIAM DAN! Bang diem sau khi sap xep:\n";
                print_student_table(ids, scores, n);
                break;
            }

            case 0:
                std::cout << "-> Dang thoat he thong. Chuc mung ban da hoan thanh Giai doan 1!\n";
                break;

            default:
                std::cout << "-> Lua chon khong hop le! Vui long chon tu 0 den 7.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}


// =============================================================================
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM (FUNCTION DEFINITIONS)
// =============================================================================

const char* get_academic_rank(double score) {
    if (score >= 9.0) return "Xuat sac";
    if (score >= 8.0) return "Gioi";
    if (score >= 6.5) return "Kha";
    if (score >= 5.0) return "Trung binh";
    return "Yeu (Hoc lai)";
}

int find_index_by_id(const int ids[], int n, int target_id) {
    for (int i = 0; i < n; ++i) {
        if (ids[i] == target_id) {
            return i; // Trả về chỉ số (0 -> n-1) ngay khi tìm thấy
        }
    }
    return -1; // Quy ước trả về -1 nếu không tìm thấy
}

void swap_int(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void swap_double(double &a, double &b) {
    double temp = a;
    a = b;
    b = temp;
}

void print_student_table(const int ids[], const double scores[], int n) {
    if (n <= 0) {
        std::cout << "\n-> Danh sach sinh vien dang trong!\n";
        return;
    }

    std::cout << "\n+------+----------+----------+-----------------+\n";
    std::cout << "| STT  |  MA SV   | DIEM SO  |    XEP LOAI     |\n";
    std::cout << "+------+----------+----------+-----------------+\n";

    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < n; ++i) {
        std::cout << "| " << std::left  << std::setw(4)  << (i + 1)
                  << " | " << std::right << std::setw(8)  << ids[i]
                  << " | " << std::right << std::setw(8)  << scores[i]
                  << " | " << std::left  << std::setw(15) << get_academic_rank(scores[i]) << " |\n";
    }
    std::cout << "+------+----------+----------+-----------------+\n";
    std::cout << "Tong so sinh vien: " << n << "\n";
}

bool add_student(int ids[], double scores[], int &n, int new_id, double new_score) {
    if (n >= MAX_STUDENTS) return false;
    ids[n] = new_id;
    scores[n] = new_score;
    ++n; // Tăng số lượng phần tử thực tế qua tham chiếu
    return true;
}

bool delete_student_by_id(int ids[], double scores[], int &n, int target_id) {
    int pos = find_index_by_id(ids, n, target_id);
    if (pos == -1) return false;

    // QUAN TRỌNG: Phải dịch đồng thời CẢ HAI MẢNG (ids và scores) sang trái 1 ô!
    for (int i = pos; i < n - 1; ++i) {
        ids[i] = ids[i + 1];
        scores[i] = scores[i + 1];
    }
    --n;
    return true;
}

bool update_score_by_id(const int ids[], double scores[], int n, int target_id, double new_score) {
    int pos = find_index_by_id(ids, n, target_id);
    if (pos == -1) return false;
    scores[pos] = new_score;
    return true;
}

void show_statistics(const int ids[], const double scores[], int n) {
    if (n <= 0) {
        std::cout << "\n-> Danh sach trong, khong the thong ke!\n";
        return;
    }

    double sum = 0.0;
    double max_score = scores[0];
    double min_score = scores[0];
    int cnt_xuatsac = 0, cnt_gioi = 0, cnt_kha = 0, cnt_tb = 0, cnt_yeu = 0;

    for (int i = 0; i < n; ++i) {
        double s = scores[i];
        sum += s;
        if (s > max_score) max_score = s;
        if (s < min_score) min_score = s;

        if (s >= 9.0)      ++cnt_xuatsac;
        else if (s >= 8.0) ++cnt_gioi;
        else if (s >= 6.5) ++cnt_kha;
        else if (s >= 5.0) ++cnt_tb;
        else               ++cnt_yeu;
    }

    double avg = sum / n;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n================ BAO CAO THONG KE LOP HOC ================\n";
    std::cout << "- Si so lop:           " << n << " sinh vien\n";
    std::cout << "- Diem trung binh lop: " << avg << "\n";

    // Liệt kê tất cả sinh viên đạt điểm Cao nhất (phòng trường hợp có nhiều thủ khoa đồng điểm)
    std::cout << "- Diem cao nhat (Max): " << max_score << " (Ma SV: ";
    for (int i = 0; i < n; ++i) {
        if (std::abs(scores[i] - max_score) < EPS) std::cout << ids[i] << " ";
    }
    std::cout << ")\n";

    std::cout << "- Diem thap nhat(Min): " << min_score << " (Ma SV: ";
    for (int i = 0; i < n; ++i) {
        if (std::abs(scores[i] - min_score) < EPS) std::cout << ids[i] << " ";
    }
    std::cout << ")\n\n";

    std::cout << "--- Phan bo hoc luc ---\n";
    std::cout << "  + Xuat sac (>= 9.0):   " << cnt_xuatsac << " SV (" << (100.0 * cnt_xuatsac / n) << "%)\n";
    std::cout << "  + Gioi     (8.0-8.9):  " << cnt_gioi    << " SV (" << (100.0 * cnt_gioi / n)    << "%)\n";
    std::cout << "  + Kha      (6.5-7.9):  " << cnt_kha     << " SV (" << (100.0 * cnt_kha / n)     << "%)\n";
    std::cout << "  + TB       (5.0-6.4):  " << cnt_tb      << " SV (" << (100.0 * cnt_tb / n)      << "%)\n";
    std::cout << "  + Yeu      (< 5.0):    " << cnt_yeu     << " SV (" << (100.0 * cnt_yeu / n)     << "%)\n";
}

void sort_by_score_desc(int ids[], double scores[], int n) {
    // Thuật toán Selection Sort: Chọn SV điểm cao nhất đưa lên đầu
    for (int i = 0; i < n - 1; ++i) {
        int best_idx = i;
        for (int j = i + 1; j < n; ++j) {
            // Nếu điểm bằng nhau, ưu tiên Mã SV nhỏ hơn đứng trước
            bool higher_score = (scores[j] - scores[best_idx]) > EPS;
            bool tie_but_smaller_id = (std::abs(scores[j] - scores[best_idx]) < EPS) && (ids[j] < ids[best_idx]);
            
            if (higher_score || tie_but_smaller_id) {
                best_idx = j;
            }
        }
        if (best_idx != i) {
            // CỰC KỲ QUAN TRỌNG: Đổi chỗ điểm thì PHẢI đổi chỗ luôn cả Mã SV tương ứng!
            swap_double(scores[i], scores[best_idx]);
            swap_int(ids[i], ids[best_idx]);
        }
    }
}
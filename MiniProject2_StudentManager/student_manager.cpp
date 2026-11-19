#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <limits>
#include <cmath>

const std::string DATA_FILE = "students.bin";
const double EPS = 1e-9;

// Struct POD (Plain Old Data) an toàn tuyệt đối cho Binary File I/O
struct Student {
    int id;
    char full_name[50];
    double gpa;
};

// Cấu trúc quản lý Mảng động trên Heap
struct StudentList {
    Student *data = nullptr;
    int size = 0;
    int capacity = 0;
};

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Quản lý bộ nhớ động trên Heap
void init_list(StudentList &list, int initial_cap = 2);
void free_list(StudentList &list);
void resize_list(StudentList &list, int new_cap);
void push_back(StudentList &list, const Student &sv);

// Tiện ích chuỗi, xếp loại & tìm kiếm
std::string normalize_name(const std::string &raw);
const char* get_rank(double gpa);
int find_index_by_id(const StudentList &list, int id);

// Các chức năng CRUD & Thống kê
void print_all(const StudentList &list);
void add_student(StudentList &list);
void update_gpa(StudentList &list);
void delete_student(StudentList &list);
void search_student(const StudentList &list);
void show_statistics(const StudentList &list);

// Sắp xếp dùng Con trỏ hàm (Callback)
bool cmp_gpa_desc(const Student &a, const Student &b);
bool cmp_id_asc(const Student &a, const Student &b);
void sort_students(StudentList &list, bool (*cmp)(const Student&, const Student&));

// Lưu & Đọc File nhị phân (.bin)
void save_to_binary(const StudentList &list, const std::string &filename);
void load_from_binary(StudentList &list, const std::string &filename);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN CHÍNH
// =============================================================================
int main() {
    StudentList list;
    init_list(list, 2);

    // Tự động nạp dữ liệu từ file nhị phân khi khởi động
    load_from_binary(list, DATA_FILE);

    int choice;
    do {
        std::cout << "\n============= QUAN LY SINH VIEN (STAGE 2) =============\n";
        std::cout << "  [Heap Status: Size = " << list.size << " | Capacity = " << list.capacity << "]\n";
        std::cout << "1. In bang diem sinh vien\n";
        std::cout << "2. Them sinh vien moi\n";
        std::cout << "3. Cap nhat GPA theo ID\n";
        std::cout << "4. Xoa sinh vien theo ID\n";
        std::cout << "5. Tra cuu sinh vien theo ID\n";
        std::cout << "6. Thong ke lop hoc (TB, Thu khoa, Ty le hoc luc)\n";
        std::cout << "7. Sap xep bang diem (GPA giam dan / ID tang dan)\n";
        std::cout << "8. Luu du lieu ra file nhi phan (" << DATA_FILE << ")\n";
        std::cout << "0. Luu va Thoat\n";
        std::cout << "=======================================================\n";
        std::cout << "Nhap lua chon (0-8): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: print_all(list); break;
            case 2: add_student(list); break;
            case 3: update_gpa(list); break;
            case 4: delete_student(list); break;
            case 5: search_student(list); break;
            case 6: show_statistics(list); break;
            case 7: {
                int mode;
                std::cout << "\nChon kieu sap xep (1: GPA giam dan | 2: ID tang dan): ";
                std::cin >> mode;
                if (mode == 1) {
                    sort_students(list, cmp_gpa_desc);
                    std::cout << "-> Da sap xep GIAM DAN theo GPA (dong diem xep theo ID).\n";
                    print_all(list);
                } else if (mode == 2) {
                    sort_students(list, cmp_id_asc);
                    std::cout << "-> Da sap xep TANG DAN theo ID.\n";
                    print_all(list);
                } else {
                    std::cout << "-> Lua chon sap xep khong hop le!\n";
                }
                break;
            }
            case 8:
                save_to_binary(list, DATA_FILE);
                break;
            case 0:
                save_to_binary(list, DATA_FILE);
                std::cout << "-> Da thu hoi bo nho Heap va thoat chuong trinh.\n";
                break;
            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    free_list(list);
    return 0;
}


// =============================================================================
// 1. QUẢN LÝ BỘ NHỚ ĐỘNG TRÊN HEAP
// =============================================================================
void init_list(StudentList &list, int initial_cap) {
    list.capacity = (initial_cap > 0) ? initial_cap : 2;
    list.size = 0;
    list.data = new Student[list.capacity];
}

void free_list(StudentList &list) {
    delete[] list.data;
    list.data = nullptr;
    list.size = 0;
    list.capacity = 0;
}

void resize_list(StudentList &list, int new_cap) {
    Student *new_data = new Student[new_cap];
    for (int i = 0; i < list.size; ++i) {
        new_data[i] = list.data[i];
    }
    delete[] list.data;
    list.data = new_data;
    list.capacity = new_cap;
}

void push_back(StudentList &list, const Student &sv) {
    if (list.size == list.capacity) {
        resize_list(list, list.capacity * 2);
    }
    list.data[list.size++] = sv;
}


// =============================================================================
// 2. TIỆN ÍCH CHUỖI, XẾP LOẠI & TÌM KIẾM
// =============================================================================
std::string normalize_name(const std::string &raw) {
    std::stringstream ss(raw);
    std::string word, res;
    while (ss >> word) {
        word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        for (size_t i = 1; i < word.length(); ++i) {
            word[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(word[i])));
        }
        if (!res.empty()) res += " ";
        res += word;
    }
    return res;
}

const char* get_rank(double gpa) {
    if (gpa >= 9.0 - EPS) return "Xuat sac";
    if (gpa >= 8.0 - EPS) return "Gioi";
    if (gpa >= 6.5 - EPS) return "Kha";
    if (gpa >= 5.0 - EPS) return "Trung binh";
    return "Yeu";
}

int find_index_by_id(const StudentList &list, int id) {
    for (int i = 0; i < list.size; ++i) {
        if (list.data[i].id == id) return i;
    }
    return -1;
}


// =============================================================================
// 3. CÁC CHỨC NĂNG CRUD & THỐNG KÊ LỚP HỌC
// =============================================================================
void print_all(const StudentList &list) {
    if (list.size == 0) {
        std::cout << "\n-> Danh sach sinh vien dang rong!\n";
        return;
    }
    std::cout << "\n+----------+------------------------------+----------+------------+\n";
    std::cout << "| " << std::left << std::setw(8) << "ID"
              << " | " << std::setw(28) << "Ho va Ten"
              << " | " << std::setw(8) << "GPA"
              << " | " << std::setw(10) << "Xep loai" << " |\n";
    std::cout << "+----------+------------------------------+----------+------------+\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < list.size; ++i) {
        std::cout << "| " << std::left << std::setw(8) << list.data[i].id
                  << " | " << std::setw(28) << list.data[i].full_name
                  << " | " << std::right << std::setw(8) << list.data[i].gpa
                  << " | " << std::left << std::setw(10) << get_rank(list.data[i].gpa) << " |\n";
    }
    std::cout << "+----------+------------------------------+----------+------------+\n";
}

void add_student(StudentList &list) {
    Student sv;
    std::cout << "\nNhap ID sinh vien: ";
    std::cin >> sv.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (find_index_by_id(list, sv.id) != -1) {
        std::cout << "-> Loi: Ma SV " << sv.id << " da ton tai!\n";
        return;
    }

    std::string raw_name;
    std::cout << "Nhap Ho va Ten: ";
    std::getline(std::cin, raw_name);
    std::string clean_name = normalize_name(raw_name);
    if (clean_name.empty()) clean_name = "Unknown";

    std::strncpy(sv.full_name, clean_name.c_str(), sizeof(sv.full_name) - 1);
    sv.full_name[sizeof(sv.full_name) - 1] = '\0';

    do {
        std::cout << "Nhap GPA (0.0 - 10.0): ";
        std::cin >> sv.gpa;
        if (sv.gpa < 0.0 || sv.gpa > 10.0) {
            std::cout << "  -> Diem khong hop le! Vui long nhap lai trong [0.0, 10.0].\n";
        }
    } while (sv.gpa < 0.0 || sv.gpa > 10.0);

    push_back(list, sv);
    std::cout << "-> Da them sinh vien '" << sv.full_name << "' thanh cong!\n";
}

void update_gpa(StudentList &list) {
    int id;
    std::cout << "\nNhap ID sinh vien can cap nhat GPA: ";
    std::cin >> id;
    int idx = find_index_by_id(list, id);
    if (idx == -1) {
        std::cout << "-> Khong tim thay sinh vien co ID " << id << "!\n";
        return;
    }
    double new_gpa;
    do {
        std::cout << "Nhap GPA moi cho '" << list.data[idx].full_name << "' (0.0 - 10.0): ";
        std::cin >> new_gpa;
        if (new_gpa < 0.0 || new_gpa > 10.0) {
            std::cout << "  -> Diem khong hop le!\n";
        }
    } while (new_gpa < 0.0 || new_gpa > 10.0);

    list.data[idx].gpa = new_gpa;
    std::cout << "-> Cap nhat thanh cong!\n";
}

void delete_student(StudentList &list) {
    int id;
    std::cout << "\nNhap ID sinh vien can xoa: ";
    std::cin >> id;
    int idx = find_index_by_id(list, id);
    if (idx == -1) {
        std::cout << "-> Khong tim thay sinh vien co ID " << id << "!\n";
        return;
    }
    for (int i = idx; i < list.size - 1; ++i) {
        list.data[i] = list.data[i + 1];
    }
    --list.size;
    std::cout << "-> Da xoa sinh vien co ID " << id << "!\n";
}

void search_student(const StudentList &list) {
    int id;
    std::cout << "\nNhap ID can tra cuu: ";
    std::cin >> id;
    int idx = find_index_by_id(list, id);
    if (idx == -1) {
        std::cout << "-> Khong tim thay sinh vien co ID " << id << "!\n";
    } else {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "-> Tim thay tai vi tri #" << idx << ":\n";
        std::cout << "   ID: " << list.data[idx].id
                  << " | Ho Ten: " << list.data[idx].full_name
                  << " | GPA: " << list.data[idx].gpa
                  << " | Xep loai: " << get_rank(list.data[idx].gpa) << "\n";
    }
}

void show_statistics(const StudentList &list) {
    if (list.size == 0) {
        std::cout << "\n-> Danh sach rong, khong co du lieu thong ke!\n";
        return;
    }

    double sum = 0.0;
    double max_gpa = list.data[0].gpa;
    double min_gpa = list.data[0].gpa;
    int c_xs = 0, c_gioi = 0, c_kha = 0, c_tb = 0, c_yeu = 0;

    for (int i = 0; i < list.size; ++i) {
        double g = list.data[i].gpa;
        sum += g;
        if (g > max_gpa) max_gpa = g;
        if (g < min_gpa) min_gpa = g;

        if (g >= 9.0 - EPS)      ++c_xs;
        else if (g >= 8.0 - EPS) ++c_gioi;
        else if (g >= 6.5 - EPS) ++c_kha;
        else if (g >= 5.0 - EPS) ++c_tb;
        else                     ++c_yeu;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=============== THONG KE LOP HOC ===============\n";
    std::cout << "  - Si so lop       : " << list.size << " sinh vien\n";
    std::cout << "  - GPA trung binh  : " << (sum / list.size) << "\n";

    std::cout << "  - GPA cao nhat (" << max_gpa << "): ";
    for (int i = 0; i < list.size; ++i) {
        if (std::abs(list.data[i].gpa - max_gpa) < EPS) {
            std::cout << "[" << list.data[i].id << " - " << list.data[i].full_name << "] ";
        }
    }
    std::cout << "\n";

    std::cout << "  - GPA thap nhat (" << min_gpa << "): ";
    for (int i = 0; i < list.size; ++i) {
        if (std::abs(list.data[i].gpa - min_gpa) < EPS) {
            std::cout << "[" << list.data[i].id << " - " << list.data[i].full_name << "] ";
        }
    }
    std::cout << "\n";

    auto pct = [&](int cnt) { return (100.0 * cnt) / list.size; };
    std::cout << "  - Phan loai hoc luc:\n";
    std::cout << "      + Xuat sac   : " << c_xs   << " (" << pct(c_xs)   << "%)\n";
    std::cout << "      + Gioi       : " << c_gioi << " (" << pct(c_gioi) << "%)\n";
    std::cout << "      + Kha        : " << c_kha  << " (" << pct(c_kha)  << "%)\n";
    std::cout << "      + Trung binh : " << c_tb   << " (" << pct(c_tb)   << "%)\n";
    std::cout << "      + Yeu        : " << c_yeu  << " (" << pct(c_yeu)  << "%)\n";
    std::cout << "================================================\n";
}


// =============================================================================
// 4. SẮP XẾP BẰNG CON TRỎ HÀM (CALLBACK)
// =============================================================================
bool cmp_gpa_desc(const Student &a, const Student &b) {
    if (std::abs(a.gpa - b.gpa) > EPS) return a.gpa < b.gpa;
    return a.id > b.id; // Tie-breaking: Đồng điểm xếp ID nhỏ hơn lên trước
}

bool cmp_id_asc(const Student &a, const Student &b) {
    return a.id > b.id;
}

void sort_students(StudentList &list, bool (*cmp)(const Student&, const Student&)) {
    for (int i = 0; i < list.size - 1; ++i) {
        for (int j = 0; j < list.size - 1 - i; ++j) {
            if (cmp(list.data[j], list.data[j + 1])) {
                Student temp = list.data[j];
                list.data[j] = list.data[j + 1];
                list.data[j + 1] = temp;
            }
        }
    }
}


// =============================================================================
// 5. LƯU & ĐỌC FILE NHỊ PHÂN (.bin)
// =============================================================================
void save_to_binary(const StudentList &list, const std::string &filename) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cerr << "-> Loi: Khong the mo file '" << filename << "' de ghi!\n";
        return;
    }
    if (list.size > 0) {
        fout.write(reinterpret_cast<const char*>(list.data),
                   static_cast<std::streamsize>(list.size) * sizeof(Student));
    }
    std::cout << "-> Da luu " << list.size << " sinh vien xuong '" << filename << "'.\n";
}

void load_from_binary(StudentList &list, const std::string &filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cout << "[Khoi tao] Chua co file '" << filename << "'. Bat dau voi danh sach rong.\n";
        return;
    }

    fin.seekg(0, std::ios::end);
    std::streamsize file_size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    int count = static_cast<int>(file_size / sizeof(Student));
    if (count > 0) {
        if (count > list.capacity) {
            resize_list(list, count);
        }
        fin.read(reinterpret_cast<char*>(list.data),
                 static_cast<std::streamsize>(count) * sizeof(Student));
        list.size = count;
        std::cout << "[Khoi tao] Da nap " << count << " sinh vien tu '" << filename << "'.\n";
    }
}
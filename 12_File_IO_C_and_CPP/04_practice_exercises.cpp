#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <climits>
#include <ctime>   // Dùng cho std::time, std::localtime (tạo Timestamp cho Logger)

// Struct POD cho Bài 4 (Cập nhật trực tiếp trên file nhị phân)
struct StudentRecord {
    int id;
    char full_name[50];
    double gpa;
};

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1: Thống kê văn bản (Lines, Words, Characters - giống lệnh wc trên Linux)
bool analyze_text_file(const std::string &filename, int &lines, int &words, int &chars);

// Bài 2: Sao chép file nhị phân bất kỳ theo khối đệm 4KB (Chunking)
bool copy_file_binary(const std::string &src_path, const std::string &dest_path);

// Bài 3: Đọc dãy số lộn xộn từ file văn bản và xuất báo cáo thống kê ra file mới
bool process_numbers_file(const std::string &in_path, const std::string &out_path);

// Bài 4: Cập nhật GPA trực tiếp trong file nhị phân (In-place update bằng seekp)
bool update_student_gpa_binary(const std::string &bin_path, int target_id, double new_gpa);
void print_binary_students(const std::string &bin_path);

// Bài 5: Hệ thống ghi nhật ký (Logger) nối đuôi kèm Timestamp
void log_message(const std::string &log_path, const std::string &level, const std::string &msg);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.7 (FILE I/O C++) ==========\n";
        std::cout << "1. Thong ke file van ban (Lines, Words, Chars - wc)\n";
        std::cout << "2. Sao chep file nhi phan theo khoi 4KB (Chunking)\n";
        std::cout << "3. Doc day so tu numbers.txt va xuat report.txt\n";
        std::cout << "4. Cap nhat GPA truc tiep trong file .bin (seekp)\n";
        std::cout << "5. He thong ghi nhat ky su kien (Logger + Timestamp)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=====================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                const std::string sample_file = "sample_wc.txt";
                // Tạo sẵn file mẫu để test
                {
                    std::ofstream fout(sample_file);
                    fout << "Lap trinh C va C++.\n";
                    fout << "Hoc File I/O chuan cong nghiep!\n";
                    fout << "Dong thu ba co 6 tu.\n";
                }

                int lines = 0, words = 0, chars = 0;
                if (analyze_text_file(sample_file, lines, words, chars)) {
                    std::cout << "\n[Bai 1] Ket qua thong ke file '" << sample_file << "':\n";
                    std::cout << "  - So dong (Lines)     : " << lines << "\n";
                    std::cout << "  - So tu (Words)       : " << words << "\n";
                    std::cout << "  - So ky tu (Chars)    : " << chars << "\n";
                }
                break;
            }

            case 2: {
                const std::string src = "sample_wc.txt";
                const std::string dest = "sample_wc_copy.txt";

                // Đảm bảo file nguồn tồn tại
                {
                    std::ofstream fout(src);
                    fout << "Du lieu goc de kiem thu sao chep nhi phan theo khoi 4KB.\n";
                }

                if (copy_file_binary(src, dest)) {
                    std::cout << "\n[Bai 2] Da sao chep thanh cong '" << src 
                              << "' -> '" << dest << "' (Dung bo dem 4KB)!\n";
                } else {
                    std::cout << "\n[Bai 2] Loi: Khong the sao chep file!\n";
                }
                break;
            }

            case 3: {
                const std::string in_file = "numbers.txt";
                const std::string out_file = "report.txt";

                // Tạo sẵn file chứa các số nguyên phân bố lộn xộn
                {
                    std::ofstream fout(in_file);
                    fout << "  15   -4   20   \n";
                    fout << "100\t\t32\n";
                    fout << "   -18   7  \n";
                }

                if (process_numbers_file(in_file, out_file)) {
                    std::cout << "\n[Bai 3] Da xu ly '" << in_file 
                              << "' va ghi ket qua ra '" << out_file << "':\n";
                    // Đọc lại report.txt in ra màn hình để kiểm chứng
                    std::ifstream fin(out_file);
                    std::string line;
                    while (std::getline(fin, line)) {
                        std::cout << "  " << line << "\n";
                    }
                }
                break;
            }

            case 4: {
                const std::string bin_file = "students_ex.bin";

                // Bước 1: Tạo sẵn file nhị phân gồm 3 sinh viên
                {
                    StudentRecord init_data[3] = {
                        {1001, "Nguyen Van An",  8.50},
                        {1002, "Tran Thi Binh",  7.25},
                        {1003, "Le Hoang Nam",   9.10}
                    };
                    std::ofstream fout(bin_file, std::ios::binary);
                    fout.write(reinterpret_cast<const char*>(init_data), sizeof(init_data));
                }

                std::cout << "\n[Bai 4] Danh sach ban dau trong '" << bin_file << "':\n";
                print_binary_students(bin_file);

                // Bước 2: Cập nhật trực tiếp GPA của SV 1002 thành 9.85 trên ổ cứng
                if (update_student_gpa_binary(bin_file, 1002, 9.85)) {
                    std::cout << "-> Da cap nhat truc tiep GPA cua SV 1002 thanh 9.85!\n";
                    std::cout << "Danh sach sau khi cap nhat:\n";
                    print_binary_students(bin_file);
                } else {
                    std::cout << "-> Khong tim thay sinh vien co ID 1002!\n";
                }
                break;
            }

            case 5: {
                const std::string log_file = "app.log";
                log_message(log_file, "INFO",  "Khoi dong he thong thanh cong.");
                log_message(log_file, "WARN",  "Bo nho Cache dat nguong 80%.");
                log_message(log_file, "ERROR", "Mat ket noi toi may chu du lieu!");

                std::cout << "\n[Bai 5] Da ghi 3 dong log vao '" << log_file << "'. Noi dung hien tai:\n";
                std::ifstream fin(log_file);
                std::string line;
                while (std::getline(fin, line)) {
                    std::cout << "  " << line << "\n";
                }
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
// ĐỊNH NGHĨA CHI TIẾT CÁC HÀM (FUNCTION DEFINITIONS)
// =============================================================================

// --- Bài 1: Thống kê Lines, Words, Characters ---
bool analyze_text_file(const std::string &filename, int &lines, int &words, int &chars) {
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    lines = 0;
    words = 0;
    chars = 0;

    std::string line;
    while (std::getline(fin, line)) {
        ++lines;
        chars += static_cast<int>(line.length()); // Tính số ký tự trên dòng (không tính '\n')

        // Dùng stringstream để đếm chính xác số từ trên dòng hiện tại
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            ++words;
        }
    }
    return true;
}

// --- Bài 2: Copy file nhị phân theo khối đệm 4KB (Chống tràn RAM với file lớn) ---
bool copy_file_binary(const std::string &src_path, const std::string &dest_path) {
    std::ifstream fin(src_path, std::ios::binary);
    std::ofstream fout(dest_path, std::ios::binary);

    if (!fin.is_open() || !fout.is_open()) return false;

    const std::streamsize BUFFER_SIZE = 4096; // Khối đệm 4 KB (Chuẩn 1 Page Size của OS)
    char buffer[BUFFER_SIZE];

    // Đọc từng khối 4KB cho đến hết file
    while (fin.read(buffer, BUFFER_SIZE) || fin.gcount() > 0) {
        // QUAN TRỌNG: Dùng fin.gcount() để chỉ ghi ĐÚNG số byte thực tế vừa đọc được!
        // (Khối cuối cùng của file thường nhỏ hơn 4096 bytes)
        fout.write(buffer, fin.gcount());
    }

    return true;
}

// --- Bài 3: Đọc dãy số lộn xộn & Xuất báo cáo ---
bool process_numbers_file(const std::string &in_path, const std::string &out_path) {
    std::ifstream fin(in_path);
    if (!fin.is_open()) return false;

    long long num;
    long long sum = 0;
    long long min_val = LLONG_MAX;
    long long max_val = LLONG_MIN;
    int count = 0;

    // Toán tử >> tự động bỏ qua mọi khoảng trắng, tab và xuống dòng lộn xộn!
    while (fin >> num) {
        sum += num;
        if (num < min_val) min_val = num;
        if (num > max_val) max_val = num;
        ++count;
    }

    std::ofstream fout(out_path);
    if (!fout.is_open()) return false;

    if (count == 0) {
        fout << "File khong chua so nguyen nao!\n";
    } else {
        fout << std::fixed << std::setprecision(2);
        fout << "So luong phan tu : " << count << "\n";
        fout << "Tong (Sum)       : " << sum << "\n";
        fout << "Trung binh (Avg) : " << (static_cast<double>(sum) / count) << "\n";
        fout << "Nho nhat (Min)   : " << min_val << "\n";
        fout << "Lon nhat (Max)   : " << max_val << "\n";
    }
    return true;
}

// --- Bài 4: Cập nhật trực tiếp (In-place Update) trong file nhị phân bằng seekp ---
bool update_student_gpa_binary(const std::string &bin_path, int target_id, double new_gpa) {
    // Mở file bằng fstream với cả 3 cờ: Đọc (in) + Ghi (out) + Nhị phân (binary)
    std::fstream fio(bin_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!fio.is_open()) return false;

    StudentRecord sv;
    while (fio.read(reinterpret_cast<char*>(&sv), sizeof(StudentRecord))) {
        if (sv.id == target_id) {
            sv.gpa = new_gpa;

            // QUAN TRỌNG: Sau khi read(), con trỏ file đã trôi qua bản ghi vừa đọc!
            // Phải lùi con trỏ ghi (seekp) lại đúng 1 bản ghi (-sizeof(StudentRecord)) từ vị trí hiện tại (ios::cur)
            fio.seekp(-static_cast<std::streamoff>(sizeof(StudentRecord)), std::ios::cur);
            fio.write(reinterpret_cast<const char*>(&sv), sizeof(StudentRecord));
            return true;
        }
    }
    return false;
}

void print_binary_students(const std::string &bin_path) {
    std::ifstream fin(bin_path, std::ios::binary);
    if (!fin.is_open()) return;

    StudentRecord sv;
    std::cout << std::fixed << std::setprecision(2);
    while (fin.read(reinterpret_cast<char*>(&sv), sizeof(StudentRecord))) {
        std::cout << "  [ID: " << sv.id << "] " 
                  << std::left << std::setw(18) << sv.full_name 
                  << " | GPA: " << sv.gpa << "\n";
    }
}

// --- Bài 5: Ghi log nối đuôi (Append) kèm thời gian thực ---
void log_message(const std::string &log_path, const std::string &level, const std::string &msg) {
    // Luôn dùng std::ios::app để không bao giờ ghi đè mất lịch sử log cũ
    std::ofstream fout(log_path, std::ios::app);
    if (!fout.is_open()) return;

    // Lấy thời gian hệ thống hiện tại và định dạng [YYYY-MM-DD HH:MM:SS]
    std::time_t now = std::time(nullptr);
    std::tm *local_tm = std::localtime(&now);
    char time_buf[32];
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", local_tm);

    fout << "[" << time_buf << "] [" << level << "] " << msg << "\n";
}
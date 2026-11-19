#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

// Struct POD (Plain Old Data) an toàn tuyệt đối cho Binary Serialization
struct StudentRecord {
    int id;
    char full_name[50]; // BẮT BUỘC dùng char[] cố định thay vì std::string!
    double gpa;
};

int main() {
    const std::string bin_file = "students.bin";

    // =========================================================================
    // 1. GHI MẢNG STRUCT NGUYÊN KHỐI XUỐNG FILE NHỊ PHÂN (.bin)
    // =========================================================================
    StudentRecord list[3] = {
        {1001, "Nguyen Van An",  8.50},
        {1002, "Tran Thi Binh",  9.25},
        {1003, "Le Hoang Nam",   7.80}
    };

    {
        // Bắt buộc thêm cờ std::ios::binary để tránh hệ điều hành tự chuyển đổi '\n' thành '\r\n'
        std::ofstream fout(bin_file, std::ios::binary);
        if (!fout.is_open()) {
            std::cerr << "Loi: Khong the mo file nhi phan de ghi!\n";
            return 1;
        }
        // Ép kiểu địa chỉ mảng sang const char* và ghi chính xác (3 * sizeof(StudentRecord)) bytes
        fout.write(reinterpret_cast<const char*>(list), sizeof(list));
    }
    std::cout << "-> Da ghi 3 ban ghi (" << sizeof(list) << " bytes) xuong '" << bin_file << "'.\n\n";

    // =========================================================================
    // 2. TRUY CẬP NGẪU NHIÊN (RANDOM ACCESS) TRONG O(1) BẰNG SEEKG
    // =========================================================================
    std::ifstream fin(bin_file, std::ios::binary);
    if (!fin.is_open()) {
        std::cerr << "Loi: Khong the mo file nhi phan de doc!\n";
        return 1;
    }

    // Bước A: Tính tổng số bản ghi trong file bằng cách nhảy con trỏ xuống cuối file (ios::end)
    fin.seekg(0, std::ios::end);
    std::streamsize file_size = fin.tellg(); // Lấy vị trí byte hiện tại = Tổng dung lượng file
    int total_records = static_cast<int>(file_size / sizeof(StudentRecord));
    std::cout << "Tong dung luong file: " << file_size << " bytes -> Co " << total_records << " sinh vien.\n";

    // Bước B: Nhảy thẳng tới sinh viên thứ 2 (chỉ số k = 1) mà KHÔNG cần đọc sinh viên thứ 1!
    int target_index = 1; // 0-based index (Tran Thi Binh)
    fin.seekg(target_index * sizeof(StudentRecord), std::ios::beg);

    StudentRecord sv;
    fin.read(reinterpret_cast<char*>(&sv), sizeof(StudentRecord));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "-> Doc truc tiep sinh vien tai chi so " << target_index << " trong O(1):\n";
    std::cout << "   ID: " << sv.id << " | Ten: " << sv.full_name << " | GPA: " << sv.gpa << "\n";

    return 0;
}
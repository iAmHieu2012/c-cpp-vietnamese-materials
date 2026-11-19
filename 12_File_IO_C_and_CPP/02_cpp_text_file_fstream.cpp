#include <iostream>
#include <fstream>  // Thư viện đọc/ghi file C++
#include <sstream>  // Tách cột CSV
#include <string>
#include <iomanip>

int main() {
    const std::string csv_file = "students.csv";

    // =========================================================================
    // 1. GHI FILE CSV (std::ofstream) & GHI NỐI ĐUÔI (std::ios::app)
    // =========================================================================
    {
        std::ofstream fout(csv_file); // Mặc định mở ở chế độ ghi đè (ios::out | ios::trunc)
        if (!fout.is_open()) {
            std::cerr << "Loi: Khong the tao file " << csv_file << "\n";
            return 1;
        }
        fout << "1001,Nguyen Van An,8.50\n";
        fout << "1002,Tran Thi Binh,9.25\n";
    } // Ra khỏi dấu '}' -> fout tự động đóng (RAII)!

    // Mở lại ở chế độ ghi nối đuôi (Append) để thêm sinh viên thứ 3 mà không xóa dữ liệu cũ
    {
        std::ofstream fout_app(csv_file, std::ios::app);
        if (fout_app.is_open()) {
            fout_app << "1003,Le Hoang Nam,7.80\n";
        }
    }

    // =========================================================================
    // 2. ĐỌC & PHÂN TÍCH FILE CSV (std::ifstream + std::getline)
    // =========================================================================
    std::ifstream fin(csv_file);
    if (!fin.is_open()) {
        std::cerr << "Loi: Khong the mo file " << csv_file << " de doc!\n";
        return 1;
    }

    std::cout << "=== DOC DU LIEU TU FILE CSV ===\n";
    std::string line;

    // QUY TẮC VÀNG: Luôn đặt std::getline() trực tiếp trong điều kiện while!
    // TUYỆT ĐỐI KHÔNG viết 'while (!fin.eof())' vì sẽ bị đọc lặp dòng cuối cùng 2 lần!
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id_str, name, gpa_str;

        std::getline(ss, id_str, ',');
        std::getline(ss, name, ',');
        std::getline(ss, gpa_str, ',');

        int id = std::stoi(id_str);
        double gpa = std::stod(gpa_str);

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  [ID: " << id << "] " << std::left << std::setw(18) << name 
                  << " | GPA: " << gpa << "\n";
    }

    return 0;
}
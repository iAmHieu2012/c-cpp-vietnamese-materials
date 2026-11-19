#include <iostream>
#include <string>

int main() {
    std::cout << "=== 1. PHEP GAN, NOI CHUOI (+) VA SO SANH TRUC TIEP ===\n";
    std::string first = "Modern";
    std::string second = "C++";
    std::string full = first + " " + second + " Programming"; // Nối chuỗi cực kỳ tự nhiên!

    std::cout << "Chuoi ghep: '" << full << "' (Do dai O(1): " << full.length() << ")\n";
    if (first < second) {
        std::cout << "'" << first << "' > '" << second << "' theo thu tu tu dien.\n\n";
    } else {
        std::cout << "'" << second << "' dung truoc '" << first << "' theo thu tu tu dien.\n\n";
    }


    std::cout << "=== 2. CAC PHUONG THUC COT LOI: FIND, SUBSTR, ERASE, INSERT, REPLACE ===\n";
    std::string text = "Toi hoc lap trinh C tai truong";
    std::cout << "Chuoi ban dau:         '" << text << "'\n";

    // 1. Tìm vị trí xuất hiện của chuỗi con bằng .find()
    // Nếu không tìm thấy, .find() trả về hằng số đặc biệt: std::string::npos
    size_t pos = text.find("C");
    if (pos != std::string::npos) {
        std::cout << "Tim thay chu 'C' tai chi so: " << pos << "\n";
        // 2. Chèn thêm chuỗi vào vị trí pos + 1 bằng .insert(pos, str)
        text.insert(pos + 1, "++");
    }
    std::cout << "Sau khi insert '++':   '" << text << "'\n";

    // 3. Cắt chuỗi con bằng .substr(vi_tri_bat_dau, so_luong_ky_tu)
    std::string sub = text.substr(8, 13); // Lấy 13 ký tự tính từ chỉ số 8 ("lap trinh C++")
    std::cout << "Chuoi con substr(8,13):'" << sub << "'\n";

    // 4. Thay thế bằng .replace(vi_tri, so_ky_tu_xoa, chuoi_thay_the)
    size_t pos_end = text.find("tai truong");
    if (pos_end != std::string::npos) {
        text.replace(pos_end, 10, "chuan cong nghiep!");
    }
    std::cout << "Sau khi replace:       '" << text << "'\n\n";


    std::cout << "=== 3. CHUYEN DOI QUA LAI GIUA CHUOI VA SO (STOI, STOD, TO_STRING) ===\n";
    std::string num_str = "2026";
    std::string pi_str  = "3.14159";

    int year    = std::stoi(num_str);  // String to int (Còn có std::stoll cho long long)
    double pi   = std::stod(pi_str);   // String to double
    std::cout << "Chuyen chuoi thanh so: " << (year + 4) << " | " << (pi * 2) << "\n";

    std::string back_to_str = "Nam: " + std::to_string(year); // Số thành chuỗi
    std::cout << "Chuyen so thanh chuoi: '" << back_to_str << "'\n";

    return 0;
}
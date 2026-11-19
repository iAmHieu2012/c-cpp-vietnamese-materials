#include <iostream>
#include <string>
#include <sstream> // Thư viện chứa std::stringstream
#include <cctype>  // Thư viện kiểm tra/biến đổi ký tự: toupper, tolower, isdigit, isalpha

// Hàm chuẩn hóa Họ Tên chuẩn mực: "   nGUYEN   vAn    aN  " -> "Nguyen Van An"
std::string normalize_full_name(const std::string &raw_name) {
    std::stringstream ss(raw_name); // Đưa chuỗi thô vào luồng stringstream
    std::string word;
    std::string result = "";

    // Toán tử >> tự động đọc từng từ một và bỏ qua toàn bộ dấu cách thừa!
    while (ss >> word) {
        // Viết hoa chữ cái đầu tiên của từ
        word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        // Viết thường tất cả các chữ cái còn lại của từ
        for (size_t i = 1; i < word.length(); ++i) {
            word[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(word[i])));
        }

        if (!result.empty()) {
            result += " "; // Ngăn cách giữa các từ bằng đúng 1 dấu cách
        }
        result += word;
    }
    return result;
}

int main() {
    std::string dirty_name = "   tRAN     tHI   mINH     kHAI   ";
    std::cout << "Chuoi goc chua chuan hoa: '" << dirty_name << "'\n";
    
    std::string clean_name = normalize_full_name(dirty_name);
    std::cout << "Chuoi sau khi chuan hoa : '" << clean_name << "'\n\n";

    // Ứng dụng 2: Tách dữ liệu phân cách bằng dấu phẩy (CSV Parsing) với std::getline
    std::string csv_line = "SV1001,Nguyen Van A,8.75,CNTT";
    std::stringstream csv_ss(csv_line);
    std::string field;

    std::cout << "=== TACH DU LIEU FILE CSV BANG STRINGSTREAM + GETLINE ===\n";
    int col = 1;
    while (std::getline(csv_ss, field, ',')) { // Đọc đến dấu ',' thì ngắt
        std::cout << "  Cot " << col++ << ": " << field << "\n";
    }

    return 0;
}
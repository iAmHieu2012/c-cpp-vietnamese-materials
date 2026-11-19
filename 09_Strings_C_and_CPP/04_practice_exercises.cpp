#include <iostream>
#include <string>
#include <sstream>
#include <cctype>  // isalnum, isdigit, tolower
#include <limits>  // numeric_limits (dùng cho cin.ignore)

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1: Kiểm tra chuỗi đối xứng (chỉ xét chữ và số, không phân biệt Hoa/Thường)
bool is_valid_palindrome(const std::string &s);

// Bài 2: Kiểm tra 2 chuỗi có phải là Đảo chữ (Anagram) của nhau không trong O(N)
bool is_anagram(const std::string &s1, const std::string &s2);

// Bài 3: Nén chuỗi theo thuật toán Run-Length Encoding (RLE: "aaabbc" -> "a3b2c1")
std::string run_length_encode(const std::string &s);

// Bài 4: Tạo địa chỉ Email tự động từ Họ và Tên ("  Nguyen Van Minh " -> "minhnv@company.com")
std::string generate_company_email(const std::string &full_name);

// Bài 5: Tính tổng tất cả các số nguyên dương ẩn bên trong chuỗi hỗn hợp
long long sum_numbers_in_string(const std::string &s);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.4 (XU LY CHUOI C/C++) ==========\n";
        std::cout << "1. Kiem tra Chuoi Doi Xung (Valid Palindrome - Two Pointers)\n";
        std::cout << "2. Kiem tra Chuoi Dao Chu (Valid Anagram - Frequency Array)\n";
        std::cout << "3. Nen chuoi Run-Length Encoding (RLE)\n";
        std::cout << "4. Tao Email cong ty tu dong tu Ho va Ten (stringstream)\n";
        std::cout << "5. Tinh tong cac so nguyen an trong chuoi hon hop\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "==========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        // XÓA BỘ ĐỆM BÀN PHÍM: Loại bỏ ký tự '\n' còn sót lại sau lệnh 'std::cin >> choice'
        // để các lệnh std::getline() bên dưới không bị trôi lệnh!
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::string s;
                std::cout << "\n[Bai 1] Nhap chuoi can kiem tra (VD: A man, a plan, a canal: Panama):\n  > ";
                std::getline(std::cin, s);

                if (is_valid_palindrome(s)) {
                    std::cout << "-> KET QUA: Chuoi DOI XUNG (Valid Palindrome)!\n";
                } else {
                    std::cout << "-> KET QUA: Chuoi KHONG doi xung!\n";
                }
                break;
            }

            case 2: {
                std::string s1, s2;
                std::cout << "\n[Bai 2] Nhap chuoi thu nhat (VD: listen): ";
                std::getline(std::cin, s1);
                std::cout << "        Nhap chuoi thu hai  (VD: silent): ";
                std::getline(std::cin, s2);

                if (is_anagram(s1, s2)) {
                    std::cout << "-> KET QUA: '" << s1 << "' va '" << s2 << "' la ANAGRAM cua nhau!\n";
                } else {
                    std::cout << "-> KET QUA: Hai chuoi KHONG phai la Anagram!\n";
                }
                break;
            }

            case 3: {
                std::string s;
                std::cout << "\n[Bai 3] Nhap chuoi can nen RLE (VD: aaabbcdddd): ";
                std::getline(std::cin, s);

                std::string encoded = run_length_encode(s);
                std::cout << "-> Chuoi sau khi nen RLE: '" << encoded << "'\n";
                break;
            }

            case 4: {
                std::string full_name;
                std::cout << "\n[Bai 4] Nhap Ho va Ten (VD:    nGUYEN   vAn    mINH  ):\n  > ";
                std::getline(std::cin, full_name);

                std::string email = generate_company_email(full_name);
                std::cout << "-> Email duoc cap: " << email << "\n";
                break;
            }

            case 5: {
                std::string s;
                std::cout << "\n[Bai 5] Nhap chuoi chua so (VD: abc120xyz35k9m): ";
                std::getline(std::cin, s);

                long long total = sum_numbers_in_string(s);
                std::cout << "-> Tong cac so nguyen trong chuoi = " << total << "\n";
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

// --- Bài 1: Kỹ thuật 2 con trỏ kiểm tra Palindrome trong O(N) Time, O(1) Space ---
bool is_valid_palindrome(const std::string &s) {
    int left = 0;
    int right = static_cast<int>(s.length()) - 1;

    while (left < right) {
        // Bỏ qua các ký tự không phải chữ cái hoặc chữ số ở bên trái
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) {
            ++left;
        }
        // Bỏ qua các ký tự không phải chữ cái hoặc chữ số ở bên phải
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) {
            --right;
        }

        // So sánh 2 ký tự sau khi đã đưa về chữ thường
        char c_left  = static_cast<char>(std::tolower(static_cast<unsigned char>(s[left])));
        char c_right = static_cast<char>(std::tolower(static_cast<unsigned char>(s[right])));

        if (c_left != c_right) {
            return false; // Phát hiện bất đối xứng -> Dừng ngay lập tức!
        }

        ++left;
        --right;
    }
    return true;
}

// --- Bài 2: Kiểm tra Anagram bằng mảng đếm tần suất 256 mã ASCII trong O(N) ---
bool is_anagram(const std::string &s1, const std::string &s2) {
    // Nếu độ dài khác nhau thì chắc chắn không thể là Anagram
    if (s1.length() != s2.length()) return false;

    int freq[256] = {0}; // Bảng đếm cho toàn bộ 256 ký tự ASCII

    // Duyệt 1 vòng duy nhất: Chuỗi 1 thì cộng tần suất (+1), chuỗi 2 thì trừ tần suất (-1)
    for (size_t i = 0; i < s1.length(); ++i) {
        freq[static_cast<unsigned char>(s1[i])]++;
        freq[static_cast<unsigned char>(s2[i])]--;
    }

    // Nếu 2 chuỗi là Anagram, mọi ô trong bảng tần suất phải triệt tiêu về 0!
    for (int i = 0; i < 256; ++i) {
        if (freq[i] != 0) return false;
    }
    return true;
}

// --- Bài 3: Nén chuỗi Run-Length Encoding (RLE) ---
std::string run_length_encode(const std::string &s) {
    if (s.empty()) return "";

    std::string encoded = "";
    size_t n = s.length();

    for (size_t i = 0; i < n; ++i) {
        int count = 1;
        // Đếm số lượng ký tự giống nhau nằm liên tiếp nhau
        while (i + 1 < n && s[i] == s[i + 1]) {
            ++count;
            ++i;
        }
        encoded += s[i];
        encoded += std::to_string(count);
    }
    return encoded;
}

// --- Bài 4: Tạo Email tự động từ Họ và Tên bằng std::stringstream ---
std::string generate_company_email(const std::string &full_name) {
    std::stringstream ss(full_name);
    std::string word;
    
    // Lưu các từ đã chuyển thành chữ thường vào mảng tạm (Họ tên người thường <= 10 từ)
    std::string words[20];
    int word_count = 0;

    while (ss >> word && word_count < 20) {
        for (size_t i = 0; i < word.length(); ++i) {
            word[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(word[i])));
        }
        words[word_count++] = word;
    }

    if (word_count == 0) return "";

    // Từ cuối cùng chính là Tên chính (First Name - ví dụ: "minh")
    std::string email = words[word_count - 1];

    // Ghép thêm chữ cái đầu tiên của Họ và Tên đệm (từ đầu tiên đến từ áp chót)
    for (int i = 0; i < word_count - 1; ++i) {
        email += words[i][0];
    }

    email += "@company.com";
    return email;
}

// --- Bài 5: Trích xuất và tính tổng các số nguyên ẩn trong chuỗi ---
long long sum_numbers_in_string(const std::string &s) {
    long long total_sum = 0;
    long long current_num = 0;
    bool in_number = false;

    for (size_t i = 0; i < s.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (std::isdigit(c)) {
            // Dịch trái các chữ số cũ (nhân 10) và cộng thêm chữ số mới (c - '0')
            current_num = current_num * 10 + (c - '0');
            in_number = true;
        } else {
            // Ngay khi gặp ký tự chữ làm ngắt mạch số -> Cộng số vừa gom được vào tổng!
            if (in_number) {
                total_sum += current_num;
                current_num = 0;
                in_number = false;
            }
        }
    }

    // CẠM BẪY KINH ĐIỂN: Đừng quên cộng số nằm ở tận cùng cuối chuỗi (ví dụ "abc123")!
    if (in_number) {
        total_sum += current_num;
    }

    return total_sum;
}
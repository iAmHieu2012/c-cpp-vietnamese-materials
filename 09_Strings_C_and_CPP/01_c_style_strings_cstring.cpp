#include <iostream>
#include <cstring> // Thư viện chứa strlen, strcpy, strncpy, strcat, strcmp, strtok

int main() {
    std::cout << "=== 1. BAN CHAT KY TU KET THUC CHUOI '\\0' (NULL-TERMINATOR) ===\n";
    char s1[] = "Hello"; // Trình biên dịch tự động thêm '\0' -> Kích thước mảng là 6 bytes!
    
    // sizeof(s1) trả về tổng số byte của mảng (6), còn strlen(s1) đếm số ký tự TRƯỚC '\0' (5)
    std::cout << "Chuoi s1: '" << s1 << "'\n";
    std::cout << "  - sizeof(s1) (So byte tren RAM, tinh ca '\\0'): " << sizeof(s1) << " bytes\n";
    std::cout << "  - strlen(s1) (So ky tu thuc te, khong tinh '\\0'): " << std::strlen(s1) << " ky tu\n\n";

    // Thử nghiệm: Đặt ký tự '\0' vào giữa chuỗi -> Chuỗi sẽ bị cắt cụt ngay tại đó!
    s1[2] = '\0';
    std::cout << "Sau khi gan s1[2] = '\\0' -> In ra: '" << s1 
              << "' (strlen chi con " << std::strlen(s1) << ")\n\n";


    std::cout << "=== 2. SAO CHEP (STRCPY) & NOI CHUOI (STRCAT) ===\n";
    // CẢNH BÁO: Trong C, KHÔNG THỂ gán trực tiếp 2 mảng: char a[20]; a = "ABC"; (Lỗi biên dịch!)
    char dest[50]; 
    std::strcpy(dest, "Lap trinh ");   // Sao chép chuỗi vào dest
    std::strcat(dest, "C va C++");     // Nối thêm vào đuôi của dest
    std::cout << "Chuoi sau khi strcpy va strcat: '" << dest << "'\n\n";


    std::cout << "=== 3. SO SANH CHUOI BANG STRCMP (KHONG DUNG '==') ===\n";
    const char *a = "Apple";
    const char *b = "Banana";
    // CẠM BẪY: Viết (a == b) là so sánh 2 ĐỊA CHỈ CON TRỎ chứ không phải nội dung chuỗi!
    // Phải dùng strcmp(a, b): Trả về 0 nếu bằng nhau, < 0 nếu a < b, > 0 nếu a > b (theo từ điển ASCII)
    int cmp = std::strcmp(a, b);
    if (cmp == 0)      std::cout << "Hai chuoi giong het nhau.\n";
    else if (cmp < 0)  std::cout << "'" << a << "' dung truoc '" << b << "' trong tu dien.\n\n";
    else               std::cout << "'" << a << "' dung sau '" << b << "' trong tu dien.\n\n";


    std::cout << "=== 4. TACH TU BANG STRTOK (TOKENIZATION TRONG C) ===\n";
    // Lưu ý: strtok sẽ sửa trực tiếp chuỗi gốc (thay các dấu phân cách bằng '\0')
    char raw_text[] = "  Nguyen   Van   An, Ha Noi - Viet Nam ";
    const char *delimiters = " ,-"; // Các ký tự phân cách cần bỏ qua

    std::cout << "Cac tu tach duoc: ";
    char *token = std::strtok(raw_text, delimiters); // Lần gọi đầu tiên truyền chuỗi gốc
    while (token != nullptr) {
        std::cout << "[" << token << "] ";
        token = std::strtok(nullptr, delimiters);    // Các lần gọi sau truyền nullptr để đi tiếp!
    }
    std::cout << "\n";

    return 0;
}
#include <iostream>
#include <iomanip> // Thư viện bắt buộc để định dạng nâng cao (setw, setprecision, setfill...)
#include <string>  // Thư viện xử lý chuỗi chuẩn của C++

int main() {
    // =========================================================================
    // 1. NHẬP DỮ LIỆU & XỬ LÝ LỖI TRÔI LỆNH (std::cin và std::getline)
    // =========================================================================
    int age;
    double gpa;
    char grade;
    std::string full_name;

    std::cout << "Nhap tuoi, GPA va xep loai (cach nhau boi dau cach): ";
    // cin tự động bỏ qua mọi khoảng trắng (Space, Tab, Enter) giữa các biến
    std::cin >> age >> gpa >> grade;

    // CẠM BẪY KINH ĐIỂN TRONG C++:
    // Khi bạn bấm Enter ở lệnh cin phía trên, ký tự '\n' vẫn nằm lại trong bộ đệm.
    // Phải dùng cin.ignore() để xóa ký tự '\n' đó trước khi dùng getline()!
    std::cin.ignore(); 

    std::cout << "Nhap ho va ten day du (co dau cach): ";
    std::getline(std::cin, full_name); // Đọc cả dòng bao gồm khoảng trắng

    std::cout << "\n--- Thong tin vua nhap ---\n";
    std::cout << "Ho ten: " << full_name << "\n"
              << "Tuoi:   " << age << "\n"
              << "Diem:   " << gpa << "\n"
              << "Hang:   " << grade << "\n";
    std::cout << "Dia chi o nho cua age: " << &age << "\n\n"; // Tự động in dạng %p


    // =========================================================================
    // 2. KIỂU BOOLEAN (ĐẶC TRƯNG CỦA C++: std::boolalpha)
    // =========================================================================
    std::cout << "=== 2. KIEU BOOLEAN ===\n";
    bool is_passed = true;
    std::cout << "Mac dinh (in ra so 1/0):    " << is_passed << "\n";
    std::cout << "Dung std::boolalpha (chu):  " << std::boolalpha << is_passed << "\n";
    std::cout << std::noboolalpha << "\n"; // Trả lại mặc định


    // =========================================================================
    // 3. HỆ ĐẾM (DEC, OCT, HEX) - Thay thế cho %d, %o, %x, %#x của C
    // =========================================================================
    std::cout << "=== 3. HE SO DEM (DEC / OCT / HEX) ===\n";
    int num = 255;
    std::cout << "He 10 (dec): " << std::dec << num << "\n";
    std::cout << "He 8  (oct): " << std::oct << num << "\n";
    std::cout << "He 16 (hex): " << std::hex << num << "\n";
    
    // Thêm tiền tố 0x, 0 và viết hoa chữ cái Hex (giống %#X trong C)
    std::cout << "Hex co tien to & chu hoa: " 
              << std::showbase << std::uppercase << std::hex << num << "\n";
    
    // QUAN TRỌNG: Phải trả luồng cout về lại hệ 10 (std::dec) cho các lệnh in sau!
    std::cout << std::dec << std::noshowbase << std::nouppercase << "\n";


    // =========================================================================
    // 4. ĐỊNH DẠNG SỐ THỰC - Thay thế cho %.2f, %e, %g của C
    // =========================================================================
    std::cout << "=== 4. DINH DANG SO THUC ===\n";
    double pi = 3.14159265358979;
    double big_num = 12300000.0;

    std::cout << "Mac dinh (6 chu so co nghia): " << pi << "\n";
    std::cout << "Khoa hoc (scientific - %e):   " << std::scientific << big_num << "\n";
    
    // Cặp bài trùng std::fixed + std::setprecision(n) <=> %.nf trong C
    std::cout << "Lay 2 so sau dau phay (%.2f): " 
              << std::fixed << std::setprecision(2) << pi << "\n";
    std::cout << "Lay 6 so sau dau phay (%.6f): " 
              << std::fixed << std::setprecision(6) << pi << "\n";
    
    std::cout.unsetf(std::ios::fixed | std::ios::scientific); // Reset định dạng số thực
    std::cout << "\n";


    // =========================================================================
    // 5. CANH LỀ & ĐỘ RỘNG - Thay thế cho %5d, %-5d, %05d của C
    // =========================================================================
    std::cout << "=== 5. CANH CHINH DO RONG (ALIGNMENT & PADDING) ===\n";
    int n = 7;
    
    // Lưu ý: std::setw(n) chỉ có tác dụng cho ĐÚNG 1 GIÁ TRỊ ngay sau nó!
    std::cout << "Do rong 5, canh phai (%5d):  [" << std::setw(5) << n << "]\n";
    std::cout << "Do rong 5, canh trai (%-5d): [" << std::left << std::setw(5) << n << "]\n";
    std::cout << "Do rong 5, bu so 0   (%05d): [" << std::right << std::setfill('0') << std::setw(5) << n << "]\n";
    
    std::cout << std::setfill(' '); // Trả lại ký tự lấp đầy mặc định là dấu cách

    return 0;
}
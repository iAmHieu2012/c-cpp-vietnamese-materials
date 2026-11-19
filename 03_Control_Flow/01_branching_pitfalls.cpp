#include <iostream>

int main()
{
    // =========================================================================
    // 1. CẠM BẪY KINH ĐIỂN TRONG IF: NHẦM LẪN GIỮA '=' (GÁN) VÀ '==' (SO SÁNH)
    // =========================================================================
    int x = 0;
    // LỖI NGUY HIỂM: if (x = 5) sẽ gán 5 cho x, mà 5 != 0 nên điều kiện LUÔN ĐÚNG!
    // Hãy luôn dùng '==' để so sánh:
    if (x == 5)
    {
        std::cout << "x bang 5\n";
    }
    else
    {
        std::cout << "x khong bang 5 (x hien tai = " << x << ")\n\n";
    }

    // =========================================================================
    // 2. XẾP LOẠI HỌC LỰC VỚI IF - ELSE IF - ELSE (Kiểm tra điều kiện biên)
    // =========================================================================
    double gpa;
    std::cout << "Nhap diem GPA (0.0 -> 4.0): ";
    std::cin >> gpa;

    if (gpa < 0.0 || gpa > 4.0)
    {
        std::cout << "-> Loi: Diem GPA khong hop le!\n\n";
    }
    else if (gpa >= 3.6)
    {
        std::cout << "-> Xep loai: Xuat sac\n\n";
    }
    else if (gpa >= 3.2)
    { // Không cần viết (gpa < 3.6 && gpa >= 3.2) vì đã lọc ở trên
        std::cout << "-> Xep loai: Gioi\n\n";
    }
    else if (gpa >= 2.5)
    {
        std::cout << "-> Xep loai: Kha\n\n";
    }
    else
    {
        std::cout << "-> Xep loai: Trung binh / Yeu\n\n";
    }

    // =========================================================================
    // 3. SWITCH - CASE & HIỆN TƯỢNG "FALL-THROUGH" (TRÔI LỆNH KHI QUÊN BREAK)
    // =========================================================================
    // Lưu ý: switch CHỈ chấp nhận kiểu số nguyên (int, char, enum), KHÔNG nhận float/double/string!
    int month, year;
    std::cout << "Nhap thang (1-12) va nam (vi du: 2 2024): ";
    std::cin >> month >> year;

    switch (month)
    {
    // Lợi dụng Fall-through có chủ đích để gom nhóm các tháng có 31 ngày:
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        std::cout << "-> Thang " << month << " co 31 ngay.\n";
        break; // Bắt buộc phải có break để thoát khỏi switch!

    case 4:
    case 6:
    case 9:
    case 11:
        std::cout << "-> Thang " << month << " co 30 ngay.\n";
        break;

    case 2:
    {
        // MẸO QUAN TRỌNG: Nếu khai báo biến mới bên trong case, BẮT BUỘC phải mở ngoặc nhọn { }
        bool is_leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
        std::cout << "-> Thang 2 nam " << year << " co " << (is_leap ? 29 : 28) << " ngay.\n";
        break;
    }

    default:
        std::cout << "-> Thang khong hop le!\n";
        break;
    }

    return 0;
}
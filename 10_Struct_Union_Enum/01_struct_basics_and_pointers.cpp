#include <iostream>
#include <string>
#include <iomanip>

// 1. Struct lồng nhau (Nested Struct): Ngày tháng năm sinh
struct Date {
    int day;
    int month;
    int year;
};

// 2. Struct Sinh viên chứa một struct Date bên trong
// Hỗ trợ Default Member Initializer (Khởi tạo giá trị mặc định từ C++11)
struct Student {
    int id = 0;
    std::string full_name = "Unknown";
    Date dob = {1, 1, 2000};
    double gpa = 0.0;
};

// HÀM CHỈ ĐỌC: Luôn truyền bằng Tham chiếu hằng (const Student &s) -> Zero Copy!
void print_student(const Student &s) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  [ID: " << s.id << "] " 
              << std::left << std::setw(18) << s.full_name
              << " | NS: " << std::right << std::setw(2) << std::setfill('0') << s.dob.day << "/"
              << std::setw(2) << s.dob.month << "/" << s.dob.year << std::setfill(' ')
              << " | GPA: " << s.gpa << "\n";
}

// HÀM CẬP NHẬT QUA CON TRỎ: Dùng toán tử mũi tên '->'
void boost_gpa_via_pointer(Student *ptr, double bonus) {
    if (ptr == nullptr) return;
    ptr->gpa += bonus; // Tương đương với: (*ptr).gpa += bonus;
    if (ptr->gpa > 10.0) {
        ptr->gpa = 10.0;
    }
}

int main() {
    std::cout << "=== 1. KHOI TAO TRUC TIEP (UNIFORM INITIALIZATION) & TOAN TU '.' ===\n";
    Student sv1 = {1001, "Nguyen Van An", {15, 8, 2005}, 8.45};
    print_student(sv1);

    std::cout << "\n=== 2. CON TRO STRUCT VA TOAN TU MUI TEN '->' ===\n";
    Student *p_sv = &sv1;
    std::cout << "Truy cap qua con tro p_sv->full_name: " << p_sv->full_name << "\n";
    
    boost_gpa_via_pointer(p_sv, 0.50);
    std::cout << "Sau khi cong 0.5 diem qua con tro:\n";
    print_student(sv1);

    std::cout << "\n=== 3. CAP PHAT DONG MANG STRUCT TREN HEAP ===\n";
    int n = 2;
    Student *class_list = new Student[n]{
        {1002, "Tran Thi Binh", {5, 12, 2005}, 9.10},
        {1003, "Le Hoang Nam",  {20, 3, 2004}, 7.60}
    };

    for (int i = 0; i < n; ++i) {
        // Có thể dùng class_list[i].id hoặc (class_list + i)->id
        print_student(class_list[i]);
    }

    delete[] class_list;
    class_list = nullptr;

    return 0;
}
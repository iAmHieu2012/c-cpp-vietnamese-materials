#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

const double EPS = 1e-9;

// =============================================================================
// ĐỊNH NGHĨA CÁC KIỂU DỮ LIỆU TỰ ĐỊNH NGHĨA (STRUCT, UNION, ENUM CLASS)
// =============================================================================

// Bài 1: Phân số
struct Fraction {
    long long num; // Tử số
    long long den; // Mẫu số
};

// Bài 2: Điểm 2D & Tam giác
struct Point2D {
    double x;
    double y;
};

struct Triangle {
    Point2D A;
    Point2D B;
    Point2D C;
};

// Bài 3: Sinh viên (Thay thế hoàn toàn Mảng song song của Giai đoạn 1)
struct Student {
    int id;
    std::string full_name;
    double gpa;
};

// Bài 4: Mốc thời gian
struct Time {
    int hours;
    int minutes;
    int seconds;
};

// Bài 5: Kiểu dữ liệu đa hình nhẹ (Tagged Union - Tiền đề của std::variant)
enum class DataType {
    INT,
    DOUBLE,
    CHAR
};

struct TaggedValue {
    DataType type;
    union {
        int i_val;
        double d_val;
        char c_val;
    } data;
};

// =============================================================================
// KHAI BÁO NGUYÊN MẪU HÀM (FUNCTION PROTOTYPES)
// =============================================================================

// Bài 1
long long gcd_ll(long long a, long long b);
void simplify_fraction(Fraction &f);
Fraction add_frac(const Fraction &a, const Fraction &b);
Fraction sub_frac(const Fraction &a, const Fraction &b);
Fraction mul_frac(const Fraction &a, const Fraction &b);
Fraction div_frac(const Fraction &a, const Fraction &b);
void print_fraction(const Fraction &f);

// Bài 2
double distance_2d(const Point2D &p1, const Point2D &p2);
double triangle_area(const Triangle &t);
double triangle_perimeter(const Triangle &t);
Point2D triangle_centroid(const Triangle &t);

// Bài 3
void swap_student(Student &a, Student &b);
void sort_students_by_gpa_desc(Student arr[], int n);
void print_students(const Student arr[], int n);

// Bài 4
int time_to_seconds(const Time &t);
Time seconds_to_time(int total_sec);
Time time_difference(const Time &t1, const Time &t2);
void print_time(const Time &t);

// Bài 5
void print_tagged_value(const TaggedValue &v);


// =============================================================================
// HÀM MAIN: MENU ĐIỀU KHIỂN & KIỂM THỬ
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 2.5 (STRUCT, UNION, ENUM) ==========\n";
        std::cout << "1. Phep toan Phan so (struct Fraction) tu dong rut gon\n";
        std::cout << "2. Hinh hoc phang 2D (struct Point2D & Triangle)\n";
        std::cout << "3. Quan ly & Sap xep Sinh vien bang struct Student\n";
        std::cout << "4. Tinh khoang cach thoi gian giua 2 moc gio (struct Time)\n";
        std::cout << "5. Kieu du lieu da hinh nhe (Tagged Union + enum class)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                Fraction f1 = {2, -4};  // Tự rút gọn thành -1/2
                Fraction f2 = {3, 6};   // Tự rút gọn thành 1/2
                std::cout << "\n[Bai 1] Cho 2 phan so ban dau: 2/-4 va 3/6\n";
                simplify_fraction(f1);
                simplify_fraction(f2);

                std::cout << "  f1 (toi gian) = "; print_fraction(f1); std::cout << "\n";
                std::cout << "  f2 (toi gian) = "; print_fraction(f2); std::cout << "\n";
                std::cout << "  f1 + f2 = "; print_fraction(add_frac(f1, f2)); std::cout << "\n";
                std::cout << "  f1 - f2 = "; print_fraction(sub_frac(f1, f2)); std::cout << "\n";
                std::cout << "  f1 * f2 = "; print_fraction(mul_frac(f1, f2)); std::cout << "\n";
                std::cout << "  f1 / f2 = "; print_fraction(div_frac(f1, f2)); std::cout << "\n";
                break;
            }

            case 2: {
                Triangle tri = {{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}};
                double area = triangle_area(tri);

                std::cout << std::fixed << std::setprecision(2);
                std::cout << "\n[Bai 2] Tam giac A(0, 0), B(4, 0), C(0, 3):\n";
                if (area < EPS) {
                    std::cout << "-> 3 diem thang hang, khong tao thanh tam giac!\n";
                } else {
                    Point2D G = triangle_centroid(tri);
                    std::cout << "  - Chu vi (Perimeter) : " << triangle_perimeter(tri) << "\n";
                    std::cout << "  - Dien tich (Area)   : " << area << "\n";
                    std::cout << "  - Trong tam G        : (" << G.x << ", " << G.y << ")\n";
                }
                break;
            }

            case 3: {
                Student list[] = {
                    {1001, "Nguyen Van An",  8.20},
                    {1002, "Tran Thi Binh",  9.45},
                    {1003, "Le Hoang Nam",   7.80},
                    {1004, "Pham Minh Chau", 9.45}
                };
                int n = sizeof(list) / sizeof(list[0]);

                std::cout << "\n[Bai 3] Danh sach truoc khi sap xep:\n";
                print_students(list, n);

                sort_students_by_gpa_desc(list, n);
                std::cout << "\n-> Danh sach sau khi sap xep GIAM DAN theo GPA (dong diem xep theo ID):\n";
                print_students(list, n);
                break;
            }

            case 4: {
                Time t1 = {8, 45, 30};
                Time t2 = {14, 15, 10};
                Time diff = time_difference(t1, t2);

                std::cout << "\n[Bai 4] Moc 1: "; print_time(t1);
                std::cout << " | Moc 2: "; print_time(t2);
                std::cout << "\n-> Khoang cach thoi gian: "; print_time(diff);
                std::cout << "\n";
                break;
            }

            case 5: {
                TaggedValue items[3];
                items[0].type = DataType::INT;    items[0].data.i_val = 2026;
                items[1].type = DataType::DOUBLE; items[1].data.d_val = 3.14159;
                items[2].type = DataType::CHAR;   items[2].data.c_val = 'A';

                std::cout << "\n[Bai 5] Mang TaggedValue chua 3 kieu du lieu khac nhau:\n";
                std::cout << "  (Kich thuoc moi TaggedValue = " << sizeof(TaggedValue) << " bytes)\n";
                for (int i = 0; i < 3; ++i) {
                    std::cout << "  items[" << i << "]: ";
                    print_tagged_value(items[i]);
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

// --- Bài 1: Phân số ---
long long gcd_ll(long long a, long long b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return (a == 0) ? 1 : a;
}

void simplify_fraction(Fraction &f) {
    if (f.den == 0) {
        std::cout << "[Loi: Mau so bang 0!] ";
        return;
    }
    // Luôn chuẩn hóa đẩy dấu trừ lên tử số để mẫu luôn dương
    if (f.den < 0) {
        f.num = -f.num;
        f.den = -f.den;
    }
    if (f.num == 0) {
        f.den = 1; // 0/5 -> 0/1
        return;
    }
    long long g = gcd_ll(f.num, f.den);
    f.num /= g;
    f.den /= g;
}

Fraction add_frac(const Fraction &a, const Fraction &b) {
    Fraction res = {a.num * b.den + b.num * a.den, a.den * b.den};
    simplify_fraction(res);
    return res;
}

Fraction sub_frac(const Fraction &a, const Fraction &b) {
    Fraction res = {a.num * b.den - b.num * a.den, a.den * b.den};
    simplify_fraction(res);
    return res;
}

Fraction mul_frac(const Fraction &a, const Fraction &b) {
    Fraction res = {a.num * b.num, a.den * b.den};
    simplify_fraction(res);
    return res;
}

Fraction div_frac(const Fraction &a, const Fraction &b) {
    if (b.num == 0) {
        std::cout << "[Loi: Chia cho phan so 0!] ";
        return {0, 1};
    }
    Fraction res = {a.num * b.den, a.den * b.num};
    simplify_fraction(res);
    return res;
}

void print_fraction(const Fraction &f) {
    if (f.den == 1) std::cout << f.num;
    else            std::cout << f.num << "/" << f.den;
}

// --- Bài 2: Hình học phẳng 2D ---
double distance_2d(const Point2D &p1, const Point2D &p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

double triangle_area(const Triangle &t) {
    // Công thức tọa độ (Shoelace / Tích có hướng): Chính xác hơn Heron, không cần tính căn
    double val = t.A.x * (t.B.y - t.C.y) +
                 t.B.x * (t.C.y - t.A.y) +
                 t.C.x * (t.A.y - t.B.y);
    return 0.5 * std::abs(val);
}

double triangle_perimeter(const Triangle &t) {
    return distance_2d(t.A, t.B) + distance_2d(t.B, t.C) + distance_2d(t.C, t.A);
}

Point2D triangle_centroid(const Triangle &t) {
    return {
        (t.A.x + t.B.x + t.C.x) / 3.0,
        (t.A.y + t.B.y + t.C.y) / 3.0
    };
}

// --- Bài 3: Sinh viên với struct ---
void swap_student(Student &a, Student &b) {
    Student temp = a;
    a = b;
    b = temp;
}

void sort_students_by_gpa_desc(Student arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int best = i;
        for (int j = i + 1; j < n; ++j) {
            bool higher_gpa = (arr[j].gpa - arr[best].gpa) > EPS;
            bool tie_smaller_id = (std::abs(arr[j].gpa - arr[best].gpa) < EPS) && (arr[j].id < arr[best].id);
            if (higher_gpa || tie_smaller_id) {
                best = j;
            }
        }
        if (best != i) {
            // Chỉ cần 1 lệnh swap duy nhất, không lo lệch dữ liệu như Mảng song song!
            swap_student(arr[i], arr[best]);
        }
    }
}

void print_students(const Student arr[], int n) {
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < n; ++i) {
        std::cout << "  [ID: " << arr[i].id << "] " 
                  << std::left << std::setw(18) << arr[i].full_name 
                  << " | GPA: " << arr[i].gpa << "\n";
    }
}

// --- Bài 4: Khoảng cách thời gian ---
int time_to_seconds(const Time &t) {
    return t.hours * 3600 + t.minutes * 60 + t.seconds;
}

Time seconds_to_time(int total_sec) {
    Time t;
    t.hours   = total_sec / 3600;
    total_sec %= 3600;
    t.minutes = total_sec / 60;
    t.seconds = total_sec % 60;
    return t;
}

Time time_difference(const Time &t1, const Time &t2) {
    int diff_sec = std::abs(time_to_seconds(t1) - time_to_seconds(t2));
    return seconds_to_time(diff_sec);
}

void print_time(const Time &t) {
    std::cout << std::setw(2) << std::setfill('0') << t.hours << ":"
              << std::setw(2) << std::setfill('0') << t.minutes << ":"
              << std::setw(2) << std::setfill('0') << t.seconds << std::setfill(' ');
}

// --- Bài 5: Tagged Union ---
void print_tagged_value(const TaggedValue &v) {
    switch (v.type) {
        case DataType::INT:
            std::cout << "[INT]    gia tri = " << v.data.i_val << "\n";
            break;
        case DataType::DOUBLE:
            std::cout << "[DOUBLE] gia tri = " << v.data.d_val << "\n";
            break;
        case DataType::CHAR:
            std::cout << "[CHAR]   gia tri = '" << v.data.c_val << "'\n";
            break;
    }
}
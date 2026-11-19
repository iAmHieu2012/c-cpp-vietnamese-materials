#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

const double EPS = 1e-9;

// =============================================================================
// BÀI 1: LỚP RECTANGLE (VALIDATION & METHOD CHAINING VỚI THIS)
// =============================================================================
class Rectangle {
private:
    double width;
    double height;

public:
    Rectangle(double w = 1.0, double h = 1.0)
        : width(w > 0.0 ? w : 1.0), height(h > 0.0 ? h : 1.0) {}

    Rectangle& set_width(double w) {
        if (w > 0.0) this->width = w;
        return *this;
    }

    Rectangle& set_height(double h) {
        if (h > 0.0) this->height = h;
        return *this;
    }

    double get_width() const { return width; }
    double get_height() const { return height; }
    double area() const { return width * height; }
    double perimeter() const { return 2.0 * (width + height); }
    bool is_square() const { return std::abs(width - height) < EPS; }
};


// =============================================================================
// BÀI 2: LỚP STUDENT TỰ ĐỘNG CẤP ID TĂNG DẦN BẰNG STATIC
// =============================================================================
class Student {
private:
    int id;
    std::string full_name;
    double gpa;

    static int next_id;
    static int active_count;

public:
    Student(const std::string &name, double g)
        : id(next_id++), full_name(name), gpa((g >= 0.0 && g <= 10.0) ? g : 0.0) {
        ++active_count;
    }

    ~Student() {
        --active_count;
    }

    void print() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  [ID: " << id << "] " << std::left << std::setw(18) << full_name
                  << " | GPA: " << gpa << "\n";
    }

    static int get_student_count() { return active_count; }
};

int Student::next_id = 1001;
int Student::active_count = 0;


// =============================================================================
// BÀI 3: LỚP VECTOR2D KẾT HỢP FRIEND FUNCTION
// =============================================================================
class Vector2D {
private:
    double x;
    double y;

public:
    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    friend double dot_product(const Vector2D &a, const Vector2D &b);
    friend double distance_between(const Vector2D &a, const Vector2D &b);
};

double dot_product(const Vector2D &a, const Vector2D &b) {
    return a.x * b.x + a.y * b.y;
}

double distance_between(const Vector2D &a, const Vector2D &b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}


// =============================================================================
// BÀI 4: LỚP INTARRAY QUẢN LÝ MẢNG ĐỘNG TRÊN HEAP (RAII & DEEP COPY)
// =============================================================================
class IntArray {
private:
    int *data;
    int size;

public:
    IntArray(int n = 0) : data(nullptr), size(n > 0 ? n : 0) {
        if (size > 0) {
            data = new int[size]();
        }
    }

    // Copy Constructor thực hiện Deep Copy (tránh 2 object trỏ cùng 1 vùng Heap -> Double Free)
    IntArray(const IntArray &other) : data(nullptr), size(other.size) {
        if (size > 0 && other.data != nullptr) {
            data = new int[size];
            for (int i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    ~IntArray() {
        delete[] data;
        data = nullptr;
    }

    void set(int idx, int val) {
        if (idx >= 0 && idx < size) data[idx] = val;
    }

    int get(int idx) const {
        return (idx >= 0 && idx < size) ? data[idx] : 0;
    }

    void print(const std::string &name) const {
        std::cout << "  " << name << " (Addr: " << data << "): ";
        for (int i = 0; i < size; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};


// =============================================================================
// BÀI 5: LỚP BANKVAULT VÀ LỚP BẠN AUDITOR (FRIEND CLASS)
// =============================================================================
class BankVault {
private:
    std::string vault_code;
    double total_cash;

public:
    BankVault(const std::string &code, double cash)
        : vault_code(code), total_cash(cash >= 0.0 ? cash : 0.0) {}

    friend class Auditor; // Chỉ Auditor được phép kiểm toán dữ liệu private
};

class Auditor {
private:
    std::string auditor_name;

public:
    Auditor(const std::string &name) : auditor_name(name) {}

    void audit_vault(const BankVault &vault) const {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "  [Kiem toan vien: " << auditor_name << "]\n";
        std::cout << "    - Ma ket sat : " << vault.vault_code << "\n";
        std::cout << "    - Tong tien  : " << vault.total_cash << " VND\n";
    }
};


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 3.1 (CLASS & OBJECT) ==========\n";
        std::cout << "1. Lop Rectangle (Validation & Method Chaining)\n";
        std::cout << "2. Lop Student tu dong cap ID bang bien static\n";
        std::cout << "3. Lop Vector2D voi friend function (Dot & Distance)\n";
        std::cout << "4. Lop IntArray quan ly Heap (RAII & Deep Copy)\n";
        std::cout << "5. Lop BankVault & Auditor (friend class)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=======================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                Rectangle r(-5.0, 4.0); // -5.0 tự ép về 1.0
                std::cout << "\n[Bai 1] Khoi tao (-5, 4) -> W=" << r.get_width()
                          << ", H=" << r.get_height() << "\n";

                r.set_width(6.0).set_height(6.0); // Method Chaining
                std::cout << "Sau chaining (6, 6): Area = " << r.area()
                          << " | Perimeter = " << r.perimeter()
                          << " | Hinh vuong? " << (r.is_square() ? "Co" : "Khong") << "\n";
                break;
            }

            case 2: {
                std::cout << "\n[Bai 2] So SV hien tai: " << Student::get_student_count() << "\n";
                {
                    Student s1("Nguyen Van An", 8.5);
                    Student s2("Tran Thi Binh", 9.2);
                    Student s3("Le Hoang Nam", 7.8);
                    s1.print();
                    s2.print();
                    s3.print();
                    std::cout << "-> Tong SV dang ton tai trong scope: "
                              << Student::get_student_count() << "\n";
                }
                std::cout << "-> Sau khi thoat scope, so SV con lai: "
                          << Student::get_student_count() << "\n";
                break;
            }

            case 3: {
                Vector2D v1(3.0, 4.0);
                Vector2D v2(6.0, 8.0);
                std::cout << "\n[Bai 3] v1 = "; v1.print();
                std::cout << ", v2 = "; v2.print(); std::cout << "\n";
                std::cout << "-> Tich vo huong (Dot Product) : " << dot_product(v1, v2) << "\n";
                std::cout << "-> Khoang cach (Distance)      : " << distance_between(v1, v2) << "\n";
                break;
            }

            case 4: {
                std::cout << "\n[Bai 4] Kiem thu Deep Copy tren Heap:\n";
                IntArray a1(4);
                for (int i = 0; i < 4; ++i) a1.set(i, (i + 1) * 10);

                IntArray a2 = a1; // Gọi Copy Constructor (Deep Copy)
                a2.set(0, 999);   // Sửa a2 không ảnh hưởng a1

                a1.print("a1 (goc) ");
                a2.print("a2 (copy)");
                break;
            }

            case 5: {
                BankVault vault("VAULT-VN-999", 5000000000.0);
                Auditor aud("Tran Kiem Toan");
                std::cout << "\n[Bai 5] Kiem toan ket sat qua friend class:\n";
                aud.audit_vault(vault);
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
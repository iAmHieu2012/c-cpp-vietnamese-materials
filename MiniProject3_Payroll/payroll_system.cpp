#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
#include <utility>

const double EPS = 1e-9;

// =============================================================================
// 1. LỚP TRỪU TƯỢNG (ABSTRACT CLASS): EMPLOYEE
// =============================================================================
class Employee {
protected:
    int id;
    std::string full_name;
    double base_salary;
    static int next_id;

public:
    Employee(const std::string &name, double base)
        : id(next_id++), full_name(name), base_salary(base >= 0.0 ? base : 0.0) {}

    // Constructor bảo toàn ID cũ khi thực hiện Deep Copy (clone)
    Employee(int existing_id, const std::string &name, double base)
        : id(existing_id), full_name(name), base_salary(base) {}

    // BẮT BUỘC: Virtual Destructor để tránh Memory Leak khi xóa qua Employee*
    virtual ~Employee() = default;

    // Các phương thức thuần ảo (Pure Virtual Functions)
    virtual double calc_salary() const = 0;
    virtual std::string get_role() const = 0;
    virtual std::string get_extra_info() const = 0;

    // Virtual Copy Constructor (Prototype Pattern cơ bản) phục vụ Deep Copy đa hình
    virtual Employee* clone() const = 0;

    int get_id() const { return id; }
    std::string get_name() const { return full_name; }

    // Nạp chồng toán tử << hỗ trợ in đa hình
    friend std::ostream& operator<<(std::ostream &os, const Employee &emp) {
        os << std::fixed << std::setprecision(0);
        os << "| " << std::left << std::setw(6) << emp.id
           << " | " << std::setw(22) << emp.full_name
           << " | " << std::setw(12) << emp.get_role()
           << " | " << std::setw(24) << emp.get_extra_info()
           << " | " << std::right << std::setw(13) << emp.calc_salary() << " |";
        return os;
    }
};

int Employee::next_id = 1001;


// =============================================================================
// 2. CÁC LỚP DẪN XUẤT (DERIVED CLASSES)
// =============================================================================
class OfficeStaff : public Employee {
private:
    int work_days;
    double daily_allowance;

public:
    OfficeStaff(const std::string &name, double base, int days, double allowance)
        : Employee(name, base), work_days(days > 0 ? days : 0),
          daily_allowance(allowance >= 0.0 ? allowance : 0.0) {}

    OfficeStaff(int existing_id, const std::string &name, double base, int days, double allowance)
        : Employee(existing_id, name, base), work_days(days), daily_allowance(allowance) {}

    double calc_salary() const override {
        return base_salary + (work_days * daily_allowance);
    }

    std::string get_role() const override { return "OfficeStaff"; }

    std::string get_extra_info() const override {
        return std::to_string(work_days) + " ngay cong";
    }

    Employee* clone() const override {
        return new OfficeStaff(id, full_name, base_salary, work_days, daily_allowance);
    }
};

class Engineer : public Employee {
private:
    int overtime_hours;
    double overtime_rate;

public:
    Engineer(const std::string &name, double base, int ot_hours, double ot_rate)
        : Employee(name, base), overtime_hours(ot_hours > 0 ? ot_hours : 0),
          overtime_rate(ot_rate >= 0.0 ? ot_rate : 0.0) {}

    Engineer(int existing_id, const std::string &name, double base, int ot_hours, double ot_rate)
        : Employee(existing_id, name, base), overtime_hours(ot_hours), overtime_rate(ot_rate) {}

    double calc_salary() const override {
        return base_salary + (overtime_hours * overtime_rate);
    }

    std::string get_role() const override { return "Engineer"; }

    std::string get_extra_info() const override {
        return std::to_string(overtime_hours) + "h OT";
    }

    Employee* clone() const override {
        return new Engineer(id, full_name, base_salary, overtime_hours, overtime_rate);
    }
};

class Manager : public Employee {
private:
    double position_allowance;
    int team_size;

public:
    Manager(const std::string &name, double base, double allowance, int team)
        : Employee(name, base), position_allowance(allowance >= 0.0 ? allowance : 0.0),
          team_size(team > 0 ? team : 0) {}

    Manager(int existing_id, const std::string &name, double base, double allowance, int team)
        : Employee(existing_id, name, base), position_allowance(allowance), team_size(team) {}

    double calc_salary() const override {
        return base_salary + position_allowance + (team_size * 500000.0);
    }

    std::string get_role() const override { return "Manager"; }

    std::string get_extra_info() const override {
        return "Team: " + std::to_string(team_size) + " nguoi";
    }

    Employee* clone() const override {
        return new Manager(id, full_name, base_salary, position_allowance, team_size);
    }
};


// =============================================================================
// 3. LỚP QUẢN LÝ HỆ THỐNG: PAYROLLSYSTEM (TUÂN THỦ RULE OF FIVE)
// =============================================================================
class PayrollSystem {
private:
    Employee **data;
    int size;
    int capacity;

    void resize(int new_cap) {
        Employee **new_data = new Employee*[new_cap];
        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_cap;
    }

    void clear_memory() {
        for (int i = 0; i < size; ++i) {
            delete data[i]; // Gọi Virtual Destructor của từng đối tượng con
        }
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }

public:
    explicit PayrollSystem(int init_cap = 4)
        : data(new Employee*[init_cap]), size(0), capacity(init_cap) {}

    // 1. Destructor (RAII)
    ~PayrollSystem() {
        clear_memory();
    }

    // 2. Copy Constructor (Deep Copy đa hình nhờ virtual clone())
    PayrollSystem(const PayrollSystem &other)
        : data(other.capacity > 0 ? new Employee*[other.capacity] : nullptr),
          size(other.size), capacity(other.capacity) {
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i]->clone();
        }
    }

    // 3. Copy Assignment Operator
    PayrollSystem& operator=(const PayrollSystem &other) {
        if (this == &other) return *this;
        clear_memory();
        capacity = other.capacity;
        size = other.size;
        data = (capacity > 0) ? new Employee*[capacity] : nullptr;
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i]->clone();
        }
        return *this;
    }

    // 4. Move Constructor (O(1))
    PayrollSystem(PayrollSystem &&other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // 5. Move Assignment Operator (O(1))
    PayrollSystem& operator=(PayrollSystem &&other) noexcept {
        if (this == &other) return *this;
        clear_memory();
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    int get_size() const { return size; }
    int get_capacity() const { return capacity; }

    void add_employee(Employee *emp) {
        if (emp == nullptr) return;
        if (size == capacity) {
            resize(capacity == 0 ? 4 : capacity * 2);
        }
        data[size++] = emp;
    }

    int find_index_by_id(int id) const {
        for (int i = 0; i < size; ++i) {
            if (data[i]->get_id() == id) return i;
        }
        return -1;
    }

    bool remove_by_id(int id) {
        int idx = find_index_by_id(id);
        if (idx == -1) return false;

        delete data[idx]; // Giải phóng đối tượng bị xóa trên Heap
        for (int i = idx; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
        return true;
    }

    void print_all() const {
        if (size == 0) {
            std::cout << "\n-> Danh sach nhan vien dang rong!\n";
            return;
        }
        std::cout << "\n+--------+------------------------+--------------+--------------------------+---------------+\n";
        std::cout << "| " << std::left << std::setw(6)  << "ID"
                  << " | " << std::setw(22) << "Ho va Ten"
                  << " | " << std::setw(12) << "Chuc vu"
                  << " | " << std::setw(24) << "Chi tiet"
                  << " | " << std::right << std::setw(13) << "Thuc linh" << " |\n";
        std::cout << "+--------+------------------------+--------------+--------------------------+---------------+\n";
        for (int i = 0; i < size; ++i) {
            std::cout << *data[i] << "\n"; // Gọi operator<< đa hình
        }
        std::cout << "+--------+------------------------+--------------+--------------------------+---------------+\n";
    }

    void search_by_id(int id) const {
        int idx = find_index_by_id(id);
        if (idx == -1) {
            std::cout << "-> Khong tim thay nhan vien co ID " << id << "!\n";
            return;
        }
        std::cout << "-> Tim thay:\n" << *data[idx] << "\n";
    }

    void show_statistics() const {
        if (size == 0) {
            std::cout << "\n-> Danh sach rong, khong co du lieu thong ke!\n";
            return;
        }
        double total = 0.0, max_sal = data[0]->calc_salary();
        double sum_office = 0.0, sum_eng = 0.0, sum_mgr = 0.0;

        for (int i = 0; i < size; ++i) {
            double sal = data[i]->calc_salary();
            total += sal;
            if (sal > max_sal) max_sal = sal;

            std::string role = data[i]->get_role();
            if (role == "OfficeStaff")    sum_office += sal;
            else if (role == "Engineer")  sum_eng += sal;
            else if (role == "Manager")   sum_mgr += sal;
        }

        std::cout << std::fixed << std::setprecision(0);
        std::cout << "\n=============== THONG KE QUY LUONG ===============\n";
        std::cout << "  - Tong nhan su      : " << size << " nguoi\n";
        std::cout << "  - Tong quy luong    : " << total << " VND\n";
        std::cout << "  - Luong trung binh  : " << (total / size) << " VND\n";
        std::cout << "  - Thu nhap cao nhat (" << max_sal << " VND): ";
        for (int i = 0; i < size; ++i) {
            if (std::abs(data[i]->calc_salary() - max_sal) < EPS) {
                std::cout << "[" << data[i]->get_id() << " - " << data[i]->get_name() << "] ";
            }
        }
        std::cout << "\n  - Phan bo theo chuc vu:\n";
        std::cout << "      + OfficeStaff : " << sum_office << " VND\n";
        std::cout << "      + Engineer    : " << sum_eng    << " VND\n";
        std::cout << "      + Manager     : " << sum_mgr    << " VND\n";
        std::cout << "==================================================\n";
    }

    void sort_by_salary_desc() {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - 1 - i; ++j) {
                double s1 = data[j]->calc_salary();
                double s2 = data[j + 1]->calc_salary();
                bool swap_needed = (s1 < s2 - EPS) ||
                                   (std::abs(s1 - s2) < EPS && data[j]->get_id() > data[j + 1]->get_id());
                if (swap_needed) std::swap(data[j], data[j + 1]);
            }
        }
    }

    void sort_by_id_asc() {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - 1 - i; ++j) {
                if (data[j]->get_id() > data[j + 1]->get_id()) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};


// =============================================================================
// 4. HÀM HỖ TRỢ NHẬP LIỆU & MAIN
// =============================================================================
void input_new_employee(PayrollSystem &sys) {
    int type;
    std::cout << "\nChon loai nhan vien (1: OfficeStaff | 2: Engineer | 3: Manager): ";
    std::cin >> type;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (type < 1 || type > 3) {
        std::cout << "-> Loai nhan vien khong hop le!\n";
        return;
    }

    std::string name;
    double base;
    std::cout << "Nhap Ho va Ten: ";
    std::getline(std::cin, name);
    std::cout << "Nhap luong co ban (VND): ";
    std::cin >> base;

    if (type == 1) {
        int days; double allow;
        std::cout << "Nhap so ngay cong va phu cap moi ngay: ";
        std::cin >> days >> allow;
        sys.add_employee(new OfficeStaff(name, base, days, allow));
    } else if (type == 2) {
        int ot_h; double ot_rate;
        std::cout << "Nhap so gio OT va don gia OT/gio: ";
        std::cin >> ot_h >> ot_rate;
        sys.add_employee(new Engineer(name, base, ot_h, ot_rate));
    } else {
        double pos_allow; int team;
        std::cout << "Nhap phu cap chuc vu va so nhan vien quan ly: ";
        std::cin >> pos_allow >> team;
        sys.add_employee(new Manager(name, base, pos_allow, team));
    }
    std::cout << "-> Da them nhan vien thanh cong!\n";
}

int main() {
    PayrollSystem sys(4);

    // Khởi tạo sẵn 3 nhân viên mẫu để test ngay
    sys.add_employee(new OfficeStaff("Nguyen Van An",  10000000, 22, 100000));
    sys.add_employee(new Engineer   ("Tran Thi Binh",  18000000, 15, 250000));
    sys.add_employee(new Manager    ("Le Hoang Nam",   25000000, 5000000, 8));

    int choice;
    do {
        std::cout << "\n============ QUAN LY QUY LUONG (STAGE 3 - OOP) ============\n";
        std::cout << "  [Heap Status: Size = " << sys.get_size() << " | Capacity = " << sys.get_capacity() << "]\n";
        std::cout << "1. In bang luong toan cong ty\n";
        std::cout << "2. Them nhan vien moi (OfficeStaff / Engineer / Manager)\n";
        std::cout << "3. Xoa nhan vien theo ID\n";
        std::cout << "4. Tra cuu nhan vien theo ID\n";
        std::cout << "5. Thong ke quy luong\n";
        std::cout << "6. Sap xep bang luong (Thuc linh giam dan / ID tang dan)\n";
        std::cout << "7. Kiem thu Deep Copy (Rule of Five + Virtual clone())\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "===========================================================\n";
        std::cout << "Nhap lua chon (0-7): ";
        std::cin >> choice;

        switch (choice) {
            case 1: sys.print_all(); break;
            case 2: input_new_employee(sys); break;
            case 3: {
                int id;
                std::cout << "\nNhap ID nhan vien can xoa: ";
                std::cin >> id;
                if (sys.remove_by_id(id)) std::cout << "-> Da xoa nhan vien ID " << id << ".\n";
                else                      std::cout << "-> Khong tim thay ID " << id << "!\n";
                break;
            }
            case 4: {
                int id;
                std::cout << "\nNhap ID can tra cuu: ";
                std::cin >> id;
                sys.search_by_id(id);
                break;
            }
            case 5: sys.show_statistics(); break;
            case 6: {
                int mode;
                std::cout << "\nChon kieu sap xep (1: Thuc linh giam dan | 2: ID tang dan): ";
                std::cin >> mode;
                if (mode == 1)      { sys.sort_by_salary_desc(); sys.print_all(); }
                else if (mode == 2) { sys.sort_by_id_asc();      sys.print_all(); }
                else                { std::cout << "-> Lua chon khong hop le!\n"; }
                break;
            }
            case 7: {
                std::cout << "\n[Test Deep Copy] Tao ban sao 'backup = sys' va xoa ID 1001 o ban sao:\n";
                PayrollSystem backup = sys; // Gọi Copy Constructor -> clone() từng nhân viên
                backup.remove_by_id(1001);
                std::cout << "  -> Size ban goc (sys)    = " << sys.get_size() << " (khong bi anh huong)\n";
                std::cout << "  -> Size ban sao (backup) = " << backup.get_size() << "\n";
                std::cout << "  -> Thoat case 7: ~backup tu dong huy an toan, khong Double Free!\n";
                break;
            }
            case 0:
                std::cout << "-> Destructor tu dong thu hoi Heap. Tam biet!\n";
                break;
            default:
                std::cout << "-> Lua chon khong hop le!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
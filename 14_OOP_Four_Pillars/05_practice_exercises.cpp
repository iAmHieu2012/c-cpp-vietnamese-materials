#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

// =============================================================================
// BÀI 1: LỚP FRACTION VỚI OPERATOR OVERLOADING ĐẦY ĐỦ (+, -, *, /, ==, <, <<)
// =============================================================================
class Fraction {
private:
    long long num;
    long long den;

    static long long gcd_ll(long long a, long long b) {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0) {
            long long r = a % b;
            a = b;
            b = r;
        }
        return (a == 0) ? 1 : a;
    }

    void simplify() {
        if (den == 0) den = 1;
        if (den < 0) {
            num = -num;
            den = -den;
        }
        if (num == 0) {
            den = 1;
            return;
        }
        long long g = gcd_ll(num, den);
        num /= g;
        den /= g;
    }

public:
    Fraction(long long n = 0, long long d = 1) : num(n), den(d) {
        simplify();
    }

    Fraction operator+(const Fraction &o) const { return Fraction(num * o.den + o.num * den, den * o.den); }
    Fraction operator-(const Fraction &o) const { return Fraction(num * o.den - o.num * den, den * o.den); }
    Fraction operator*(const Fraction &o) const { return Fraction(num * o.num, den * o.den); }
    Fraction operator/(const Fraction &o) const {
        if (o.num == 0) return Fraction(0, 1);
        return Fraction(num * o.den, den * o.num);
    }

    bool operator==(const Fraction &o) const { return num == o.num && den == o.den; }
    bool operator<(const Fraction &o) const { return num * o.den < o.num * den; }

    friend std::ostream& operator<<(std::ostream &os, const Fraction &f) {
        if (f.den == 1) os << f.num;
        else            os << f.num << "/" << f.den;
        return os;
    }
};


// =============================================================================
// BÀI 2: HỆ THỐNG ĐỘNG VẬT ĐA HÌNH (ABSTRACT CLASS ANIMAL)
// =============================================================================
class Animal {
protected:
    std::string name;
public:
    Animal(const std::string &n) : name(n) {}
    virtual void speak() const = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    Dog(const std::string &n) : Animal(n) {}
    void speak() const override { std::cout << "  [Dog]  " << name << ": Gau gau!\n"; }
};

class Cat : public Animal {
public:
    Cat(const std::string &n) : Animal(n) {}
    void speak() const override { std::cout << "  [Cat]  " << name << ": Meo meo!\n"; }
};

class Duck : public Animal {
public:
    Duck(const std::string &n) : Animal(n) {}
    void speak() const override { std::cout << "  [Duck] " << name << ": Quack quack!\n"; }
};


// =============================================================================
// BÀI 3: GIAO DIỆN THANH TOÁN (INTERFACE IPAYMENT)
// =============================================================================
class IPayment {
public:
    virtual void pay(double amount) const = 0;
    virtual ~IPayment() = default;
};

class CreditCardPayment : public IPayment {
private:
    std::string card_last4;
public:
    CreditCardPayment(const std::string &last4) : card_last4(last4) {}
    void pay(double amount) const override {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "  [CreditCard ****" << card_last4 << "] Da thanh toan " << amount << " VND.\n";
    }
};

class MomoPayment : public IPayment {
private:
    std::string phone;
public:
    MomoPayment(const std::string &p) : phone(p) {}
    void pay(double amount) const override {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "  [Momo " << phone << "] Da thanh toan " << amount << " VND.\n";
    }
};

// Hàm checkout hoàn toàn độc lập với phương thức thanh toán cụ thể (Dependency Inversion)
void checkout(const IPayment &method, double total) {
    std::cout << "-> Dang xu ly don hang...\n";
    method.pay(total);
}


// =============================================================================
// BÀI 4: HỆ THỐNG LƯƠNG NHÂN VIÊN ĐA HÌNH (ABSTRACT CLASS EMPLOYEE)
// =============================================================================
class Employee {
protected:
    std::string full_name;
public:
    Employee(const std::string &name) : full_name(name) {}
    virtual double calc_salary() const = 0;
    std::string get_name() const { return full_name; }
    virtual ~Employee() = default;
};

class FullTimeEmployee : public Employee {
private:
    double base_salary;
    double bonus;
public:
    FullTimeEmployee(const std::string &name, double base, double b)
        : Employee(name), base_salary(base), bonus(b) {}
    double calc_salary() const override { return base_salary + bonus; }
};

class PartTimeEmployee : public Employee {
private:
    int hours;
    double hourly_rate;
public:
    PartTimeEmployee(const std::string &name, int h, double rate)
        : Employee(name), hours(h), hourly_rate(rate) {}
    double calc_salary() const override { return hours * hourly_rate; }
};


// =============================================================================
// BÀI 5: GIẢI QUYẾT DIAMOND PROBLEM (PERSON -> STUDENT, WORKER -> WORKINGSTUDENT)
// =============================================================================
class Person {
protected:
    std::string full_name;
public:
    Person(const std::string &name = "Unknown") : full_name(name) {}
    std::string get_name() const { return full_name; }
};

class Student : virtual public Person {
protected:
    int student_id;
public:
    Student(const std::string &name, int id) : Person(name), student_id(id) {}
};

class Worker : virtual public Person {
protected:
    double salary;
public:
    Worker(const std::string &name, double sal) : Person(name), salary(sal) {}
};

class WorkingStudent : public Student, public Worker {
public:
    WorkingStudent(const std::string &name, int id, double sal)
        : Person(name), Student(name, id), Worker(name, sal) {}

    void print_profile() const {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "  Ho Ten: " << full_name // Không nhập nhằng nhờ virtual public Person
                  << " | Ma SV: " << student_id
                  << " | Luong: " << salary << " VND\n";
    }
};


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 3.2 (4 PILLARS OF OOP) ==========\n";
        std::cout << "1. Nap chong toan tu cho lop Fraction (+, -, *, /, ==, <, <<)\n";
        std::cout << "2. Da hinh Runtime voi Abstract Class Animal (Zoo)\n";
        std::cout << "3. Giao dien thanh toan IPayment (CreditCard & Momo)\n";
        std::cout << "4. Tinh tong quy luong cong ty (FullTime & PartTime)\n";
        std::cout << "5. Da ke thua giai quyet Diamond Problem (WorkingStudent)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "=========================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                Fraction f1(2, -4); // -1/2
                Fraction f2(3, 4);  // 3/4
                std::cout << "\n[Bai 1] f1 = " << f1 << ", f2 = " << f2 << "\n";
                std::cout << "  f1 + f2 = " << (f1 + f2) << "\n";
                std::cout << "  f1 - f2 = " << (f1 - f2) << "\n";
                std::cout << "  f1 * f2 = " << (f1 * f2) << "\n";
                std::cout << "  f1 / f2 = " << (f1 / f2) << "\n";
                std::cout << "  f1 < f2 ? " << (f1 < f2 ? "True" : "False") << "\n";
                break;
            }

            case 2: {
                std::cout << "\n[Bai 2] Vuon thu da hinh (Animal*):\n";
                Animal *zoo[3] = {
                    new Dog("Milo"),
                    new Cat("Kitty"),
                    new Duck("Donald")
                };
                for (int i = 0; i < 3; ++i) zoo[i]->speak();
                for (int i = 0; i < 3; ++i) delete zoo[i];
                break;
            }

            case 3: {
                std::cout << "\n[Bai 3] Thanh toan qua Interface IPayment:\n";
                CreditCardPayment visa("8899");
                MomoPayment momo("0901234567");
                checkout(visa, 1250000);
                checkout(momo, 350000);
                break;
            }

            case 4: {
                std::cout << "\n[Bai 4] Bang luong cong ty:\n";
                Employee *staff[3] = {
                    new FullTimeEmployee("Nguyen Van An", 15000000, 3000000),
                    new PartTimeEmployee("Tran Thi Binh", 80, 65000),
                    new FullTimeEmployee("Le Hoang Nam",  20000000, 5000000)
                };
                double total_payroll = 0.0;
                std::cout << std::fixed << std::setprecision(0);
                for (int i = 0; i < 3; ++i) {
                    double sal = staff[i]->calc_salary();
                    total_payroll += sal;
                    std::cout << "  - " << std::left << std::setw(18) << staff[i]->get_name()
                              << " : " << sal << " VND\n";
                }
                std::cout << "-> Tong quy luong: " << total_payroll << " VND\n";
                for (int i = 0; i < 3; ++i) delete staff[i];
                break;
            }

            case 5: {
                std::cout << "\n[Bai 5] Ho so WorkingStudent (Virtual Inheritance):\n";
                WorkingStudent ws("Pham Minh Chau", 202601, 7500000);
                ws.print_profile();
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

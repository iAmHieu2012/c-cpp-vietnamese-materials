#include <iostream>
#include <cstring>
#include <cstdio>
#include <utility> // std::move, std::swap
#include <iomanip>

// =============================================================================
// BÀI 1: LỚP MYSTRING TUÂN THỦ RULE OF THREE (+ NẠP CHỒNG + VÀ <<)
// =============================================================================
class MyString {
private:
    char *data;
    size_t len;

public:
    MyString(const char *s = "") {
        len = std::strlen(s);
        data = new char[len + 1];
        std::strcpy(data, s);
    }

    // 1. Destructor
    ~MyString() {
        delete[] data;
    }

    // 2. Copy Constructor
    MyString(const MyString &other) : len(other.len) {
        data = new char[len + 1];
        std::strcpy(data, other.data);
    }

    // 3. Copy Assignment Operator
    MyString& operator=(const MyString &other) {
        if (this == &other) return *this;
        delete[] data;
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
        return *this;
    }

    MyString operator+(const MyString &other) const {
        size_t new_len = len + other.len;
        char *buf = new char[new_len + 1];
        std::strcpy(buf, data);
        std::strcat(buf, other.data);
        MyString res(buf);
        delete[] buf;
        return res;
    }

    friend std::ostream& operator<<(std::ostream &os, const MyString &s) {
        return os << s.data;
    }
};


// =============================================================================
// BÀI 2: LỚP MATRIX2D (MẢNG PHẲNG 1D TRÊN HEAP) TUÂN THỦ RULE OF FIVE
// =============================================================================
class Matrix2D {
private:
    size_t rows, cols;
    double *data;

public:
    Matrix2D(size_t r = 0, size_t c = 0)
        : rows(r), cols(c), data((r * c > 0) ? new double[r * c]() : nullptr) {}

    ~Matrix2D() { delete[] data; }

    // Copy Constructor
    Matrix2D(const Matrix2D &other)
        : rows(other.rows), cols(other.cols),
          data((other.rows * other.cols > 0) ? new double[other.rows * other.cols] : nullptr) {
        for (size_t i = 0; i < rows * cols; ++i) data[i] = other.data[i];
    }

    // Copy Assignment
    Matrix2D& operator=(const Matrix2D &other) {
        if (this == &other) return *this;
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = (rows * cols > 0) ? new double[rows * cols] : nullptr;
        for (size_t i = 0; i < rows * cols; ++i) data[i] = other.data[i];
        return *this;
    }

    // Move Constructor
    Matrix2D(Matrix2D &&other) noexcept
        : rows(other.rows), cols(other.cols), data(other.data) {
        other.rows = other.cols = 0;
        other.data = nullptr;
    }

    // Move Assignment
    Matrix2D& operator=(Matrix2D &&other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        other.rows = other.cols = 0;
        other.data = nullptr;
        return *this;
    }

    double& at(size_t r, size_t c) { return data[r * cols + c]; }
    double at(size_t r, size_t c) const { return data[r * cols + c]; }

    void print(const char *label) const {
        std::cout << "  " << label << " (" << rows << "x" << cols << "):\n";
        for (size_t i = 0; i < rows; ++i) {
            std::cout << "    ";
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(5) << at(i, j);
            }
            std::cout << "\n";
        }
    }
};


// =============================================================================
// BÀI 3: KỸ THUẬT COPY-AND-SWAP IDIOM CHO OPERATOR=
// =============================================================================
class SwapBuffer {
private:
    int *data;
    size_t size;

public:
    SwapBuffer(size_t n = 0, int init_val = 0)
        : data(n > 0 ? new int[n] : nullptr), size(n) {
        for (size_t i = 0; i < size; ++i) data[i] = init_val;
    }

    ~SwapBuffer() { delete[] data; }

    SwapBuffer(const SwapBuffer &other)
        : data(other.size > 0 ? new int[other.size] : nullptr), size(other.size) {
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
    }

    SwapBuffer(SwapBuffer &&other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // Copy-and-Swap Idiom: Gộp chung cả Copy Assignment & Move Assignment, tự an toàn Self-Assignment
    SwapBuffer& operator=(SwapBuffer other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
        return *this;
    }

    void print(const char *label) const {
        std::cout << "  " << label << " (size=" << size << "): ";
        for (size_t i = 0; i < size; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};


// =============================================================================
// BÀI 4: LỚP UNIQUEFILE (RAII CHO FILE*, CẤM COPY, CHỈ CHO PHÉP MOVE)
// =============================================================================
class UniqueFile {
private:
    FILE *fp;

public:
    UniqueFile(const char *filename, const char *mode) {
        fp = std::fopen(filename, mode);
    }

    ~UniqueFile() {
        if (fp != nullptr) {
            std::fclose(fp);
            std::cout << "  [~UniqueFile] Da tu dong dong file (RAII).\n";
        }
    }

    // Cấm sao chép để tránh 2 đối tượng cùng gọi fclose() trên 1 FILE*
    UniqueFile(const UniqueFile&) = delete;
    UniqueFile& operator=(const UniqueFile&) = delete;

    // Cho phép chuyển giao quyền sở hữu (Move)
    UniqueFile(UniqueFile &&other) noexcept : fp(other.fp) {
        other.fp = nullptr;
    }

    UniqueFile& operator=(UniqueFile &&other) noexcept {
        if (this != &other) {
            if (fp != nullptr) std::fclose(fp);
            fp = other.fp;
            other.fp = nullptr;
        }
        return *this;
    }

    bool is_open() const { return fp != nullptr; }

    void write_line(const char *text) {
        if (fp != nullptr) std::fprintf(fp, "%s\n", text);
    }
};


// =============================================================================
// BÀI 5: TỰ VIẾT SIMPLEUNIQUEPTR (TIỀN ĐỀ CỦA STD::UNIQUE_PTR)
// =============================================================================
class SimpleUniquePtr {
private:
    int *ptr;

public:
    explicit SimpleUniquePtr(int *p = nullptr) : ptr(p) {}

    ~SimpleUniquePtr() {
        delete ptr;
    }

    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;

    SimpleUniquePtr(SimpleUniquePtr &&other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    SimpleUniquePtr& operator=(SimpleUniquePtr &&other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    int& operator*() const { return *ptr; }
    int* operator->() const { return ptr; }
    int* get() const { return ptr; }
};


// =============================================================================
// HÀM MAIN: MENU KIỂM THỬ 5 BÀI TẬP
// =============================================================================
int main() {
    int choice;
    do {
        std::cout << "\n========== MENU BAI TAP 3.3 (RESOURCE MANAGEMENT) ==========\n";
        std::cout << "1. Lop MyString (Rule of Three + toan tu + va <<)\n";
        std::cout << "2. Lop Matrix2D mang phang Heap (Rule of Five)\n";
        std::cout << "3. Ky thuat Copy-and-Swap Idiom cho operator=\n";
        std::cout << "4. Lop UniqueFile (RAII cho FILE*, Move-Only)\n";
        std::cout << "5. Tu viet SimpleUniquePtr (Tien de std::unique_ptr)\n";
        std::cout << "0. Thoat chuong trinh\n";
        std::cout << "============================================================\n";
        std::cout << "Nhap lua chon (0-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                MyString s1("Modern ");
                MyString s2("C++17");
                MyString s3 = s1 + s2; // Gọi operator+
                MyString s4;
                s4 = s3;               // Gọi Copy Assignment
                std::cout << "\n[Bai 1] s1 + s2 = \"" << s3 << "\" | s4 (copy) = \"" << s4 << "\"\n";
                break;
            }

            case 2: {
                Matrix2D m1(2, 2);
                m1.at(0, 0) = 1; m1.at(0, 1) = 2;
                m1.at(1, 0) = 3; m1.at(1, 1) = 4;

                Matrix2D m2 = m1;            // Copy Constructor
                Matrix2D m3 = std::move(m1); // Move Constructor (m1 trở thành 0x0)

                std::cout << "\n[Bai 2] Kiem thu Matrix2D (Rule of Five):\n";
                m1.print("m1 (sau khi bi move)");
                m2.print("m2 (copy tu m1)    ");
                m3.print("m3 (move tu m1)    ");
                break;
            }

            case 3: {
                SwapBuffer b1(3, 10);
                SwapBuffer b2(5, 99);
                b1 = b2;            // Copy-and-Swap với Lvalue
                b2 = SwapBuffer(2, 7); // Copy-and-Swap với Rvalue
                std::cout << "\n[Bai 3] Ket qua Copy-and-Swap Idiom:\n";
                b1.print("b1");
                b2.print("b2");
                break;
            }

            case 4: {
                std::cout << "\n[Bai 4] Kiem thu UniqueFile (Move-Only RAII):\n";
                {
                    UniqueFile f1("raii_test.txt", "w");
                    if (f1.is_open()) {
                        f1.write_line("Dong 1: Ghi tu f1.");
                    }
                    // UniqueFile f2 = f1; // LỖI BIÊN DỊCH vì Copy đã bị = delete!
                    UniqueFile f2 = std::move(f1); // Chuyển quyền sở hữu sang f2
                    std::cout << "  f1.is_open() = " << (f1.is_open() ? "True" : "False")
                              << " | f2.is_open() = " << (f2.is_open() ? "True" : "False") << "\n";
                    f2.write_line("Dong 2: Ghi tu f2 sau khi std::move.");
                } // Ra khỏi scope -> ~f2 tự động fclose()
                break;
            }

            case 5: {
                std::cout << "\n[Bai 5] Kiem thu SimpleUniquePtr:\n";
                SimpleUniquePtr p1(new int(42));
                std::cout << "  *p1 = " << *p1 << " (Addr: " << p1.get() << ")\n";

                SimpleUniquePtr p2 = std::move(p1);
                std::cout << "  Sau std::move -> p1.get() = " << p1.get()
                          << " | *p2 = " << *p2 << " (Addr: " << p2.get() << ")\n";
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
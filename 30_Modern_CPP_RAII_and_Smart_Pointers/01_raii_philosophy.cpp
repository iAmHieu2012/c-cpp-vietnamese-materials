// ============================================================================
// File: 01_raii_philosophy.cpp
// Bài 6.3: Triết lý RAII (Resource Acquisition Is Initialization & Exception Safety)
// ============================================================================

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

// ============================================================================
// 1. TỰ CÀI ĐẶT LỚP RAII QUẢN LÝ FILE C (GẮN VÒNG ĐỜI FILE* VỚI ĐỐI TƯỢNG STACK)
// ============================================================================
class ScopedFileGuard {
    std::FILE *fp_ = nullptr;
    std::string filename_;

public:
    ScopedFileGuard(const std::string &filename, const char *mode)
        : fp_(std::fopen(filename.c_str(), mode)), filename_(filename) {
        if (!fp_) {
            throw std::runtime_error("Khong the mo file: " + filename_);
        }
        std::cout << "  [RAII Constructor] Da mo file: " << filename_ << "\n";
    }

    // Destructor tự động chạy 100% khi biến ra khỏi khối {} (kể cả khi có Exception)
    ~ScopedFileGuard() {
        if (fp_) {
            std::fclose(fp_);
            std::cout << "  [~RAII Destructor] Da tu dong fclose() file: "
                      << filename_ << "\n";
        }
    }

    // Cấm Copy để tránh 2 đối tượng cùng fclose() 1 con trỏ FILE* (Double Free)
    ScopedFileGuard(const ScopedFileGuard &) = delete;
    ScopedFileGuard &operator=(const ScopedFileGuard &) = delete;

    void writeLine(const std::string &line) const {
        std::fprintf(fp_, "%s\n", line.c_str());
    }
};

// ============================================================================
// 2. TỰ CÀI ĐẶT LỚP RAII QUẢN LÝ MẢNG ĐỘNG TRÊN HEAP
// ============================================================================
class ScopedIntBuffer {
    int *buffer_;
    size_t size_;

public:
    explicit ScopedIntBuffer(size_t n) : buffer_(new int[n]()), size_(n) {
        std::cout << "  [RAII Constructor] Cap phat Heap " << size_ << " ints.\n";
    }

    ~ScopedIntBuffer() {
        delete[] buffer_;
        std::cout << "  [~RAII Destructor] Tu dong delete[] " << size_
                  << " ints (Khong bao gio Memory Leak!).\n";
    }

    ScopedIntBuffer(const ScopedIntBuffer &) = delete;
    ScopedIntBuffer &operator=(const ScopedIntBuffer &) = delete;

    int &operator[](size_t idx) { return buffer_[idx]; }
};

void simulateRiskyOperation(bool throwError) {
    ScopedFileGuard logFile("raii_audit.log", "w");
    ScopedIntBuffer tempBuf(1024);

    logFile.writeLine("Bat dau giao dich quan trong...");
    tempBuf[0] = 999;

    if (throwError) {
        std::cout << "  [!] Phat sinh ngoai le dot ngot giua ham!\n";
        throw std::runtime_error("Mat ket noi co so du lieu!");
    }

    logFile.writeLine("Giao dich hoan tat!");
}

int main() {
    std::cout << "=== TRIET LY RAII: GAN VONG DOI TAI NGUYEN VOI STACK ===\n\n";

    std::cout << "--- Kich ban 1: Thoat khoi scope binh thuong ---\n";
    simulateRiskyOperation(false);

    std::cout << "\n--- Kich ban 2: Nem Exception giua chung (Stack Unwinding) ---\n";
    try {
        simulateRiskyOperation(true);
    } catch (const std::exception &ex) {
        std::cout << "  [Catch] Da bat ngoai le: " << ex.what() << "\n";
    }

    std::remove("raii_audit.log");
    return 0;
}

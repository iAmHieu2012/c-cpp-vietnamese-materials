#include <iostream>
#include <iomanip>

int main() {
    int rows = 3, cols = 4;

    // =========================================================================
    // CÁCH 1: DÙNG CON TRỎ CẤP 2 (int **mat) - MỖI HÀNG LÀ 1 MẢNG ĐỘNG RIÊNG
    // =========================================================================
    std::cout << "=== CACH 1: MA TRAN DONG BANG CON TRO CAP 2 (int **mat) ===\n";
    
    // Bước 1: Cấp phát mảng quản lý gồm 'rows' con trỏ cấp 1 (int*)
    int **mat2d = new int*[rows];

    // Bước 2: Với mỗi con trỏ hàng, cấp phát 1 mảng gồm 'cols' số nguyên
    for (int i = 0; i < rows; ++i) {
        mat2d[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            mat2d[i][j] = (i + 1) * 10 + (j + 1);
        }
    }

    // In ma trận và địa chỉ bắt đầu của từng hàng (Bạn sẽ thấy các hàng cách xa nhau!)
    for (int i = 0; i < rows; ++i) {
        std::cout << "  Hang " << i << " (Dia chi: " << mat2d[i] << "): ";
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << mat2d[i][j];
        }
        std::cout << "\n";
    }

    // Bước 3: GIẢI PHÓNG THEO THỨ TỰ NGƯỢC LẠI (Xóa từng hàng con trước -> Xóa mảng cha sau!)
    for (int i = 0; i < rows; ++i) {
        delete[] mat2d[i]; // Giải phóng từng hàng
    }
    delete[] mat2d;        // Giải phóng mảng các con trỏ quản lý
    mat2d = nullptr;


    // =========================================================================
    // CÁCH 2: TRẢI PHẲNG THÀNH 1 KHỐI LIÊN TIẾP (CONTIGUOUS 1D BLOCK - TỐI ƯU NHẤT)
    // =========================================================================
    std::cout << "\n=== CACH 2: MA TRAN DONG LIEN TIEP 1 KHOI (CONTIGUOUS 1D) ===\n";
    
    // Chỉ cần 1 lần cấp phát duy nhất cho toàn bộ (rows * cols) phần tử!
    int *flat_mat = new int[rows * cols];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Công thức ánh xạ tọa độ 2D (i, j) sang 1D: [i * cols + j]
            flat_mat[i * cols + j] = (i + 1) * 100 + (j + 1);
        }
    }

    for (int i = 0; i < rows; ++i) {
        std::cout << "  Hang " << i << " (Dia chi: " << &flat_mat[i * cols] << "): ";
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(5) << flat_mat[i * cols + j];
        }
        std::cout << "\n";
    }

    // Giải phóng cực gọn chỉ với 1 dòng lệnh duy nhất:
    delete[] flat_mat;
    flat_mat = nullptr;

    return 0;
}
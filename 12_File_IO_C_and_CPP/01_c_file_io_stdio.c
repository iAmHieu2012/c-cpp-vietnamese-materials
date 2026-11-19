#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *filename = "c_notes.txt";

    // =========================================================================
    // 1. GHI FILE VĂN BẢN (Chế độ "w": Tạo mới nếu chưa có, ghi đè nếu đã có)
    // =========================================================================
    FILE *f_out = fopen(filename, "w");
    if (f_out == NULL) { // LUÔN kiểm tra NULL phòng trường hợp không có quyền ghi hoặc sai đường dẫn
        printf("Loi: Khong the mo file '%s' de ghi!\n", filename);
        return 1;
    }

    fprintf(f_out, "Dong 1: Lap trinh C voi FILE*\n");
    fprintf(f_out, "Dong 2: Nam %d - Phien ban %.1f\n", 2026, 1.0);
    fclose(f_out); // Bắt buộc đóng file để đẩy dữ liệu từ Buffer xuống ổ cứng!
    printf("-> Da ghi thanh cong vao '%s'.\n\n", filename);

    // =========================================================================
    // 2. ĐỌC FILE VĂN BẢN TỪNG DÒNG (Chế độ "r": Chỉ đọc)
    // =========================================================================
    FILE *f_in = fopen(filename, "r");
    if (f_in == NULL) {
        printf("Loi: Khong tim thay file '%s' de doc!\n", filename);
        return 1;
    }

    printf("=== NOI DUNG DOC TU '%s' ===\n", filename);
    char buffer[256];
    // Dùng fgets() an toàn hơn fscanf() vì chống tràn bộ đệm và đọc được cả dấu cách
    while (fgets(buffer, sizeof(buffer), f_in) != NULL) {
        printf("  %s", buffer); // buffer đã chứa sẵn ký tự '\n' từ file
    }

    fclose(f_in);
    return 0;
}
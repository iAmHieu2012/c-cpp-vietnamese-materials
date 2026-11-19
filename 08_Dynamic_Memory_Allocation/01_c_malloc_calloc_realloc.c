#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = 5;

    // 1. MALLOC: Xin cấp phát 5 số int (5 * 4 = 20 bytes), chứa giá trị rác
    int *arr_m = (int*)malloc(n * sizeof(int));
    if (arr_m == NULL) { // LUÔN kiểm tra NULL phòng trường hợp hết RAM
        printf("Loi: Khong du bo nho Heap!\n");
        return 1;
    }
    for (int i = 0; i < n; ++i) arr_m[i] = (i + 1) * 10;

    // 2. CALLOC: Xin cấp phát 5 số int và tự động điền toàn bộ = 0
    int *arr_c = (int*)calloc(n, sizeof(int));
    printf("=== 1. SO SANH MALLOC (DA GAN) VA CALLOC (TU DONG = 0) ===\n");
    printf("arr_m (malloc): ");
    for (int i = 0; i < n; ++i) printf("%d ", arr_m[i]);
    printf("\narr_c (calloc): ");
    for (int i = 0; i < n; ++i) printf("%d ", arr_c[i]);
    printf("\n\n");

    // 3. REALLOC: Mở rộng mảng arr_m từ 5 phần tử lên 8 phần tử
    printf("=== 2. MO RONG MANG DONG VOI REALLOC (5 -> 8 PHAN TU) ===\n");
    int new_n = 8;
    // QUY TẮC VÀNG: Luôn hứng kết quả realloc vào một con trỏ tạm (temp_ptr)!
    // Vì nếu realloc thất bại nó trả về NULL, nếu gán thẳng arr_m = realloc(arr_m, ...) 
    // sẽ làm mất luôn địa chỉ mảng cũ gây Memory Leak!
    int *temp_ptr = (int*)realloc(arr_m, new_n * sizeof(int));
    if (temp_ptr != NULL) {
        arr_m = temp_ptr; // Cập nhật con trỏ sang vùng nhớ mới thành công
        for (int i = n; i < new_n; ++i) {
            arr_m[i] = (i + 1) * 10; // Điền tiếp các phần tử mới (60, 70, 80)
        }
        n = new_n;
    }

    printf("arr_m sau khi realloc len %d phan tu: ", n);
    for (int i = 0; i < n; ++i) printf("%d ", arr_m[i]);
    printf("\n");

    // 4. FREE: Giải phóng bộ nhớ và gán NULL để chống Dangling Pointer
    free(arr_m);
    arr_m = NULL;

    free(arr_c);
    arr_c = NULL;

    return 0;
}
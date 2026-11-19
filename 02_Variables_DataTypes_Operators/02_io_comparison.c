#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main() {
    // Thiết lập locale ngay đầu hàm main để hỗ trợ Unicode tốt nhất
    setlocale(LC_ALL, "");

    int age;
    double gpa;
    char grade;

    printf("Nhap tuoi, diem trung binh, xep loai (cach nhau boi dau cach): ");
    // LƯU Ý: Chỉ để dấu cách giữa các specifier. 
    // Dấu cách trước %c rất quan trọng: giúp bỏ qua mọi khoảng trắng/Enter thừa trước khi đọc ký tự.
    if (scanf("%d %lf %c", &age, &gpa, &grade) == 3) {
        printf("Thong tin:\n- Tuoi: %d\n- Diem: %.2lf\n- Xep loai: %c\n", age, gpa, grade);
    } else {
        printf("Loi: Ban nhap sai dinh dang!\n");
    }
    printf("Dia chi o nho cua bien age: %p\n\n", (void*)&age);


    printf("=== 1. KIEU SO NGUYEN (INTEGERS) ===\n");
    int i = -123;
    unsigned int u = 456;
    short h = -12;
    unsigned short hu = 34;
    long l = -123456L;
    unsigned long lu = 789012UL;
    long long ll = -9876543210LL;
    unsigned long long llu = 123456789012ULL;
    size_t sz = sizeof(long long);
    
    printf("%%d   (int co dau):          %d\n", i);
    printf("%%i   (int co dau):          %i\n", i);
    printf("%%u   (unsigned int):        %u\n", u);
    printf("%%hd  (short):               %hd\n", h);
    printf("%%hu  (unsigned short):      %hu\n", hu);
    printf("%%ld  (long):                %ld\n", l);
    printf("%%lu  (unsigned long):       %lu\n", lu);
    printf("%%lld (long long):           %lld\n", ll);
    printf("%%llu (unsigned long long):  %llu\n\n", llu);
    printf("%%zu  (size_t - kich thuoc):  %zu bytes (Kich thuoc ban than size_t: %zu bytes)\n\n", sz, sizeof(size_t));
    

    printf("=== 2. KIEU KY TU VA CHUOI (CHARACTERS & STRINGS) ===\n");
    char c = 'A';
    char str[] = "Xin chao";
    wchar_t wc = L'Ω';             // Ky tu rong Omega
    wchar_t wstr[] = L"Unicode C"; // Chuoi ky tu rong

    printf("%%c  (char don):      %c\n", c);
    printf("%%s  (chuoi char):    %s\n", str);
    printf("%%lc (wchar_t don):   %lc\n", wc);
    printf("%%ls (chuoi wchar_t): %ls\n\n", wstr);


    printf("=== 3. KIEU SO THUC (FLOATING-POINT NUMBERS) ===\n");
    float f = 3.14159f;
    double d = 1234.56789;
    long double Ld = 98765.43210L;

    printf("%%f  (float/double tieu chuan):     %f\n", f);
    printf("%%lf (double in ra giong %%f):       %lf\n", d);
    printf("%%Lf (long double):                 %Lf\n", Ld);
    printf("%%e  (dang khoa hoc, chu thuong):   %e\n", d);
    printf("%%E  (dang khoa hoc, chu hoa):      %E\n", d);
    printf("%%g  (tu dong rut gon tu %%f/%%e):    %g\n", 0.0000123);
    printf("%%G  (tu dong rut gon, chu hoa):    %G\n\n", 12300000.0);


    printf("=== 4. HE SO DEM KHAC (OCTAL & HEXADECIMAL) ===\n");
    int num = 255;
    printf("So goc he 10 la:                      %d\n", num);
    printf("%%o  (Bat phan - Octal):               %o (Co tien to: %#o)\n", num, num);
    printf("%%x  (Thap luc phan - Hex chu thuong): %x (Co tien to: %#x)\n", num, num);
    printf("%%X  (Thap luc phan - Hex chu hoa):    %X (Co tien to: %#X)\n\n", num, num);


    printf("=== 5. DINH DANG DAC BIET KHAC ===\n");
    int count_chars = 0;

    printf("%%p  (Dia chi con tro):        %p\n", (void*)&i);
    printf("%%%%  (In ra ky tu phan tram):  100%%\n");
    
    // %n dem so ky tu da duoc in truoc do trong cung lenh printf va luu vao bien
    printf("Dem ky tu tai day ->%n va luu vao bien.\n", &count_chars);
    printf("So ky tu da in truoc dau mui ten la: %d\n\n", count_chars);


    printf("=== 6. MEO TUY BIEN NANG CAO (CANH CHINH) ===\n");
    int n = 7;
    float pi = 3.14159265f;
    
    printf("%%5d   (Do rong 5, mac dinh canh phai): [%5d]\n", n);
    printf("%%-5d  (Do rong 5, canh trai):          [%-5d]\n", n);
    printf("%%05d  (Do rong 5, bu so 0):            [%05d]\n", n);
    printf("%%.2f  (Gioi han 2 chu so thap phan):   [%.2f]\n", pi);
    printf("%%7.3f (Do rong 7, lay 3 so thap phan): [%7.3f]\n", pi);

    return 0;
}
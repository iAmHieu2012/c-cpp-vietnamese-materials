# Bài 7.5: Nguyên lý Thiết kế Hướng đối tượng SOLID (SOLID Principles)

## 1. Mục tiêu

Nắm vững **5 nguyên lý SOLID** của Robert C. Martin (Uncle Bob) — nền tảng cốt lõi của Clean Architecture và mọi Design Pattern hiện đại. Mỗi nguyên lý được tách thành **1 file `.cpp` độc lập**, trình bày trực quan theo cấu trúc **Trước khi Refactor (Vi phạm - Anti-Pattern)** và **Sau khi Refactor (Tuân thủ SOLID)**:

- **S — Single Responsibility Principle (SRP - Nguyên lý Đơn trách nhiệm):** Một lớp chỉ nên có **duy nhất một lý do để thay đổi** (tách biệt Nghiệp vụ tính toán, Định dạng báo cáo và Lưu trữ dữ liệu).
- **O — Open/Closed Principle (OCP - Nguyên lý Đóng/Mở):** Các thực thể phần mềm nên **Mở để mở rộng (Open for Extension)** nhưng **Đóng để chỉnh sửa (Closed for Modification)** — thêm tính năng mới bằng Đa hình thay vì sửa chuỗi `if-else` / `switch-case` cũ.
- **L — Liskov Substitution Principle (LSP - Nguyên lý Thay thế Liskov):** Các đối tượng của lớp con phải có thể thay thế lớp cha mà không làm hỏng tính đúng đắn hay gây bất ngờ (Exception/Side-effect sai lệch) cho chương trình.
- **I — Interface Segregation Principle (ISP - Nguyên lý Phân tách Interface):** Không nên ép các lớp Client phải phụ thuộc vào những phương thức mà chúng không sử dụng (chia nhỏ "Fat Interface" thành các "Role Interface" chuyên biệt).
- **D — Dependency Inversion Principle (DIP - Nguyên lý Đảo ngược Phụ thuộc):** Module cấp cao không được phụ thuộc trực tiếp vào Module cấp thấp; cả hai phải phụ thuộc vào **Abstraction (Interface)** thông qua kỹ thuật **Dependency Injection (DI)**.

---

## 2. Cấu trúc thư mục & Mã nguồn

- `01_single_responsibility_srp.cpp`: **Single Responsibility Principle (SRP)** — Refactor lớp `GodInvoice` ("ôm đồm" cả tính thuế, xuất PDF và lưu SQL) thành 3 lớp độc lập: `Invoice`, `InvoiceFormatter`, `InvoiceRepository`.
- `02_open_closed_ocp.cpp`: **Open/Closed Principle (OCP)** — Hệ thống lọc sản phẩm thương mại điện tử theo **Specification Pattern** (`ISpecification<T>`, `AndSpecification<T>`) mở rộng tiêu chí lọc không giới hạn mà không sửa lớp `ProductFilter`.
- `03_liskov_substitution_lsp.cpp`: **Liskov Substitution Principle (LSP)** — Phân tích cái bẫy kinh điển **Hình chữ nhật & Hình vuông (`Rectangle` vs `Square`)** và bài toán **Tài khoản ngân hàng (`WithdrawableAccount` vs `FixedTermDepositAccount`)** để thiết kế cây kế thừa an toàn.
- `04_interface_segregation_isp.cpp`: **Interface Segregation Principle (ISP)** — Tách interface khổng lồ `IMultiFunctionMachine` (`print`, `scan`, `fax`, `staple`) thành các giao diện nhỏ gọn `IPrinter`, `IScanner`, `IFax` để máy in cơ bản không phải quăng ngoại lệ `NotImplemented`.
- `05_dependency_inversion_dip.cpp`: **Dependency Inversion Principle (DIP)** — Thiết kế `OrderNotificationService` nhận `IMessageSender` và `IPaymentGateway` qua **Constructor Dependency Injection**, cho phép hoán đổi giữa Production và Unit Test Mock dễ dàng.
- `06_practice_exercises.cpp`: **Bài tập thực hành tổng hợp** (Menu tương tác `do-while` + `switch-case` với 5 bài tập nhận diện vi phạm và Refactor mã nguồn theo chuẩn SOLID).

---

## 3. Hướng dẫn Biên dịch & Chạy (Terminal)

```bash
g++ -Wall -Wextra -std=c++17 01_single_responsibility_srp.cpp -o 01_single_responsibility_srp
./01_single_responsibility_srp

g++ -Wall -Wextra -std=c++17 02_open_closed_ocp.cpp -o 02_open_closed_ocp
./02_open_closed_ocp

g++ -Wall -Wextra -std=c++17 03_liskov_substitution_lsp.cpp -o 03_liskov_substitution_lsp
./03_liskov_substitution_lsp

g++ -Wall -Wextra -std=c++17 04_interface_segregation_isp.cpp -o 04_interface_segregation_isp
./04_interface_segregation_isp

g++ -Wall -Wextra -std=c++17 05_dependency_inversion_dip.cpp -o 05_dependency_inversion_dip
./05_dependency_inversion_dip

g++ -Wall -Wextra -std=c++17 06_practice_exercises.cpp -o 06_practice_exercises
./06_practice_exercises
```

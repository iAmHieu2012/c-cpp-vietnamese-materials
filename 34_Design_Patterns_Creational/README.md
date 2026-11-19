# Bài 7.2: Creational Design Patterns (5 Mẫu Thiết Kế Khởi Tạo: Abstract Factory, Builder, Factory Method, Prototype, Singleton)

Tài liệu hệ thống hóa trọn bộ **5 Mẫu Thiết Kế Khởi Tạo (Creational Patterns)** theo đúng thứ tự chuẩn **Refactoring.Guru**, mỗi mẫu được tách thành **1 file `.cpp` độc lập** cài đặt bằng **Modern C++17 (`std::unique_ptr`, Move Semantics, Thread-Safe Static)**.

---

## 1. Bảng Tổng Quan 5 Mẫu Creational Patterns

| STT | Mẫu thiết kế | File mã nguồn riêng | Bài toán giải quyết (Problem) | Giải pháp cốt lõi (Solution) |
| :-: | :--- | :--- | :--- | :--- |
| **1** | **Abstract Factory** | `01_abstract_factory.cpp` | Cần khởi tạo **cả một họ các đối tượng liên quan nhau** (ví dụ: `Button` + `Checkbox` của cùng hệ điều hành Windows hoặc macOS) mà không phụ thuộc lớp cụ thể. | Tạo giao diện `IGUIFactory` có bộ phương thức tạo từng sản phẩm trong họ (`create_button()`, `create_checkbox()`). |
| **2** | **Builder** | `02_builder.cpp` | Khởi tạo một đối tượng phức tạp có **quá nhiều tham số tùy chọn** (tránh *Telescoping Constructor* 10 tham số dễ nhầm thứ tự). | Tách quá trình lắp ráp thành từng bước nhỏ (`set_cpu()`, `set_ram()`) qua lớp `Builder` (và `Director`). |
| **3** | **Factory Method** | `03_factory_method.cpp` | Lớp cha cần khởi tạo đối tượng xử lý nhưng muốn **nhường quyền quyết định kiểu đối tượng cụ thể cho các lớp con**. | Thay toán tử `new` trực tiếp bằng một phương thức ảo `virtual unique_ptr<Product> create() const = 0;`. |
| **4** | **Prototype** | `04_prototype.cpp` | Cần tạo bản sao y hệt của một đối tượng đa hình lúc chạy mà **không biết lớp cụ thể** của nó (tránh *Object Slicing*). | Định nghĩa phương thức ảo **`virtual unique_ptr<Base> clone() const = 0;`** ngay trong lớp cơ sở. |
| **5** | **Singleton** | `05_singleton.cpp` | Đảm bảo toàn hệ thống chỉ có **duy nhất 1 thực thể** của lớp (Database Config, Logger) kèm điểm truy cập toàn cục an toàn đa luồng. | Khóa Constructor vào `private`, cấm Copy/Move (`= delete`), dùng biến `static` cục bộ trong `get_instance()` (**Meyers' Thread-safe**). |

---

## 2. Chi Tiết Kiến Trúc Từng Mẫu Thiết Kế

### 2.1. Abstract Factory vs Factory Method (`01_abstract_factory.cpp`, `03_factory_method.cpp`)

```text
  1. FACTORY METHOD (1 Phương thức ảo tạo ra 1 loại Sản phẩm):
     Logistics (Creator) ──► virtual create_transport() = 0
        ├── RoadLogistics ──► trả về std::make_unique<Truck>()
        └── SeaLogistics  ──► trả về std::make_unique<Ship>()

  2. ABSTRACT FACTORY (1 Giao diện Nhà máy tạo ra CẢ MỘT HỌ nhiều Sản phẩm đồng bộ):
     IGUIFactory
        ├── WinFactory ──► tạo WinButton  + WinCheckbox  (Đồng bộ giao diện Windows)
        └── MacFactory ──► tạo MacButton  + MacCheckbox  (Đồng bộ giao diện macOS)
```

### 2.2. Builder Pattern & Director (`02_builder.cpp`)

* **`Builder`:** Cung cấp các phương thức thiết lập từng bộ phận (`set_cpu()`, `set_gpu()`, `set_ram()`) trả về tham chiếu `Builder&` để gọi nối chuỗi (**Fluent Interface**), kết thúc bằng `.build()`.
* **`Director` (Tùy chọn):** Đóng gói các công thức lắp ráp tiêu chuẩn thường dùng (ví dụ: `build_gaming_pc(builder)` vs `build_office_pc(builder)`).

### 2.3. Prototype Pattern & Prototype Registry (`04_prototype.cpp`)

* Khi khởi tạo một đối tượng từ đầu quá tốn kém (phải đọc file cấu hình, nạp tài nguyên 3D), ta tạo sẵn một bộ đối tượng mẫu trong **`PrototypeRegistry` (`unordered_map<string, unique_ptr<Prototype>>`)**.
* Mỗi khi cần đối tượng mới, chỉ cần gọi `registry["Boss"]->clone()` để nhân bản trong tích tắc qua Copy Constructor của lớp con.

### 2.4. Singleton Pattern: Naïve vs Meyers' Thread-Safe Singleton (`05_singleton.cpp`)

```text
  • Naïve Singleton (C++98 cũ — Bị Race Condition khi 2 luồng cùng gọi lần đầu & dễ quên delete):
    if (instance == nullptr) instance = new Singleton(); // 2 luồng cùng lọt qua if -> tạo 2 instance!

  • Meyers' Singleton (Chuẩn vàng C++11/17 — Thread-Safe 100% + Tự động hủy trên Data/BSS Segment):
    static AppConfig& get_instance() {
        static AppConfig instance; // Chuẩn C++11 cam kết khởi tạo biến static cục bộ là ATOMIC THREAD-SAFE!
        return instance;
    }
```

---

## 3. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_abstract_factory.cpp`: Họ giao diện đa nền tảng (`WinFactory`, `MacFactory` tạo đồng bộ `IButton` và `ICheckbox`).
* `02_builder.cpp`: Lắp ráp cấu hình máy tính từng bước với `ComputerBuilder` (Fluent Chaining) và `ComputerDirector`.
* `03_factory_method.cpp`: Hệ thống điều phối vận tải `Logistics` (`RoadLogistics` tạo `Truck`, `SeaLogistics` tạo `Ship`).
* `04_prototype.cpp`: Nhân bản đa hình qua `virtual clone()` và kho mẫu `PrototypeRegistry`.
* `05_singleton.cpp`: Đối chiếu Naïve Singleton và Meyers' Thread-Safe Singleton đa luồng.
* `06_practice_exercises.cpp`: Giải 5 bài tập thực chiến tương ứng với cả 5 mẫu Creational.

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_abstract_factory.cpp -o cr_abstract_factory
./cr_abstract_factory

g++ -Wall -Wextra -std=c++17 02_builder.cpp -o cr_builder
./cr_builder

g++ -Wall -Wextra -std=c++17 03_factory_method.cpp -o cr_factory_method
./cr_factory_method

g++ -Wall -Wextra -std=c++17 04_prototype.cpp -o cr_prototype
./cr_prototype

g++ -Wall -Wextra -std=c++17 -pthread 05_singleton.cpp -o cr_singleton
./cr_singleton

g++ -Wall -Wextra -std=c++17 06_practice_exercises.cpp -o cr_exercises
./cr_exercises
```

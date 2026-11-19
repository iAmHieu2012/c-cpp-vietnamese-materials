# Bài 7.3: Structural Design Patterns (7 Mẫu Thiết Kế Cấu Trúc: Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy)

Tài liệu hệ thống hóa trọn bộ **7 Mẫu Thiết Kế Cấu Trúc (Structural Patterns)** theo đúng thứ tự chuẩn **Refactoring.Guru**, mỗi mẫu được tách thành **1 file `.cpp` độc lập** cài đặt bằng **Modern C++17**.

---

## 1. Bảng Tổng Quan 7 Mẫu Structural Patterns

| STT | Mẫu thiết kế | File mã nguồn riêng | Bài toán giải quyết (Problem) | Giải pháp cốt lõi (Solution) |
| :-: | :--- | :--- | :--- | :--- |
| **1** | **Adapter** | `01_adapter.cpp` | Hai lớp/thư viện có **giao diện không tương thích nhau** (ví dụ: Hệ thống mới dùng `JSON` nhưng thư viện cũ chỉ nhận `XML`). | Tạo lớp trung gian `Adapter` kế thừa giao diện mới và **bọc (hoặc đa kế thừa)** thư viện cũ để dịch lời gọi hàm. |
| **2** | **Bridge** | `02_bridge.cpp` | Tránh sự bùng nổ tổ hợp lớp con ($M \times N$ lớp) khi một hệ thống mở rộng theo **2 chiều độc lập** (*Hình dạng* $\times$ *API Vẽ OpenGL/Vulkan*). | Tách thành 2 nhánh lớp độc lập: **Abstraction** (Hình dạng) giữ một tham chiếu/con trỏ cầu nối sang **Implementation** (Renderer). |
| **3** | **Composite** | `03_composite.cpp` | Cần xử lý các đối tượng đơn lẻ (**Leaf**) và nhóm đối tượng lồng nhau (**Container**) theo **cấu trúc hình cây** bằng cùng một giao diện. | Cả `File` (Leaf) và `Folder` (Composite chứa `vector<unique_ptr<INode>>`) cùng kế thừa chung `IFileSystemNode`. |
| **4** | **Decorator** | `04_decorator.cpp` | Muốn **gắn thêm nhiều tính năng mới** (Nén, Mã hóa, Ghi log) cho một đối tượng lúc chạy mà không sửa code gốc hay tạo hàng chục lớp con. | Tạo các lớp vỏ bọc (**Wrappers**) cùng thực thi giao diện gốc và chứa con trỏ bọc lấy đối tượng bên trong như búp bê Nga. |
| **5** | **Facade** | `05_facade.cpp` | Một hệ thống con gồm **quá nhiều lớp phức tạp** (Decoder, AudioMixer, VideoFilter) khiến người dùng khó gọi trực tiếp. | Cung cấp **1 lớp mặt tiền (`Facade`) duy nhất** gom toàn bộ quy trình phức tạp vào 1 hàm đơn giản `convert_video()`. |
| **6** | **Flyweight** | `06_flyweight.cpp` | Cần tạo **hàng triệu đối tượng tương tự nhau** (1 triệu hạt mưa/cây cối trên bản đồ Game) làm tràn bộ nhớ RAM. | Tách dữ liệu nặng dùng chung (**Intrinsic State**: 3D Mesh, Texture) ra kho `FlyweightFactory` chia sẻ qua `shared_ptr`. |
| **7** | **Proxy** | `07_proxy.cpp` | Cần **kiểm soát truy cập** vào một đối tượng nặng hoặc nhạy cảm (chỉ khởi tạo khi thực sự dùng, kiểm tra quyền, hoặc lưu Cache). | Tạo lớp đại diện `Proxy` có cùng giao diện với `RealService`, đứng chặn ở giữa để kiểm tra/khởi tạo lười (**Lazy Init**). |

---

## 2. Chi Tiết Kiến Trúc Từng Mẫu Thiết Kế

### 2.1. Adapter & Bridge (`01_adapter.cpp`, `02_bridge.cpp`)

* **2 cách cài đặt Adapter trong C++:**
  1. **Object Adapter (Composition):** Chứa một thực thể/con trỏ của `Adaptee` bên trong lớp `Adapter`.
  2. **Class Adapter (Multiple Inheritance):** Kế thừa `public ITarget` (giao diện mới) và `private LegacyAdaptee` (cài đặt cũ).
* **Bridge Pattern:** Thay vì tạo $M \times N$ lớp con kết hợp, ta tách nhánh **Abstraction** và nhánh **Implementation** nối với nhau qua con trỏ/tham chiếu $\implies$ giảm số lớp xuống còn $M + N$.

### 2.2. Composite vs Decorator (`03_composite.cpp`, `04_decorator.cpp`)

```text
  1. COMPOSITE (Cấu trúc Cây 1 - Nhiều):           2. DECORATOR (Vỏ bọc nối tiếp 1 - 1):
          [Folder: root]                              [EncryptionDecorator]
             ╱      ╲                                          │ (bọc)
     [File: a.txt]  [Folder: src]                     [CompressionDecorator]
                        │                                      │ (bọc)
                    [File: main.cpp]                    [FileDataSource]
```

### 2.3. Facade, Flyweight & Proxy (`05_facade.cpp`, `06_flyweight.cpp`, `07_proxy.cpp`)

* **Facade:** Tạo giao diện tối giản ở tầng trên cho cả một cụm thư viện phức tạp.
* **Flyweight:** Phân tách trạng thái của đối tượng thành **Intrinsic State** (bất biến, nặng, chia sẻ chung cho hàng vạn đối tượng) và **Extrinsic State** (tọa độ nhẹ riêng biệt của từng đối tượng).
* **Proxy:** Đứng trước một đối tượng thật để kiểm soát quyền (**Protection Proxy**), trì hoãn khởi tạo đối tượng nặng (**Virtual/Lazy Proxy**) hoặc lưu kết quả (**Caching Proxy**).

---

## 3. Danh Sách File Thực Hành & Lệnh Biên Dịch

* `01_adapter.cpp`: Object Adapter và Multiple Inheritance Class Adapter chuyển đổi giao diện JSON sang XML.
* `02_bridge.cpp`: Tách nhánh `Shape` (`Circle`, `Square`) và nhánh `IRenderAPI` (`OpenGL`, `Vulkan`).
* `03_composite.cpp`: Cây thư mục `FileItem` & `FolderItem` tính tổng dung lượng đệ quy.
* `04_decorator.cpp`: Bọc nhiều lớp tính năng động (`EncryptionDecorator` bọc `CompressionDecorator` bọc `FileDataStream`).
* `05_facade.cpp`: Lớp mặt tiền `VideoConverterFacade` điều phối bộ giải mã âm thanh, hình ảnh và đóng gói file.
* `06_flyweight.cpp`: Trồng 1,000 cây trên bản đồ Game nhưng chỉ tốn 2 đối tượng `TreeTypeFlyweight` trên RAM.
* `07_proxy.cpp`: `LazyImageProxy` trì hoãn tải ảnh 8K cho tới khi hiển thị và `ProtectedDatabaseProxy` kiểm tra quyền Admin.
* `08_practice_exercises.cpp`: Giải 5 bài tập tổng hợp thực chiến cho nhóm Structural Patterns.

**Lệnh biên dịch và khởi chạy trên Terminal:**

```bash
g++ -Wall -Wextra -std=c++17 01_adapter.cpp -o st_adapter
./st_adapter

g++ -Wall -Wextra -std=c++17 02_bridge.cpp -o st_bridge
./st_bridge

g++ -Wall -Wextra -std=c++17 03_composite.cpp -o st_composite
./st_composite

g++ -Wall -Wextra -std=c++17 04_decorator.cpp -o st_decorator
./st_decorator

g++ -Wall -Wextra -std=c++17 05_facade.cpp -o st_facade
./st_facade

g++ -Wall -Wextra -std=c++17 06_flyweight.cpp -o st_flyweight
./st_flyweight

g++ -Wall -Wextra -std=c++17 07_proxy.cpp -o st_proxy
./st_proxy

g++ -Wall -Wextra -std=c++17 08_practice_exercises.cpp -o st_exercises
./st_exercises
```

# Bài 7.4: Design Patterns — Nhóm Hành Vi (Behavioral Patterns)

## 1. Mục tiêu

Làm chủ **10 mẫu thiết kế nhóm Hành vi (Behavioral Design Patterns)** kinh điển theo hệ thống **GoF / Refactoring.Guru** — mỗi mẫu được tách thành **1 file `.cpp` độc lập**:

- **Chain of Responsibility (Chuỗi trách nhiệm):** Truyền yêu cầu (Request) dọc theo một chuỗi các bộ xử lý (Handler). Mỗi Handler quyết định tự xử lý hoặc chuyển tiếp cho mắt xích tiếp theo.
- **Command (Mệnh lệnh):** Đóng gói một yêu cầu thành một đối tượng độc lập chứa toàn bộ thông tin về thao tác đó — cho phép truyền lệnh như tham số, xếp hàng đợi (Queue), ghi log và hỗ trợ **Undo / Redo**.
- **Iterator (Bộ lặp):** Duyệt qua các phần tử của một tập hợp phức tạp (Danh sách, Cây, Đồ thị...) mà không làm lộ cấu trúc biểu diễn bên trong.
- **Mediator (Trung gian):** Giảm sự phụ thuộc chéo hỗn loạn (Spaghetti Dependencies) giữa nhiều đối tượng bằng cách buộc chúng chỉ giao tiếp qua một đối tượng Điều phối viên trung tâm.
- **Memento (Bản ghi nhớ / Snapshot):** Lưu lại và khôi phục trạng thái trước đó của đối tượng mà không vi phạm tính đóng gói (Encapsulation).
- **Observer (Người quan sát / Pub-Sub):** Định nghĩa cơ chế đăng ký (Subscription) để tự động thông báo cho nhiều đối tượng (Subscribers) khi đối tượng được quan sát (Publisher/Subject) thay đổi trạng thái.
- **State (Trạng thái / State Machine):** Cho phép đối tượng thay đổi hành vi khi trạng thái nội tại thay đổi, giống như thể đối tượng đó vừa đổi sang một Class khác (loại bỏ rừng `if-else` / `switch-case` khổng lồ).
- **Strategy (Chiến lược):** Định nghĩa một họ các thuật toán, đóng gói từng thuật toán vào một Class riêng và cho phép hoán đổi linh hoạt ngay lúc chạy (Runtime).
- **Template Method (Phương thức mẫu):** Định nghĩa bộ khung (Skeleton) của một thuật toán ở lớp cha, nhưng cho phép các lớp con nạp chồng (Override) các bước cụ thể mà không làm thay đổi cấu trúc tổng thể.
- **Visitor (Khách thăm / Double Dispatch):** Tách biệt các thuật toán mới ra khỏi cấu trúc đối tượng mà chúng thao tác, giúp thêm tính năng mới mà không cần sửa đổi các lớp phần tử hiện có.

---

## 2. Cấu trúc thư mục & Mã nguồn

- `01_chain_of_responsibility.cpp`: **Chain of Responsibility Pattern** — Pipeline Middleware xác thực HTTP Request (Authentication $\rightarrow$ Rate Limiting $\rightarrow$ Role Authorization $\rightarrow$ Payload Sanitization).
- `02_command.cpp`: **Command Pattern** — Trình soạn thảo văn bản (Text Editor) với các lệnh `InsertCommand`, `DeleteCommand` và bộ quản lý lịch sử `CommandHistory` hỗ trợ **Undo / Redo** đa cấp.
- `03_iterator.cpp`: **Iterator Pattern** — Bộ lặp tùy chỉnh (`PreOrderIterator` & `BFSIterator`) duyệt qua cấu trúc cây tổ chức doanh nghiệp mà không lộ con trỏ Node bên trong.
- `04_mediator.cpp`: **Mediator Pattern** — Đài kiểm soát không lưu (`AirTrafficControlTower`) điều phối đường băng cho các máy bay (`Aircraft`) cất/hạ cánh an toàn mà không cần các máy bay liên lạc trực tiếp với nhau.
- `05_memento.cpp`: **Memento Pattern** — Hệ thống Save/Load Checkpoint cho nhân vật Game RPG (`GameOriginator`, `CheckpointMemento`, `SaveManagerCaretaker`).
- `06_observer.cpp`: **Observer Pattern** — Sàn giao dịch chứng khoán (`StockTicker`) phát sự kiện biến động giá thời gian thực tới các `IObserver` (`MobileAlertApp`, `AutoTradingBot`, `AnalyticsDashboard`) an toàn bộ nhớ với `std::weak_ptr`.
- `07_state.cpp`: **State Pattern** — Máy trạng thái đơn hàng thương mại điện tử (`DraftState` $\rightarrow$ `PaidState` $\rightarrow$ `ShippedState` $\rightarrow$ `DeliveredState` / `CancelledState`).
- `08_strategy.cpp`: **Strategy Pattern** — Hệ thống định tuyến bản đồ (`NavigationContext`) hoán đổi linh hoạt thuật toán tìm đường (`FastestHighwayStrategy`, `ScenicRouteStrategy`, `PublicTransitStrategy`) và chiến lược tính phí vận chuyển.
- `09_template_method.cpp`: **Template Method Pattern** — Bộ khung khai thác dữ liệu tài liệu (`DataMiner::mineDocument()`) cố định quy trình 5 bước, cho phép `PDFDataMiner` và `CSVDataMiner` tùy biến bước đọc và phân tích cú pháp.
- `10_visitor.cpp`: **Visitor Pattern** — Kỹ thuật **Double Dispatch** cho phép chạy các thao tác xuất báo cáo (`XMLExportVisitor`) và tính phí bảo hiểm (`InsuranceCostVisitor`) trên cây đối tượng hình học/tài sản mà không sửa lớp gốc.
- `11_practice_exercises.cpp`: **Bài tập thực hành tổng hợp** (Menu tương tác `do-while` + `switch-case` với 5 bài tập thiết kế kiến trúc Behavioral Patterns).

---

## 3. Hướng dẫn Biên dịch & Chạy (Terminal)

```bash
g++ -Wall -Wextra -std=c++17 01_chain_of_responsibility.cpp -o 01_chain_of_responsibility
./01_chain_of_responsibility

g++ -Wall -Wextra -std=c++17 02_command.cpp -o 02_command
./02_command

g++ -Wall -Wextra -std=c++17 03_iterator.cpp -o 03_iterator
./03_iterator

g++ -Wall -Wextra -std=c++17 04_mediator.cpp -o 04_mediator
./04_mediator

g++ -Wall -Wextra -std=c++17 05_memento.cpp -o 05_memento
./05_memento

g++ -Wall -Wextra -std=c++17 06_observer.cpp -o 06_observer
./06_observer

g++ -Wall -Wextra -std=c++17 07_state.cpp -o 07_state
./07_state

g++ -Wall -Wextra -std=c++17 08_strategy.cpp -o 08_strategy
./08_strategy

g++ -Wall -Wextra -std=c++17 09_template_method.cpp -o 09_template_method
./09_template_method

g++ -Wall -Wextra -std=c++17 10_visitor.cpp -o 10_visitor
./10_visitor

g++ -Wall -Wextra -std=c++17 11_practice_exercises.cpp -o 11_practice_exercises
./11_practice_exercises
```

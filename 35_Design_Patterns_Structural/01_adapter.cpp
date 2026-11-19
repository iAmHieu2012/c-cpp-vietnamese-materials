#include <iostream>
#include <string>

// =============================================================================
// ADAPTER PATTERN (OBJECT ADAPTER & MULTIPLE INHERITANCE CLASS ADAPTER)
// =============================================================================

// Giao diện mới mà hệ thống Client mong muốn
class IModernJsonLogger {
public:
    virtual ~IModernJsonLogger() = default;
    virtual void log_json(const std::string &event, int code) const = 0;
};

// Thư viện cũ (Adaptee) có giao diện hoàn toàn khác biệt, không thể sửa mã nguồn của nó
class LegacyXmlLogger {
public:
    void write_xml_entry(const std::string &xml_payload) const {
        std::cout << "  [LegacyXmlLogger] Ghi log: " << xml_payload << "\n";
    }
};

// Cách 1: Object Adapter (Dùng Composition - Chứa đối tượng LegacyXmlLogger bên trong)
class XmlToJsonObjectAdapter : public IModernJsonLogger {
private:
    LegacyXmlLogger legacy_logger;
public:
    void log_json(const std::string &event, int code) const override {
        std::string converted = "<log><event>" + event + "</event><code>" + std::to_string(code) + "</code></log>";
        legacy_logger.write_xml_entry(converted);
    }
};

// Cách 2: Class Adapter (Dùng Đa kế thừa trong C++: public Interface mới, private Lớp cũ)
class XmlToJsonClassAdapter : public IModernJsonLogger, private LegacyXmlLogger {
public:
    void log_json(const std::string &event, int code) const override {
        std::string converted = "<log mode='class_adapter'><ev>" + event + "</ev><c>" + std::to_string(code) + "</c></log>";
        write_xml_entry(converted);
    }
};

int main() {
    std::cout << "=== ADAPTER PATTERN (OBJECT ADAPTER & CLASS ADAPTER) ===\n";
    XmlToJsonObjectAdapter obj_adapter;
    XmlToJsonClassAdapter  cls_adapter;

    obj_adapter.log_json("UserLogin", 200);
    cls_adapter.log_json("PaymentSuccess", 201);

    return 0;
}

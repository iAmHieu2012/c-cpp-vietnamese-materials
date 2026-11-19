#include <iostream>
#include <string>
#include <memory>

// =============================================================================
// DECORATOR PATTERN (BỌC ĐỐI TƯỢNG ĐỂ GẮN THÊM HÀNH VI ĐỘNG)
// =============================================================================
class IDataStream {
public:
    virtual ~IDataStream() = default;
    virtual void write(const std::string &data) const = 0;
};

// Đối tượng lõi ban đầu
class FileDataStream : public IDataStream {
private:
    std::string filename;
public:
    explicit FileDataStream(std::string fname) : filename(std::move(fname)) {}
    void write(const std::string &data) const override {
        std::cout << "  [Disk Write -> " << filename << "]: " << data << "\n";
    }
};

// Lớp Decorator cơ sở giữ con trỏ bọc một IDataStream bên trong
class StreamDecorator : public IDataStream {
protected:
    std::unique_ptr<IDataStream> wrappee;
public:
    explicit StreamDecorator(std::unique_ptr<IDataStream> s) : wrappee(std::move(s)) {}
};

class CompressionDecorator : public StreamDecorator {
public:
    using StreamDecorator::StreamDecorator;
    void write(const std::string &data) const override {
        wrappee->write("GZIP(" + data + ")");
    }
};

class EncryptionDecorator : public StreamDecorator {
public:
    using StreamDecorator::StreamDecorator;
    void write(const std::string &data) const override {
        wrappee->write("AES256(" + data + ")");
    }
};

int main() {
    std::cout << "=== DECORATOR PATTERN (BOC NEN + MA HOA LUONG DU LIEU) ===\n";
    // Lắp ghép động như búp bê Nga: Encryption bọc ngoài Compression, Compression bọc ngoài FileDataStream
    std::unique_ptr<IDataStream> stream = std::make_unique<FileDataStream>("secret.dat");
    stream = std::make_unique<CompressionDecorator>(std::move(stream));
    stream = std::make_unique<EncryptionDecorator>(std::move(stream));

    stream->write("Mat_Khau_Kho_Bac_2026");

    return 0;
}

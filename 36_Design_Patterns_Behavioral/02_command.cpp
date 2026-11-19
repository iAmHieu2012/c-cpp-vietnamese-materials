// ============================================================================
// File: 02_command.cpp
// Bài 7.4: Behavioral Design Patterns - Command Pattern (Mệnh lệnh & Undo/Redo)
// Tham khảo: https://refactoring.guru/design-patterns/command
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// 1. RECEIVER (Đối tượng thực sự chứa nghiệp vụ: TextDocument)
// ============================================================================
class TextDocument {
    std::string content_;

public:
    void insertText(size_t pos, const std::string &text) {
        if (pos > content_.size()) pos = content_.size();
        content_.insert(pos, text);
    }

    std::string deleteText(size_t pos, size_t length) {
        if (pos >= content_.size()) return "";
        std::string removed = content_.substr(pos, length);
        content_.erase(pos, length);
        return removed;
    }

    const std::string &getContent() const { return content_; }
};

// ============================================================================
// 2. COMMAND INTERFACE (Giao diện mệnh lệnh có execute() và undo())
// ============================================================================
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string describe() const = 0;
};

// ============================================================================
// 3. CONCRETE COMMANDS (Các mệnh lệnh cụ thể tự lưu trạng thái để Undo)
// ============================================================================
class InsertTextCommand : public ICommand {
    TextDocument &doc_;
    size_t position_;
    std::string text_;

public:
    InsertTextCommand(TextDocument &doc, size_t pos, std::string text)
        : doc_(doc), position_(pos), text_(std::move(text)) {}

    void execute() override {
        doc_.insertText(position_, text_);
    }

    void undo() override {
        doc_.deleteText(position_, text_.size());
    }

    std::string describe() const override {
        return "Insert(\"" + text_ + "\" at " + std::to_string(position_) + ")";
    }
};

class DeleteTextCommand : public ICommand {
    TextDocument &doc_;
    size_t position_;
    size_t length_;
    std::string backupDeletedText_; // Lưu lại chuỗi đã xóa để khôi phục khi Undo

public:
    DeleteTextCommand(TextDocument &doc, size_t pos, size_t len)
        : doc_(doc), position_(pos), length_(len) {}

    void execute() override {
        backupDeletedText_ = doc_.deleteText(position_, length_);
    }

    void undo() override {
        doc_.insertText(position_, backupDeletedText_);
    }

    std::string describe() const override {
        return "Delete(pos=" + std::to_string(position_) + ", len=" + std::to_string(length_) + ")";
    }
};

// ============================================================================
// 4. INVOKER (Bộ điều khiển thực thi và quản lý lịch sử Undo / Redo)
// ============================================================================
class EditorInvoker {
    std::vector<std::unique_ptr<ICommand>> undoStack_;
    std::vector<std::unique_ptr<ICommand>> redoStack_;

public:
    void executeCommand(std::unique_ptr<ICommand> cmd) {
        std::cout << "  [Execute] " << cmd->describe() << "\n";
        cmd->execute();
        undoStack_.push_back(std::move(cmd));
        redoStack_.clear(); // Khi có lệnh mới, xóa nhánh Redo cũ
    }

    void undo() {
        if (undoStack_.empty()) {
            std::cout << "  [Undo] Khong co lenh nao de Undo!\n";
            return;
        }
        auto cmd = std::move(undoStack_.back());
        undoStack_.pop_back();
        std::cout << "  [Undo]    " << cmd->describe() << "\n";
        cmd->undo();
        redoStack_.push_back(std::move(cmd));
    }

    void redo() {
        if (redoStack_.empty()) {
            std::cout << "  [Redo] Khong co lenh nao de Redo!\n";
            return;
        }
        auto cmd = std::move(redoStack_.back());
        redoStack_.pop_back();
        std::cout << "  [Redo]    " << cmd->describe() << "\n";
        cmd->execute();
        undoStack_.push_back(std::move(cmd));
    }
};

int main() {
    std::cout << "=== COMMAND PATTERN (TEXT EDITOR VOI UNDO / REDO) ===\n\n";

    TextDocument doc;
    EditorInvoker editor;

    editor.executeCommand(std::make_unique<InsertTextCommand>(doc, 0, "Hello C++"));
    std::cout << "  -> Van ban: \"" << doc.getContent() << "\"\n\n";

    editor.executeCommand(std::make_unique<InsertTextCommand>(doc, 9, " Design Patterns!"));
    std::cout << "  -> Van ban: \"" << doc.getContent() << "\"\n\n";

    editor.executeCommand(std::make_unique<DeleteTextCommand>(doc, 5, 4)); // Xoa " C++"
    std::cout << "  -> Van ban: \"" << doc.getContent() << "\"\n\n";

    // Thử nghiệm Undo 2 lần liên tiếp
    editor.undo();
    std::cout << "  -> Sau Undo 1: \"" << doc.getContent() << "\"\n";

    editor.undo();
    std::cout << "  -> Sau Undo 2: \"" << doc.getContent() << "\"\n\n";

    // Thử nghiệm Redo lại
    editor.redo();
    std::cout << "  -> Sau Redo 1: \"" << doc.getContent() << "\"\n";

    return 0;
}

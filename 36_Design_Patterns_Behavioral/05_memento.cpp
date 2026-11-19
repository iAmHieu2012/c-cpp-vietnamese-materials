// ============================================================================
// File: 05_memento.cpp
// Bài 7.4: Behavioral Design Patterns - Memento Pattern (Lưu & Khôi phục Snapshot)
// Tham khảo: https://refactoring.guru/design-patterns/memento
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// 1. MEMENTO (Đối tượng bất biến lưu giữ Snapshot trạng thái nội bộ)
// ============================================================================
class HeroCheckpointMemento {
    // Trạng thái nội bộ bị khóa kín (private), chỉ cho phép Originator truy cập
    int level_;
    int hp_;
    int mana_;
    std::string location_;
    std::string timestampTag_;

    friend class GameHeroOriginator; // Chỉ GameHeroOriginator được đọc dữ liệu mật

public:
    HeroCheckpointMemento(int lvl, int hp, int mp, std::string loc, std::string tag)
        : level_(lvl), hp_(hp), mana_(mp),
          location_(std::move(loc)), timestampTag_(std::move(tag)) {}

    // Giao diện hẹp (Narrow Interface) cho Caretaker hiển thị danh sách bản lưu
    std::string getSaveMetadata() const {
        return "[" + timestampTag_ + "] Location: " + location_ +
               " (Lv." + std::to_string(level_) + ")";
    }
};

// ============================================================================
// 2. ORIGINATOR (Nhân vật Game tạo Snapshot và tự khôi phục từ Snapshot)
// ============================================================================
class GameHeroOriginator {
    int level_ = 1;
    int hp_ = 100;
    int mana_ = 50;
    std::string location_ = "Starter Village";

public:
    void playAndProgress(int newLevel, int newHp, int newMana, const std::string &newLoc) {
        level_ = newLevel;
        hp_ = newHp;
        mana_ = newMana;
        location_ = newLoc;
    }

    void printStatus() const {
        std::cout << "  [Hero Status] Lv." << level_
                  << " | HP: " << hp_ << " | Mana: " << mana_
                  << " | Vi tri: " << location_ << "\n";
    }

    std::unique_ptr<HeroCheckpointMemento> saveCheckpoint(const std::string &tag) const {
        std::cout << "  [Save] Dang dong goi Snapshot: " << tag << "...\n";
        return std::make_unique<HeroCheckpointMemento>(level_, hp_, mana_, location_, tag);
    }

    void restoreFromCheckpoint(const HeroCheckpointMemento &memento) {
        level_ = memento.level_;
        hp_ = memento.hp_;
        mana_ = memento.mana_;
        location_ = memento.location_;
        std::cout << "  [Restore] Da khoi phuc trang thai tu Checkpoint: "
                  << memento.getSaveMetadata() << "\n";
    }
};

// ============================================================================
// 3. CARETAKER (Quản lý danh sách các bản lưu mà không xâm phạm dữ liệu bên trong)
// ============================================================================
class SaveManagerCaretaker {
    GameHeroOriginator &hero_;
    std::vector<std::unique_ptr<HeroCheckpointMemento>> checkpoints_;

public:
    explicit SaveManagerCaretaker(GameHeroOriginator &hero) : hero_(hero) {}

    void backup(const std::string &tag) {
        checkpoints_.push_back(hero_.saveCheckpoint(tag));
    }

    void rollbackToLastCheckpoint() {
        if (checkpoints_.empty()) {
            std::cout << "  [SaveManager] Khong co Checkpoint nao de khoi phuc!\n";
            return;
        }
        auto lastSnap = std::move(checkpoints_.back());
        checkpoints_.pop_back();
        hero_.restoreFromCheckpoint(*lastSnap);
    }

    void listCheckpoints() const {
        std::cout << "  [SaveManager] Danh sach Checkpoints dang luu ("
                  << checkpoints_.size() << "):\n";
        for (size_t i = 0; i < checkpoints_.size(); ++i) {
            std::cout << "    #" << (i + 1) << ": " << checkpoints_[i]->getSaveMetadata() << "\n";
        }
    }
};

int main() {
    std::cout << "=== MEMENTO PATTERN (GAME CHECKPOINT SAVE / LOAD) ===\n\n";

    GameHeroOriginator hero;
    SaveManagerCaretaker saveManager(hero);

    hero.printStatus();
    saveManager.backup("Slot 1 - Bat dau hanh trinh");

    std::cout << "\n--- Tien vao ham nguc & danh quai ---\n";
    hero.playAndProgress(15, 450, 200, "Dragon Boss Gate");
    hero.printStatus();
    saveManager.backup("Slot 2 - Truoc cua Phong Boss");

    std::cout << "\n";
    saveManager.listCheckpoints();

    std::cout << "\n--- Danh Boss Rong that bai (HP = 0) ---\n";
    hero.playAndProgress(15, 0, 10, "Dragon Lair (DEFEATED)");
    hero.printStatus();

    std::cout << "\n--- Tai lai Checkpoint gan nhat ---\n";
    saveManager.rollbackToLastCheckpoint();
    hero.printStatus();

    return 0;
}

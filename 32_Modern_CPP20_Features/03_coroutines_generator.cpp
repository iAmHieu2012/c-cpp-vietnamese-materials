#include <iostream>
#include <coroutine>
#include <utility>

// =============================================================================
// CÀI ĐẶT GENERATOR<T> CHUẨN C++20 COROUTINE (<COROUTINE>)
// Hỗ trợ tạm dừng (Suspend) và tiếp tục (Resume) bằng từ khóa `co_yield`
// =============================================================================
template <typename T>
class Generator {
public:
    struct promise_type {
        T current_value{};

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept {
            current_value = value;
            return {}; // Tạm dừng Coroutine ngay sau khi co_yield!
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle h) : coro_handle(h) {}
    ~Generator() {
        if (coro_handle) coro_handle.destroy();
    }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator &&other) noexcept : coro_handle(other.coro_handle) {
        other.coro_handle = nullptr;
    }

    // Đánh thức (Resume) Coroutine chạy tiếp tới lệnh co_yield kế tiếp
    bool next() {
        if (!coro_handle || coro_handle.done()) return false;
        coro_handle.resume();
        return !coro_handle.done();
    }

    T value() const noexcept {
        return coro_handle.promise().current_value;
    }

private:
    Handle coro_handle;
};

// =============================================================================
// HÀM COROUTINE: SINH DÃY FIBONACCI LƯỜI BẰNG CO_YIELD (O(1) MEMORY!)
// =============================================================================
Generator<long long> fibonacci_coroutine(int count) {
    long long a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a; // Đóng băng hàm tại đây và trả `a` về cho main()!
        long long next_val = a + b;
        a = b;
        b = next_val;
    }
}

int main() {
    std::cout << "=== C++20 COROUTINES: LAZY GENERATOR VOI CO_YIELD ===\n";
    auto fib_gen = fibonacci_coroutine(12);

    std::cout << "  12 so Fibonacci dau tien (Suspend & Resume tung buoc, O(1) RAM):\n  ";
    while (fib_gen.next()) {
        std::cout << fib_gen.value() << " ";
    }
    std::cout << "\n";

    return 0;
}

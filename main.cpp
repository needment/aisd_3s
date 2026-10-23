#include <cstddef>
#include <utility>

template <typename T>
class Set {
private:
    T* elements_;
    std::size_t size_;
    std::size_t capacity_;

    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        T* new_elements = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_elements[i] = std::move(elements_[i]);
        }
        delete[] elements_;
        elements_ = new_elements;
        capacity_ = new_capacity;
    }

public:
    Set() : elements_(nullptr), size_(0), capacity_(0) {}

    ~Set() {
        delete[] elements_;
    }

    std::size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }
};

int main() {
    Set<int> s;
    return 0;
}
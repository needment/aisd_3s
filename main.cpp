#include <iostream>
#include <cstddef>
#include <cmath>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <random>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p) {
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

template <typename T>
class Set {
private:
    static constexpr double kEpsilon = 1e-6;

    T* elements_;
    std::size_t size_;
    std::size_t capacity_;

    static bool are_equal(const T& left_val, const T& right_val) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::fabs(left_val - right_val) <= static_cast<T>(kEpsilon);
        } else {
            return left_val == right_val;
        }
    }

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

    std::size_t find_index(const T& value) const {
        for (std::size_t i = 0; i < size_; ++i) {
            if (are_equal(elements_[i], value)) {
                return i;
            }
        }
        return size_;
    }

public:
    void swap(Set& other) noexcept {
        std::swap(elements_, other.elements_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    Set() : elements_(nullptr), size_(0), capacity_(0) {}

    Set(const T* items, std::size_t count) : Set() {
        if (items != nullptr && count > 0) {
            reserve(count);
            for (std::size_t i = 0; i < count; ++i) {
                *this += items[i];
            }
        }
    }

    Set(std::size_t count, const T& min_val, const T& max_val) : Set() {
        if constexpr (std::is_arithmetic_v<T>) {
            if (count == 0) {
                return;
            }
            reserve(count);
            std::random_device rd;
            std::mt19937 rng(rd());
            std::size_t attempts = 0;
            const std::size_t max_attempts = count * 1000;

            while (size_ < count && attempts < max_attempts) {
                if constexpr (std::is_integral_v<T>) {
                    std::uniform_int_distribution<T> dist(min_val, max_val);
                    *this += dist(rng);
                } else {
                    std::uniform_real_distribution<T> dist(min_val, max_val);
                    *this += dist(rng);
                }
                ++attempts;
            }
        } else {
            (void)count;
            (void)min_val;
            (void)max_val;
        }
    }

    Set(const Set& other) : elements_(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            elements_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) {
                elements_[i] = other.elements_[i];
            }
        }
    }

    Set(Set&& other) noexcept : Set() {
        swap(other);
    }

    Set& operator=(Set other) noexcept {
        swap(other);
        return *this;
    }

    ~Set() {
        delete[] elements_;
    }

    std::size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    bool contains(const T& value) const {
        return find_index(value) != size_;
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return elements_[index];
    }

    Set& operator+=(const T& value) {
        if (contains(value)) {
            return *this;
        }
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 2 : capacity_ * 2);
        }
        elements_[size_++] = value;
        return *this;
    }

    Set operator+(const T& value) const {
        Set result = *this;
        result += value;
        return result;
    }

    Set& operator-=(const T& value) {
        std::size_t index = find_index(value);
        if (index == size_) {
            return *this;
        }
        for (std::size_t i = index; i + 1 < size_; ++i) {
            elements_[i] = elements_[i + 1];
        }
        --size_;
        return *this;
    }

    Set operator-(const T& value) const {
        Set result = *this;
        result -= value;
        return result;
    }

    Set& operator+=(const Set& other) {
        for (std::size_t i = 0; i < other.size_; ++i) {
            *this += other.elements_[i];
        }
        return *this;
    }

    Set operator+(const Set& other) const {
        Set result = *this;
        result += other;
        return result;
    }

    Set& operator-=(const Set& other) {
        for (std::size_t i = 0; i < other.size_; ++i) {
            *this -= other.elements_[i];
        }
        return *this;
    }

    Set operator-(const Set& other) const {
        Set result = *this;
        result -= other;
        return result;
    }

    Set intersection(const Set& other) const {
        Set result;
        for (std::size_t i = 0; i < size_; ++i) {
            if (other.contains(elements_[i])) {
                result += elements_[i];
            }
        }
        return result;
    }

    bool operator==(const Set& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (std::size_t i = 0; i < size_; ++i) {
            if (!other.contains(elements_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Set& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& out, const Set<T>& s) {
        out << "{ ";
        for (std::size_t i = 0; i < s.size_; ++i) {
            out << s.elements_[i];
            if (i + 1 < s.size_) {
                out << ", ";
            }
        }
        out << " }";
        return out;
    }
};

template <typename T>
bool are_all_elements_present(const T* first, std::size_t first_size,
                              const T* second, std::size_t second_size) {
    if (first_size == 0) {
        return true;
    }
    if (second_size == 0) {
        return false;
    }
    Set<T> second_set(second, second_size);
    for (std::size_t i = 0; i < first_size; ++i) {
        if (!second_set.contains(first[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    int arr1[] = {1, 2};
    int arr2[] = {1, 2, 3};
    bool res = are_all_elements_present(arr1, 2, arr2, 3);
    std::cout << "Result: " << res << std::endl;
    return 0;
}
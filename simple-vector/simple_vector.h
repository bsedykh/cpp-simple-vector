#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // ������� ������ �� size ���������, ������������������ ��������� �� ���������
    explicit SimpleVector(size_t size) : items_(size), size_(size), capacity_(size) {
    }

    // ������� ������ �� size ���������, ������������������ ��������� value
    SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size) {
        std::fill(begin(), end(), value);
    }

    // ������� ������ �� std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector(const SimpleVector& other) : items_(other.GetSize()), size_(other.GetSize()), capacity_(other.GetSize()) {
        std::copy(other.begin(), other.end(), begin());
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            auto copy(rhs);
            swap(copy);
        }
        return *this;
    }

    SimpleVector(SimpleVector&& other) noexcept {
        swap(other);
    }

    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this != &rhs) {
            items_ = std::move(rhs.items_);
            size_ = std::exchange(rhs.size_, 0);
            capacity_ = std::exchange(rhs.capacity_, 0);
        }
        return *this;
    }

    // ��������� ������� � ����� �������
    // ��� �������� ����� ����������� ����� ����������� �������
    void PushBack(const Type& item) {
        Insert_(end(), item);
    }

    void PushBack(Type&& item) {
        Insert_(end(), std::move(item));
    }

    // ��������� ������� � ��������� �������
    Iterator Insert(ConstIterator pos, const Type& value) {
        return Insert_(const_cast<Iterator>(pos), value);
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        return Insert_(const_cast<Iterator>(pos), std::move(value));
    }

    // ������� ��������� ������� �������
    void PopBack() noexcept {
        assert(!IsEmpty());
        --size_;
    }

    // ������� ������� ������� � ��������� �������
    Iterator Erase(ConstIterator pos) {
        assert(cbegin() <= pos && pos < cend());

        const Iterator it = const_cast<Iterator>(pos);
        std::move(it + 1, end(), it);
        --size_;

        return it;
    }

    // ���������� �������� � ������ ��������
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // ���������� ���������� ��������� � �������
    size_t GetSize() const noexcept {
        return size_;
    }

    // ���������� ����������� �������
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // ��������, ������ �� ������
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // ���������� ������ �� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // ���������� ����������� ������ �� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    // �������� ������ �������, �� ������� ��� �����������
    void Clear() noexcept {
        size_ = 0;
    }

    // �������� ������ �������.
    // ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            const size_t new_capacity = std::max(new_size, capacity_ * 2);
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), end(), new_items.Get());

            items_ = std::move(new_items);
            capacity_ = new_capacity;
        }
        else {
            // ��������� �������� [size, new_size) ���������� �� ���������
            for (size_t i = size_; i < new_size; ++i) {
                items_[i] = Type();
            }
        }

        size_ = new_size;
    }

    // �������� ����������� �������
    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        ArrayPtr<Type> new_items(new_capacity);
        std::move(begin(), end(), new_items.Get());

        items_ = std::move(new_items);
        capacity_ = new_capacity;
    }

    // ���������� �������� �� ������ �������
    Iterator begin() noexcept {
        return items_.Get();
    }

    // ���������� �������� �� �������, ��������� �� ���������
    Iterator end() noexcept {
        return begin() + size_;
    }

    // ���������� ����������� �������� �� ������ �������
    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    // ���������� �������� �� �������, ��������� �� ���������
    ConstIterator end() const noexcept {
        return begin() + size_;
    }

    // ���������� ����������� �������� �� ������ �������
    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    // ���������� �������� �� �������, ��������� �� ���������
    ConstIterator cend() const noexcept {
        return begin() + size_;
    }

private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;

    // ��������� �������� value � ������� pos.
    // ���������� �������� �� ����������� ��������
    template<typename T>
    Iterator Insert_(Iterator pos, T&& value) {
        assert(begin() <= pos && pos <= end());

        const size_t insert_index = pos - begin();

        if (size_ < capacity_) {
            std::move_backward(pos, end(), end() + 1);
            items_[insert_index] = std::forward<T>(value);
        }
        else {
            const size_t new_capacity = std::max(static_cast<size_t>(1), capacity_ * 2);
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), pos, new_items.Get());
            new_items[insert_index] = std::forward<T>(value);
            std::move(pos, end(), new_items.Get() + insert_index + 1);

            items_ = std::move(new_items);
            capacity_ = new_capacity;
        }
        ++size_;

        return &items_[insert_index];
    }
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}

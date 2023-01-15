#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

struct ReserveProxyObj {
    size_t capacity_to_reserve;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj{ capacity_to_reserve };
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : items_(size), size_(size), capacity_(size) {
    }

    explicit SimpleVector(ReserveProxyObj obj) : items_(obj.capacity_to_reserve), size_(0), capacity_(obj.capacity_to_reserve) {
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size) {
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector(const SimpleVector& other) : items_(other.GetSize()), size_(other.GetSize()), capacity_(other.GetSize()) {
        std::copy(other.begin(), other.end(), begin());
    }

    SimpleVector(SimpleVector&& other) noexcept {
        swap(other);
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            auto copy(rhs);
            swap(copy);
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this != &rhs) {
            auto moved_to(std::move(rhs));
            swap(moved_to);
        }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        Insert_(end(), item);
    }

    void PushBack(Type&& item) {
        Insert_(end(), std::move(item));
    }

     Iterator Insert(ConstIterator pos, const Type& value) {
        return Insert_(const_cast<Iterator>(pos), value);
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        return Insert_(const_cast<Iterator>(pos), std::move(value));
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(!IsEmpty());
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(!IsEmpty());

        const Iterator it = const_cast<Iterator>(pos);
        std::move(it + 1, end(), it);
        --size_;

        return it;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }


    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            const size_t new_capacity = std::max(new_size, capacity_ * 2);
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), end(), new_items.Get());

            items_.swap(new_items);
            capacity_ = new_capacity;
        }
        else {
            // fill with zero values for range: [size, new_size)
            for (size_t i = size_; i < new_size; ++i) {
                items_[i] = Type();
            }
        }

        size_ = new_size;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        ArrayPtr<Type> new_items(new_capacity);
        std::move(begin(), end(), new_items.Get());

        items_.swap(new_items);
        capacity_ = new_capacity;
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return begin() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return begin() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return begin() + size_;
    }

private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    template<typename T>
    Iterator Insert_(Iterator pos, T&& value) {
        const size_t insert_index = pos - begin();

        if (size_ < capacity_) {
            std::move_backward(pos, end(), end() + 1);
            items_[insert_index] = std::forward<T>(value);       
        }
        else {
            // copy and double the capacity
            const size_t new_capacity = std::max(static_cast<size_t>(1), capacity_ * 2);
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), pos, new_items.Get());

            // assign new value
            new_items[insert_index] = std::forward<T>(value);    

            // copy remaining values
            std::move(pos, end(), new_items.Get() + insert_index + 1);

            // swap
            items_.swap(new_items);
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

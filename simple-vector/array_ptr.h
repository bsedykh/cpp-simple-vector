#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;

    // Создает массив из size элементов, инициализированных значением по умолчанию
    explicit ArrayPtr(size_t size) {
        if (size != 0) {
            raw_ptr_ = new Type[size]();
        }
    }

    // Конструктор из сырого указателя, хранящего адрес массива
    explicit ArrayPtr(Type* raw_ptr) noexcept : raw_ptr_(raw_ptr) {
    }

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr(ArrayPtr&& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

    ArrayPtr& operator=(ArrayPtr&& rhs) noexcept {
        delete[] raw_ptr_;
        raw_ptr_ = rhs.raw_ptr_;
        rhs.raw_ptr_ = nullptr;
        return *this;
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    [[nodiscard]] Type* Release() noexcept {
        Type* p = raw_ptr_;
        raw_ptr_ = nullptr;
        return p;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        assert(raw_ptr_);
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(raw_ptr_);
        return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return raw_ptr_;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};

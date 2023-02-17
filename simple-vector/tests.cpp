#include <cassert>
#include <iostream>
#include <numeric>
#include <utility>

#include "tests.h"

namespace tests {

    namespace detail {

        X::X() : X(5) {
        }

        X::X(size_t num) : x_(num) {
        }

        X::X(X&& other) noexcept {
            x_ = std::exchange(other.x_, 0);
        }

        X& X::operator=(X&& other) noexcept {
            x_ = std::exchange(other.x_, 0);
            return *this;
        }

        size_t X::GetX() const {
            return x_;
        }

        SimpleVector<int> GenerateVector(size_t size) {
            SimpleVector<int> v(size);
            std::iota(v.begin(), v.end(), 1);
            return v;
        }

    }

    void DefaultConstructor() {
        using namespace std::string_literals;
        std::cout << "Test default constructor"s << std::endl;
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void SizeConstructor() {
        using namespace std::string_literals;
        std::cout << "Test size constructor"s << std::endl;
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void SizeValueConstructor() {
        using namespace std::string_literals;
        std::cout << "Test size & value constructor"s << std::endl;
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void InitializerListConstructor() {
        using namespace std::string_literals;
        std::cout << "Test initializer list constructor"s << std::endl;
        SimpleVector<int> v{ 1, 2, 3 };
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void At() {
        using namespace std::string_literals;
        std::cout << "Test at"s << std::endl;
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false); 
        }
        catch (const std::out_of_range&) {
        }
        catch (...) {
            assert(false); 
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Clear() {
        using namespace std::string_literals;
        std::cout << "Test clear"s << std::endl;
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Resize() {
        using namespace std::string_literals;
        std::cout << "Test resize"s << std::endl;
        {
            SimpleVector<int> v(3);
            v[2] = 17;
            v.Resize(7);
            assert(v.GetSize() == 7);
            assert(v.GetCapacity() >= v.GetSize());
            assert(v[2] == 17);
            assert(v[3] == 0);
        }
        {
            SimpleVector<int> v(3);
            v[0] = 42;
            v[1] = 55;
            const size_t old_capacity = v.GetCapacity();
            v.Resize(2);
            assert(v.GetSize() == 2);
            assert(v.GetCapacity() == old_capacity);
            assert(v[0] == 42);
            assert(v[1] == 55);
        }
        {
            const size_t old_size = 3;
            SimpleVector<int> v(3);
            v.Resize(old_size + 5);
            v[3] = 42;
            v.Resize(old_size);
            v.Resize(old_size + 2);
            assert(v[3] == 0);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Iterate() {
        using namespace std::string_literals;
        std::cout << "Test iterate"s << std::endl;
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void PushBack() {
        using namespace std::string_literals;
        std::cout << "Test push back"s << std::endl;
        {
            SimpleVector<int> v(1);
            v.PushBack(42);
            assert(v.GetSize() == 2);
            assert(v.GetCapacity() >= v.GetSize());
            assert(v[0] == 0);
            assert(v[1] == 42);
        }

        // Если хватает места, PushBack не увеличивает Capacity
        {    
            SimpleVector<int> v(2);
            v.Resize(1);
            const size_t old_capacity = v.GetCapacity();
            v.PushBack(123);
            assert(v.GetSize() == 2);
            assert(v.GetCapacity() == old_capacity);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void PopBack() {
        using namespace std::string_literals;
        std::cout << "Test pop back"s << std::endl;
        SimpleVector<int> v{ 0, 1, 2, 3 };
        const size_t old_capacity = v.GetCapacity();
        const auto old_begin = v.begin();
        v.PopBack();
        assert(v.GetCapacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void CopyConstructor() {
        using namespace std::string_literals;
        std::cout << "Test copy constructor"s << std::endl;
        SimpleVector<int> numbers{ 1, 2 };
        auto numbers_copy(numbers);
        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.GetSize() == numbers.GetSize());
        for (size_t i = 0; i < numbers.GetSize(); ++i) {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Compare() {
        using namespace std::string_literals;
        std::cout << "Test compare"s << std::endl;
        assert((SimpleVector{ 1, 2, 3 } == SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } != SimpleVector{ 1, 2, 2 }));

        assert((SimpleVector{ 1, 2, 3 } < SimpleVector{ 1, 2, 3, 1 }));
        assert((SimpleVector{ 1, 2, 3 } > SimpleVector{ 1, 2, 2, 1 }));

        assert((SimpleVector{ 1, 2, 3 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 4 } >= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 3 }));
        assert((SimpleVector{ 1, 2, 3 } <= SimpleVector{ 1, 2, 4 }));
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Swap() {
        using namespace std::string_literals;
        std::cout << "Test swap"s << std::endl;
        SimpleVector<int> v1{ 42, 666 };
        SimpleVector<int> v2;
        v2.PushBack(0);
        v2.PushBack(1);
        v2.PushBack(2);
        const int* const begin1 = &v1[0];
        const int* const begin2 = &v2[0];

        const size_t capacity1 = v1.GetCapacity();
        const size_t capacity2 = v2.GetCapacity();

        const size_t size1 = v1.GetSize();
        const size_t size2 = v2.GetSize();

        v1.swap(v2);
        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.GetSize() == size2);
        assert(v2.GetSize() == size1);
        assert(v1.GetCapacity() == capacity2);
        assert(v2.GetCapacity() == capacity1);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void CopyAssignment() {
        using namespace std::string_literals;
        std::cout << "Test copy assignment"s << std::endl;
        SimpleVector<int> src_vector{ 1, 2, 3, 4 };
        SimpleVector<int> dst_vector{ 1, 2, 3, 4, 5, 6 };
        dst_vector = src_vector;
        assert(dst_vector == src_vector);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Insert() {
        using namespace std::string_literals;
        std::cout << "Test insert"s << std::endl;
        SimpleVector<int> v{ 1, 2, 3, 4 };
        v.Insert(v.begin() + 2, 42);
        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void Erase() {
        using namespace std::string_literals;
        std::cout << "Test erase"s << std::endl;
        SimpleVector<int> v{ 1, 2, 3, 4 };
        v.Erase(v.cbegin() + 2);
        assert((v == SimpleVector<int>{1, 2, 4}));
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void MoveConstructor() {
        using namespace std::string_literals;
        const size_t size = 1000000;
        std::cout << "Test move constructor"s << std::endl;
        SimpleVector<int> vector_to_move = detail::GenerateVector(size);
        assert(vector_to_move.GetSize() == size);

        SimpleVector<int> moved_vector(std::move(vector_to_move));
        assert(moved_vector.GetSize() == size);
        assert(vector_to_move.GetSize() == 0);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void MoveAssignment() {
        using namespace std::string_literals;
        const size_t size = 1000000;
        std::cout << "Test move assignment"s << std::endl;
        SimpleVector<int> moved_vector;
        assert(moved_vector.GetSize() == 0);
        SimpleVector<int> vector_to_move = detail::GenerateVector(size);
        assert(vector_to_move.GetSize() == size);

        moved_vector = std::move(vector_to_move);
        assert(moved_vector.GetSize() == size);
        assert(vector_to_move.GetSize() == 0);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void NoncopiableMoveConstructor() {
        using namespace std::string_literals;
        const size_t size = 5;
        std::cout << "Test noncopiable object, move constructor"s << std::endl;
        SimpleVector<detail::X> vector_to_move;
        for (size_t i = 0; i < size; ++i) {
            vector_to_move.PushBack(detail::X(i));
        }

        SimpleVector<detail::X> moved_vector = std::move(vector_to_move);
        assert(moved_vector.GetSize() == size);
        assert(vector_to_move.GetSize() == 0);

        for (size_t i = 0; i < size; ++i) {
            assert(moved_vector[i].GetX() == i);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void NoncopiablePushBack() {
        using namespace std::string_literals;
        const size_t size = 5;
        std::cout << "Test noncopiable push back"s << std::endl;
        SimpleVector<detail::X> v;
        for (size_t i = 0; i < size; ++i) {
            v.PushBack(detail::X(i));
        }

        assert(v.GetSize() == size);

        for (size_t i = 0; i < size; ++i) {
            assert(v[i].GetX() == i);
        }
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void NoncopiableInsert() {
        using namespace std::string_literals;
        const size_t size = 5;
        std::cout << "Test noncopiable insert"s << std::endl;
        SimpleVector<detail::X> v;
        for (size_t i = 0; i < size; ++i) {
            v.PushBack(detail::X(i));
        }

        // в начало
        v.Insert(v.begin(), detail::X(size + 1));
        assert(v.GetSize() == size + 1);
        assert(v.begin()->GetX() == size + 1);
        // в конец
        v.Insert(v.end(), detail::X(size + 2));
        assert(v.GetSize() == size + 2);
        assert((v.end() - 1)->GetX() == size + 2);
        // в середину
        v.Insert(v.begin() + 3, detail::X(size + 3));
        assert(v.GetSize() == size + 3);
        assert((v.begin() + 3)->GetX() == size + 3);
        std::cout << "Done!"s << std::endl << std::endl;
    }

    void NoncopiableErase() {
        using namespace std::string_literals;
        const size_t size = 3;
        std::cout << "Test noncopiable erase"s << std::endl;
        SimpleVector<detail::X> v;
        for (size_t i = 0; i < size; ++i) {
            v.PushBack(detail::X(i));
        }

        auto it = v.Erase(v.begin());
        assert(it->GetX() == 1);
        std::cout << "Done!"s << std::endl << std::endl;
    }

}

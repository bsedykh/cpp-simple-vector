#pragma once

#include "simple_vector.h"

namespace tests {

    namespace detail {

        class X {
        public:
            X();

            X(size_t num);

            X(const X& other) = delete;
            X& operator=(const X& other) = delete;

            X(X&& other) noexcept;

            X& operator=(X&& other) noexcept;

            size_t GetX() const;

        private:
            size_t x_;
        };

        SimpleVector<int> GenerateVector(size_t size);

    }

    void DefaultConstructor();

    void SizeConstructor();

    void SizeValueConstructor();

    void InitializerListConstructor();

    void At();

    void Clear();

    void Resize();

    void Iterate();

    void PushBack();

    void PopBack();

    void CopyConstructor();

    void Compare();

    void Swap();

    void CopyAssignment();
    
    void Insert();

    void Erase();

    void MoveConstructor();

    void MoveAssignment();

    void NoncopiableMoveConstructor();

    void NoncopiablePushBack();

    void NoncopiableInsert();

    void NoncopiableErase(); 

}

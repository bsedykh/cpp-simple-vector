#pragma once

#include "simple_vector.h"

namespace tests {

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

    void TemporaryObjConstructor();

    void TemporaryObjOperator();

    void NamedMoveConstructor();

    void NamedMoveOperator();

    void NoncopiableMoveConstructor();

    void NoncopiablePushBack();

    void NoncopiableInsert();

    void NoncopiableErase(); 
}

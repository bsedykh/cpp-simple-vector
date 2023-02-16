#include "simple_vector.h"
#include "tests.h"

int main() {
    tests::TemporaryObjConstructor();
    tests::TemporaryObjOperator();
    tests::NamedMoveConstructor();
    tests::NamedMoveOperator();
    tests::NoncopiableMoveConstructor();
    tests::NoncopiablePushBack();
    tests::NoncopiableInsert();
    tests::NoncopiableErase();
}

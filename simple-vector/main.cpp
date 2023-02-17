#include "simple_vector.h"
#include "tests.h"

int main() {
    tests::DefaultConstructor();
    tests::SizeConstructor();
    tests::SizeValueConstructor();
    tests::InitializerListConstructor();
    tests::At();
    tests::Clear();
    tests::Resize();
    tests::Iterate();
    tests::PushBack();
    tests::PopBack();
    tests::CopyConstructor();
    tests::Compare();
    tests::Swap();
    tests::CopyAssignment();
    tests::Insert();
    tests::Erase();
    tests::MoveConstructor();
    tests::MoveAssignment();
    tests::NoncopiableMoveConstructor();
    tests::NoncopiablePushBack();
    tests::NoncopiableInsert();
    tests::NoncopiableErase();
}

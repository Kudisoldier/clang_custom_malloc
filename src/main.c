#include "tests.h"

int main() {
    void* heap = test1();
    test2(heap);
    test3(heap);
    test4(heap);
    test5(heap);
    test6(heap);

    return 0;
}

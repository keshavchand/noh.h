#define FN_IMPL_FLAG TEST_IMPLEMENTATION
#define FN_DYNAMIC_LOADER_FLAG  TEST_DYNMAIC_LOADER
#include "../noh.h"

FN(int, add, int a, int b) (
    int result = a + b;
    printf("%s %d %d = %d\n", __FUNCTION__, a, b, result);

    return result;
)

FN(int, sub, int a, int b) (
    int result = a - b;

    printf("%s %d %d = %d\n", __FUNCTION__, a, b, result);
    return result;
)


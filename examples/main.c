#include "test.h"

int main() {
    fnLoadSymbols_main("./test.so");
    add(1, 2);
    sub(2, 3);
}

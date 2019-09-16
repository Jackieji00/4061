#include "b.h"
#include "a.h"
#include <stdio.h>

int funcb() {
    return 2 * libfuncb();
}

int main(){
    printf("%d\n", funcb() + 3 * libfunca() + 1);
    return 0;
}

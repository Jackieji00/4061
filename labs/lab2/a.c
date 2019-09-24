#include "lib1.h"
#include "a.h"

int libfunca(){
    return 9;
}

int funca(){
    return 1 + libfunc() + libfunca();
}

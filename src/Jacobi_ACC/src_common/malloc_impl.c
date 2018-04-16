#if defined(__MACH__)

#include <stdlib.h>

void *memalign(int a, int b) {
    return malloc(b);
}

#endif //DEEPFLOW_MALLOC_H

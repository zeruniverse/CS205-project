//
// Created by Hongxiang Qiu on 4/10/18.
//

#ifndef DEEPFLOW_MALLOC_H
#define DEEPFLOW_MALLOC_H

#if defined(__MACH__)
#include <stdlib.h>
void* memalign(int a,int b);
#else
#include <malloc.h>
#endif

#endif //DEEPFLOW_MALLOC_H

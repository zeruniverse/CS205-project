/*
Harvard CS 205, Spring 2018, Team 5

Based on original implementation

Solver changed from SOR to Jacobi since SOR can't be easily parallelized
*/
#include <stdio.h>
#include <stdlib.h>

#include "image.h"

// Perform n iterations of the Jacobi algorithm for a system of the form as described in opticalflow.c
void jacobi(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                     const image_t *b1, const image_t *b2, const image_t *dpsis_horiz, const image_t *dpsis_vert,
                     const int iterations);

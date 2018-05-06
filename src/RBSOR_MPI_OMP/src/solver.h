/*
Harvard CS 205, Spring 2018, Team 5

Implementation of Red Black SOR
*/
#include <stdio.h>
#include <stdlib.h>

#include "image.h"

// Perform n iterations of the sor_coupled algorithm for a system of the form as described in opticalflow.c
void sor_coupled(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22, const image_t *b1, const image_t *b2, const image_t *dpsis_horiz, const image_t *dpsis_vert, const int iterations, const float omega);

/*
Copyright (C) 2013 Philippe Weinzaepfel

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "image_acc.h"
#include "solver_acc.h"

#include "openacc.h"

void calculate_constants(float *A11m, float *A12m, float *A22m,
                         image_t *du, image_t *dv,
                         const image_t *a11, const image_t *a12, const image_t *a22,
                         const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                         const image_t *dpsis_vert) {
    for (int j = 0; j < du->height; j++) {
        for (int i = 0; i < dv->width; i++) {
            float sum_dpsis = 0.0f, A11, A22, A12, det;
            if (j > 0) {
                sum_dpsis += dpsis_vert->data[(j - 1) * du->stride + i];
            }
            if (i > 0) {
                sum_dpsis += dpsis_horiz->data[j * du->stride + i - 1];
            }
            if (j < du->height - 1) {
                sum_dpsis += dpsis_vert->data[j * du->stride + i];
            }
            if (i < du->width - 1) {
                sum_dpsis += dpsis_horiz->data[j * du->stride + i];
            }
            A11 = a11->data[j * du->stride + i] + sum_dpsis;
            A12 = a12->data[j * du->stride + i];
            A22 = a22->data[j * du->stride + i] + sum_dpsis;
            det = A11 * A22 - A12 * A12;
            A11m[j * du->stride + i] = A11 / det;
            A12m[j * du->stride + i] = A12 / det;
            A22m[j * du->stride + i] = A22 / det;
        }
    }
}

void sor_coupled_acc(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                     const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                     const image_t *dpsis_vert, const int iterations, const float omega) {
    const int N = du->stride * du->height;
    const int stride = du->stride;
    const int H = du->height;
    const int W = du->width;

    float *from_u = du->data;
    float *from_v = dv->data;

    if (from_u == NULL) {
        printf("Failed to alloc memory of size %d for u", N);
        exit(-1);
    }
    if (from_v == NULL) {
        printf("Failed to alloc memory of size %d for v", N);
        exit(-1);
    }

    float *to_u = (float *) malloc(N * sizeof(float));
    float *to_v = (float *) malloc(N * sizeof(float));

    if (to_u == NULL) {
        printf("Failed to alloc memory of size %d for u", N);
        exit(-1);
    }
    if (to_v == NULL) {
        printf("Failed to alloc memory of size %d for v", N);
        exit(-1);
    }

    float *A11m = (float *) malloc(N * sizeof(float));
    float *A12m = (float *) malloc(N * sizeof(float));
    float *A22m = (float *) malloc(N * sizeof(float));

    calculate_constants(A11m, A12m, A22m,
                        du, dv, a11, a12, a22, b1, b2, dpsis_horiz, dpsis_vert);

    const float *b1_data = b1->data;
    const float *b2_data = b2->data;

    const float *dph = dpsis_horiz->data;
    const float *dpv = dpsis_vert->data;


#pragma acc data copyin(dph[0:N], dpv[0:N], A11m[0:N], A12m[0:N], A22m[0:N], b1_data[0:N], b2_data[0:N], to_u[0:N], to_v[0:N]) copy(from_u[0:N], from_v[0:N])
    {
        for (int iter = 0; iter < iterations / 2; iter++) {
#pragma acc parallel loop independent
            for (int j = 0; j < H; j++) {
#pragma acc loop independent private(j)
                for (int i = 0; i < W; i++) {
                    float sigma_u, sigma_v, A11, A22, A12, B1, B2;
                    sigma_u = 0.0f;
                    sigma_v = 0.0f;
                    if (j > 0) {
                        sigma_u -= dpv[(j - 1) * stride + i] * from_u[(j - 1) * stride + i];
                        sigma_v -= dpv[(j - 1) * stride + i] * from_v[(j - 1) * stride + i];
                    }
                    if (i > 0) {
                        sigma_u -= dph[j * stride + i - 1] * from_u[j * stride + i - 1];
                        sigma_v -= dph[j * stride + i - 1] * from_v[j * stride + i - 1];
                    }
                    if (j < H - 1) {
                        sigma_u -= dpv[j * stride + i] * from_u[(j + 1) * stride + i];
                        sigma_v -= dpv[j * stride + i] * from_v[(j + 1) * stride + i];
                    }
                    if (i < W - 1) {
                        sigma_u -= dph[j * stride + i] * from_u[j * stride + i + 1];
                        sigma_v -= dph[j * stride + i] * from_v[j * stride + i + 1];
                    }
                    B1 = b1_data[j * stride + i] - sigma_u;
                    B2 = b2_data[j * stride + i] - sigma_v;

                    A11 = A11m[j * stride + i];
                    A12 = A12m[j * stride + i];
                    A22 = A22m[j * stride + i];

                    float new_u = A22 * B1 - A12 * B2;
                    float new_v = -A12 * B1 + A11 * B2;

                    to_u[j * stride + i] = new_u;
                    to_v[j * stride + i] = new_v;
                }
            }
#pragma acc parallel loop
            for (int j = 0; j < H; j++) {
#pragma acc loop
                for (int i = 0; i < W; i++) {
                    float sigma_u, sigma_v, A11, A22, A12, B1, B2;
                    sigma_u = 0.0f;
                    sigma_v = 0.0f;
                    if (j > 0) {
                        sigma_u -= dpv[(j - 1) * stride + i] * to_u[(j - 1) * stride + i];
                        sigma_v -= dpv[(j - 1) * stride + i] * to_v[(j - 1) * stride + i];
                    }
                    if (i > 0) {
                        sigma_u -= dph[j * stride + i - 1] * to_u[j * stride + i - 1];
                        sigma_v -= dph[j * stride + i - 1] * to_v[j * stride + i - 1];
                    }
                    if (j < H - 1) {
                        sigma_u -= dpv[j * stride + i] * to_u[(j + 1) * stride + i];
                        sigma_v -= dpv[j * stride + i] * to_v[(j + 1) * stride + i];
                    }
                    if (i < W - 1) {
                        sigma_u -= dph[j * stride + i] * to_u[j * stride + i + 1];
                        sigma_v -= dph[j * stride + i] * to_v[j * stride + i + 1];
                    }
                    B1 = b1_data[j * stride + i] - sigma_u;
                    B2 = b2_data[j * stride + i] - sigma_v;

                    A11 = A11m[j * stride + i];
                    A12 = A12m[j * stride + i];
                    A22 = A22m[j * stride + i];

                    float new_u = A22 * B1 - A12 * B2;
                    float new_v = -A12 * B1 + A11 * B2;

                    from_u[j * stride + i] = A22 * B1 - A12 * B2;
                    from_v[j * stride + i] = -A12 * B1 + A11 * B2;
                }
            }
        }
    }

    free(A11m);
    free(A12m);
    free(A22m);
    free(to_u);
    free(to_v);
}

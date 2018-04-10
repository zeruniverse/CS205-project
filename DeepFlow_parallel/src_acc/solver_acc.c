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

void swap(float **a, float **b) {
    float *temp = *a;
    *a = *b;
    *b = temp;
}

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

void sor_coupled_mpi(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                     const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                     const image_t *dpsis_vert, const int iterations, float omega) {
    float *buffer_u = (float *) memalign(16, du->stride * du->height * sizeof(float));
    float *buffer_v = (float *) memalign(16, dv->stride * dv->height * sizeof(float));

    float *from_u = du->data;
    float *from_v = dv->data;

    float *to_u = buffer_u;
    float *to_v = buffer_v;

    float *A11m = (float *) memalign(16, du->stride * du->height * sizeof(float));
    float *A12m = (float *) memalign(16, du->stride * du->height * sizeof(float));
    float *A22m = (float *) memalign(16, du->stride * du->height * sizeof(float));

    calculate_constants(A11m, A12m, A22m,
                        du, dv, a11, a12, a22, b1, b2, dpsis_horiz, dpsis_vert);

    swap(&from_u, &to_u);
    swap(&from_v, &to_v);

    omega = 1.0;

    for (int iter = 0; iter < iterations; iter++) {
        for (int p = 0; p <= 0; p++) {
            swap(&from_u, &to_u);
            swap(&from_v, &to_v);
#pragma omp parallel for
            for (int j = 0; j < du->height; j++) {
                float sigma_u, sigma_v, A11, A22, A12, B1, B2;
                for (int i = 0; i < du->width; i++) {
                    sigma_u = 0.0f;
                    sigma_v = 0.0f;
                    if (j > 0) {
                        sigma_u -= dpsis_vert->data[(j - 1) * du->stride + i] * from_u[(j - 1) * du->stride + i];
                        sigma_v -= dpsis_vert->data[(j - 1) * du->stride + i] * from_v[(j - 1) * du->stride + i];
                    }
                    if (i > 0) {
                        sigma_u -= dpsis_horiz->data[j * du->stride + i - 1] * from_u[j * du->stride + i - 1];
                        sigma_v -= dpsis_horiz->data[j * du->stride + i - 1] * from_v[j * du->stride + i - 1];
                    }
                    if (j < du->height - 1) {
                        sigma_u -= dpsis_vert->data[j * du->stride + i] * from_u[(j + 1) * du->stride + i];
                        sigma_v -= dpsis_vert->data[j * du->stride + i] * from_v[(j + 1) * du->stride + i];
                    }
                    if (i < du->width - 1) {
                        sigma_u -= dpsis_horiz->data[j * du->stride + i] * from_u[j * du->stride + i + 1];
                        sigma_v -= dpsis_horiz->data[j * du->stride + i] * from_v[j * du->stride + i + 1];
                    }
                    B1 = b1->data[j * du->stride + i] - sigma_u;
                    B2 = b2->data[j * du->stride + i] - sigma_v;

                    A11 = A11m[j * du->stride + i];
                    A12 = A12m[j * du->stride + i];
                    A22 = A22m[j * du->stride + i];

                    to_u[j * du->stride + i] =
                            (1.0f - omega) * from_u[j * du->stride + i] + omega * (A22 * B1 - A12 * B2);
                    to_v[j * du->stride + i] =
                            (1.0f - omega) * from_v[j * du->stride + i] + omega * (-A12 * B1 + A11 * B2);
                }
            }
        }
    }

    if (to_u != du->data) {
        memcpy(du->data, to_u, du->stride * du->height * sizeof(float));
        memcpy(dv->data, to_v, dv->stride * dv->height * sizeof(float));
    }

    free(A11m);
    free(A12m);
    free(A22m);
    free(buffer_u);
    free(buffer_v);
}

void sor_coupled_mpi_ori(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                         const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                         const image_t *dpsis_vert, const int iterations, const float omega) {
    for (int iter = 0; iter < iterations; iter++) {
        for (int j = 0; j < du->height; j++) {
            float sigma_u, sigma_v, sum_dpsis, A11, A22, A12, B1, B2, det;
            for (int i = 0; i < du->width; i++) {
                sigma_u = 0.0f;
                sigma_v = 0.0f;
                sum_dpsis = 0.0f;
                if (j > 0) {
                    sigma_u -= dpsis_vert->data[(j - 1) * du->stride + i] * du->data[(j - 1) * du->stride + i];
                    sigma_v -= dpsis_vert->data[(j - 1) * du->stride + i] * dv->data[(j - 1) * du->stride + i];
                    sum_dpsis += dpsis_vert->data[(j - 1) * du->stride + i];
                }
                if (i > 0) {
                    sigma_u -= dpsis_horiz->data[j * du->stride + i - 1] * du->data[j * du->stride + i - 1];
                    sigma_v -= dpsis_horiz->data[j * du->stride + i - 1] * dv->data[j * du->stride + i - 1];
                    sum_dpsis += dpsis_horiz->data[j * du->stride + i - 1];
                }
                if (j < du->height - 1) {
                    sigma_u -= dpsis_vert->data[j * du->stride + i] * du->data[(j + 1) * du->stride + i];
                    sigma_v -= dpsis_vert->data[j * du->stride + i] * dv->data[(j + 1) * du->stride + i];
                    sum_dpsis += dpsis_vert->data[j * du->stride + i];
                }
                if (i < du->width - 1) {
                    sigma_u -= dpsis_horiz->data[j * du->stride + i] * du->data[j * du->stride + i + 1];
                    sigma_v -= dpsis_horiz->data[j * du->stride + i] * dv->data[j * du->stride + i + 1];
                    sum_dpsis += dpsis_horiz->data[j * du->stride + i];
                }
                A11 = a11->data[j * du->stride + i] + sum_dpsis;
                A12 = a12->data[j * du->stride + i];
                A22 = a22->data[j * du->stride + i] + sum_dpsis;
                det = A11 * A22 - A12 * A12;
                B1 = b1->data[j * du->stride + i] - sigma_u;
                B2 = b2->data[j * du->stride + i] - sigma_v;
                du->data[j * du->stride + i] =
                        (1.0f - omega) * du->data[j * du->stride + i] + omega * (A22 * B1 - A12 * B2) / det;
                dv->data[j * du->stride + i] =
                        (1.0f - omega) * dv->data[j * du->stride + i] + omega * (-A12 * B1 + A11 * B2) / det;
            }
        }
    }
}

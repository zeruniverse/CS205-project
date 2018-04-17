/*
Harvard CS 205, Spring 2018, Team 5

Based on original implementation

Solver changed from SOR to RBSOR
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../src_common/malloc.h"

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


void sor_coupled(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                       const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                       const image_t *dpsis_vert, const int iterations, float omega) {
    const int stride = du->stride;
    const int H = du->height;
    const int W = du->width;
    const int N = stride * H;

    float *dud = du->data;
    float *dvd = dv->data;

    float *A11m = (float *) memalign(16, N * sizeof(float));
    float *A12m = (float *) memalign(16, N * sizeof(float));
    float *A22m = (float *) memalign(16, N * sizeof(float));

    calculate_constants(A11m, A12m, A22m,
                        du, dv, a11, a12, a22, b1, b2, dpsis_horiz, dpsis_vert);

    const float *dpv = dpsis_vert->data;
    const float *dph = dpsis_horiz->data;
    const float *b1d = b1->data;
    const float *b2d = b2->data;
    #pragma acc data copyin(dph[0:N], dpv[0:N], A11m[0:N], A12m[0:N], A22m[0:N], b1d[0:N], b2d[0:N]) copy(dud[0:N], dvd[0:N])
    {
        for (int iter = 0; iter < iterations; iter++) {
            #pragma acc parallel loop independent
            for (int j = 0; j < H; j++) {
                #pragma acc loop independent private(j)
                for (int i = (j % 2) + 1; i < W; i += 2) {
                    float sigma_u, sigma_v, A11, A22, A12, B1, B2;
                    sigma_u = 0.0f;
                    sigma_v = 0.0f;
                    if (j > 0) {
                        sigma_u -= dpv[(j - 1) * stride + i] * dud[(j - 1) * stride + i];
                        sigma_v -= dpv[(j - 1) * stride + i] * dvd[(j - 1) * stride + i];
                    }
                    if (i > 0) {
                        sigma_u -= dph[j * stride + i - 1] * dud[j * stride + i - 1];
                        sigma_v -= dph[j * stride + i - 1] * dvd[j * stride + i - 1];
                    }
                    if (j < H - 1) {
                        sigma_u -= dpv[j * stride + i] * dud[(j + 1) * stride + i];
                        sigma_v -= dpv[j * stride + i] * dvd[(j + 1) * stride + i];
                    }
                    if (i < W - 1) {
                        sigma_u -= dph[j * stride + i] * dud[j * stride + i + 1];
                        sigma_v -= dph[j * stride + i] * dvd[j * stride + i + 1];
                    }
                    B1 = b1d[j * stride + i] - sigma_u;
                    B2 = b2d[j * stride + i] - sigma_v;

                    A11 = A11m[j * stride + i];
                    A12 = A12m[j * stride + i];
                    A22 = A22m[j * stride + i];

                    dud[j * stride + i] =
                            (1.0f - omega) * dud[j * stride + i] + omega * (A22 * B1 - A12 * B2);
                    dvd[j * stride + i] =
                            (1.0f - omega) * dvd[j * stride + i] + omega * (-A12 * B1 + A11 * B2);
                }
            }
            #pragma acc parallel loop independent
            for (int j = 0; j < H; j++) {
                #pragma acc loop independent private(j)
                for (int i = (j % 2); i < W; i += 2) {
                    float sigma_u, sigma_v, A11, A22, A12, B1, B2;
                    sigma_u = 0.0f;
                    sigma_v = 0.0f;
                    if (j > 0) {
                        sigma_u -= dpv[(j - 1) * stride + i] * dud[(j - 1) * stride + i];
                        sigma_v -= dpv[(j - 1) * stride + i] * dvd[(j - 1) * stride + i];
                    }
                    if (i > 0) {
                        sigma_u -= dph[j * stride + i - 1] * dud[j * stride + i - 1];
                        sigma_v -= dph[j * stride + i - 1] * dvd[j * stride + i - 1];
                    }
                    if (j < H - 1) {
                        sigma_u -= dpv[j * stride + i] * dud[(j + 1) * stride + i];
                        sigma_v -= dpv[j * stride + i] * dvd[(j + 1) * stride + i];
                    }
                    if (i < W - 1) {
                        sigma_u -= dph[j * stride + i] * dud[j * stride + i + 1];
                        sigma_v -= dph[j * stride + i] * dvd[j * stride + i + 1];
                    }
                    B1 = b1d[j * stride + i] - sigma_u;
                    B2 = b2d[j * stride + i] - sigma_v;

                    A11 = A11m[j * stride + i];
                    A12 = A12m[j * stride + i];
                    A22 = A22m[j * stride + i];

                    dud[j * stride + i] =
                            (1.0f - omega) * dud[j * stride + i] + omega * (A22 * B1 - A12 * B2);
                    dvd[j * stride + i] =
                            (1.0f - omega) * dvd[j * stride + i] + omega * (-A12 * B1 + A11 * B2);
                }
            }
        }
    }

    free(A11m);
    free(A12m);
    free(A22m);
}
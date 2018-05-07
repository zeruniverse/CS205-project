/*
Harvard CS 205, Spring 2018, Team 5

Implementation of Red Black SOR
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "malloc.h"

#include "image.h"
#include "solver.h"
#include "mpi.h"

#include <xmmintrin.h>

typedef __v4sf v4sf;

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

void rbsor_step(const int red,
                float *du, float *dv,
                const float omega,
                const int W, const int H, const int stride,
                const float *dph, const float *dpv,
                const float *A11m, const float *A12m, const float *A22m,
                const float *b1d, const float *b2d) {
    int rank,size,numworker;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);



    for (int j = 0; j < H; j++) {
        for (int i = (j % 2) + 1 - red; i < W; i += 2) {
            float sigma_u, sigma_v, A11, A22, A12, B1, B2;
            sigma_u = 0.0f;
            sigma_v = 0.0f;
            if (j == 0){
                if (rank != 1 && rank != 0){
                    sigma_u -= dpv[(j - 1) * stride + i] * du[(j - 1) * stride + i];
                    sigma_v -= dpv[(j - 1) * stride + i] * dv[(j - 1) * stride + i];
                }
            }
            if (j > 0) {
                sigma_u -= dpv[(j - 1) * stride + i] * du[(j - 1) * stride + i];
                sigma_v -= dpv[(j - 1) * stride + i] * dv[(j - 1) * stride + i];
            }
            if (i > 0) {
                sigma_u -= dph[j * stride + i - 1] * du[j * stride + i - 1];
                sigma_v -= dph[j * stride + i - 1] * dv[j * stride + i - 1];
            }
            if (j == H-1){
                if (rank != size-1 && rank != 0){
                    sigma_u -= dpv[j * stride + i] * du[(j + 1) * stride + i];
                    sigma_v -= dpv[j * stride + i] * dv[(j + 1) * stride + i];
                }
            }
            if (j < H - 1) {
                sigma_u -= dpv[j * stride + i] * du[(j + 1) * stride + i];
                sigma_v -= dpv[j * stride + i] * dv[(j + 1) * stride + i];
            }
            if (i < W - 1) {
                sigma_u -= dph[j * stride + i] * du[j * stride + i + 1];
                sigma_v -= dph[j * stride + i] * dv[j * stride + i + 1];
            }
            B1 = b1d[j * stride + i] - sigma_u;
            B2 = b2d[j * stride + i] - sigma_v;

            A11 = A11m[j * stride + i];
            A12 = A12m[j * stride + i];
            A22 = A22m[j * stride + i];

            du[j * stride + i] =
                    (1.0f - omega) * du[j * stride + i] + omega * (A22 * B1 - A12 * B2);
            dv[j * stride + i] =
                    (1.0f - omega) * dv[j * stride + i] + omega * (-A12 * B1 + A11 * B2);
        }
    }
}

void sor_coupled(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22,
                       const image_t *b1, const image_t *b2, const image_t *dpsis_horiz,
                       const image_t *dpsis_vert, const int iterations, float omega) {
    int stride = du->stride;
    int H = du->height;
    int W = du->width;
    int N = stride * H;

    float *dud = du->data;
    float *dvd = dv->data;

    float *A11m = (float *) memalign(16, N * sizeof(float));
    float *A12m = (float *) memalign(16, N * sizeof(float));
    float *A22m = (float *) memalign(16, N * sizeof(float));


    calculate_constants(A11m, A12m, A22m,
                        du, dv, a11, a12, a22, b1, b2, dpsis_horiz, dpsis_vert);
    float *dpv = dpsis_vert->data;
    float *dph = dpsis_horiz->data;
    float *b1d = b1->data;
    float *b2d = b2->data;

    // prepare for MPI
    int rank,size,numworker,tag,FROM_MASTER = 1,FROM_WORKER = 2,offset,source,rows,averow,extra,dest,mark;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    numworker = size-1;
    if (W*H > 1000){
    // for master node
    if (rank == 0){
        averow = H/numworker;
        extra = H%numworker;
        offset = 0;
        tag = FROM_MASTER;
        for (dest = 1;dest<=numworker;dest++){
	        rows = (dest <= extra) ? averow+1:averow;
	        MPI_Send(&rows,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
	        MPI_Send(&stride,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
            if (dest == 1){
                mark = 1;
                MPI_Send(&mark,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dud,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dvd,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dpv,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            } else if (dest == numworker){
                mark = 2;
                MPI_Send(&mark,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dud+(offset-1)*stride,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dvd+(offset-1)*stride,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dpv+(offset-1)*stride,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            }
            else{
                mark = 0;
                MPI_Send(&mark,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dud+(offset-1)*stride,(rows+2)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dvd+(offset-1)*stride,(rows+2)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
                MPI_Send(dpv+(offset-1)*stride,(rows+1)*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            }
            MPI_Send(&offset,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(dph+offset*stride,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(A11m+stride*offset,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(A12m+stride*offset,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(A22m+stride*offset,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(b1d+stride*offset,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            MPI_Send(b2d+stride*offset,rows*stride,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            
            MPI_Send(&omega,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
	        MPI_Send(&W,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
            offset = offset+rows;
        }
        tag = FROM_WORKER;
        for (int i = 1;i<=numworker;i++){
            source = i;
            // receive data from worker
            // notice that only dv & du are changed
            MPI_Recv(&offset,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(&rows,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dud+offset*stride,rows*stride,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dvd+offset*stride,rows*stride,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
        }
    }
    if (rank > 0){
        tag = FROM_MASTER;
	    MPI_Recv(&rows,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
	    MPI_Recv(&stride,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(&mark,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
        if (mark == 0){
            MPI_Recv(dud,(rows+2)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dvd,(rows+2)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dpv,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
	    dud = dud+stride;
	    dvd = dvd+stride;
	    dpv = dpv+stride;
        } else if (mark == 1){
            MPI_Recv(dud,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dvd,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dpv,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        } else {
            MPI_Recv(dud,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dvd,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
            MPI_Recv(dpv,(rows+1)*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
	    dud = dud+stride;
        dvd = dvd+stride;
	    dpv = dpv+stride;
        }

        MPI_Recv(&offset,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(dph,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(A11m,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(A12m,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(A22m,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(b1d,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(b2d,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
        
        MPI_Recv(&omega,1,MPI_FLOAT,0,tag,MPI_COMM_WORLD,&status);
	    MPI_Recv(&W,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);


        // update
        tag = FROM_WORKER;
        MPI_Send(&offset,1,MPI_INT,0,tag,MPI_COMM_WORLD);
        MPI_Send(&rows,1,MPI_INT,0,tag,MPI_COMM_WORLD);

        if (mark == 0){
        // begin sor
            for (int iter = 0;iter<iterations;iter++){
                rbsor_step(0,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
                rbsor_step(1,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
            }
            //dud = dud-stride;
	        //dvd = dvd-stride;
	        //dpv = dpv-stride;
        }
	    if (mark == 1){
            for (int iter = 0;iter<iterations;iter++){
                rbsor_step(0,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
                rbsor_step(1,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
            }
	    }
        if (mark == 2) {
            for (int iter = 0;iter<iterations;iter++){
                rbsor_step(0,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
                rbsor_step(1,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
            }
	        //dud = dud-stride;
	        //dvd = dvd-stride;
	        //dpv = dpv-stride;
	    }
        //send results back to master node
        MPI_Send(du,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD);
        MPI_Send(dv,rows*stride,MPI_FLOAT,0,tag,MPI_COMM_WORLD);
    }
    }
    else{
        for (int iter = 0;iter<iterations;iter++){
                rbsor_step(0,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
                rbsor_step(1,dud,dvd,omega,W,rows,stride,dph,dpv,A11m,A12m,A22m,b1d,b2d);
            }
    }

    free(A11m);
    free(A12m);
    free(A22m);
}

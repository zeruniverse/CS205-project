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
#include "malloc.h"

#include "image.h"
#include "solver.h"

#include <xmmintrin.h>
typedef __v4sf v4sf;

//THIS IS A SLOW VERSION BUT READABLE
//Perform n iterations of the sor_coupled algorithm
//du and dv are used as initial guesses
//The system form is the same as in opticalflow.c
void sor_coupled(image_t *du, image_t *dv, const image_t *a11, const image_t *a12, const image_t *a22, const image_t *b1, const image_t *b2, const image_t *dpsis_horiz, const image_t *dpsis_vert, const int iterations, const float omega){
    int i,j,iter;
    float sigma_u,sigma_v,sum_dpsis,A11,A22,A12,B1,B2,det;
    for(iter = 0 ; iter<iterations ; iter++){
        for(j=0 ; j<du->height ; j++){
	        for(i=0 ; i<du->width ; i++){
	            sigma_u = 0.0f;
	            sigma_v = 0.0f;
	            sum_dpsis = 0.0f;
	            if(j>0){
		            sigma_u -= dpsis_vert->data[(j-1)*du->stride+i]*du->data[(j-1)*du->stride+i];
		            sigma_v -= dpsis_vert->data[(j-1)*du->stride+i]*dv->data[(j-1)*du->stride+i];
		            sum_dpsis += dpsis_vert->data[(j-1)*du->stride+i];
		        }
	            if(i>0){
                    sigma_u -= dpsis_horiz->data[j*du->stride+i-1]*du->data[j*du->stride+i-1];
                    sigma_v -= dpsis_horiz->data[j*du->stride+i-1]*dv->data[j*du->stride+i-1];
                    sum_dpsis += dpsis_horiz->data[j*du->stride+i-1];
		        }
	            if(j<du->height-1){
		            sigma_u -= dpsis_vert->data[j*du->stride+i]*du->data[(j+1)*du->stride+i];
		            sigma_v -= dpsis_vert->data[j*du->stride+i]*dv->data[(j+1)*du->stride+i];
		            sum_dpsis += dpsis_vert->data[j*du->stride+i];
		        }
	            if(i<du->width-1){
		            sigma_u -= dpsis_horiz->data[j*du->stride+i]*du->data[j*du->stride+i+1];
		            sigma_v -= dpsis_horiz->data[j*du->stride+i]*dv->data[j*du->stride+i+1];
		            sum_dpsis += dpsis_horiz->data[j*du->stride+i];
		        }
                A11 = a11->data[j*du->stride+i]+sum_dpsis;
                A12 = a12->data[j*du->stride+i];
                A22 = a22->data[j*du->stride+i]+sum_dpsis;
                det = A11*A22-A12*A12;
                B1 = b1->data[j*du->stride+i]-sigma_u;
                B2 = b2->data[j*du->stride+i]-sigma_v;
                du->data[j*du->stride+i] = (1.0f-omega)*du->data[j*du->stride+i] +omega*( A22*B1-A12*B2)/det;
                dv->data[j*du->stride+i] = (1.0f-omega)*dv->data[j*du->stride+i] +omega*(-A12*B1+A11*B2)/det;
	        }
	    }
    }
}
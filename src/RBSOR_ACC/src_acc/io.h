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
#ifndef __IO_H__
#define __IO_H__

#include <stdlib.h>

#include "image_acc.h"
#include "opticalflow_acc.h"

/* read a flow file and returns a pointer with two images containing the flow along x and y axis */
image_t** readFlowFile(const char* filename);

/* write a flow to a file */
void writeFlowFile(const char* filename, const image_t *flowx, const image_t *flowy);

/* load a color image from a file in jpg, png or ppm*/
color_image_t *color_image_load(const char *fname);


#define EXE_OPTIONS 0
#define MATLAB_OPTIONS 1
#define PYTHON_OPTIONS 2
/* print usage */
void usage(const int language);

/* parse options and eventually return the matches given as argument */
image_t** parse_options(optical_flow_params_t* params, int argc, char **argv, const int language, const int width, const int height);

#endif

/*
Copyright (c) 2012-2021 Carsten Burstedde, Donna Calhoun, Matt Zettergren
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FC3D_GEMINI_FORT_H
#define FC3D_GEMINI_FORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#if 0
/* fix syntax highlighting */
#endif


/* --------------------------------- Gemini functions ------------------------------- */

#define GEMINI_BC3_DEFAULT FCLAW_F77_FUNC(gemini_bc3_default,GEMINI_BC3_DEFAULT)

void GEMINI_BC3_DEFAULT(const int* blockno, const int* meqn, const int* mbc, 
                            const int* mx, const int* my,const int* mz,  
                            const double* xlower, const double* ylower, 
                            const double* zlower,
                            const double* dx, const double* dy, const double *dz,
                            const double q[], const int* maux, const double aux[], 
                            const double* t, const double* dt, const int mthbc[]);

#define GEMINI_SET_CAPACITY FCLAW_F77_FUNC(gemini_set_capacity,  \
                                               GEMINI_SET_CAPACITY)
void GEMINI_SET_CAPACITY(const int* blockno, const int* mx, const int *my, const int* mz, 
                             const int *mbc, const double *dx, const double* dy, 
                             double *dz, double area[],
							 const int *mcapa, const int* maux, double aux[]);


/* ----------------------------- Misc Gemini specific functions ------------------------------ */


#define GEMINI_SET_BLOCK FCLAW_F77_FUNC(gemini_set_block,GEMINI_SET_BLOCK)
void GEMINI_SET_BLOCK(int* blockno);

#define FC3D_GEMINI_GET_BLOCK FCLAW_F77_FUNC(fc3d_gemini_get_block, \
												 FC3D_GEMINI_GET_BLOCK)
int FC3D_GEMINI_GET_BLOCK();


#define GEMINI_UNSET_BLOCK FCLAW_F77_FUNC(gemini_unset_block, \
											  GEMINI_UNSET_BLOCK)
void GEMINI_UNSET_BLOCK();




#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif


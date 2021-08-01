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

#ifndef GEMINI_USER_FORT_H
#define GEMINI_USER_FORT_H

#ifdef __cplusplus
extern "C"
{
#endif


/* --------------------------------------------------------------------------------
   Gemini routines

   These are provided for user convenience.  These files are not compiled
   into the library, but should be provided by the user.

   These signatures can be used if the user file matches these signatures 
   and subroutine name. Otherwise, the user should provide their own headers.   
   ------------------------------------------------------------------------------- */

#define SETPROB            FCLAW_F77_FUNC(setprob,           SETPROB)
void SETPROB();

/* ------------------------------- User defined routines ------------------------------ */

#define GEMINI_SETPROB FCLAW_F77_FUNC(clawpack_setprob,GEMINI_SETPROB)
void GEMINI_SETPROB();

#define GEMINI_QINIT   FCLAW_F77_FUNC(gemini_qinit,  GEMINI_QINIT)
void GEMINI_QINIT(const int* blockno, const int* meqn, const int* mbc,
                      const int* mx, const int* my, const int* mz,
                      const double* xlower, const double* ylower,
                      const double* zlower, 
                      const double* dx, const double* dy, const double *dz,
                      double q[], const int* maux, double aux[]);
  
#define GEMINI_SETAUX  FCLAW_F77_FUNC(gemini_setaux, GEMINI_SETAUX)
void GEMINI_SETAUX(const int* blockno, const int* mbc, const int* mx, 
                   const int* my, const int* mz, 
                   const double* xlower, const double* ylower, const double* zlower,
                   const double* dx, const double* dy, const double* dz,
                   const int* maux, double aux[]);


#define GEMINI_BC3     FCLAW_F77_FUNC(gemini_bc3,    GEMINI_bc3)
void GEMINI_BC3(const int* blockno, const int* meqn, const int* mbc, 
                const int* mx, const int* my, const int* mz,  
                const double* xlower, const double* ylower, const double* zlower,
                const double* dx, const double* dy, const double *dz,
                const double q[], const int* maux, const double aux[], 
                const double* t, const double* dt, const int mthbc[]);


#define GEMINI_B4STEP3 FCLAW_F77_FUNC(gemini_b4step3,GEMINI_B4STEP3)
void GEMINI_B4STEP3(const int* blockno, 
                    const int* mbc, const int* mx, const int* my, const int* mz,
                    const int* meqn,
                    double q[], const double* xlower, const double* ylower,
                    const double* zlower,
                    const double* dx, const double* dy, const double* dz,
                    const double* t, const double* dt,
                    const int* maux, double aux[]);

#define GEMINI_SRC3    FCLAW_F77_FUNC(gemini_src3,   GEMINI_SRC3)
void GEMINI_SRC3(const int* blockno, const int* meqn,
                 const int* mbc, const int* mx,const int* my, const int* mz,
                 const double* xlower, const double* ylower, const double* zlower,
                 const double* dx, const double* dy, const double* dz,
                 double q[],
                 const int* maux, double aux[], const double* t,
                 const double* dt);


#ifdef __cplusplus
}
#endif

#endif

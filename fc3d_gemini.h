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

#ifndef FC3D_GEMINI_H
#define FC3D_GEMINI_H

#include <fclaw_base.h>   /* Needed for FCLAW_F77_FUNC */

#ifdef __cplusplus
extern "C"
{
#endif

struct fclaw2d_global;
struct fclaw2d_patch;

typedef  struct fc3d_gemini_vtable  fc3d_gemini_vtable_t;


/* --------------------------- Gemini solver functions ------------------------------ */

/* Virtualize clawpack-specific wrapper functions */
typedef void (*gemini_src3_t)(struct fclaw2d_global* glob,
								  struct fclaw2d_patch *patch,
								  int blockno,
								  int patchno,
								  double t,
								  double dt);
	
typedef void (*gemini_b4step3_t)(struct fclaw2d_global* glob,
									 struct fclaw2d_patch *patch,
									 int blockno,
									 int patchno,
									 double t,
									 double dt);

/* ---------------------- Clawpack solver functions (Fortran) ------------------------- */

typedef void (*gemini_fort_setprob_t)(void);

typedef void (*gemini_fort_bc3_t)(const int* blockno, const int* meqn, const int* mbc,
                                  const int* mx, const int* my, const int* mz,
                                  const double* xlower, const double* ylower,
                                  const double* zlower, 
                                  const double* dx, const double* dy, 
                                  const double* dz,
                                  const double q[], const int* maux,
                                  const double aux[], const double* t,
                                  const double* dt, const int mthbc[]);

typedef  void (*gemini_fort_qinit_t)(const int* blockno, 
                                     const int* meqn,const int* mbc,
                                     const int* mx, const int* my, const int* mz,
                                     const double* xlower, const double* ylower,
                                     const double* zlower,
                                     const double* dx, const double* dy,
                                     const double *dz,
                                     double q[], const int* maux, double aux[]);

typedef void (*gemini_fort_setaux_t)(const int* blockno, const int* mbc,
                                     const int* mx, const int* my, const int *mz,
                                     const double* xlower, const double* ylower,
                                     const double *zlower,
                                     const double* dx, const double* dy,
                                     const double* dz,
                                     const int* maux, double aux[]);



typedef void (*gemini_fort_b4step3_t)(const int* blockno, const int* mbc,
                                      const int* mx, const int* my, const int* mz,
                                      const int* meqn,
                                      double q[], const double* xlower,
                                      const double* ylower, const double *zlower,
                                      const double* dx, const double* dy,
                                      const double* dz,
                                      const double* t, const double* dt,
                                      const int* maux, double aux[]);

typedef void (*gemini_fort_src3_t)(const int* blockno, const int* meqn,
                                   const int* mbc, const int* mx,const int* my,
                                   const int* mz, 
                                   const double* xlower, const double* ylower,
                                   const double* zlower, 
                                   const double* dx, const double* dy, const 
                                   double *dz, double q[],
                                   const int* maux, double aux[], const double* t,
                                   const double* dt);


/* --------------------------------- Virtual table ------------------------------------ */

struct fc3d_gemini_vtable
{

	/* C wrappers */
	gemini_b4step3_t   b4step3;
	gemini_src3_t      src3;

	/* Fortran routines */
	gemini_fort_setprob_t   fort_setprob;
	gemini_fort_bc3_t       fort_bc3;
	gemini_fort_qinit_t     fort_qinit;
	gemini_fort_setaux_t    fort_setaux;
	gemini_fort_b4step3_t   fort_b4step3;
	gemini_fort_src3_t      fort_src3;
	
	int is_set;

};

void fc3d_gemini_solver_initialize(void);

fc3d_gemini_vtable_t* fc3d_gemini_vt(void);


/* ----------------------------- User access to solver functions ---------------------- */

void fc3d_gemini_setprob(struct fclaw2d_global* glob);


void fc3d_gemini_setaux(struct fclaw2d_global* glob,
							struct fclaw2d_patch *patch,
							int blockno,
							int patchno);

void fc3d_gemini_set_capacity(struct fclaw2d_global* glob,
								  struct fclaw2d_patch *patch,
								  int blockno,
								  int patchno);

void fc3d_gemini_qinit(struct fclaw2d_global* glob,
						   struct fclaw2d_patch *patch,
						   int blockno,
						   int patchno);

void fc3d_gemini_b4step3(struct fclaw2d_global* glob,
							 struct fclaw2d_patch *patch,
							 int blockno,
							 int patchno,
							 double t,
							 double dt);

void fc3d_gemini_bc3(struct fclaw2d_global *glob,
						 struct fclaw2d_patch *patch,
						 int blockno,
						 int patchno,
						 double t,
						 double dt,
						 int intersects_bc[],
						 int time_interp);

void fc3d_gemini_src3(struct fclaw2d_global* glob,
						  struct fclaw2d_patch *patch,
						  int blockno,
						  int patchno,
						  double t,
						  double dt);


#ifdef __cplusplus
}
#endif


#endif /* !FC3D_CLAWPACH46_H */

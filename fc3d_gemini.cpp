/*
Copyright (c) 2012-2021 Donna Calhoun, Matt Zettergren, Carsten Burstedde
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

#include "fc3d_gemini.h"
#include "fc3d_gemini_options.h"
#include "fc3d_gemini_fort.h"

#include <fclaw2d_clawpatch.hpp>
#include <fclaw2d_clawpatch.h>

#include <fclaw2d_clawpatch_options.h>
#include <fclaw2d_clawpatch_output_ascii.h> 
#include <fclaw2d_clawpatch_output_vtk.h>
#include <fclaw2d_clawpatch_fort.h>

#include <fclaw2d_clawpatch_conservation.h>

#include <fclaw2d_patch.h>
#include <fclaw2d_global.h>
#include <fclaw2d_vtable.h>
#include <fclaw2d_options.h>
#include <fclaw2d_defs.h>


static fc3d_gemini_vtable_t s_gemini_vt;

/* --------------------- Clawpack solver functions (required) ------------------------- */

static
void gemini_setprob(fclaw2d_global_t *glob)
{
	fc3d_gemini_vtable_t*  claw46_vt = fc3d_gemini_vt();
	if (claw46_vt->fort_setprob != NULL)
	{
		claw46_vt->fort_setprob();
	}
}


static
void gemini_qinit(fclaw2d_global_t *glob,
					  fclaw2d_patch_t *patch,
					  int blockno,
					  int patchno)
{
	int mx,my,mz,mbc;
	double dx,dy,dz, xlower,ylower, zlower;
	fclaw2d_clawpatch_grid_data(glob,patch,&mx,&my,&mz,&mbc,
								&xlower,&ylower,&zlower, 
								&dx,&dy,&dz);

	int meqn;
	double *q;
	fclaw2d_clawpatch_soln_data(glob,patch,&q,&meqn);

	/* Auxillary data, such as metric terms */
	int maux;
	double *aux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();

	FCLAW_ASSERT(gemini_vt->fort_qinit != NULL); /* Must be initialized */

	/* Call to classic Clawpack 'qinit' routine.  This must be user defined */
	gemini_vt->fort_qinit(&blockno, &meqn,&mbc,&mx,&my,&mz, &xlower,&ylower,&zlower, 
	                      &dx,&dy,&dz, q, &maux,aux);
}


/* Physical boundary conditions */
static
void gemini_bc3(fclaw2d_global_t *glob,
					fclaw2d_patch_t *patch,
					int blockno,
					int patchno,
					double t,
					double dt,
					int intersects_phys_bdry[],
					int time_interp)
{


	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();
	FCLAW_ASSERT(gemini_vt->fort_bc3 != NULL);

	int mx,my,mz, mbc;
	double dx,dy,dz, xlower,ylower, zlower;
	fclaw2d_clawpatch_grid_data(glob,patch, &mx,&my,&mz,&mbc,
								&xlower,&ylower,&zlower, &dx,&dy,&dz);

	double *aux;
	int maux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	fc3d_gemini_options_t *gemopt = fc3d_gemini_get_options(glob);
	int *block_mthbc = gemopt->mthbc;

	/* Set a local copy of mthbc that can be used for a patch. */
	int mthbc[6];
	for(int i = 0; i < 6; i++)
	{
		if (i < 4)			
			if (intersects_phys_bdry[i])
				mthbc[i] = block_mthbc[i];
			else
				mthbc[i] = -1;
		else
			mthbc[i] = block_mthbc[i];
	}

	/*
	  We may be imposing boundary conditions on time-interpolated data;
	  and is being done just to help with fine grid interpolation.
	  In this case, this boundary condition won't be used to update
	  anything
	*/
	double *q;
	int meqn;
	fclaw2d_clawpatch_timesync_data(glob,patch,time_interp,&q,&meqn);

	gemini_vt->fort_bc3(&blockno, &meqn,&mbc,&mx,&my,&mz,&xlower,&ylower,&zlower,
						&dx,&dy,&dz, q,&maux,aux,&t,&dt,mthbc);
}



/* Tasks that need to happen between taking time steps */
static
void gemini_b4step3(fclaw2d_global_t *glob,
						fclaw2d_patch_t *patch,
						int blockno,
						int patchno,
						double t, double dt)

{
	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();
	if (gemini_vt->fort_b4step3 == NULL)
		return;

	int mx,my,mz,mbc;
	double xlower,ylower,zlower,dx,dy,dz;
	fclaw2d_clawpatch_grid_data(glob,patch, &mx,&my,&mz, &mbc,
								&xlower,&ylower,&zlower,&dx,&dy,&dz);

	int meqn;
	double *q;
	fclaw2d_clawpatch_soln_data(glob,patch,&q,&meqn);

	int maux;
	double *aux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	gemini_vt->fort_b4step3(&blockno,&mbc,&mx,&my,&mz, &meqn,q, 
	                        &xlower,&ylower,&zlower,
							&dx,&dy,&dz, &t,&dt,&maux,aux);
}

static
void gemini_src3(fclaw2d_global_t *glob,
					 fclaw2d_patch_t *patch,
					 int blockno,
					 int patchno,
					 double t,
					 double dt)
{
	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();

	if (gemini_vt->fort_src3 == NULL)
		return;


	int mx,my,mz, mbc;
	double xlower,ylower,zlower,dx,dy,dz;
	fclaw2d_clawpatch_grid_data(glob,patch, &mx,&my,&mz, &mbc,
								&xlower,&ylower,&zlower, &dx,&dy, &dz);

	double *q;
	int meqn;
	fclaw2d_clawpatch_soln_data(glob,patch,&q,&meqn);

	double *aux;
	int maux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	gemini_vt->fort_src3(&blockno, &meqn,&mbc,&mx,&my,&mz, &xlower,&ylower,&zlower,
						 &dx,&dy,&dz, q,&maux,aux,&t,&dt);
}


/* This can be used as a value for patch_vt->patch_setup */
static
void gemini_setaux(fclaw2d_global_t *glob,
					   fclaw2d_patch_t *patch,
					   int blockno,
					   int patchno)
	{
	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();

	if (gemini_vt->fort_setaux == NULL)
		return;

	if (fclaw2d_patch_is_ghost(patch))
	{
		/* This is going to be removed at some point */
		return;
	}

	int mx,my,mz,mbc;
	double xlower,ylower,zlower, dx,dy, dz;
	fclaw2d_clawpatch_grid_data(glob,patch, &mx,&my,&mz,&mbc,
								&xlower,&ylower,&zlower, &dx,&dy, &dz);
	int maux;
	double *aux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	gemini_vt->fort_setaux(&blockno,&mbc,&mx,&my,&mz, &xlower,&ylower,&zlower, 
	                       &dx,&dy,&dz, &maux,aux);
}

/* This is called from the single_step callback. and is of type 'flaw_single_step_t' */
static
double gemini_step3(fclaw2d_global_t *glob,
						fclaw2d_patch_t *patch,
						int blockno,
						int patchno,
						double t,
						double dt)
{
	// const fclaw_options_t* fclaw_opt = fclaw2d_get_options(glob);

	fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();

	const fc3d_gemini_options_t* gemopt = 
	                fc3d_gemini_get_options(glob);

	int level = patch->level;

	int maux;
	double *aux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

	fclaw2d_clawpatch_save_current_step(glob, patch);

	int mx, my, mz, mbc;
	double xlower, ylower, zlower, dx,dy, dz;
	fclaw2d_clawpatch_grid_data(glob,patch,&mx,&my,&mz,&mbc,
								&xlower,&ylower,&zlower, &dx,&dy,&dz);

	int meqn;
	double *qold;
	fclaw2d_clawpatch_soln_data(glob,patch,&qold,&meqn);

	/* Take a step on a single patch; return a cfl number (or not :-)) */
	double cflgrid = 0.9;   

	return cflgrid;
}

static
double gemini_update(fclaw2d_global_t *glob,
                     fclaw2d_patch_t *patch,
                     int blockno,
                     int patchno,
                     double t,
                     double dt, 
                     void* user)
{

    fc3d_gemini_vtable_t*  gemini_vt = fc3d_gemini_vt();
    if (gemini_vt->b4step3 != NULL)
    {
        fclaw2d_timer_start_threadsafe(&glob->timers[FCLAW2D_TIMER_ADVANCE_B4STEP2]);               
        gemini_vt->b4step3(glob,
                           patch,
                           blockno,
                           patchno,t,dt);

        fclaw2d_timer_stop_threadsafe(&glob->timers[FCLAW2D_TIMER_ADVANCE_B4STEP2]);               
    }

    fclaw2d_timer_start_threadsafe(&glob->timers[FCLAW2D_TIMER_ADVANCE_STEP2]);       

    double maxcfl = gemini_step3(glob,
                                     patch,
                                     blockno,
                                     patchno,t,dt);

    fclaw2d_timer_stop_threadsafe(&glob->timers[FCLAW2D_TIMER_ADVANCE_STEP2]);       

#if 0
    const fc3d_gemini_options_t* gemopt = fc3d_gemini_get_options(glob);
    if (gemopt->src_term > 0 && gemini_vt->src3 != NULL)
    {
        gemini_vt->src3(glob,
                        patch,
                        blockno,
                        patchno,t,dt);
    }
#endif    
    return maxcfl;
}


/* ---------------------------------- Output functions -------------------------------- */

static
void gemini_output(fclaw2d_global_t *glob, int iframe)
{
	const fc3d_gemini_options_t* gemopt 
	                  = fc3d_gemini_get_options(glob);

	if (gemopt->vtk_out != 0)
		fclaw2d_clawpatch_output_vtk(glob,iframe);

	if (gemopt->hdf5_out != 0)
		fclaw_global_essentialf("HDF5 : Not yet implemented\n");
		// fc3d_gemini_output_hdf5(glob,iframe);

}



/* ---------------------------------- Virtual table  ---------------------------------- */

static
fc3d_gemini_vtable_t* gemini_vt_init()
{
	FCLAW_ASSERT(s_gemini_vt.is_set == 0);
	return &s_gemini_vt;
}

void fc3d_gemini_solver_initialize()
{
	/* Initialize this virtual table */
	fc3d_gemini_vtable_t*  gemini_vt = gemini_vt_init();

	/* Decide on data layout */
	int claw_version = 4;
	fclaw2d_clawpatch_vtable_initialize(claw_version);

    //fclaw2d_clawpatch_vtable_t*      clawpatch_vt = fclaw2d_clawpatch_vt();


	/* ForestClaw core vtable items */
	fclaw2d_vtable_t*  fclaw_vt = fclaw2d_vt();
	fclaw_vt->output_frame      = gemini_output;
	fclaw_vt->problem_setup     = gemini_setprob;    

	/* These could be over-written by user specific settings */
	fclaw2d_patch_vtable_t* patch_vt = fclaw2d_patch_vt();  
	patch_vt->initialize          = gemini_qinit;
	patch_vt->setup               = gemini_setaux;  
	patch_vt->physical_bc         = gemini_bc3;
	patch_vt->single_step_update  = gemini_update;

	/* Wrappers so that user can change argument list */
	gemini_vt->b4step3  = gemini_b4step3;
	gemini_vt->src3     = gemini_src3;

	/* Required functions  - error if NULL */
	gemini_vt->fort_bc3       = GEMINI_BC3_DEFAULT;
	gemini_vt->fort_qinit     = NULL;

	/* Optional functions - call only if non-NULL */
	gemini_vt->fort_setprob   = NULL;
	gemini_vt->fort_setaux    = NULL;
	gemini_vt->fort_b4step3   = NULL;
	gemini_vt->fort_src3      = NULL;

	gemini_vt->is_set = 1;
}


/* ----------------------------- User access to solver functions --------------------------- */


/* These are here in case the user wants to call Clawpack routines directly */

fc3d_gemini_vtable_t* fc3d_gemini_vt()
{
	FCLAW_ASSERT(s_gemini_vt.is_set != 0);
	return &s_gemini_vt;
}

/* This should only be called when a new fclaw2d_clawpatch_t is created. */
void fc3d_gemini_set_capacity(fclaw2d_global_t *glob,
                              fclaw2d_patch_t *patch,
                              int blockno,
                              int patchno)
{
	fc3d_gemini_options_t *gemopt = fc3d_gemini_get_options(glob);

	int mx,my,mz, mbc;
	double dx,dy,dz, xlower,ylower,zlower;
	fclaw2d_clawpatch_grid_data(glob,patch, &mx,&my,&mz,&mbc,
								&xlower,&ylower,&zlower,&dx,&dy,&dz);

	double *area = fclaw2d_clawpatch_get_area(glob,patch);

	int maux;
	double *aux;
	fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

#if 0
	GEMINI_SET_CAPACITY(&mx,&my,&mz,&mbc,&dx,&dy,&dz,area,&mcapa,
							&maux,aux);
#endif							
}



/* -------------------------- Public interface to Clawpack wrappers --------------------*/

/* Some wrapper functions provided as convenience to users */

void fc3d_gemini_setprob(fclaw2d_global_t *glob)
{
	gemini_setprob(glob);
}

/* This can be set to claw46_vt->src3 */
void fc3d_gemini_src3(fclaw2d_global_t* glob,
						  fclaw2d_patch_t *patch,
						  int blockno,
						  int patchno,
						  double t,
						  double dt)
{
	gemini_src3(glob,patch,blockno,blockno,t,dt);
}


void fc3d_gemini_setaux(fclaw2d_global_t *glob,
							fclaw2d_patch_t *patch,
							int blockno,
							int patchno)
{
	gemini_setaux(glob,patch,blockno,patchno);
}


void fc3d_gemini_qinit(fclaw2d_global_t *glob,
						   fclaw2d_patch_t *patch,
						   int blockno,
						   int patchno)
{
	gemini_qinit(glob,patch,blockno,patchno);
}

void fc3d_gemini_b4step3(fclaw2d_global_t* glob,
							 fclaw2d_patch_t *patch,
							 int blockno,
							 int patchno,
							 double t,
							 double dt)
{
	gemini_b4step3(glob,patch,blockno,patchno,t,dt);
}

void fc3d_gemini_bc3(fclaw2d_global_t *glob,
						 fclaw2d_patch_t *patch,
						 int blockno,
						 int patchno,
						 double t,
						 double dt,
						 int intersects_bc[],
						 int time_interp)
{
	gemini_bc3(glob,patch,blockno,blockno,t,dt,
				   intersects_bc,time_interp);
}















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

#include "fc3d_gemini_options.h"

#include <fclaw2d_clawpatch_options.h>

#include <fclaw2d_global.h>
#include <fclaw_options.h>
#include <fclaw_package.h>

static int s_gemini_options_package_id = -1;

static void*
gemini_register (fc3d_gemini_options_t* gemopt, sc_options_t * opt)
{    

    sc_options_add_int (opt, 0, "flag", &gemopt->flag, 0,
                        "[Gemini] Sample flag [0]");


    fclaw_options_add_int_array (opt, 0, "array", &gemopt->array_string, "1 2 3",
                               &gemopt->array, 3, "[Gemini] Example array [1 2 3]");


    /* Array of NumFaces=4 values */
    fclaw_options_add_int_array (opt, 0, "mthbc", &gemopt->mthbc_string, "1 1 1 1 1 1",
                                 &gemopt->mthbc, 6,
                                 "[gemini] Physical boundary condition type [1 1 1 1 1 1]");
    
    sc_options_add_bool (opt, 0, "hdf5-out", &gemopt->hdf5_out, 0,
                           "Output in HDF5 format [F]");

    sc_options_add_bool (opt, 0, "vtk-out", &gemopt->vtk_out, 0,
                           "Output VTK formatted data [F]");


    gemopt->is_registered = 1;
    return NULL;
}

static fclaw_exit_type_t
gemini_postprocess (fc3d_gemini_options_t * gemopt)
{
    fclaw_options_convert_int_array (gemopt->array_string, &gemopt->array,3);
    
    return FCLAW_NOEXIT;
}


static fclaw_exit_type_t
gemini_check(fc3d_gemini_options_t *gemopt,
                 fclaw2d_clawpatch_options_t *clawpatch_opt)
{

    if (0)        
    {
        /* Demonstration of how to return an error */
        fclaw_global_essentialf("gemini : Error\n");
        return FCLAW_EXIT_ERROR;
    }

    return FCLAW_NOEXIT;
}

static
void gemini_destroy (fc3d_gemini_options_t * gemopt)
{
    fclaw_options_destroy_array (gemopt->array);
}

/* ------------------------------------------------------
   Generic calls to options handling;  each calls 
   gemini-specific options call back
   ------------------------------------------------------ */

static void*
options_register (fclaw_app_t * app, void *package, sc_options_t * opt)
{
    fc3d_gemini_options_t *gemopt;

    FCLAW_ASSERT (app != NULL);
    FCLAW_ASSERT (package != NULL);

    gemopt = (fc3d_gemini_options_t*) package;

    return gemini_register(gemopt,opt);
}


static fclaw_exit_type_t
options_postprocess (fclaw_app_t * app, void *package, void *registered)
{
    fc3d_gemini_options_t *gemopt;

    FCLAW_ASSERT (app != NULL);
    FCLAW_ASSERT (package != NULL);
    FCLAW_ASSERT (registered == NULL);

    gemopt = (fc3d_gemini_options_t*) package;
    FCLAW_ASSERT (gemopt->is_registered);

    return gemini_postprocess (gemopt);
}


static fclaw_exit_type_t
options_check (fclaw_app_t * app, void *package, void *registered)
{
    fc3d_gemini_options_t *gemopt;
    fclaw2d_clawpatch_options_t *clawpatch_opt;

    FCLAW_ASSERT (app != NULL);
    FCLAW_ASSERT (package != NULL);
    FCLAW_ASSERT (registered == NULL);

    gemopt = (fc3d_gemini_options_t*) package;
    FCLAW_ASSERT (gemopt->is_registered);

    clawpatch_opt = (fclaw2d_clawpatch_options_t *)
        fclaw_app_get_attribute(app,"clawpatch",NULL);
    FCLAW_ASSERT(clawpatch_opt->is_registered);

    return gemini_check(gemopt,clawpatch_opt);    
}

static void
options_destroy (fclaw_app_t * app, void *package, void *registered)
{
    fc3d_gemini_options_t *gemopt;

    FCLAW_ASSERT (app != NULL);
    FCLAW_ASSERT (package != NULL);
    FCLAW_ASSERT (registered == NULL);

    gemopt = (fc3d_gemini_options_t*) package;
    FCLAW_ASSERT (gemopt->is_registered);

    gemini_destroy (gemopt);

    FCLAW_FREE (gemopt);
}

static const fclaw_app_options_vtable_t gemini_options_vtable = {
    options_register,
    options_postprocess,
    options_check,
    options_destroy,
};

/* ----------------------------------------------------------
   Public interface to gemini options
   ---------------------------------------------------------- */
fc3d_gemini_options_t*  fc3d_gemini_options_register (fclaw_app_t * app,
                                                              const char *configfile)
{
    fc3d_gemini_options_t *gemopt;

    FCLAW_ASSERT (app != NULL);

    gemopt = FCLAW_ALLOC (fc3d_gemini_options_t, 1);
    fclaw_app_options_register (app, "gemini", configfile,
                                &gemini_options_vtable, gemopt);
    
    fclaw_app_set_attribute(app,"claw3",gemopt);
    return gemopt;
}

fc3d_gemini_options_t* fc3d_gemini_get_options(fclaw2d_global_t *glob)
{
    int id = s_gemini_options_package_id;
    return (fc3d_gemini_options_t*) fclaw_package_get_options(glob,id);
}

void fc3d_gemini_options_store (fclaw2d_global_t* glob, fc3d_gemini_options_t* gemopt)
{
    int id = fclaw_package_container_add_pkg(glob,gemopt);
    s_gemini_options_package_id = id;
}

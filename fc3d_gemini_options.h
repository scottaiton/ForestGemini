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

#ifndef FC3D_GEMINI_OPTIONS_H
#define FC3D_GEMINI_OPTIONS_H

#include <fclaw_base.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct fclaw2d_global;
typedef struct fc3d_gemini_options fc3d_gemini_options_t;

struct fc3d_gemini_options
{
    int flag;

    const char *array_string;
    int *array;

    const char *mthbc_string;
    int *mthbc;

    /* Output */
    int hdf5_out;
    int vtk_out;

    int is_registered;
};

fclaw_exit_type_t fc3d_gemini_postprocess (fc3d_gemini_options_t *
                                               geminiopt);

fclaw_exit_type_t fc3d_gemini_check (fc3d_gemini_options_t * geminiopt);

void fc3d_gemini_reset (fc3d_gemini_options_t * geminiopt);

fc3d_gemini_options_t*  fc3d_gemini_options_register (fclaw_app_t * app,
                                                              const char *configfile);

void fc3d_gemini_package_register(fclaw_app_t* app,
                                      fc3d_gemini_options_t* geminiopt);

fc3d_gemini_options_t* fc3d_gemini_get_options(struct fclaw2d_global *glob);

void fc3d_gemini_options_store (struct fclaw2d_global* glob, 
                                    fc3d_gemini_options_t* geminiopt);


#ifdef __cplusplus
}
#endif

#endif

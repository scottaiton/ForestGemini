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

#ifndef SIMPLE_USER_H
#define SIMPLE_USER_H

#include <fclaw2d_include_all.h>


#include <fc3d_gemini.h>
#include <fc3d_gemini_options.h>
#include <fc3d_gemini_fort.h>
#include <fc3d_gemini_fort_user.h>

/* Headers for both Clawpack 4.6 and  Clawpack 5.0 */
#include <fclaw3dx_clawpatch.h>
#include <fclaw3dx_clawpatch_options.h>
#include <fclaw3dx_clawpatch_fort.h>
#include <fclaw3dx_clawpatch46_fort.h>
// #include <fclaw2d_clawpatch_pillow.h>


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct user_options
{
    int example;

    int claw_version;
    int is_registered;

    const char* sim_dir;

} user_options_t;


void simple_link_solvers(fclaw2d_global_t *glob);

/* ------------------------------------- Options ---------------------------------------*/

user_options_t* simple_options_register (fclaw_app_t * app,
                                        const char *configfile);

void simple_options_store (fclaw2d_global_t* glob, user_options_t* user);

const user_options_t* simple_get_options(fclaw2d_global_t* glob);

#ifdef __cplusplus
}
#endif

#endif

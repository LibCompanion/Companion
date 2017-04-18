/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMPANION_EXPORT_API_DEFINITIONS_H
#define COMPANION_EXPORT_API_DEFINITIONS_H

/**
 * Calling convention types.
 */
#if defined WIN32 || defined _WIN32
    #define COMP_CDECL __cdecl
    #define COMP_STDCALL __stdcall
#else
    #define COMP_CDECL
    #define COMP_STDCALL
#endif

/**
 * Enables function exporting when building a shared library.
 */
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
    #ifdef Companion_EXPORTS
        #define COMP_EXPORTS __declspec(dllexport)
    #elif Companion_Include
        #define COMP_EXPORTS __declspec(dllimport) // You have to manually define "Companion_Include" for DLL imports
    #endif
#elif defined __GNUC__ && __GNUC__ >= 4
    #define COMP_EXPORTS __attribute__ ((visibility ("default")))
#else
    #define COMP_EXPORTS
#endif

/**
 * Disables name mangling for exported functions.
 */
#ifndef COMP_EXTERN_C
    #ifdef __cplusplus
        #define COMP_EXTERN_C extern "C"
    #else
        #define COMP_EXTERN_C
    #endif
#endif

/**
 * Final export API declaration to export functions when building a shared library.
 */
#ifndef COMPAPI
    #define COMPAPI(rettype) COMP_EXTERN_C COMP_EXPORTS rettype COMP_CDECL
#endif

#endif //COMPANION_EXPORT_API_DEFINITIONS_H
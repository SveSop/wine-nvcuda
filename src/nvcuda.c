/*
 * Copyright (C) 2025 Sveinar Søpler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>

#define __WINESRC__

#include "winternl.h"
#include "winbase.h"
#include "wine/unixlib.h"
#include "wine/debug.h"

#include "cuda.h"
#include "unixlib.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvcuda);

#define NVCUDA_CALL(func, params) WINE_UNIX_CALL(unix_ ## func, params)

#if defined(_WIN32) && !defined(_WIN64)
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include "libloaderapi.h"
#endif

CUresult CUDAAPI cuInit(unsigned int Flags)
{
    struct cuInit_params params = { Flags, CUDA_SUCCESS };

    TRACE("(%d)\n", Flags);

    return NVCUDA_CALL(cuInit, &params);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    NTSTATUS status;

    TRACE("(%p, %lu, %p)\n", instance, (unsigned long)reason, reserved);

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(instance);
            if ((status = __wine_init_unix_call()))
            {
                ERR("__wine_init_unix_call failed, status %lx\n", (unsigned long)status);
                return FALSE;
            }
            if ((status = NVCUDA_CALL(attach, NULL)))
            {
                ERR("WINE_UNIX_CALL(attach) failed, status %lx\n", (unsigned long)status);
                return FALSE;
            }
            break;
        case DLL_PROCESS_DETACH:
            if (reserved)
                break;
            NVCUDA_CALL(detach, NULL);
            break;
    }

    return TRUE;
}

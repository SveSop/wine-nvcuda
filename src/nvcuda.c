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

#define __WINESRC__

#include "winternl.h"
#include "winbase.h"
#include "wine/unixlib.h"
#include "wine/debug.h"

#include "cuda_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(nvcuda);

#define NVCUDA_CALL(func, params) WINE_UNIX_CALL(unix_ ## func, params)

#if defined(_WIN32) && !defined(_WIN64)
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include "libloaderapi.h"

CUresult CUDAAPI cuInit(unsigned int Flags)
{
    CUresult ret;
    TRACE("(%d)\n", Flags);

    return NVCUDA_CALL(cuInit, &ret)
        ? CUDA_ERROR_UNKNOWN
        : ret;
}

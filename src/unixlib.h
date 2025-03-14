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

#ifndef __WINE_NVCUDA_UNIXLIB_H
#define __WINE_NVCUDA_UNIXLIB_H

extern NTSTATUS attach(void *args);
extern NTSTATUS detach(void *args);
extern NTSTATUS wine_cuInit(void *args);
extern NTSTATUS wine_cuDeviceGet(void *args);
extern NTSTATUS wine_cuCtxCreate(void *args);
extern NTSTATUS wine_cuCtxDestroy(void *args);

struct cuInit_params
{
    unsigned int Flags;
    CUresult ret;
};

struct cuDeviceGet_params
{
    CUdevice *device;
    int ordinal;
    CUresult ret;
};

struct cuCtxCreate_params
{
    CUcontext *pctx;
    unsigned int flags;
    CUdevice dev;
    CUresult ret;
};

struct cuCtxDestroy_params
{
    CUcontext ctx;
    CUresult ret;
};

enum nvcuda_funcs
{
    unix_attach,
    unix_detach,
    unix_cuInit,
    unix_cuDeviceGet,
    unix_cuCtxCreate,
    unix_cuCtxDestroy,
};

#endif

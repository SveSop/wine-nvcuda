#if 0
#pragma makedep unix
#endif

#define WINE_UNIX_LIB

#include <stdarg.h>
#include <dlfcn.h>

#include "winternl.h"
#include "winbase.h"
#include "ntstatus.h"
#include "wine/unixlib.h"

#include "cuda.h"
#include "unixlib.h"

#ifdef _WIN64

typedef ULONG PTR32;

static NTSTATUS wow64_cuInit(void *args)
{
    struct
    {
        unsigned int Flags;
        CUresult ret;
    } *params32 = args;
    struct cuInit_params params =
    {
        .Flags = params32->Flags,
    };

    NTSTATUS status = wine_cuInit(&params);
    params32->ret = params.ret;

    return status;
}

static NTSTATUS wow64_cuDeviceGet(void *args)
{
    struct
    {
        PTR32 device;
        int ordinal;
        CUresult ret;
    } *params32 = args;
    struct cuDeviceGet_params params =
    {
       .device = ULongToPtr(params32->device),
       .ordinal = params32->ordinal,
    };

    NTSTATUS status = wine_cuDeviceGet(&params);
    params32->ret = params.ret;

    return status;
}

static NTSTATUS wow64_cuCtxCreate(void *args)
{
    struct
    {
        PTR32 pctx;
        unsigned int flags;
        CUdevice dev;
        CUresult ret;
    } *params32 = args;
    struct cuCtxCreate_params params =
    {
        .pctx = ULongToPtr(params32->pctx),
        .flags = params32->flags,
        .dev = params32->dev,
    };

    NTSTATUS status = wine_cuCtxCreate(&params);
    params32->ret = params.ret;

    return status;
}

static NTSTATUS wow64_cuCtxDestroy(void *args)
{
    struct
    {
        PTR32 ctx;
        CUresult ret;
    } *params32 = args;
    struct cuCtxDestroy_params params =
    {
        .ctx = UlongToPtr(params32->ctx),
    };

    NTSTATUS status = wine_cuCtxDestroy(&params);
    params32->ret = params.ret;

    return status;
}

const unixlib_entry_t __wine_unix_call_wow64_funcs[] =
{
    attach,
    detach,
    wow64_cuInit,
    wow64_cuDeviceGet,
    wow64_cuCtxCreate,
    wow64_cuCtxDestroy,
};

#endif // _WIN64

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

void *libcuda_handle = NULL;

static CUresult CUDAAPI (*pcuInit)(unsigned int Flags) = NULL;
static CUresult CUDAAPI (*pcuCtxCreate)(CUcontext *pctx, unsigned int flags, CUdevice dev) = NULL;
static CUresult CUDAAPI (*pcuCtxDestroy)(CUcontext ctx) = NULL;

NTSTATUS wine_cuInit(void *args)
{
    struct cuInit_params *params = args;
    params->ret = (pcuInit
        ? pcuInit(params->Flags)
        : CUDA_ERROR_NOT_FOUND);
    return STATUS_SUCCESS;
}

NTSTATUS wine_cuCtxCreate(void *args)
{
    struct cuCtxCreate_params *params = args;
    params->ret = (pcuCtxCreate
        ? pcuCtxCreate(params->pctx, params->flags, params->dev)
        : CUDA_ERROR_NOT_FOUND);
    return STATUS_SUCCESS;
}

NTSTATUS wine_cuCtxDestroy(void *args)
{
    struct cuCtxDestroy_params *params = args;
    params->ret = (pcuCtxDestroy
        ? pcuCtxDestroy(params->ctx)
        : CUDA_ERROR_NOT_FOUND);
    return STATUS_SUCCESS;
}

NTSTATUS attach(void *args)
{
    if (!(libcuda_handle = dlopen("libcuda.so.1", RTLD_NOW))) return STATUS_DLL_NOT_FOUND;

#define TRY_LOAD_FUNCPTR(f) *(void **)(&p##f) = dlsym(libcuda_handle, #f)

    TRY_LOAD_FUNCPTR(cuInit);
    TRY_LOAD_FUNCPTR(cuCtxCreate);
    TRY_LOAD_FUNCPTR(cuCtxDestroy);

#undef TRY_LOAD_FUNCPTR

    return STATUS_SUCCESS;
}

NTSTATUS detach(void *args)
{
    dlclose(libcuda_handle);
    return STATUS_SUCCESS;
}

const unixlib_entry_t __wine_unix_call_funcs[] =
{
    attach,
    detach,
    wine_cuInit,
    wine_cuCtxCreate,
    wine_cuCtxDestroy,
};

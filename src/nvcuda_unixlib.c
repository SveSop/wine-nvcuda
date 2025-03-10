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

#include "cuda_private.h"

static void *libcuda_handle = NULL;

static CUresult CUDAAPI (*pcuInit)(unsigned int Flags) = NULL;

static NTSTATUS wine_cuInit(void *args)
{
    CUresult *ret = args;
    *ret = (pcuInit
        ? pcuInit(Flags)
        : CUDA_ERROR_NOT_FOUND);
    return CUDA_SUCCESS;
}

static NTSTATUS attach(void *args)
{
    if (!(libcuda_handle = dlopen("libcuda.so.1", RTLD_NOW))) return STATUS_DLL_NOT_FOUND;

#define TRY_LOAD_FUNCPTR(f) *(void **)(&p##f) = dlsym(libcuda_handle, #f)

    TRY_LOAD_FUNCPTR(cuInit);

#undef TRY_LOAD_FUNCPTR

    return STATUS_SUCCESS;
}

static NTSTATUS detach(void *args)
{
    dlclose(libcuda_handle);
    return STATUS_SUCCESS;
}

const unixlib_entry_t __wine_unix_call_funcs[] =
{
    attach,
    detach,
    wine_cuInit,
};


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

static NTSTATUS wow64_cuInit(void *args)
{
    struct
    {
        unsigned int Flags;
        CUresult ret;
    } *params32 = args;
    struct cuInit_params params =
    {
        params32->Flags,
        params32->ret
    };

    params.ret = wine_cuInit(&params);
    params32->ret = params.ret;

    return STATUS_SUCCESS;
}

const unixlib_entry_t __wine_unix_call_wow64_funcs[] =
{
    attach,
    detach,
    wow64_cuInit,
};

#endif // _WIN64

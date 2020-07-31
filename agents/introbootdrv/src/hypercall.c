#include "hypercall.h"

// Define the segment that will host our hypercall machine code.
#pragma section("hcall", read,execute,nopage)

#pragma warning(disable:4055)   //'type cast' : from data pointer to function pointer

#ifdef AMD64

__declspec(allocate("hcall"))
BYTE gHypercallCode[45] =
{
    0x33, 0xc0, 0x48, 0xff, 0xC8, 0x90, 0x51, 0x52, 0x53, 0x56, 0x57, 0x48, 0x89, 0xc8, 0x48, 0x89,
    0xd1, 0x4c, 0x89, 0xc2, 0x4c, 0x89, 0xcb, 0x48, 0x8b, 0x74, 0x24, 0x50, 0x48, 0x8b, 0x7c, 0x24,
    0x58, 0x0f, 0x01, 0xc1, 0x48, 0x89, 0xd0, 0x5f, 0x5e, 0x5b, 0x5a, 0x59, 0xc3,
};

#else

__declspec(allocate("hcall"))
BYTE gHypercallCode[46] =
{
    0x33, 0xc0, 0xff, 0xC8, 0x90, 0x90, 0x55, 0x89, 0xe5, 0x51, 0x52, 0x53, 0x56, 0x57, 0x8b, 0x45,
    0x08, 0x8b, 0x4d, 0x0c, 0x8b, 0x55, 0x10, 0x8b, 0x5d, 0x14, 0x8b, 0x75, 0x18, 0x8b, 0x7d, 0x1c,
    0x0f, 0x01, 0xc1, 0x89, 0xd0, 0x5f, 0x5e, 0x5b, 0x5a, 0x59, 0x5d, 0xc2, 0x18, 0x00,
};

#endif

typedef SIZE_T
(*PFUNC_Hypercall)(
    __in SIZE_T Eax,
    __in SIZE_T Ecx,
    __in SIZE_T Edx,
    __in SIZE_T Ebx,
    __in SIZE_T Esi,
    __in SIZE_T Edi
    );


//
// Hypercall
//
SIZE_T
Hypercall(
    __in DWORD MaxOutputSize,
    __in PBYTE Buffer,
    __in DWORD Number
    )
{
    SIZE_T res;

    PFUNC_Hypercall hypercall = (PFUNC_Hypercall)&gHypercallCode;

#ifdef AMD64
    res = hypercall(34, (SIZE_T)Buffer, Number, MaxOutputSize, 0, 24);
#else
    res = hypercall(34, 0, Number, 24, (SIZE_T)Buffer, MaxOutputSize);
#endif

    return res;
}
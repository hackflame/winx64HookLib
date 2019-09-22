#pragma once
#include <Windows.h>
NTSTATUS MyAllocateVirtualMemory(
 HANDLE hProcess,
_Inout_ PVOID * BaseAddress,
_In_     SIZE_T RegionSize,
_In_ ULONG AllocationType,
_In_ ULONG Protect
);

EXTERN_C PVOID MyAllocateVirtual(
	_Inout_ PVOID BaseAddress,
	_In_     SIZE_T RegionSize,
	_In_ ULONG AllocationType,
	_In_ ULONG Protect
);

#include "stdafx.h"
#include "tools.h"
#include "asm.h"

typedef NTSTATUS(__fastcall *ZwAllocateVirtualMemoryProc)(

	HANDLE hProcess,
	_Inout_ PVOID * BaseAddress,
	_In_ ULONG_PTR ZeroBits,
	_Inout_ PSIZE_T RegionSize,
	_In_ ULONG AllocationType,
	_In_ ULONG Protect
	);




ULONG GetFuncTemplate(char * moduleName, char * funcName)
{
	HMODULE moudule = GetModuleHandleA(moduleName);
	if (moudule == NULL) return -1;

	ULONG64 func = (ULONG64)GetProcAddress(moudule, funcName);
	if (func == 0) return -2;
	ULONG number = *(PULONG)(func + 4);

	return number;
}

NTSTATUS MyAllocateVirtualMemory(
	HANDLE hProcess,
	_Inout_ PVOID * BaseAddress,
	_In_ SIZE_T RegionSize,
	_In_ ULONG AllocationType,
	_In_ ULONG Protect
)
{
	ULONG number = GetFuncTemplate("ntdll.dll", "ZwAllocateVirtualMemory");
	ULONG64 sysCallAddr = (ULONG64)__syscall;
	_setNumber(number);
	SIZE_T size = RegionSize;
	ZwAllocateVirtualMemoryProc  syscallFunc = (ZwAllocateVirtualMemoryProc)sysCallAddr;
	NTSTATUS status = syscallFunc(hProcess, BaseAddress, 0, &size, AllocationType, Protect);
	return status;
}

EXTERN_C PVOID MyAllocateVirtual(_Inout_ PVOID BaseAddress, _In_ SIZE_T RegionSize, _In_ ULONG AllocationType, _In_ ULONG Protect)
{
	PVOID Base = BaseAddress;
	NTSTATUS status = MyAllocateVirtualMemory(GetCurrentProcess(), &Base, RegionSize, AllocationType, Protect);
	if (status == 0)
	{
		return Base;
	}

	return NULL;
}
#include "stdafx.h"
#include "tools.h"
#include "asm.h"



typedef NTSTATUS(*CallProc)(...);

template<typename ...Args>
NTSTATUS callSysCall(DWORD SerivceNumber,Args ...args)
{
	ULONG64 sysCallAddr = (ULONG64)__syscall;
	CallProc  syscallFunc = (CallProc)sysCallAddr;
	_setNumber(SerivceNumber);
	return syscallFunc(args...);
}



ULONG GetFuncNumber(char * moduleName, char * funcName)
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
	ULONG number = GetFuncNumber("ntdll.dll", "ZwAllocateVirtualMemory");
	if (number <= 0)  return number;

	SIZE_T size = RegionSize;
	NTSTATUS status = callSysCall(number,hProcess, BaseAddress, 0, &size, AllocationType, Protect);
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


BOOL WINAPI MySetWindowDisplayAffinity(_In_ HWND hWnd, _In_ DWORD dwAffinity)
{
	ULONG number = GetFuncNumber("user32.dll", "SetWindowDisplayAffinity");
	if (number <= 0)  return FALSE;
	return callSysCall(number, hWnd, dwAffinity);
}

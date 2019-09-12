#pragma once
#include <Windows.h>
#include "asm.h"

typedef enum _HookType
{
	HOOK_E9 = 0,
	HOOK_E8,
	HOOK_FFCALL,

	
}HookType;


typedef struct _RegisterContext 
{
	ULONG64 rax;
	ULONG64 rcx;
	ULONG64 rdx;
	ULONG64 rbx;
	ULONG64 rbp;
	ULONG64 rsp;
	ULONG64 rsi;
	ULONG64 rdi;
	ULONG64 r8;
	ULONG64 r9;
	ULONG64 r10;
	ULONG64 r11;
	ULONG64 r12;
	ULONG64 r13;
	ULONG64 r14;
	ULONG64 r15;
	ULONG64 eflgs;
}RegisterContext,*PRegisterContext;

class Hook;

typedef ULONG64(*HookCallBackProc)(Hook * hook,PRegisterContext pRegisterContext);

class Hook
{
private:
	ULONG64 hookAddress;
	HookCallBackProc hookCallBack;
	ULONG64 retAddress;
	bool misHook;
	HookType hookType;
	ULONG copySize;
	char hookBufCode[0x100];
	ULONG64 shellCodeAddr;

	
public:
	Hook(ULONG64 hookAddress, HookCallBackProc hookCallBack, ULONG copySize,ULONG64 retAddress = 0,HookType hookType = HookType::HOOK_E8);
	virtual ~Hook();
	virtual bool funcHook();
	virtual void resetHook();
	ULONG64 getRetAddress();
	ULONG64 getHookAddress();
	HookCallBackProc getHookCallBack();
	bool isHook();
	void setIsHook(bool isHook);
	char * getOriginalCode();
	ULONG getOriginalCodeSize();
	void setShellCodeAddr(ULONG64);
};


void mymemcpy(PVOID desc, PVOID src, ULONG size);

extern "C" 
{
	ULONG64 HookHandlerDispath(Hook * hook, PRegisterContext registerContext);
	bool funcE9Hook(Hook * hook, char * calcCode, ULONG * size);


	typedef bool(*FuncHookProc)(Hook * hook, char * calcCode, ULONG * size);
}



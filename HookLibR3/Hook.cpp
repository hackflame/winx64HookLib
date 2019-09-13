#include "stdafx.h"
#include "Hook.h"


ULONG64 HookHandlerDispath(Hook * hook, PRegisterContext registerContext)
{
	HookCallBackProc callback = hook->getHookCallBack();
	return callback(hook, registerContext);
}

#define HookFuncMax 100

FuncHookProc  funcCallback[HookFuncMax] =
{
	funcE9Hook ,
	
};

bool funcE9Hook(Hook * hook, char * calcCode, ULONG * size)
{
	PCHAR  shellcode = (PCHAR)VirtualAllocEx(GetCurrentProcess(), NULL, 100, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (shellcode == NULL) return false;
	ZeroMemory(shellcode, 100);
	char bufcode[100] =
	{
		
		0x51,//push rcx
		0x48, 0xB9, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00, //mov rcx,0x123456789
		0x48, 0xB8, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00,
		0xff, 0xd0,															//0xE8, 0x00, 0x00, 0x00, 0x00, //call xxxx
		0x59, //pop
		0x58,
		0,
	};

	mymemcpy(bufcode + 25, hook->getOriginalCode(), hook->getOriginalCodeSize());

	char * nextOffsetCode = bufcode + 25 + hook->getOriginalCodeSize();

	//修复
	*(PULONG64)&bufcode[3] = (ULONG64)hook;

	ULONG offset = nextOffsetCode - bufcode;
	mymemcpy(shellcode, bufcode, offset + 1);

	//计算要Call的地址
	
	*(PULONG64)&shellcode[13] = (ULONG64)e9Hook;

	shellcode[offset] = 0x50;
	shellcode[offset+1] = 0x48;
	shellcode[offset+2] = 0xb8;
	*(PULONG64)&shellcode[offset+3] = hook->getRetAddress();
	shellcode[offset + 11] = 0x48; //48 89 44 24 08
	shellcode[offset + 12] = 0x89; //48 89 44 24 08
	shellcode[offset + 13] = 0x44; //48 89 44 24 08
	shellcode[offset + 14] = 0x24; //48 89 44 24 08
	shellcode[offset + 15] = 0x08; //48 89 44 24 08
	shellcode[offset + 16] = 0x58; //48 89 44 24 08
	shellcode[offset + 17] = 0x67; //67 FF 34 24 
	shellcode[offset + 18] = 0xFF; //67 FF 34 24 
	shellcode[offset + 19] = 0x34; //67 FF 34 24 
	shellcode[offset + 20] = 0x24; //67 FF 34 24 
	shellcode[offset + 21] = 0xc3; //67 FF 34 24 

	char code[13] = { 0x50,0x48 ,0xB8 ,0x89 ,0x67 ,0x45 ,0x23 ,0x01 ,0x00 ,0x00 ,0x00 ,0xFF,0xE0 };

	*(PULONG64)&code[3] = (ULONG64)shellcode;
	mymemcpy(calcCode, code, sizeof(code));
	*size = sizeof(code);

	

	hook->setShellCodeAddr((ULONG64)shellcode);

	return true;
}


Hook::Hook(ULONG64 hookAddress, HookCallBackProc hookCallBack,  ULONG64 retAddress, HookType hookType)
{
	this->hookAddress = hookAddress;
	this->hookCallBack = hookCallBack;
	this->hookType = hookType;
	this->retAddress = retAddress;
	this->misHook = false;
	this->copySize = retAddress - hookAddress;
	memset(&this->hookBufCode, 0, 0x100);
	this->shellCodeAddr = 0;
}


Hook::~Hook()
{
	this->resetHook();
}

void mymemcpy(PVOID desc, PVOID src, ULONG size)
{
	char * md = (char *)desc;
	char * msrc = (char *)src;
	ULONG msize = size;
	while (msize) 
	{
		*md++ = *msrc++;
		msize--;
	}
}

bool Hook::funcHook()
{
	
	mymemcpy(this->hookBufCode,(PVOID)this->hookAddress,this->copySize);
	FuncHookProc func = funcCallback[this->hookType];
	if (func == nullptr) return false;
	char code[0x20] = {0};
	ULONG codeSize = 0;
	bool isHookFlags = func(this,code,&codeSize);
	
	this->setIsHook(isHookFlags);
	if (isHookFlags)
	{
		DWORD oldP = 0;
		VirtualProtect((PVOID)this->hookAddress, this->copySize, PAGE_EXECUTE_READWRITE, &oldP);
		mymemcpy((PVOID)this->hookAddress, (PVOID)code, codeSize);
		VirtualProtect((PVOID)this->hookAddress, this->copySize, oldP, &oldP);
	}
	return isHookFlags;
}

void Hook::resetHook()
{
	if (this->misHook) 
	{
		DWORD oldP = 0;
		
		VirtualProtect((PVOID)this->hookAddress, this->copySize, PAGE_EXECUTE_READWRITE, &oldP);
		
		mymemcpy((PVOID)this->hookAddress, this->hookBufCode, this->copySize);
		
		VirtualProtect((PVOID)this->hookAddress, this->copySize, oldP, &oldP);

		if (this->shellCodeAddr != 0)
		{
			VirtualFree((LPVOID)this->shellCodeAddr, 100, MEM_RELEASE);
			this->shellCodeAddr = 0;
		}

	}
	
}



ULONG64 Hook::getRetAddress()
{
	return this->retAddress;
}

ULONG64 Hook::getHookAddress()
{
	return this->hookAddress;
}

HookCallBackProc Hook::getHookCallBack()
{
	return this->hookCallBack;
}

bool Hook::isHook()
{
	return this->misHook;
}

void Hook::setIsHook(bool isHook)
{
	this->misHook = isHook;
}

char * Hook::getOriginalCode()
{
	return this->hookBufCode;
}

ULONG Hook::getOriginalCodeSize()
{
	return this->copySize;
}

void Hook::setShellCodeAddr(ULONG64 Shellcode)
{
	this->shellCodeAddr = Shellcode;
}
// HookLibR3.cpp : 定义控制台应用程序的入口点。
//

//火哥 封装的x64Hook 库 2019/09/13

#include "stdafx.h"
#include <Windows.h>
#include "HookEngine.h"

ULONG64 addr = (ULONG64)0x000007FEFD913360;
char buf[100] = {0};

void initCrc() 
{
	memcpy(buf, (char*)addr, 100);
}

DWORD WINAPI crcTest(PVOID CONTEXT) 
{
	bool isHook = false;
	printf("进入检测\n");
	printf("进入检测\n");
	printf("进入检测\n");
	printf("进入检测\n");
	int j = 0;

	
	while (1) 
	{
		Sleep(3000);
		for (int i = 0; i < 100; i+=4) 
		{
			j++;
			
			ULONG32 code = *(PULONG32)(addr + i);
			j += 1;
			ULONG32 code1 = *(PULONG32)(buf + i);
			if (code != code1) 
			{
				printf("你居然敢hook 我\r\n");
				isHook = true;
				break;
			}
		}

		if (isHook) break;
	}

	if (isHook) 
	{
		printf("打死你，打死你\r\n");
	}
	return 0;
}

ULONG64 callback(Hook * hook, PRegisterContext pRegisterContext)
{
	pRegisterContext->r13 = 0x12345678;
	printf("-------------------------------\r\n");
	return 0;
}



ULONG64 crccallback(Hook * hook, PRegisterContext pRegisterContext)
{
	static int i = 0;
	
	if (i == 100) 
	{
		i = 0;
	}
	pRegisterContext->rax = *(PULONG)&buf[i];
	i += 4;
	
	printf("---------------让你检测不到我----------------\r\n");
	return 0;
}

int main()
{
	initCrc();
	HANDLE hThread = CreateThread(NULL, NULL, crcTest, NULL, 0, 0);
	printf("error %d\n", GetLastError());
	//HOOK前
	ULONG64 x = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	auto engine = HookEngine::GetInstance();
	Hook * hook = new Hook(addr, callback, addr + 15, HOOK_E9);
	Hook * hook1 = new Hook((ULONG64)crcTest+0xA9, crccallback, (ULONG64)crcTest + 0xA9+15, HOOK_E9);
	engine->AddHook(hook);
	engine->AddHook(hook1);

	

	

	//Hook后
	ULONG64 y = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");



	//HookEngine::DestoryInstance();

	//Hook释放后 是否正常
	//ULONG64 z = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	
	getchar();
	return 0;
}


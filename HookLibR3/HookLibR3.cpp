// HookLibR3.cpp : 定义控制台应用程序的入口点。
//

//火哥 封装的x64Hook 库 2019/09/13

#include "stdafx.h"
#include <Windows.h>
#include "HookEngine.h"

ULONG64 callback(Hook * hook, PRegisterContext pRegisterContext)
{
	printf("-------------------------------\r\n");
	return 0;
}

int main()
{
	//这是0x000007FEFD913360 地址 您可以自己修改
	ULONG64 addr  = (ULONG64)0x000007FEFD913360;
	//HOOK前
	ULONG64 x = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	auto engine = HookEngine::GetInstance();
	Hook * hook = new Hook(addr, callback, 15, addr+15, HOOK_E9);
	engine->AddHook(hook);

	//Hook后
	ULONG64 y = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	HookEngine::DestoryInstance();

	//Hook释放后 是否正常
	ULONG64 z = (ULONG64)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	/*
	//HWND hwnd = FindWindowA("#32770", "注入小工具   By:小_火 QQ:471194425");
	//HWND hwnd = FindWindowA("Chrome_WidgetWin_1", "新标签页 - Google Chrome");
	HWND hwnd = FindWindowA("Q360SafeMainClass", "360安全卫士 ");
	//0x0000000000210744
	SendMessageA(hwnd, WM_CLOSE, 0, 0);
	*/
	getchar();
	return 0;
}


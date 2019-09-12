# winx64HookLib
x64HOOK库

支持系统:win64

项目环境:Vs2015

作者:火哥

QQ群：1026716399


使用案例:
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
 }

1.此库没有任何汇编解析引擎

2.小巧简单


注意: 由于没有做相对地址的处理，导致在HOOK完毕 跳回要模拟执行原有的指令会有如下问题

1.碰见立即数内存寻址

2.碰见JCC,CALL,JMP等指令 会出现异常


有时间会不定期的更新，希望大家共同开发！打造一个完美，强大的HOOK 库





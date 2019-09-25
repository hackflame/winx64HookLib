#pragma once
class Hook;
extern "C" void e9Hook(Hook * hook);
extern "C" void __syscall();
extern "C" void _setNumber(ULONG64 number);

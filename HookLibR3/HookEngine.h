#pragma once
#include <vector>
#include "Hook.h"
using namespace std;
class HookEngine
{

private:
	vector<Hook *> hooks;
private:
	HookEngine();
	static HookEngine * sinstance;
public:
	~HookEngine();
	static HookEngine * GetInstance();
	static void  DestoryInstance();
	vector<Hook *> * GetHooks();
	bool AddHook(Hook * hook);
	bool isHookExis(Hook * hook);
};


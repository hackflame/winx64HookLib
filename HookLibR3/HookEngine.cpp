#include "stdafx.h"
#include "HookEngine.h"

HookEngine * HookEngine::sinstance = nullptr;

HookEngine::HookEngine()
{
	hooks.clear();
}


HookEngine::~HookEngine()
{
	for (int i = this->hooks.size() - 1; i >= 0; i--) 
	{
		Hook * hook = this->hooks[i];
		delete hook;
	}
}


HookEngine * HookEngine::GetInstance()
{
	if (sinstance == nullptr) sinstance = new HookEngine();
	return sinstance;
}

void HookEngine::DestoryInstance()
{
	if (sinstance != nullptr) delete sinstance;
	sinstance = nullptr;
}


vector<Hook *> * HookEngine::GetHooks()
{
	return &this->hooks;
}

bool HookEngine::isHookExis(Hook * hook)
{
	bool isFind = false;
	for (int i = 0; i < this->hooks.size(); i++)
	{
		if (hook == this->hooks[i])
		{
			isFind = true;
			break;
		}
	}

	return isFind;
}

bool HookEngine::AddHook(Hook * hook)
{
	bool isExis = isHookExis(hook);
	if (isExis) return false;

	//根据Hook的点开始

	isExis = hook->funcHook();

	if (isExis)  this->hooks.push_back(hook);

	return isExis;

}


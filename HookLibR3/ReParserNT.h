#pragma once
#include <Windows.h>

class ReParserNT
{

private:
	ReParserNT();

	

private:
	static ReParserNT * instance;
	char * ntdllImageBase;
	size_t size;

public:
	virtual ~ReParserNT();
	ULONG64 GetZwFunctionAddr(char * funcName);
	int GetZwFunctionIndex(char * funcName);
	PVOID RtlAddVEHFunc(BOOLEAN isFisrt, PVECTORED_EXCEPTION_HANDLER func);
public:
	static ReParserNT * GetInstance();
	static void DestoryInstance();
};


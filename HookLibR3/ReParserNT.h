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

public:
	static ReParserNT * GetInstance();
	static void DestoryInstance();
};


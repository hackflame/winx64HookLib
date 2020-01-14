#pragma once
#include <Windows.h>


typedef struct _VEHList 
{
	SRWLOCK lock;
	LIST_ENTRY list;
}VEHList,*PVEHList;

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
	ULONG64 GetRtlAddVEHFunc();
	void clearVEH();
	ULONG64 GetRtlRemoveVEHFunc();
public:
	static ReParserNT * GetInstance();
	static void DestoryInstance();
};


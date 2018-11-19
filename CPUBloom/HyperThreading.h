#pragma once
#include <string>

class HyperThreading
{
public:
	HyperThreading(void);
	~HyperThreading(void);

	static bool GetHyperThreadingInfo (unsigned int& processorPackageCount, unsigned int& processorCoreCount, unsigned int& logicalProcessorCount, 
		unsigned int& processorL1CacheSize, unsigned int& processorL2CacheSize, unsigned int& processorL3CacheSize, 
		unsigned int& processorL1CacheLineSize, unsigned int& processorL2CacheLineSize, unsigned int& processorL3CacheLineSize,
		bool& bError, std::wstring& errMsg);

	static DWORD CountSetBits(ULONG_PTR bitMask);

};


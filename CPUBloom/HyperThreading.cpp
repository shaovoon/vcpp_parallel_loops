#include "stdafx.h"
#include "HyperThreading.h"

typedef BOOL (WINAPI *LPFN_GLPI)(
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, 
	PDWORD);


HyperThreading::HyperThreading(void)
{
}


HyperThreading::~HyperThreading(void)
{
}

bool HyperThreading::GetHyperThreadingInfo (unsigned int& processorPackageCount, unsigned int& processorCoreCount, unsigned int& logicalProcessorCount, 
	unsigned int& processorL1CacheSize, unsigned int& processorL2CacheSize, unsigned int& processorL3CacheSize, 
	unsigned int& processorL1CacheLineSize, unsigned int& processorL2CacheLineSize, unsigned int& processorL3CacheLineSize,
	bool& bError, std::wstring& errMsg)
{
	LPFN_GLPI glpi;
	BOOL done = FALSE;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	DWORD returnLength = 0;
	DWORD byteOffset = 0;
	PCACHE_DESCRIPTOR Cache;

	processorPackageCount = 0;
	processorCoreCount = 0;
	logicalProcessorCount = 0;

	processorL1CacheSize = 0;
	processorL2CacheSize = 0;
	processorL3CacheSize = 0;

	processorL1CacheLineSize = 0;
	processorL2CacheLineSize = 0;
	processorL3CacheLineSize = 0;

	glpi = (LPFN_GLPI) GetProcAddress(
		GetModuleHandle(L"kernel32"),
		"GetLogicalProcessorInformation");
	if (NULL == glpi) 
	{
		bError = true;
		errMsg = L"GetLogicalProcessorInformation is not supported.";
		return false;
	}

	while (!done)
	{
		DWORD rc = glpi(buffer, &returnLength);

		if (FALSE == rc) 
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
			{
				if (buffer) 
					free(buffer);

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
					returnLength);

				if (NULL == buffer) 
				{
					bError = true;
					errMsg = L"\nError: Allocation failure\n";
					return false;
				}
			} 
			else 
			{
				bError = true;
				wchar_t buf[100];
				memset(buf, 0, sizeof(buf));
				wsprintf(buf, L"Error %d", GetLastError());
				errMsg = buf;
				return false;
			}
		} 
		else
		{
			done = TRUE;
		}
	}

	ptr = buffer;

	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) 
	{
		switch (ptr->Relationship) 
		{
		case RelationProcessorCore:
			processorCoreCount++;

			// A hyperthreaded core supplies more than one logical processor.
			logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
			break;

		case RelationCache:
			// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
			Cache = &ptr->Cache;
			if (Cache->Level == 1)
			{
				processorL1CacheSize+=Cache->Size;
				processorL1CacheLineSize = Cache->LineSize;
			}
			else if (Cache->Level == 2)
			{
				processorL2CacheSize+=Cache->Size;
				processorL2CacheLineSize = Cache->LineSize;
			}
			else if (Cache->Level == 3)
			{
				processorL3CacheSize+=Cache->Size;
				processorL3CacheLineSize = Cache->LineSize;
			}
			break;

		case RelationProcessorPackage:
			// Logical processors share a physical package.
			processorPackageCount++;
			break;

		default:
			break;
		}
		byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		ptr++;
	}

	free(buffer);

	return logicalProcessorCount > processorCoreCount;
}

// Helper function to count set bits in the processor mask.
DWORD HyperThreading::CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest)?1:0);
		bitTest/=2;
	}

	return bitSetCount;
}

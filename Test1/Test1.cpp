#include "pch.h"
#include <iostream>

void RemoveDump(char* pStr)
{
	char* pRead = pStr + 1;

	while (*pRead)
	{
		if (*pRead != *pStr)
		{
			*++pStr = *pRead++;
		}
		else
			++pRead;
	}
	*++pStr = '\0';
};

int main()
{
	char string[] = "AAA BBB CCC";
	RemoveDump(string);
	printf(string);
	return 0;
}

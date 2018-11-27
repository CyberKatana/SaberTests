#include "pch.h"
#include <iostream>

void RemoveDump(char* pStr)
{
	size_t writeIndex = 0;
	for (size_t readIndex = 1; readIndex < strlen(pStr); readIndex++)
	{
		if (pStr[readIndex] != pStr[writeIndex])
			pStr[++writeIndex] = pStr[readIndex];
	}
	pStr[++writeIndex] = '\0';
};

int main()
{
	char string[] = "AAA BBB CCC";
	RemoveDump(string);
	printf("%s", string);
	return 0;
}

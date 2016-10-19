#include "common.h"

// place your code here
char* DWORD2Time(uint32_t dwTime)
{
	if(!dwTime)
		return(strdup(""));

	uint32_t	dwHours		= dwTime/3600;
	uint32_t	dwMinutes	= (dwTime-(dwHours*3600))/60;
	uint32_t	dwSeconds	= (dwTime-(dwHours*3600)-dwMinutes*60);
	char		szStr[100];

	if(dwHours)
		sprintf(szStr, "%d:%02d:%02d", dwHours, dwMinutes, dwSeconds);
	else
		sprintf(szStr, "%d:%02d", dwMinutes, dwSeconds);

	return(strdup(szStr));
}

char* DWORD2String(uint32_t dwValue)
{
	if(!dwValue)
		return(strdup(""));

	char	szStr[100];

	sprintf(szStr, "%d", dwValue);
	return(strdup(szStr));
}

uint32_t Time2DWORD(char* lpszTime)
{
	if(!strlen(lpszTime))
		return(0);

	char	szHour[100]		= {0};
	char	szMinute[100]	= {0};
	char	szSecond[100]	= {0};
	char	sz[100];

	strcpy(sz, lpszTime);
	if(strrchr(sz, ':'))
	{
		strcpy(szSecond, strrchr(sz, ':')+1);
		*strrchr(sz, ':') = 0;
		if(strchr(sz, ':'))
		{
			strcpy(szMinute, strchr(sz, ':')+1);
			*strchr(sz, ':') = 0;
			strcpy(szHour, sz);
		}
		else
			strcpy(szMinute, sz);
	}
	else
		strcpy(szSecond, sz);


	return(atol(szHour)*3600+atol(szMinute)*60+atol(szSecond));
}

uint32_t String2DWORD(char* lpszStr)
{
	if(!strlen(lpszStr))
		return(0);
	return(atol(lpszStr));
}

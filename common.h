#ifndef COMMON_H
#define COMMON_H

#include <QMessageBox>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "ctime.h"

#define DELETE(x)		if(x){delete x;x=0;}

#define MESSAGE(x)			QMessageBox::information(this, tr("qtTrainingPlan"), x)
#define MESSAGE_QUESTION(x)	QMessageBox::question(this, tr("qtTrainingPlan"), x, QMessageBox::Yes | QMessageBox::No)

#define HOURS(x)		((x)/3600)
#define MINUTES(x)		(((x)/60)%60)
#define SECONDS(x)		((x)%60)

// place your code here
char* DWORD2Time(uint32_t dwTime);
char* DWORD2String(uint32_t dwValue);
uint32_t String2DWORD(char* lpszStr);
uint32_t Time2DWORD(char* lpszTime);

#endif // COMMON_H

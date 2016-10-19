#ifndef CTIME_H
#define CTIME_H

#include <QTime>
#include <stdint.h>
#include "common.h"

class CTime : public QTime
{
public:
	CTime();
	CTime(int h, int m = 0, int s = 0, int ms = 0);
	CTime(const QTime& param);
	CTime(uint32_t dwTime);
	CTime(const QString& time);
	QString	Format();
	const CTime& operator=(const CTime& param);
	uint32_t toDWORD();
};

#endif // CTIME_H

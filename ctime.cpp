#include "ctime.h"


CTime::CTime() : QTime()
{
}

CTime::CTime(int h, int m, int s, int ms) : QTime(h, m, s, ms)
{
}

CTime::CTime(const QTime& param)
{
	setHMS(param.hour(), param.minute(), param.second(), param.msec());
}

CTime::CTime(uint32_t dwTime)
{
	setHMS(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime));
}

CTime::CTime(const QString& time)
{
	QString	szHour;
	QString	szMinute;
	QString	szSecond;
	QString	tmp	= time;

	if(tmp.contains(":"))
	{
		szSecond	= tmp.mid(tmp.lastIndexOf(":")+1);
		tmp			= tmp.left(tmp.lastIndexOf(":"));
	}
	else
	{
		szSecond	= tmp;
		tmp			= "";
	}

	if(tmp.contains(":"))
	{
		szMinute	= tmp.mid(tmp.lastIndexOf(":")+1);
		tmp			= tmp.left(tmp.lastIndexOf(":"));
	}
	else
	{
		szMinute	= tmp;
		tmp			= "";
	}
	szHour = tmp;
	setHMS(szHour.toInt(), szMinute.toInt(), szSecond.toInt());
}

QString	CTime::Format()
{
	if(!hour() && !minute() && !second())
		return(QString(""));
	QString	szTime;
	if(hour())
		szTime = toString(QString("h:mm:ss"));
	else
		szTime = toString(QString("mm:ss"));
	return(szTime);
}

const CTime& CTime::operator=(const CTime& param)
{
	setHMS(param.hour(), param.minute(), param.second(), param.msec());
	return(*this);
}

uint32_t CTime::toDWORD()
{
	uint32_t	h	= hour();
	uint32_t	m	= minute();
	uint32_t	s	= second();
	uint32_t	t	= h*3600+m*60+s;
	return(t);
}

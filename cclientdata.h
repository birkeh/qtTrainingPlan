#ifndef CCLIENTDATA_H
#define CCLIENTDATA_H


#include "ctime.h"
#include "common.h"


typedef struct _DATA_PERSON
{
	QString		szFirstName;
	QString		szLastName;
	QDate		DayOfBirth;
	uint32_t	dwHeight;
	uint32_t	dwWeight;
}	DATA_PERSON, *LPDATA_PERSON;

typedef struct _DATA_HR
{
	QString		szName;
	QString		szDesc;
	uint32_t	dwMin;
	uint32_t	dwMax;
}	DATA_HR, *LPDATA_HR;

typedef struct _DATA_SPORT
{
	QString		szName;
	uint32_t	dwMaxHR;
	bool		bPace;
}	DATA_SPORT, *LPDATA_SPORT;

typedef struct _DATA_DAY
{
	int			iNum;
	QString		szDesc;
}	DATA_DAY, *LPDATA_DAY;

typedef struct _DATA_SESSION
{
	int			iNum;
	QString		szDesc;
	QString		szSport;
}	DATA_SESSION, *LPDATA_SESSION;

typedef struct _DATA_TASK
{
	int			iNum;
	QString		szDesc;
	uint32_t	dwCount;
	CTime		Time;
	uint32_t	dwDist;
	QString		szHR;
	uint32_t	dwPace;
	CTime		PauseTime;
	uint32_t	dwPauseDist;
	QString		szPauseHR;
	uint32_t	dwPausePace;
}	DATA_TASK, *LPDATA_TASK;

class CClientData
{
public:
	enum FLAGS
	{
		DAY		= 1,
		SESSION	= 2,
		TASK	= 3,
		SPORT	= 4,
		HR		= 5,
		PERSON	= 6
	};
	// DAY
	CClientData(int iDay, const QString& szDesc);
	// SESSION
	CClientData(int iSession, const QString& szDesc, const QString& szSport);
	// TASK
	CClientData(int iTask, const QString& szDesc, uint32_t dwCount, CTime Time, uint32_t dwDist, const QString& szHR, uint32_t dwPace, CTime PauseTime = CTime(0, 0, 0), uint32_t dwPauseDist = 0, const QString& szPauseHR = QString(""), uint32_t dwPausePace = 0);
	// SPORT
	CClientData(const QString& szSport, uint32_t dwMaxHR, bool bPace);
	//
	CClientData(const QString& szName, const QString& szDesc, uint32_t dwMin, uint32_t dwMax);
	CClientData(const QString& szFirstName, const QString& szLastName, const QDate& DayOfBirth, uint32_t dwHeight, uint32_t dwWeight);
	~CClientData();

	void		Clear(bool bDelete = true);
	void		SetDay(int iDay);
	void		SetDayDesc(const QString& szDesc);
	void		SetSession(int iSession);
	void		SetSessionDesc(const QString& szDesc);
	void		SetSessionSport(const QString& szSport);
	void		SetTask(int iTask);
	void		SetTaskDesc(const QString& szDesc);
	void		SetTaskCount(uint32_t dwCount);
	void		SetTaskTime(const CTime& Time);
	void		SetTaskDist(uint32_t dwDist);
	void		SetTaskHR(const QString& szHR);
	void		SetTaskPace(uint32_t dwPace);
	void		SetTaskPauseTime(const CTime& PauseTime);
	void		SetTaskPauseDist(uint32_t dwPauseDist);
	void		SetTaskPauseHR(const QString& szPauseHR);
	void		SetTaskPausePace(uint32_t dwPausePace);
	void		SetSportName(const QString& szName);
	void		SetSportMaxHR(uint32_t dwMaxHR);
	void		SetSportPace(bool bPace);
	void		SetHRName(const QString& szName);
	void		SetHRDesc(const QString& szDesc);
	void		SetHRMin(uint32_t dwMin);
	void		SetHRMax(uint32_t dwMax);
	void		SetPersonFirstName(const QString& szFirstName);
	void		SetPersonLastName(const QString& szLastName);
	void		SetPersonDayOfBirth(const QDate& DayOfBirth);
	void		SetPersonHeight(uint32_t dwHeight);
	void		SetPersonWeight(uint32_t dwWeight);
	int			GetDay();
	QString		GetDayDesc(bool bWithDayNum = true);
	int			GetSession();
	QString		GetSessionDesc();
	QString		GetSessionSport();
	int			GetTask();
	QString		GetTaskDesc();
	uint32_t	GetTaskCount();
	CTime		GetTaskTime();
	uint32_t	GetTaskDist();
	QString		GetTaskHR();
	uint32_t	GetTaskPace();
	CTime		GetTaskPauseTime();
	uint32_t	GetTaskPauseDist();
	QString		GetTaskPauseHR();
	uint32_t	GetTaskPausePace();
	QString		GetSportName();
	uint32_t	GetSportMaxHR();
	bool		GetSportPace();
	QString		GetHRName();
	QString		GetHRDesc();
	uint32_t	GetHRMin();
	uint32_t	GetHRMax();
	QString		GetPersonFirstName();
	QString		GetPersonLastName();
	QDate		GetPersonDayOfBirth();
	uint32_t	GetPersonHeight();
	uint32_t	GetPersonWeight();
	int			GetType();
protected:
	LPDATA_DAY		m_lpDay;
	LPDATA_SESSION	m_lpSession;
	LPDATA_TASK		m_lpTask;
	LPDATA_SPORT	m_lpSport;
	LPDATA_HR		m_lpHR;
	LPDATA_PERSON	m_lpPerson;
};

#endif // CCLIENTDATA_H

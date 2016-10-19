#include "cclientdata.h"

// DAY
CClientData::CClientData(int iDay, const QString& szDesc)
{
	Clear(false);
	m_lpDay			= new DATA_DAY;
	m_lpDay->iNum	= iDay;
	m_lpDay->szDesc	= szDesc;
}

// SESSION
CClientData::CClientData(int iSession, const QString& szDesc, const QString& szSport)
{
	Clear(false);
	m_lpSession				= new DATA_SESSION;
	m_lpSession->iNum		= iSession;
	m_lpSession->szDesc		= szDesc;
	m_lpSession->szSport	= szSport;
}

// TASK
CClientData::CClientData(int iTask, const QString& szDesc, uint32_t dwCount, CTime Time, uint32_t dwDist, const QString& szHR, uint32_t dwPace, CTime PauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace)
{
	Clear(false);
	m_lpTask				= new DATA_TASK;
	m_lpTask->iNum			= iTask;
	m_lpTask->szDesc		= szDesc;
	m_lpTask->dwCount		= dwCount;
	m_lpTask->Time			= Time;
	m_lpTask->dwDist		= dwDist;
	m_lpTask->szHR			= szHR;
	m_lpTask->dwPace		= dwPace;
	m_lpTask->PauseTime		= PauseTime;
	m_lpTask->dwPauseDist	= dwPauseDist;
	m_lpTask->szPauseHR		= szPauseHR;
	m_lpTask->dwPausePace	= dwPausePace;
}

// SPORT
CClientData::CClientData(const QString& szSport, uint32_t dwMaxHR, bool bPace)
{
	Clear(false);
	m_lpSport			= new DATA_SPORT;
	m_lpSport->szName	= szSport;
	m_lpSport->dwMaxHR	= dwMaxHR;
	m_lpSport->bPace	= bPace;
}

//
CClientData::CClientData(const QString& szName, const QString& szDesc, uint32_t dwMin, uint32_t dwMax)
{
	Clear(false);
	m_lpHR			= new DATA_HR;
	m_lpHR->szName	= szName;
	m_lpHR->szDesc	= szDesc;
	m_lpHR->dwMin	= dwMin;
	m_lpHR->dwMax	= dwMax;
}

CClientData::CClientData(const QString& szFirstName, const QString& szLastName, const QDate& DayOfBirth, uint32_t dwHeight, uint32_t dwWeight)
{
	Clear(false);
	m_lpPerson				= new DATA_PERSON;
	m_lpPerson->szFirstName	= szFirstName;
	m_lpPerson->szLastName	= szLastName;
	m_lpPerson->DayOfBirth	= DayOfBirth;
	m_lpPerson->dwHeight	= dwHeight;
	m_lpPerson->dwWeight	= dwWeight;
}

CClientData::~CClientData()
{
	Clear();
}

void CClientData::Clear(bool bDelete)
{
	if(bDelete)
	{
		DELETE(m_lpDay);
		DELETE(m_lpSession);
		DELETE(m_lpTask);
		DELETE(m_lpSport);
		DELETE(m_lpHR);
		DELETE(m_lpPerson);
	}
	m_lpDay		= 0;
	m_lpSession	= 0;
	m_lpTask	= 0;
	m_lpSport	= 0;
	m_lpHR		= 0;
	m_lpPerson	= 0;
}

void CClientData::SetDay(int iDay)
{
	if(m_lpDay)
		m_lpDay->iNum			= iDay;
}

void CClientData::SetDayDesc(const QString& szDesc)
{
	if(m_lpDay)
		m_lpDay->szDesc			= szDesc;
}

void CClientData::SetSession(int iSession)
{
	if(m_lpSession)
		m_lpSession->iNum		= iSession;
}

void CClientData::SetSessionDesc(const QString& szDesc)
{
	if(m_lpSession)
		m_lpSession->szDesc		= szDesc;
}

void CClientData::SetSessionSport(const QString& szSport)
{
	if(m_lpSession)
		m_lpSession->szSport	= szSport;
}

void CClientData::SetTask(int iTask)
{
	if(m_lpTask)
	m_lpTask->iNum			= iTask;
}

void CClientData::SetTaskDesc(const QString& szDesc)
{
	if(m_lpTask)
		m_lpTask->szDesc		= szDesc;
}
void CClientData::SetTaskCount(uint32_t dwCount)
{
	if(m_lpTask)
		m_lpTask->dwCount		= dwCount;
}

void CClientData::SetTaskTime(const CTime& Time)
{
	if(m_lpTask)
		m_lpTask->Time			= Time;
}

void CClientData::SetTaskDist(uint32_t dwDist)
{
	if(m_lpTask)
		m_lpTask->dwDist		= dwDist;
}

void CClientData::SetTaskHR(const QString& szHR)
{
	if(m_lpTask)
		m_lpTask->szHR			= szHR;
}

void CClientData::SetTaskPace(uint32_t dwPace)
{
	if(m_lpTask)
		m_lpTask->dwPace		= dwPace;
}

void CClientData::SetTaskPauseTime(const CTime& PauseTime)
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		m_lpTask->PauseTime		= PauseTime;
}

void CClientData::SetTaskPauseDist(uint32_t dwPauseDist)
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		m_lpTask->dwPauseDist	= dwPauseDist;
}

void CClientData::SetTaskPauseHR(const QString& szPauseHR)
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		m_lpTask->szPauseHR		= szPauseHR;
}

void CClientData::SetTaskPausePace(uint32_t dwPausePace)
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		m_lpTask->dwPausePace	= dwPausePace;
}

void CClientData::SetSportName(const QString& szName)
{
	if(m_lpSport)
		m_lpSport->szName		= szName;
}

void CClientData::SetSportMaxHR(uint32_t dwMaxHR)
{
	if(m_lpSport)
		m_lpSport->dwMaxHR		= dwMaxHR;
}

void CClientData::SetSportPace(bool bPace)
{
	if(m_lpSport)
		m_lpSport->bPace		= bPace;
}

void CClientData::SetHRName(const QString& szName)
{
	if(m_lpHR)
		m_lpHR->szName			= szName;
}

void CClientData::SetHRDesc(const QString& szDesc)
{
	if(m_lpHR)
		m_lpHR->szDesc			= szDesc;
}

void CClientData::SetHRMin(uint32_t dwMin)
{
	if(m_lpHR)
		m_lpHR->dwMin			= dwMin;
}

void CClientData::SetHRMax(uint32_t dwMax)
{
	if(m_lpHR)
		m_lpHR->dwMax			= dwMax;
}

void CClientData::SetPersonFirstName(const QString& szFirstName)
{
	if(m_lpPerson)
		m_lpPerson->szFirstName	= szFirstName;
}

void CClientData::SetPersonLastName(const QString& szLastName)
{
	if(m_lpPerson)
		m_lpPerson->szLastName	= szLastName;
}

void CClientData::SetPersonDayOfBirth(const QDate& DayOfBirth)
{
	if(m_lpPerson)
		m_lpPerson->DayOfBirth	= DayOfBirth;
}

void CClientData::SetPersonHeight(uint32_t dwHeight)
{
	if(m_lpPerson)
		m_lpPerson->dwHeight	= dwHeight;
}

void CClientData::SetPersonWeight(uint32_t dwWeight)
{
	if(m_lpPerson)
		m_lpPerson->dwWeight	= dwWeight;
}

int CClientData::GetDay()
{
	if(m_lpDay)
		return(m_lpDay->iNum);
	return(-1);
}

QString CClientData::GetDayDesc(bool bWithDayNum)
{
	if(m_lpDay)
	{
		if(bWithDayNum)
			return(m_lpDay->szDesc);
		else
			return(m_lpDay->szDesc.mid(m_lpDay->szDesc.indexOf(")")+1));
	}
	return(QString(""));
}

int CClientData::GetSession()
{
	if(m_lpSession)
		return(m_lpSession->iNum);
	return(-1);
}

QString CClientData::GetSessionDesc()
{
	if(m_lpSession)
		return(m_lpSession->szDesc);
	return(QString(""));
}

QString CClientData::GetSessionSport()
{
	if(m_lpSession)
		return(m_lpSession->szSport);
	return(QString(""));
}

int CClientData::GetTask()
{
	if(m_lpTask)
		return(m_lpTask->iNum);
	return(-1);
}

QString CClientData::GetTaskDesc()
{
	if(m_lpTask)
		return(m_lpTask->szDesc);
	return(QString(""));
}

uint32_t CClientData::GetTaskCount()
{
	if(m_lpTask)
		return(m_lpTask->dwCount);
	return(0);
}

CTime CClientData::GetTaskTime()
{
	if(m_lpTask)
		return(m_lpTask->Time);
	return(CTime(0, 0, 0));
}

uint32_t CClientData::GetTaskDist()
{
	if(m_lpTask)
		return(m_lpTask->dwDist);
	return(0);
}

QString CClientData::GetTaskHR()
{
	if(m_lpTask)
		return(m_lpTask->szHR);
	return(QString(""));
}

uint32_t CClientData::GetTaskPace()
{
	if(m_lpTask)
		return(m_lpTask->dwPace);
	return(0);
}

CTime CClientData::GetTaskPauseTime()
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		return(m_lpTask->PauseTime);
	return(CTime(0, 0, 0));
}

uint32_t CClientData::GetTaskPauseDist()
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		return(m_lpTask->dwPauseDist);
	return(0);
}

QString CClientData::GetTaskPauseHR()
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		return(m_lpTask->szPauseHR);
	return(QString(""));
}

uint32_t CClientData::GetTaskPausePace()
{
	if(m_lpTask && m_lpTask->dwCount > 1)
		return(m_lpTask->dwPausePace);
	return(0);
}

QString CClientData::GetSportName()
{
	if(m_lpSport)
		return(m_lpSport->szName);
	return(QString(""));
}

uint32_t CClientData::GetSportMaxHR()
{
	if(m_lpSport)
		return(m_lpSport->dwMaxHR);
	return(0);
}

bool CClientData::GetSportPace()
{
	if(m_lpSport)
		return(m_lpSport->bPace);
	return(0);
}

QString CClientData::GetHRName()
{
	if(m_lpHR)
		return(m_lpHR->szName);
	return(QString(""));
}

QString CClientData::GetHRDesc()
{
	if(m_lpHR)
		return(m_lpHR->szDesc);
	return(QString(""));
}

uint32_t CClientData::GetHRMin()
{
	if(m_lpHR)
		return(m_lpHR->dwMin);
	return(0);
}

uint32_t CClientData::GetHRMax()
{
	if(m_lpHR)
		return(m_lpHR->dwMax);
	return(0);
}

QString CClientData::GetPersonFirstName()
{
	if(m_lpPerson)
		return(m_lpPerson->szFirstName);
	return(QString(""));
}

QString CClientData::GetPersonLastName()
{
	if(m_lpPerson)
		return(m_lpPerson->szLastName);
	return(QString(""));
}

QDate CClientData::GetPersonDayOfBirth()
{
	if(m_lpPerson)
		return(m_lpPerson->DayOfBirth);
	return(QDate(0, 0, 0));
}

uint32_t CClientData::GetPersonHeight()
{
	if(m_lpPerson)
		return(m_lpPerson->dwHeight);
	return(0);
}

uint32_t CClientData::GetPersonWeight()
{
	if(m_lpPerson)
		return(m_lpPerson->dwWeight);
	return(0);
}

int CClientData::GetType()
{
	if(m_lpTask)
		return(CClientData::TASK);
	else if(m_lpSession)
		return(CClientData::SESSION);
	else if(m_lpDay)
		return(CClientData::DAY);
	else if(m_lpSport)
		return(CClientData::SPORT);
	else if(m_lpHR)
		return(CClientData::HR);
	else if(m_lpPerson)
		return(CClientData::PERSON);
	return(0);
}

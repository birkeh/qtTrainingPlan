#ifndef CTRAINING_H
#define CTRAINING_H

#include <QDate>
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include "common.h"


class CPerson
{
public:
	CPerson()
	{
		m_szFirstName	= "";
		m_szLastName	= "";
		m_dwWeight		= 0;
		m_dwWeight		= 0;
	};
	CPerson(const QString& szFirstName, const QString& szLastName, const QDate& DayOfBirth, uint32_t dwHeight, uint32_t dwWeight)
	{
		m_szFirstName	= "";
		m_szLastName	= "";
		Set(szFirstName, szLastName, DayOfBirth, dwHeight, dwWeight);
	};
	~CPerson()
	{
	};
	void SetFirstName(const QString& szFirstName)	{	m_szFirstName	= szFirstName;	}
	void SetLastName(const QString& szLastName)		{	m_szLastName	= szLastName;	}
	void SetDayOfBirth(const QDate& DayOfBirth)		{	m_DayOfBirth	= DayOfBirth;	}
	void SetHeight(uint32_t dwHeight)				{	m_dwHeight		= dwHeight;		}
	void SetWeight(uint32_t dwWeight)				{	m_dwWeight		= dwWeight;		}
	void Set(const QString& szFirstName, const QString& szLastName, const QDate& DayOfBirth, uint32_t dwHeight, uint32_t dwWeight)
	{
		SetFirstName(szFirstName);
		SetLastName(szLastName);
		SetDayOfBirth(DayOfBirth);
		SetHeight(dwHeight);
		SetWeight(dwWeight);
	};
	QString		GetFirstName()	{	return(m_szFirstName);	};
	QString		GetLastName()	{	return(m_szLastName);	};
	QDate		GetDayOfBirth()	{	return(m_DayOfBirth);	};
	uint32_t	GetHeight()		{	return(m_dwHeight);		};
	uint32_t	GetWeight()		{	return(m_dwWeight);		};
protected:
	QString		m_szFirstName;
	QString		m_szLastName;
	QDate		m_DayOfBirth;
	uint32_t	m_dwHeight;
	uint32_t	m_dwWeight;
};

class CHR
{
public:
	CHR(const QString& szName, const QString& szDesc = "", uint32_t dwMin = 0, uint32_t dwMax = 0, CHR* lpChild = 0)
	{
		m_szName	= "";
		m_szDesc	= "";
		m_lpChild	= 0;
		Set(szName, szDesc, dwMin, dwMax, lpChild);
	};
	~CHR()
	{
		DELETE(m_lpChild);
	}
	CHR* Add(const QString& szName, const QString& szDesc = "", uint32_t dwMin = 0, uint32_t dwMax = 0, CHR* lpChild = 0)
	{
		if(!m_lpChild)
			m_lpChild = new CHR(szName, szDesc, dwMin, dwMax, lpChild);
		else
			m_lpChild->Set(szName, szDesc, dwMin, dwMax, lpChild);
		return(m_lpChild);
	};
	void SetName(const QString& szName)	{	m_szName	= szName;	};
	void SetDesc(const QString& szDesc)	{	m_szDesc	= szDesc;	};
	void SetMin(uint32_t dwMin)			{	m_dwMin		= dwMin;	};
	void SetMax(uint32_t dwMax)			{	m_dwMax		= dwMax;	};
	void SetChild(CHR* lpChild)			{	m_lpChild	= lpChild;	};
	void Set(const QString& szName, const QString& szDesc = "", uint32_t dwMin = 0, uint32_t dwMax = 0, CHR* lpChild = 0)
	{
		SetName(szName);
		SetDesc(szDesc);
		SetMin(dwMin);
		SetMax(dwMax);
		SetChild(lpChild);
	};
	QString		GetName()		{	return(m_szName);	};
	QString		GetDesc()		{	return(m_szDesc);	};
	uint32_t	GetMin()		{	return(m_dwMin);	};
	uint32_t	GetMax()		{	return(m_dwMax);	};
	CHR*		GetChild()		{	return(m_lpChild);	};
	CHR*		Find(const QString& szName)
	{
		if(m_szName == szName)
			return(this);
		if(m_lpChild)
			return(m_lpChild->Find(szName));
		return(0);
	};
	bool HasName()						{	if(m_szName.length()) return(true); return(false);	};
	bool HasDesc()						{	if(m_szDesc.length()) return(true); return(false);	};
protected:
	QString		m_szName;
	QString		m_szDesc;
	uint32_t	m_dwMin;
	uint32_t	m_dwMax;
	CHR*		m_lpChild;
};

class CSport
{
public:
	CSport(const QString& szName, uint32_t dwMaxHR = 0, CHR* lpHR = 0, bool bPace = true, CSport* lpChild = 0)
	{
		m_szName	= "";
		m_lpHR		= 0;
		m_lpChild	= 0;
		Set(szName, dwMaxHR, lpHR, bPace, lpChild);
	};
	~CSport()
	{
		DELETE(m_lpHR);
		DELETE(m_lpChild);
	};
	CSport* Add(const QString& szName, uint32_t dwMaxHR = 0, CHR* lpHR = 0, bool bPace = true, CSport* lpChild = 0)
	{
		if(!m_lpChild)
			m_lpChild = new CSport(szName, dwMaxHR, lpHR, bPace, lpChild);
		else
			m_lpChild->Set(szName, dwMaxHR, lpHR, bPace, lpChild);
		return(m_lpChild);
	};
	CHR* AddHR(const QString& szName, const QString& szDesc = "", uint32_t dwMin = 0, uint32_t dwMax = 0, CHR* lpChild = 0)
	{
		CHR* lpHRDummy = m_lpHR;
		if(!lpHRDummy)
			lpHRDummy = m_lpHR = new CHR(szName, szDesc, dwMin, dwMax, lpChild);
		else
		{
			while(lpHRDummy->GetChild())
				lpHRDummy = lpHRDummy->GetChild();
			lpHRDummy = lpHRDummy->Add(szName, szDesc, dwMin, dwMax, lpChild);
		}
		return(lpHRDummy);
	};
	void SetName(const QString& szName)	{	m_szName	= szName;	};
	void SetMaxHR(uint32_t dwMaxHR)		{	m_dwMaxHR	= dwMaxHR;	};
	void SetHR(CHR* lpHR)				{	m_lpHR		= lpHR;		};
	void SetPace(bool bPace)			{	m_bPace		= bPace;	};
	void SetChild(CSport* lpChild)		{	m_lpChild	= lpChild;	};
	void Set(const QString& szName, uint32_t dwMaxHR = 0, CHR* lpHR = 0, bool bPace = true, CSport* lpChild = 0)
	{
		SetName(szName);
		SetMaxHR(dwMaxHR);
		SetHR(lpHR);
		SetPace(bPace);
		SetChild(lpChild);
	};
	QString		GetName()		{	return(m_szName);	};
	uint32_t	GetMaxHR()		{	return(m_dwMaxHR);	};
	CHR*		GetHR()			{	return(m_lpHR);		};
	bool		GetPace()		{	return(m_bPace);	};
	CSport*		GetChild()		{	return(m_lpChild);	};
	CSport*		Find(const QString& szName)
	{
		if(m_szName == szName)
			return(this);
		if(m_lpChild)
			return(m_lpChild->Find(szName));
		return(0);
	};
	bool HasName()						{	if(m_szName.length()) return(true); return(false);	};
protected:
	QString		m_szName;
	uint32_t	m_dwMaxHR;
	CHR*		m_lpHR;
	bool		m_bPace;
	CSport*		m_lpChild;
};

class CTrainingTask
{
public:
	CTrainingTask(int iNum, const QString& szDesc = "", uint32_t dwCount = 0, uint32_t dwTime = 0, uint32_t dwDist = 0, const QString& szHR = "", uint32_t dwPace = 0, uint32_t dwPauseTime = 0, uint32_t dwPauseDist = 0, const QString& szPauseHR = "", uint32_t dwPausePace = 0, CTrainingTask* lpChild = 0)
	{
		m_szDesc	= "";
		m_szHR		= "";
		m_szPauseHR	= "";
		m_lpChild	= 0;
		Set(iNum, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace, lpChild);
	};
	~CTrainingTask()
	{
		DELETE(m_lpChild);
	};
	CTrainingTask* Add(int iNum, const QString& szDesc = "", uint32_t dwCount = 0, uint32_t dwTime = 0, uint32_t dwDist = 0, const QString& szHR = "", uint32_t dwPace = 0, uint32_t dwPauseTime = 0, uint32_t dwPauseDist = 0, const QString& szPauseHR = "", uint32_t dwPausePace = 0, CTrainingTask* lpChild = 0)
	{
		if(!m_lpChild)
			m_lpChild = new CTrainingTask(iNum, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace, lpChild);
		else
			m_lpChild->Set(iNum, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace, lpChild);
		return(m_lpChild);
	};
	void SetNum(int iNum)						{	m_iNum					= iNum;			};
	void SetDesc(const QString& szDesc)			{	m_szDesc				= szDesc;		};
	void SetCount(uint32_t dwCount)				{	m_dwCount				= dwCount;		};
	void SetTime(uint32_t dwTime)				{	m_Time					= CTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime));	};
	void SetDist(uint32_t dwDist)				{	m_dwDist				= dwDist;		};
	void SetHR(const QString& szHR)				{	m_szHR					= szHR;			};
	void SetPace(uint32_t dwPace)				{	m_dwPace				= dwPace;		};
	void SetPauseTime(uint32_t dwPauseTime)		{	m_PauseTime				= CTime(HOURS(dwPauseTime), MINUTES(dwPauseTime), SECONDS(dwPauseTime));	};
	void SetPauseDist(uint32_t dwPauseDist)		{	m_dwPauseDist			= dwPauseDist;	};
	void SetPauseHR(const QString& szPauseHR)	{	m_szPauseHR				= szPauseHR;	};
	void SetPausePace(uint32_t dwPausePace)		{	m_dwPausePace			= dwPausePace;	};
	void SetChild(CTrainingTask* lpChild)		{	m_lpChild				= lpChild;		};
	void Set(int iNum, const QString& szDesc = "", uint32_t dwCount = 0, uint32_t dwTime = 0, uint32_t dwDist = 0, const QString& szHR = "", uint32_t dwPace = 0, uint32_t dwPauseTime = 0, uint32_t dwPauseDist = 0, const QString& szPauseHR = "", uint32_t dwPausePace = 0, CTrainingTask* lpChild = 0)
	{
		SetNum(iNum);
		SetDesc(szDesc);
		SetCount(dwCount);
		SetTime(dwTime);
		SetDist(dwDist);
		SetHR(szHR);
		SetPace(dwPace);
		SetPauseTime(dwPauseTime);
		SetPauseDist(dwPauseDist);
		SetPauseHR(szPauseHR);
		SetPausePace(dwPausePace);
		SetChild(lpChild);
	};
	int				GetNum()			{	return(m_iNum);					};
	QString			GetDesc()			{	return(m_szDesc);				};
	uint32_t		GetCount()			{	return(m_dwCount);				};
	uint32_t		GetTime()			{	return(m_Time.toDWORD());		};
	uint32_t		GetDist()			{	return(m_dwDist);				};
	QString			GetHR()				{	return(m_szHR);					};
	uint32_t		GetPace()			{	return(m_dwPace);				};
	uint32_t		GetPauseTime()		{	return(m_PauseTime.toDWORD());	};
	uint32_t		GetPauseDist()		{	return(m_dwPauseDist);			};
	QString			GetPauseHR()		{	return(m_szPauseHR);			};
	uint32_t		GetPausePace()		{	return(m_dwPausePace);			};
	CTrainingTask*	GetChild()			{	return(m_lpChild);				};
	void			CalcPace()			{	m_dwPace		= m_Time.toDWORD()*1000/m_dwDist;				};
	void			CalcTime()			{	uint32_t dwTime	= m_dwPace*m_dwDist/1000;	m_Time	= CTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)); };
	void			CalcDist()			{	m_dwDist		= m_Time.toDWORD()*1000/m_dwPace;				};
	void			CalcPausePace()		{	m_dwPausePace	= m_PauseTime.toDWORD()*1000/m_dwPauseDist;		};
	void			CalcPauseTime()		{	uint32_t dwTime	= m_dwPausePace*m_dwPauseDist/1000;	m_PauseTime	= CTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)); };
	void			CalcPauseDist()		{	m_dwPauseDist	= m_PauseTime.toDWORD()*1000/m_dwPausePace;		};
	bool			HasHR()				{	if(m_szHR.length())			return(true); return(false);	};
	bool			HasPauseHR()		{	if(m_szPauseHR.length())	return(true); return(false);	};
	bool			HasDesc()			{	if(m_szDesc.length())		return(true); return(false);	};
protected:
	int				m_iNum;
	QString			m_szDesc;
	uint32_t		m_dwCount;
	CTime			m_Time;
	uint32_t		m_dwDist;
	QString			m_szHR;
	uint32_t		m_dwPace;
	CTime			m_PauseTime;
	uint32_t		m_dwPauseDist;
	QString			m_szPauseHR;
	uint32_t		m_dwPausePace;
	CTrainingTask*	m_lpChild;
};

class CTrainingSession
{
public:
	CTrainingSession(int iNum, const QString& szDesc = "", const QString& szSport = "", CTrainingTask* lpTrainingTask = 0, CTrainingSession* lpChild = 0)
	{
		m_szDesc			= "";
		m_szSport			= "";
		m_lpTrainingTask	= 0;
		m_lpChild			= 0;
		Set(iNum, szDesc, szSport, lpTrainingTask, lpChild);
	};
	~CTrainingSession()
	{
		DELETE(m_lpTrainingTask);
		DELETE(m_lpChild);
	};
	CTrainingSession* Add(int iNum, const QString& szDesc = "", const QString& szSport = "", CTrainingTask* lpTrainingTask = 0, CTrainingSession* lpChild = 0)
	{
		if(!m_lpChild)
			m_lpChild = new CTrainingSession(iNum, szDesc, szSport, lpTrainingTask, lpChild);
		else
			m_lpChild->Set(iNum, szDesc, szSport, lpTrainingTask, lpChild);
		return(m_lpChild);
	};
	CTrainingTask* AddTask(int iNum, const QString& szDesc = "", uint32_t dwCount = 0, uint32_t dwTime = 0, uint32_t dwDist = 0, const QString& szHR = "", uint32_t dwPace = 0, uint32_t dwPauseTime = 0, uint32_t dwPauseDist = 0, const QString& szPauseHR = "", uint32_t dwPausePace = 0, CTrainingTask* lpChild = 0)
	{
		CTrainingTask* lpTrainingTaskDummy = m_lpTrainingTask;
		if(!m_lpTrainingTask)
			m_lpTrainingTask = lpTrainingTaskDummy = new CTrainingTask(iNum, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace, lpChild);
		else
		{
			while(lpTrainingTaskDummy->GetChild())
				lpTrainingTaskDummy = lpTrainingTaskDummy->GetChild();
			lpTrainingTaskDummy = lpTrainingTaskDummy->Add(iNum, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace, lpChild);
		}
		return(lpTrainingTaskDummy);
	};
	void SetNum(int iNum)								{	m_iNum				= iNum;				};
	void SetDesc(const QString& szDesc)					{	m_szDesc			= szDesc;			};
	void SetSport(const QString& szSport)				{	m_szSport			= szSport;			};
	void SetTrainingTask(CTrainingTask* lpTrainingTask)	{	m_lpTrainingTask	= lpTrainingTask;	};
	void SetChild(CTrainingSession* lpChild)			{	m_lpChild			= lpChild;			};
	void Set(int iNum, const QString& szDesc, const QString& szSport, CTrainingTask* lpTrainingTask = 0, CTrainingSession* lpChild = 0)
	{
		SetNum(iNum);
		SetDesc(szDesc);
		SetSport(szSport);
		SetTrainingTask(lpTrainingTask);
		SetChild(lpChild);
	};
	int					GetNum()			{	return(m_iNum);				};
	QString				GetDesc()			{	return(m_szDesc);			};
	QString				GetSport()			{	return(m_szSport);			};
	CTrainingTask*		GetTrainingTask()	{	return(m_lpTrainingTask);	};
	CTrainingSession*	GetChild()			{	return(m_lpChild);			};
	bool HasSport()							{	if(m_szSport.length())	return(true); return(false);	};
	bool HasDesc()							{	if(m_szDesc.length())	return(true); return(false);	};
	CTrainingSession* Find(int iSession)
	{
		if(m_iNum == iSession)
			return(this);
		if(m_lpChild)
			return(m_lpChild->Find(iSession));
		return(0);
	};
protected:
	int					m_iNum;
	QString				m_szDesc;
	QString				m_szSport;
	CTrainingTask*		m_lpTrainingTask;
	CTrainingSession*	m_lpChild;
};

class CTrainingDay
{
public:
	CTrainingDay(int iNum, const QString& szDesc, CTrainingSession* lpTrainingSession = 0, CTrainingDay* lpChild = 0)
	{
		m_szDesc			= "";
		m_lpTrainingSession	= 0;
		m_lpChild			= 0;
		Set(iNum, szDesc, lpTrainingSession, lpChild);
	};
	~CTrainingDay()
	{
		DELETE(m_lpTrainingSession);
		DELETE(m_lpChild);
	};
	CTrainingDay* Add(int iNum, const QString& szDesc = "", CTrainingSession* lpTrainingSession = 0, CTrainingDay* lpChild = 0)
	{
		if(!m_lpChild)
			m_lpChild = new CTrainingDay(iNum, szDesc, lpTrainingSession, lpChild);
		else
			m_lpChild->Set(iNum, szDesc, lpTrainingSession, lpChild);
		return(m_lpChild);
	};
	CTrainingSession* AddTrainingSession(int iNum, const QString& szDesc = "", const QString& szSport = "", CTrainingTask* lpTrainingTask = 0, CTrainingSession* lpChild = 0)
	{
		CTrainingSession* lpTrainingSessionDummy = m_lpTrainingSession;
		if(!m_lpTrainingSession)
			m_lpTrainingSession = lpTrainingSessionDummy = new CTrainingSession(iNum, szDesc, szSport, lpTrainingTask, lpChild);
		else
		{
			while(lpTrainingSessionDummy->GetChild())
				lpTrainingSessionDummy = lpTrainingSessionDummy->GetChild();
			lpTrainingSessionDummy = lpTrainingSessionDummy->Add(iNum, szDesc, szSport, lpTrainingTask, lpChild);
		}
		return(lpTrainingSessionDummy);
	};
	CTrainingSession* AddTrainingSession(CTrainingSession* lpTrainingSession)
	{
		CTrainingSession* lpTrainingSessionDummy = m_lpTrainingSession;
		if(!m_lpTrainingSession)
			m_lpTrainingSession = lpTrainingSessionDummy = lpTrainingSession;
		else
		{
			int		iNum	= 0;
			while(lpTrainingSessionDummy->GetChild())
			{
				iNum++;
				lpTrainingSessionDummy->SetNum(iNum);
				lpTrainingSessionDummy = lpTrainingSessionDummy->GetChild();
			}
			lpTrainingSessionDummy->SetChild(lpTrainingSession);
			if(!iNum)
				iNum = 1;
			while(lpTrainingSessionDummy->GetChild())
			{
				iNum++;
				lpTrainingSessionDummy->SetNum(iNum);
				lpTrainingSessionDummy = lpTrainingSessionDummy->GetChild();
			}
		}
		return(lpTrainingSession);
	};
	void DeleteTrainingSession(CTrainingSession* lpTrainingSession)
	{
		CTrainingSession*	lpTemp;
		CTrainingSession*	lpNext	= lpTrainingSession->GetChild();
		if(m_lpTrainingSession == lpTrainingSession)
		{
			m_lpTrainingSession = lpNext;
		}
		else
		{
			lpTemp = m_lpTrainingSession;
			while(lpTemp->GetChild())
			{
				if(lpTemp->GetChild() == lpTrainingSession)
				{
					lpTemp->SetChild(lpNext);
					break;
				}
				lpTemp = lpTemp->GetChild();
			}
		}
	};
	void SetNum(int iNum)											{	m_iNum				= iNum;					};
	void SetDesc(const QString& szDesc)								{	m_szDesc			= szDesc;				};
	void SetTrainingSession(CTrainingSession* lpTrainingSession)	{	m_lpTrainingSession	= lpTrainingSession;	};
	void SetChild(CTrainingDay* lpChild)							{	m_lpChild			= lpChild;				};
	void Set(int iNum, const QString& szDesc, CTrainingSession* lpTrainingSession = 0, CTrainingDay* lpChild = 0)
	{
		SetNum(iNum);
		SetDesc(szDesc);
		SetTrainingSession(lpTrainingSession);
		SetChild(lpChild);
	};
	int					GetNum()				{	return(m_iNum);					};
	QString				GetDesc()				{	return(m_szDesc);				};
	CTrainingSession*	GetTrainingSession()	{	return(m_lpTrainingSession);	};
	CTrainingDay*		GetChild()				{	return(m_lpChild);				};
	bool				HasDesc()				{	if(m_szDesc.length()) return(true); return(false);	};
	CTrainingDay* Find(int iDay)
	{
		if(m_iNum == iDay)
			return(this);
		if(m_lpChild)
			return(m_lpChild->Find(iDay));
		return(0);
	};
protected:
	int					m_iNum;
	QString				m_szDesc;
	CTrainingSession*	m_lpTrainingSession;
	CTrainingDay*		m_lpChild;
};

class CTrainingPlan
{
public:
	CTrainingPlan(const QString& szName, const QString& szDesc = "", CTrainingDay* lpTrainingDay = 0, CTrainingPlan* lpChild = 0)
	{
		m_szName			= szName;
		m_szDesc			= szDesc;
		m_lpTrainingDay		= lpTrainingDay;
		m_lpChild			= lpChild;

		m_TrainingStart		= QDate::currentDate();
	};
	~CTrainingPlan()
	{
		DELETE(m_lpTrainingDay);
		DELETE(m_lpChild);
	};
	CTrainingDay* AddDay(int iNum, const QString& szDesc = "", CTrainingSession* lpTrainingSession = 0, CTrainingDay* lpChild = 0)
	{
		CTrainingDay* lpTrainingDayDummy = m_lpTrainingDay;
		if(!m_lpTrainingDay)
			m_lpTrainingDay = lpTrainingDayDummy = new CTrainingDay(iNum, szDesc, lpTrainingSession, lpChild);
		else
		{
			while(lpTrainingDayDummy->GetChild())
				lpTrainingDayDummy = lpTrainingDayDummy->GetChild();
			lpTrainingDayDummy = lpTrainingDayDummy->Add(iNum, szDesc, lpTrainingSession, lpChild);
		}
		return(lpTrainingDayDummy);
	};
	void SetName(const QString& szName)					{	m_szName			= szName;			};
	void SetDesc(const QString& szDesc)					{	m_szDesc			= szDesc;			};
	void SetTrainingDay(CTrainingDay* lpTrainingDay)	{	m_lpTrainingDay		= lpTrainingDay;	};
	void SetChild(CTrainingPlan* lpChild)				{	m_lpChild			= lpChild;			};
	void SetTrainingStart(const QDate& TrainingStart)	{	m_TrainingStart		= TrainingStart;	};
	void Set(const QString& szName, const QString& szDesc = "", CTrainingDay* lpTrainingDay = 0, CTrainingPlan* lpChild = 0)
	{
		SetName(szName);
		SetDesc(szDesc);
		SetTrainingDay(lpTrainingDay);
		SetChild(lpChild);
	};
	QString		GetName()				{	return(m_szName);							};
	QString		GetDesc()				{	return(m_szDesc);							};
	CTrainingDay*	GetTrainingDay()	{	return(m_lpTrainingDay);					};
	CTrainingPlan*	GetChild()			{	return(m_lpChild);							};
	QDate&			GetTrainingStart()	{	return(m_TrainingStart);					};
	bool HasName()						{	if(m_szName.length()) return(true); return(false);	};
	bool HasDesc()						{	if(m_szDesc.length()) return(true); return(false);	};
	void DeleteTrainingDay(int iDay)
	{
		CTrainingDay*	lpNext;
		CTrainingDay*	lpPrev	= m_lpTrainingDay;
		if(lpPrev->GetNum() == iDay)
		{
			lpNext	= lpPrev->GetChild();
			lpPrev->SetChild(0);
			delete lpPrev;
			m_lpTrainingDay = lpNext;
		}
		else
		{
			lpNext	= lpPrev->GetChild();
			while(lpNext)
			{
				if(lpNext->GetNum() == iDay)
				{
					lpPrev->SetChild(lpNext->GetChild());
					lpNext->SetChild(0);
					delete lpNext;
					break;
				}
				lpPrev = lpNext;
				lpNext = lpNext->GetChild();
			}
		}
	};
protected:
	QString			m_szName;
	QString			m_szDesc;
	CTrainingDay*	m_lpTrainingDay;
	CTrainingPlan*	m_lpChild;
	QDate			m_TrainingStart;
};


class CTraining
{
public:
	CTraining();
	CTraining(const QString& szFileName);
	~CTraining();

	bool Load(const QString& szFileName);
	bool Save(const QString& szFileName);
	void Recalculate();

	void ParsePerson(const QDomElement& Element);
	CSport* ParseSport(const QDomElement& Element, CSport* lpSportDummy);
	CTrainingDay* ParseDay(const QDomElement& Element, CTrainingDay* lpTrainingDayDummy);
	CTrainingSession* ParseSession(const QDomElement& Element, CTrainingDay* lpTrainingDay, CTrainingSession* lpTrainingSessionDummy);
	CTrainingTask* ParseTask(const QDomElement& Element, CTrainingSession* lpTrainingSession, CTrainingTask* lpTrainingTaskDummy);

	bool GetPerson(QString& szFirstName, QString& szLastName, QDate& DayOfBirth, uint32_t& dwHeight, uint32_t& dwWeight);

	bool GetFirstSport(QString& szName, uint32_t& dwMaxHR, bool& bPace);
	bool GetNextSport(QString& szName, uint32_t& dwMaxHR, bool& bPace);

	bool GetFirstHR(QString& szName, QString& szDesc, uint32_t& dwMin, uint32_t& dwMax);
	bool GetNextHR(QString& szName, QString& szDesc, uint32_t& dwMin, uint32_t& dwMax);

	bool GetFirstTraining(QString& szName, QString& szDesc, QDate& TrainingStart);
	bool GetNextTraining(QString& szName, QString& szDesc, QDate& TrainingStart);

	bool GetFirstDay(int& iNum, QString& szDesc);
	bool GetNextDay(int& iNum, QString& szDesc);

	bool GetFirstSession(int& iNum, QString& szDesc, QString& szSport);
	bool GetNextSession(int& iNum, QString& szDesc, QString& szSport);

	bool GetFirstTask(int& iNum, QString& szDesc, uint32_t& dwCount, uint32_t& dwTime, uint32_t& dwDist, QString& szHR, uint32_t& dwPace, uint32_t& dwPauseTime, uint32_t& dwPauseDist, QString& szPauseHR, uint32_t& dwPausePace);
	bool GetNextTask(int& iNum, QString& szDesc, uint32_t& dwCount, uint32_t& dwTime, uint32_t& dwDist, QString& szHR, uint32_t& dwPace, uint32_t& dwPauseTime, uint32_t& dwPauseDist, QString& szPauseHR, uint32_t& dwPausePace);

	void SetTraining(QString szTraining, QDate TrainingStart);
	void SetPerson(QString szFirstName, QString szLastName, QDate DayOfBirth, uint32_t dwHeight, uint32_t dwWeight);
	void SetDay(int iDay, int iNewDay, QString szNewDesc);
	void SetSession(int iDay, int iNewDay, int iSession, QString szDesc, QString szSport);

	void AddSport(QString szName, uint32_t dwMaxHR, bool bPace);
	void AddHR(QString szSport, QString szName, QString szDesc, uint32_t dwMin, uint32_t dwMax);
	void AddDay(int iDay, QString szDesc);
	void AddSession(int iDay, int iSession, QString szDesc, QString szSport);
	void AddTask(int iDay, int iSession, int iTask, QString szDesc, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, QString szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, QString szPauseHR, uint32_t dwPausePace);
protected:
	QString				m_szFileName;
	CPerson*			m_lpPerson;
	CSport*				m_lpSport;
	CSport*				m_lpCurSport;
	CHR*				m_lpCurHR;
	CTrainingPlan*		m_lpTrainingPlan;
	CTrainingPlan*		m_lpCurTrainingPlan;
	CTrainingDay*		m_lpCurTrainingDay;
	CTrainingSession*	m_lpCurTrainingSession;
	CTrainingTask*		m_lpCurTrainingTask;

	void Clean();
	void CleanPerson();
	void CleanSport();
	void CleanTrainingPlan();
private:
};

#endif // CTRAINING_H

#include "ctraining.h"
#include "common.h"


CTraining::CTraining()
{
	m_lpPerson				= 0;
	m_lpSport				= 0;
	m_lpTrainingPlan		= 0;

	m_lpCurSport			= 0;
	m_lpCurHR				= 0;
	m_lpCurTrainingPlan		= 0;
	m_lpCurTrainingDay		= 0;
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;
}

CTraining::CTraining(const QString& szFileName)
{
	m_lpPerson				= 0;
	m_lpSport				= 0;
	m_lpTrainingPlan		= 0;

	m_lpCurSport			= 0;
	m_lpCurHR				= 0;
	m_lpCurTrainingPlan		= 0;
	m_lpCurTrainingDay		= 0;
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;

	Load(szFileName);
}

CTraining::~CTraining()
{
	Clean();
}

void CTraining::Clean()
{
	CleanPerson();
	CleanSport();
	CleanTrainingPlan();
}

void CTraining::CleanPerson()
{
	DELETE(m_lpPerson);
}

void CTraining::CleanSport()
{
	DELETE(m_lpSport);
}

void CTraining::CleanTrainingPlan()
{
	DELETE(m_lpTrainingPlan);
}

void CTraining::ParsePerson(const QDomElement& Element)
{
	QDomNode	Child	= Element.firstChild();
	while(!Child.isNull())
	{
		if(Child.toElement().tagName().toLower() == "firstname")
		{
			if(!m_lpPerson)
				m_lpPerson = new CPerson;
			m_lpPerson->SetFirstName(Child.toElement().text());
		}
		else if(Child.toElement().tagName().toLower() == "lastname")
		{
			if(!m_lpPerson)
				m_lpPerson = new CPerson;
			m_lpPerson->SetLastName(Child.toElement().text());
		}
		else if(Child.toElement().tagName().toLower() == "dayofbirth")
		{
			if(!m_lpPerson)
				m_lpPerson = new CPerson;
			m_lpPerson->SetDayOfBirth(QDate::fromString(Child.toElement().text(), "yyyyMMdd"));
		}
		else if(Child.toElement().tagName().toLower() == "height")
		{
			if(!m_lpPerson)
				m_lpPerson = new CPerson;
			m_lpPerson->SetHeight(Child.toElement().text().toInt());
		}
		else if(Child.toElement().tagName().toLower() == "weight")
		{
			if(!m_lpPerson)
				m_lpPerson = new CPerson;
			m_lpPerson->SetWeight(Child.toElement().text().toInt());
		}

		Child	= Child.nextSibling();
	}
}

CSport* CTraining::ParseSport(const QDomElement& Element, CSport* lpSportDummy)
{
	QDomNode			Child	= Element.firstChild();
	QDomNode			Child1;
	CHR*				lpHRDummy;

	if(!m_lpSport)
		m_lpSport = lpSportDummy = new CSport(Element.attribute("name"));
	else
		lpSportDummy = lpSportDummy->Add(Element.attribute("name"));

	while(!Child.isNull())
	{
		if(Child.toElement().tagName().toLower() == "maxhr")
			lpSportDummy->SetMaxHR(Child.toElement().text().toInt());
		else if(Child.toElement().tagName().toLower() == "hr")
		{
			lpHRDummy = lpSportDummy->AddHR(Child.toElement().attribute("name"));
			Child1 = Child.toElement().firstChild();
			while(!Child1.isNull())
			{
				if(Child1.toElement().tagName().toLower() == "desc")
					lpHRDummy->SetDesc(Child1.toElement().text());
				else if(Child1.toElement().tagName().toLower() == "min")
					lpHRDummy->SetMin(Child1.toElement().text().toInt());
				else if(Child1.toElement().tagName().toLower() == "max")
					lpHRDummy->SetMax(Child1.toElement().text().toInt());
				Child1 = Child1.nextSibling();
			}
		}
		else if(Child.toElement().tagName().toLower() == "pace")
		{
			if(Child.toElement().text().toLower() == "true")
				lpSportDummy->SetPace(true);
			else
				lpSportDummy->SetPace(false);
		}
		Child	= Child.nextSibling();
	}
	return(lpSportDummy);
}

CTrainingDay* CTraining::ParseDay(const QDomElement& Element, CTrainingDay* lpTrainingDayDummy)
{
	QDomNode				Child	= Element.firstChild();
	int						iNum;
	CTrainingSession*		lpTrainingSessionDummy = 0;

	iNum				= Element.attribute("num").toInt();
	if(iNum != -1)
	{
		lpTrainingDayDummy = m_lpTrainingPlan->AddDay(iNum);

		while(!Child.isNull())
		{
			if(Child.toElement().tagName().toLower() == "desc")
				lpTrainingDayDummy->SetDesc(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "session")
				lpTrainingSessionDummy = ParseSession(Child.toElement(), lpTrainingDayDummy, lpTrainingSessionDummy);

			Child	= Child.nextSibling();
		}
	}
	return(lpTrainingDayDummy);
}

CTrainingSession* CTraining::ParseSession(const QDomElement& Element, CTrainingDay* lpTrainingDay, CTrainingSession* lpTrainingSessionDummy)
{
	QDomNode			Child	= Element.firstChild();
	int					iNum;
	CTrainingTask*		lpTrainingTaskDummy = 0;

	iNum				= Element.attribute("num").toInt();
	if(iNum != -1)
	{
		lpTrainingSessionDummy = lpTrainingDay->AddTrainingSession(iNum);

		while(!Child.isNull())
		{
			if(Child.toElement().tagName().toLower() == "desc")
				lpTrainingSessionDummy->SetDesc(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "sport")
				lpTrainingSessionDummy->SetSport(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "task")
				lpTrainingTaskDummy = ParseTask(Child.toElement(), lpTrainingSessionDummy, lpTrainingTaskDummy);

			Child	= Child.nextSibling();
		}
	}
	return(lpTrainingSessionDummy);
}

CTrainingTask* CTraining::ParseTask(const QDomElement& Element, CTrainingSession* lpTrainingSession, CTrainingTask* lpTrainingTaskDummy)
{
	QDomNode				Child	= Element.firstChild();
	int						iNum;

	iNum			= Element.attribute("num").toInt();
	if(iNum != -1)
	{
		lpTrainingTaskDummy = lpTrainingSession->AddTask(iNum);

		while(!Child.isNull())
		{
			if(Child.toElement().tagName().toLower() == "desc")
				lpTrainingTaskDummy->SetDesc(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "count")
				lpTrainingTaskDummy->SetCount(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "time")
				lpTrainingTaskDummy->SetTime(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "dist")
				lpTrainingTaskDummy->SetDist(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "hr")
				lpTrainingTaskDummy->SetHR(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "pace")
				lpTrainingTaskDummy->SetPace(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "pausetime")
				lpTrainingTaskDummy->SetPauseTime(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "pausedist")
				lpTrainingTaskDummy->SetPauseDist(Child.toElement().text().toInt());
			else if(Child.toElement().tagName().toLower() == "pausehr")
				lpTrainingTaskDummy->SetPauseHR(Child.toElement().text());
			else if(Child.toElement().tagName().toLower() == "pausepace")
				lpTrainingTaskDummy->SetPausePace(Child.toElement().text().toInt());

			Child	= Child.nextSibling();
		}
	}
	return(lpTrainingTaskDummy);
}

bool CTraining::Load(const QString& szFileName)
{
	Clean();
	m_szFileName		= szFileName;

	QFile				File(szFileName);
	if(!File.open(QFile::ReadOnly | QFile::Text))
		return(false);

	QDomDocument		Doc;
	QString				errorStr;
	int					errorLine;
	int					errorColumn;
	if(!Doc.setContent(&File, false, &errorStr, &errorLine, &errorColumn))
		return(false);

	QDomElement			Root	= Doc.documentElement();
	if(Root.tagName().toLower() != "trainingplan")
		return(false);

	CTrainingDay*		lpTrainingDayDummy	= 0;
	CSport*				lpSportDummy		= 0;
	QDomNode			Child				= Root.firstChild();

	while(!Child.isNull())
	{
		if(Child.toElement().tagName().toLower() == "person")
			ParsePerson(Child.toElement());
		else if(Child.toElement().tagName().toLower() == "sport")
			lpSportDummy = ParseSport(Child.toElement(), lpSportDummy);
		else if(Child.toElement().tagName().toLower() == "name")
		{
			if(!m_lpTrainingPlan)
				m_lpTrainingPlan = new CTrainingPlan(Child.toElement().text());
		}
		else if(Child.toElement().tagName().toLower() == "startdate")
		{
			if(m_lpTrainingPlan)
				m_lpTrainingPlan->SetTrainingStart(QDate::fromString(Child.toElement().text(), "yyyyMMdd"));
		}
		else if(Child.toElement().tagName().toLower() == "desc")
		{
			if(m_lpTrainingPlan)
				m_lpTrainingPlan->SetDesc(Child.toElement().text());
		}
		else if(Child.toElement().tagName().toLower() == "day")
			lpTrainingDayDummy = ParseDay(Child.toElement(), lpTrainingDayDummy);

		Child	= Child.nextSibling();
	}

	Recalculate();
	return(true);
}

bool CTraining::Save(const QString& szFileName)
{
	QFile	File(szFileName);
	if(!File.open(QFile::WriteOnly | QFile::Text))
		return(false);

	QXmlStreamWriter xmlWriter(&File);
	xmlWriter.setCodec("UTF-8");

	xmlWriter.setAutoFormatting(true);
	xmlWriter.setAutoFormattingIndent(-1);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("TrainingPlan");

	if(m_lpPerson)
	{
		xmlWriter.writeStartElement("Person");
		if(m_lpPerson->GetFirstName().length())
			xmlWriter.writeTextElement("FirstName", m_lpPerson->GetFirstName());
		if(m_lpPerson->GetLastName().length())
			xmlWriter.writeTextElement("LastName", m_lpPerson->GetLastName());
		if(m_lpPerson->GetDayOfBirth().year())
			xmlWriter.writeTextElement("DayOfBirth", m_lpPerson->GetDayOfBirth().toString("yyyyMMdd"));
		if(m_lpPerson->GetHeight())
			xmlWriter.writeTextElement("Height", QString("%1").arg(m_lpPerson->GetHeight()));
		if(m_lpPerson->GetWeight())
			xmlWriter.writeTextElement("Weight", QString("%1").arg(m_lpPerson->GetWeight()));
		xmlWriter.writeEndElement();
	}

	CSport*	lpSport = m_lpSport;
	while(lpSport)
	{
		xmlWriter.writeStartElement("Sport");
		xmlWriter.writeAttribute("name", lpSport->GetName());
		xmlWriter.writeTextElement("Pace", lpSport->GetPace() ? "true" : "false");
		if(lpSport->GetMaxHR())
			xmlWriter.writeTextElement("MaxHR", QString("%1").arg(lpSport->GetMaxHR()));

		CHR*	lpHR	= lpSport->GetHR();
		while(lpHR)
		{
			xmlWriter.writeStartElement("HR");
			xmlWriter.writeAttribute("name", lpHR->GetName());
			if(lpHR->GetDesc().length())
				xmlWriter.writeTextElement("Desc", lpHR->GetDesc());
			xmlWriter.writeTextElement("Min", QString("%1").arg(lpHR->GetMin()));
			xmlWriter.writeTextElement("Max", QString("%1").arg(lpHR->GetMax()));
			xmlWriter.writeEndElement();
			lpHR = lpHR->GetChild();
		}
		xmlWriter.writeEndElement();
		lpSport = lpSport->GetChild();
	}

	if(m_lpTrainingPlan)
	{
		xmlWriter.writeTextElement("Name", m_lpTrainingPlan->GetName());
		if(m_lpTrainingPlan->HasDesc())
			xmlWriter.writeTextElement("Desc", m_lpTrainingPlan->GetDesc());
		if(m_lpTrainingPlan->GetTrainingStart().year())
			xmlWriter.writeTextElement("StartDate", m_lpTrainingPlan->GetTrainingStart().toString("yyyyMMdd"));
		CTrainingDay*	lpDay	= m_lpTrainingPlan->GetTrainingDay();
		while(lpDay)
		{
			xmlWriter.writeStartElement("Day");
			xmlWriter.writeAttribute("num", QString("%1").arg(lpDay->GetNum()));
			if(lpDay->HasDesc())
				xmlWriter.writeTextElement("Desc", lpDay->GetDesc());
			CTrainingSession* lpSession = lpDay->GetTrainingSession();
			while(lpSession)
			{
				xmlWriter.writeStartElement("Session");
				xmlWriter.writeAttribute("num", QString("%1").arg(lpSession->GetNum()));
				if(lpSession->HasSport())
					xmlWriter.writeTextElement("Sport", lpSession->GetSport());
				if(lpSession->HasDesc())
					xmlWriter.writeTextElement("Desc", lpSession->GetDesc());
				CTrainingTask* lpTask = lpSession->GetTrainingTask();
				while(lpTask)
				{
					xmlWriter.writeStartElement("Task");
					xmlWriter.writeAttribute("num", QString("%1").arg(lpTask->GetNum()));
					if(lpTask->HasDesc())
						xmlWriter.writeTextElement("Desc", lpTask->GetDesc());
					xmlWriter.writeTextElement("Count", QString("%1").arg(lpTask->GetCount()));
					if(lpTask->GetDist())
						xmlWriter.writeTextElement("Dist", QString("%1").arg(lpTask->GetDist()));
					if(lpTask->HasHR())
						xmlWriter.writeTextElement("HR", lpTask->GetHR());
					if(lpTask->GetPace())
						xmlWriter.writeTextElement("Pace", QString("%1").arg(lpTask->GetPace()));
					if(lpTask->GetTime())
						xmlWriter.writeTextElement("Time", QString("%1").arg(lpTask->GetTime()));
					if(lpTask->GetCount() > 1)
					{
						if(lpTask->GetPauseDist())
							xmlWriter.writeTextElement("PauseDist", QString("%1").arg(lpTask->GetPauseDist()));
						if(lpTask->HasPauseHR())
							xmlWriter.writeTextElement("PauseHR", lpTask->GetPauseHR());
						if(lpTask->GetPausePace())
							xmlWriter.writeTextElement("PausePace", QString("%1").arg(lpTask->GetPausePace()));
						if(lpTask->GetPauseTime())
							xmlWriter.writeTextElement("PauseTime", QString("%1").arg(lpTask->GetPauseTime()));
					}
					xmlWriter.writeEndElement();
					lpTask = lpTask->GetChild();
				}
				xmlWriter.writeEndElement();
				lpSession = lpSession->GetChild();
			}
			xmlWriter.writeEndElement();
			lpDay = lpDay->GetChild();
		}
	}

	xmlWriter.writeEndDocument();
	File.close();
	if(File.error())
		return(false);
	return(true);
}

void CTraining::Recalculate()
{
	CTrainingDay*		lpTrainingDayDummy;
	CTrainingSession*	lpTrainingSessionDummy;
	CTrainingTask*		lpTrainingTaskDummy;


	if(m_lpTrainingPlan)
	{
		lpTrainingDayDummy = m_lpTrainingPlan->GetTrainingDay();
		while(lpTrainingDayDummy)
		{
			lpTrainingSessionDummy = lpTrainingDayDummy->GetTrainingSession();
			while(lpTrainingSessionDummy)
			{
				lpTrainingTaskDummy = lpTrainingSessionDummy->GetTrainingTask();
				while(lpTrainingTaskDummy)
				{
					if(!lpTrainingTaskDummy->GetCount())
						lpTrainingTaskDummy->SetCount(1);

					if(lpTrainingTaskDummy->GetTime() && lpTrainingTaskDummy->GetDist())
						lpTrainingTaskDummy->CalcPace();
					else if(lpTrainingTaskDummy->GetDist() && lpTrainingTaskDummy->GetPace())
						lpTrainingTaskDummy->CalcTime();
					else if(lpTrainingTaskDummy->GetTime() && lpTrainingTaskDummy->GetPace())
						lpTrainingTaskDummy->CalcDist();

					if(lpTrainingTaskDummy->GetCount() > 1)
					{
						if(lpTrainingTaskDummy->GetPauseTime() && lpTrainingTaskDummy->GetPauseDist())
							lpTrainingTaskDummy->CalcPausePace();
						else if(lpTrainingTaskDummy->GetPauseDist() && lpTrainingTaskDummy->GetPausePace())
							lpTrainingTaskDummy->CalcPauseTime();
						else if(lpTrainingTaskDummy->GetPauseTime() && lpTrainingTaskDummy->GetPausePace())
							lpTrainingTaskDummy->CalcPauseDist();
					}
					else
					{
						lpTrainingTaskDummy->SetPauseTime(0);
						lpTrainingTaskDummy->SetPauseDist(0);
						lpTrainingTaskDummy->SetPausePace(0);
					}

					lpTrainingTaskDummy = lpTrainingTaskDummy->GetChild();
				}
				lpTrainingSessionDummy = lpTrainingSessionDummy->GetChild();
			}
			lpTrainingDayDummy = lpTrainingDayDummy->GetChild();
		}
	}
}

bool CTraining::GetPerson(QString& szFirstName, QString& szLastName, QDate& DayOfBirth, uint32_t& dwHeight, uint32_t& dwWeight)
{
	if(!m_lpPerson)
		return(false);

	szFirstName		= m_lpPerson->GetFirstName();
	szLastName		= m_lpPerson->GetLastName();
	DayOfBirth		= m_lpPerson->GetDayOfBirth();
	dwHeight		= m_lpPerson->GetHeight();
	dwWeight		= m_lpPerson->GetWeight();

	return(true);
}

bool CTraining::GetFirstSport(QString& szName, uint32_t& dwMaxHR, bool& bPace)
{
	m_lpCurHR		= 0;
	m_lpCurSport	= m_lpSport;
	if(!m_lpCurSport)
		return(false);

	szName	= m_lpCurSport->GetName();
	dwMaxHR = m_lpCurSport->GetMaxHR();
	bPace	= m_lpCurSport->GetPace();

	return(true);
}

bool CTraining::GetNextSport(QString& szName, uint32_t& dwMaxHR, bool& bPace)
{
	m_lpCurHR		= 0;
	if(!m_lpCurSport)
		return(false);
	m_lpCurSport = m_lpCurSport->GetChild();
	if(!m_lpCurSport)
		return(false);

	szName	= m_lpCurSport->GetName();
	dwMaxHR = m_lpCurSport->GetMaxHR();
	bPace	= m_lpCurSport->GetPace();

	return(true);
}

bool CTraining::GetFirstHR(QString& szName, QString& szDesc, uint32_t& dwMin, uint32_t& dwMax)
{
	if(!m_lpCurSport)
		return(false);
	m_lpCurHR = m_lpCurSport->GetHR();
	if(!m_lpCurHR)
		return(false);

	szName	= m_lpCurHR->GetName();
	szDesc	= m_lpCurHR->GetDesc();
	dwMin	= m_lpCurHR->GetMin();
	dwMax	= m_lpCurHR->GetMax();

	return(true);
}

bool CTraining::GetNextHR(QString& szName, QString& szDesc, uint32_t& dwMin, uint32_t& dwMax)
{
	if(!m_lpCurHR)
		return(false);
	m_lpCurHR = m_lpCurHR->GetChild();
	if(!m_lpCurHR)
		return(false);

	szName	= m_lpCurHR->GetName();
	szDesc	= m_lpCurHR->GetDesc();
	dwMin	= m_lpCurHR->GetMin();
	dwMax	= m_lpCurHR->GetMax();

	return(true);
}

bool CTraining::GetFirstTraining(QString& szName, QString& szDesc, QDate& TrainingStart)
{
	m_lpCurTrainingDay		= 0;
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;
	m_lpCurTrainingPlan 	= m_lpTrainingPlan;
	if(!m_lpCurTrainingPlan)
		return(false);

	szName			= m_lpCurTrainingPlan->GetName();
	szDesc			= m_lpCurTrainingPlan->GetDesc();
	TrainingStart	= m_lpCurTrainingPlan->GetTrainingStart();
	return(true);
}

bool CTraining::GetNextTraining(QString& szName, QString& szDesc, QDate& TrainingStart)
{
	m_lpCurTrainingDay		= 0;
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;
	szName					= szName;
	szDesc					= szDesc;
	TrainingStart			= TrainingStart;
	return(false);
}

bool CTraining::GetFirstDay(int& iNum, QString& szDesc)
{
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;
	if(!m_lpCurTrainingPlan)
		return(false);
	m_lpCurTrainingDay = m_lpCurTrainingPlan->GetTrainingDay();
	if(!m_lpCurTrainingDay)
		return(false);

	iNum	= m_lpCurTrainingDay->GetNum();
	szDesc	= m_lpCurTrainingDay->GetDesc();

	return(true);
}

bool CTraining::GetNextDay(int& iNum, QString& szDesc)
{
	m_lpCurTrainingSession	= 0;
	m_lpCurTrainingTask		= 0;
	if(!m_lpCurTrainingDay)
		return(false);
	m_lpCurTrainingDay = m_lpCurTrainingDay->GetChild();
	if(!m_lpCurTrainingDay)
		return(false);

	iNum	= m_lpCurTrainingDay->GetNum();
	szDesc	= m_lpCurTrainingDay->GetDesc();

	return(true);
}

bool CTraining::GetFirstSession(int& iNum, QString& szDesc, QString& szSport)
{
	m_lpCurTrainingTask		= 0;
	if(!m_lpCurTrainingDay)
		return(false);
	m_lpCurTrainingSession = m_lpCurTrainingDay->GetTrainingSession();
	if(!m_lpCurTrainingSession)
		return(false);

	iNum	= m_lpCurTrainingSession->GetNum();
	szDesc	= m_lpCurTrainingSession->GetDesc();
	szSport	= m_lpCurTrainingSession->GetSport();

	return(true);
}

bool CTraining::GetNextSession(int& iNum, QString& szDesc, QString& szSport)
{
	m_lpCurTrainingTask		= 0;
	if(!m_lpCurTrainingSession)
		return(false);
	m_lpCurTrainingSession = m_lpCurTrainingSession->GetChild();
	if(!m_lpCurTrainingSession)
		return(false);

	iNum	= m_lpCurTrainingSession->GetNum();
	szDesc	= m_lpCurTrainingSession->GetDesc();
	szSport	= m_lpCurTrainingSession->GetSport();

	return(true);
}

bool CTraining::GetFirstTask(int& iNum, QString& szDesc, uint32_t& dwCount, uint32_t& dwTime, uint32_t& dwDist, QString& szHR, uint32_t& dwPace, uint32_t& dwPauseTime, uint32_t& dwPauseDist, QString& szPauseHR, uint32_t& dwPausePace)
{
	if(!m_lpCurTrainingSession)
		return(false);
	m_lpCurTrainingTask = m_lpCurTrainingSession->GetTrainingTask();
	if(!m_lpCurTrainingTask)
		return(false);

	iNum		= m_lpCurTrainingTask->GetNum();
	szDesc		= m_lpCurTrainingTask->GetDesc();
	dwCount		= m_lpCurTrainingTask->GetCount();
	dwTime		= m_lpCurTrainingTask->GetTime();
	dwDist		= m_lpCurTrainingTask->GetDist();
	szHR		= m_lpCurTrainingTask->GetHR();
	dwPace		= m_lpCurTrainingTask->GetPace();
	dwPauseTime	= m_lpCurTrainingTask->GetPauseTime();
	dwPauseDist	= m_lpCurTrainingTask->GetPauseDist();
	szPauseHR	= m_lpCurTrainingTask->GetPauseHR();
	dwPausePace	= m_lpCurTrainingTask->GetPausePace();

	return(true);
}

bool CTraining::GetNextTask(int& iNum, QString& szDesc, uint32_t& dwCount, uint32_t& dwTime, uint32_t& dwDist, QString& szHR, uint32_t& dwPace, uint32_t& dwPauseTime, uint32_t& dwPauseDist, QString& szPauseHR, uint32_t& dwPausePace)
{
	if(!m_lpCurTrainingTask)
		return(false);
	m_lpCurTrainingTask = m_lpCurTrainingTask->GetChild();
	if(!m_lpCurTrainingTask)
		return(false);

	iNum		= m_lpCurTrainingTask->GetNum();
	szDesc		= m_lpCurTrainingTask->GetDesc();
	dwCount		= m_lpCurTrainingTask->GetCount();
	dwTime		= m_lpCurTrainingTask->GetTime();
	dwDist		= m_lpCurTrainingTask->GetDist();
	szHR		= m_lpCurTrainingTask->GetHR();
	dwPace		= m_lpCurTrainingTask->GetPace();
	dwPauseTime	= m_lpCurTrainingTask->GetPauseTime();
	dwPauseDist	= m_lpCurTrainingTask->GetPauseDist();
	szPauseHR	= m_lpCurTrainingTask->GetPauseHR();
	dwPausePace	= m_lpCurTrainingTask->GetPausePace();

	return(true);
}

void CTraining::SetTraining(QString szTraining, QDate TrainingStart)
{
	if(!m_lpTrainingPlan)
		m_lpTrainingPlan = new CTrainingPlan(szTraining);
	else
		m_lpTrainingPlan->SetName(szTraining);
	m_lpTrainingPlan->SetTrainingStart(TrainingStart);
}

void CTraining::SetPerson(QString szFirstName, QString szLastName, QDate DayOfBirth, uint32_t dwHeight, uint32_t dwWeight)
{
	DELETE(m_lpPerson);
	m_lpPerson	= new CPerson(szFirstName, szLastName, DayOfBirth, dwHeight, dwWeight);
}

void CTraining::SetDay(int iDay, int iNewDay, QString szNewDesc)
{
	CTrainingDay*	lpOldDay;
	CTrainingDay*	lpNewDay;

	if(!m_lpTrainingPlan)
		return;

	lpOldDay	= m_lpTrainingPlan->GetTrainingDay()->Find(iDay);
	lpNewDay	= m_lpTrainingPlan->GetTrainingDay()->Find(iNewDay);

	if(!lpNewDay)
	{
		lpOldDay->SetNum(iNewDay);
		lpOldDay->SetDesc(szNewDesc);
	}
	else
	{
		lpNewDay->AddTrainingSession(lpOldDay->GetTrainingSession());
		lpOldDay->SetTrainingSession(0);
		m_lpTrainingPlan->DeleteTrainingDay(iDay);
	}
}

void CTraining::SetSession(int iDay, int iNewDay, int iSession, QString szDesc, QString szSport)
{
	if(!m_lpTrainingPlan)
		return;

	CTrainingDay*		lpOldDay;
	CTrainingDay*		lpNewDay;
	CTrainingSession*	lpSession;

	lpOldDay	= m_lpTrainingPlan->GetTrainingDay()->Find(iDay);
	if(!lpOldDay)
		return;

	lpSession	= lpOldDay->GetTrainingSession()->Find(iSession);
	if(!lpSession)
		return;

	lpSession->SetDesc(szDesc);
	lpSession->SetSport(szSport);

	if(iDay == iNewDay)
		return;

	lpNewDay	= m_lpTrainingPlan->GetTrainingDay()->Find(iNewDay);
	if(!lpNewDay)
		return;

	lpNewDay->AddTrainingSession(lpSession);
	lpOldDay->DeleteTrainingSession(lpSession);
}

void CTraining::AddSport(QString szName, uint32_t dwMaxHR, bool bPace)
{
	if(!m_lpSport)
		m_lpSport = new CSport(szName, dwMaxHR, 0, bPace);
	else
		m_lpSport->Add(szName, dwMaxHR, 0, bPace);
}

void CTraining::AddHR(QString szSport, QString szName, QString szDesc, uint32_t dwMin, uint32_t dwMax)
{
	if(!m_lpSport)
		return;
	CSport* lpSport = m_lpSport->Find(szSport);
	if(!lpSport)
		return;
	lpSport->AddHR(szName, szDesc, dwMin, dwMax);
}

void CTraining::AddDay(int iDay, QString szDesc)
{
	QString	szDescTemp	= szDesc;

	if(szDescTemp.indexOf(")"))
		szDescTemp = szDescTemp.mid(szDescTemp.indexOf(")")+2);
	m_lpTrainingPlan->AddDay(iDay, szDescTemp);
}

void CTraining::AddSession(int iDay, int iSession, QString szDesc, QString szSport)
{
	CTrainingDay* lpDay = m_lpTrainingPlan->GetTrainingDay()->Find(iDay);
	if(!lpDay)
		return;
	lpDay->AddTrainingSession(iSession, szDesc, szSport);
}

void CTraining::AddTask(int iDay, int iSession, int iTask, QString szDesc, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, QString szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, QString szPauseHR, uint32_t dwPausePace)
{
	CTrainingDay* 		lpDay		= m_lpTrainingPlan->GetTrainingDay()->Find(iDay);
	if(!lpDay)
		return;
	CTrainingSession*	lpSession	= lpDay->GetTrainingSession()->Find(iSession);
	if(!lpSession)
		return;

	lpSession->AddTask(iTask, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace);
}

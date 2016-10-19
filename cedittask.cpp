#include "cedittask.h"
#include "ui_cedittask.h"
#include "common.h"
#include "ctime.h"

CEditTask::CEditTask(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CEditTask)
{
	m_bChanging = false;
	m_ui->setupUi(this);
	m_ui->m_lpGroupPause->setVisible(false);
}

CEditTask::~CEditTask()
{
    delete m_ui;
}

void CEditTask::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CEditTask::on_buttonBox_accepted()
{
	if(!m_ui->m_lpDesc->text().length())
	{
		MESSAGE(tr("Description is empty!"));
		return;
	}
	if(!m_ui->m_lpHR->currentText().length())
	{
		MESSAGE(tr("No heart rate selected!"));
		return;
	}
	if(m_ui->m_lpCount->value() > 1 && !m_ui->m_lpPauseHR->currentText().length())
	{
		MESSAGE(tr("No pause heart rate selected!"));
		return;
	}
	accept();
}

void CEditTask::on_buttonBox_rejected()
{
	reject();
}


void CEditTask::on_m_lpCount_valueChanged(int iCount)
{
	if(iCount > 1)
		m_ui->m_lpGroupPause->setVisible(true);
	else
		m_ui->m_lpGroupPause->setVisible(false);

	CalcTotal();
}

QString CEditTask::GetDesc()
{
	return(m_ui->m_lpDesc->text());
}

uint32_t CEditTask::GetCount()
{
	return(m_ui->m_lpCount->value());
}

CTime CEditTask::GetTime()
{
	return((CTime)m_ui->m_lpTime->time());
}

uint32_t CEditTask::GetDist()
{
	return(m_ui->m_lpDist->value());
}

QString CEditTask::GetHR()
{
	return(m_ui->m_lpHR->currentText());
}

uint32_t CEditTask::GetPace()
{
	QTime Time = m_ui->m_lpPace->time();
	return((uint32_t)Time.hour()*3600+(uint32_t)Time.minute()*60+(uint32_t)Time.second());
}

CTime CEditTask::GetPauseTime()
{
	return(m_ui->m_lpPauseTime->time());
}

uint32_t CEditTask::GetPauseDist()
{
	return(m_ui->m_lpPauseDist->value());
}

QString CEditTask::GetPauseHR()
{
	return(m_ui->m_lpPauseHR->currentText());
}

uint32_t CEditTask::GetPausePace()
{
	QTime Time = m_ui->m_lpPausePace->time();
	return((uint32_t)Time.hour()*3600+(uint32_t)Time.minute()*60+(uint32_t)Time.second());
}

void CEditTask::Set(const QStringList& HRList, const QString& szDesc, uint32_t dwCount, const QTime& Time, uint32_t dwDist, const QString& szHR, uint32_t dwPace, const QTime& PauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace)
{
	int	i;

	for(i = 0;i < HRList.size();i++)
	{
		m_ui->m_lpHR->addItem(HRList.at(i));
		m_ui->m_lpPauseHR->addItem(HRList.at(i));
	}

	m_ui->m_lpDesc->setText(szDesc);
	m_ui->m_lpCount->setValue(dwCount);
	m_ui->m_lpTime->setTime(Time);
	m_ui->m_lpDist->setValue(dwDist);
	m_ui->m_lpHR->setCurrentIndex(m_ui->m_lpHR->findText(szHR));
	m_ui->m_lpPace->setTime(QTime(HOURS(dwPace), MINUTES(dwPace), SECONDS(dwPace)));
	if(dwCount > 1)
	{
		m_ui->m_lpGroupPause->setVisible(true);
		m_ui->m_lpPauseTime->setTime(PauseTime);
		m_ui->m_lpPauseDist->setValue(dwPauseDist);
		m_ui->m_lpPauseHR->setCurrentIndex(m_ui->m_lpHR->findText(szPauseHR));
		m_ui->m_lpPausePace->setTime(QTime(HOURS(dwPausePace), MINUTES(dwPausePace), SECONDS(dwPausePace)));
	}

	CalcTotal();
}

void CEditTask::on_m_lpTime_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(time).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpDist->value();
	uint32_t	dwPace	= CTime(m_ui->m_lpPace->time()).toDWORD();

	if(!dwTime)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwPace = dwTime*1000/dwDist;
		m_ui->m_lpPace->setTime(QTime(HOURS(dwPace), MINUTES(dwPace), SECONDS(dwPace)));
	}
	else if(dwPace)
	{
		m_bChanging = true;
		dwDist = dwPace*dwTime*1000;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::on_m_lpDist_valueChanged(int dist)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpTime->time()).toDWORD();
	uint32_t	dwDist	= dist;
	uint32_t	dwPace	= CTime(m_ui->m_lpPace->time()).toDWORD();

	if(!dwDist)
		return;

	if(dwTime)
	{
		m_bChanging = true;
		dwPace = dwTime*1000/dwDist;
		m_ui->m_lpPace->setTime(QTime(HOURS(dwPace), MINUTES(dwPace), SECONDS(dwPace)));
	}
	else if(dwPace)
	{
		m_bChanging = true;
		dwDist = dwPace*dwTime*1000;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::on_m_lpPace_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpDist->value();
	uint32_t	dwPace	= CTime(time).toDWORD();

	if(!dwPace)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwTime = dwPace*dwDist/1000;
		m_ui->m_lpTime->setTime(QTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)));
	}
	else if(dwTime)
	{
		m_bChanging = true;
		dwDist = dwTime*dwPace*1000;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::on_m_lpPauseTime_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(time).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwPace	= CTime(m_ui->m_lpPausePace->time()).toDWORD();

	if(!dwTime)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwPace = dwTime*1000/dwDist;
		m_ui->m_lpPausePace->setTime(QTime(HOURS(dwPace), MINUTES(dwPace), SECONDS(dwPace)));
	}
	else if(dwPace)
	{
		m_bChanging = true;
		dwDist = dwPace*dwTime*1000;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::on_m_lpPauseDist_valueChanged(int dist)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpPauseTime->time()).toDWORD();
	uint32_t	dwDist	= dist;
	uint32_t	dwPace	= CTime(m_ui->m_lpPausePace->time()).toDWORD();

	if(!dwDist)
		return;

	if(dwTime)
	{
		m_bChanging = true;
		dwPace = dwTime*1000/dwDist;
		m_ui->m_lpPausePace->setTime(QTime(HOURS(dwPace), MINUTES(dwPace), SECONDS(dwPace)));
	}
	else if(dwPace)
	{
		m_bChanging = true;
		dwDist = dwPace*dwTime*1000;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::on_m_lpPausePace_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpPauseTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwPace	= CTime(time).toDWORD();

	if(!dwPace)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwTime = dwPace*dwDist/1000;
		m_ui->m_lpPauseTime->setTime(QTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)));
	}
	else if(dwTime)
	{
		m_bChanging = true;
		dwDist = dwTime*dwPace*1000;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTask::CalcTotal()
{
	int			iCount		= m_ui->m_lpCount->value();
	uint32_t	dwTime		= CTime(m_ui->m_lpTime->time()).toDWORD();
	uint32_t	dwDist		= m_ui->m_lpDist->value();
	uint32_t	dwPauseTime	= CTime(m_ui->m_lpPauseTime->time()).toDWORD();
	uint32_t	dwPauseDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwTotalTime;
	uint32_t	dwTotalDist;

	dwTotalTime	= dwTime*iCount;
	dwTotalDist	= dwDist*iCount;
	if(iCount > 1)
	{
		dwTotalTime	+= dwPauseTime*(iCount-1);
		dwTotalDist	+= dwPauseDist*(iCount-1);
	}
	m_ui->m_lpTotalDist->setText(QString(tr("%1")).arg(dwTotalDist));

	uint32_t h = HOURS(dwTotalTime);
	uint32_t m = MINUTES(dwTotalTime);
	uint32_t s = SECONDS(dwTotalTime);
	m_ui->m_lpTotalTime->setTime(QTime(h, m, s));
}

#include "cedittasks.h"
#include "ui_cedittasks.h"

CEditTaskS::CEditTaskS(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CEditTaskS)
{
	m_bChanging = false;
	m_ui->setupUi(this);
	m_ui->m_lpGroupPause->setVisible(false);
}

CEditTaskS::~CEditTaskS()
{
    delete m_ui;
}

void CEditTaskS::changeEvent(QEvent *e)
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

void CEditTaskS::on_buttonBox_accepted()
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

void CEditTaskS::on_buttonBox_rejected()
{
	reject();
}

void CEditTaskS::on_m_lpCount_valueChanged(int iCount)
{
	if(iCount > 1)
		m_ui->m_lpGroupPause->setVisible(true);
	else
		m_ui->m_lpGroupPause->setVisible(false);

	CalcTotal();
}

QString CEditTaskS::GetDesc()
{
	return(m_ui->m_lpDesc->text());
}

uint32_t CEditTaskS::GetCount()
{
	return(m_ui->m_lpCount->value());
}

CTime CEditTaskS::GetTime()
{
	return(m_ui->m_lpTime->time());
}

uint32_t CEditTaskS::GetDist()
{
	return(m_ui->m_lpDist->value());
}

QString CEditTaskS::GetHR()
{
	return(m_ui->m_lpHR->currentText());
}

uint32_t CEditTaskS::GetPace()
{
	if(!m_ui->m_lpSpeed->value())
		return(0);
	return(3600/m_ui->m_lpSpeed->value());
}

CTime CEditTaskS::GetPauseTime()
{
	return(m_ui->m_lpPauseTime->time());
}

uint32_t CEditTaskS::GetPauseDist()
{
	return(m_ui->m_lpPauseDist->value());
}

QString CEditTaskS::GetPauseHR()
{
	return(m_ui->m_lpPauseHR->currentText());
}

uint32_t CEditTaskS::GetPausePace()
{
	if(!m_ui->m_lpPauseSpeed->value())
		return(0);
	return(3600/m_ui->m_lpPauseSpeed->value());
}

void CEditTaskS::Set(const QStringList& HRList, const QString& szDesc, uint32_t dwCount, const QTime& Time, uint32_t dwDist, const QString& szHR, uint32_t dwPace, const QTime& PauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace)
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
	if(dwPace)
		m_ui->m_lpSpeed->setValue(3600/dwPace);
	if(dwCount > 1)
	{
		m_ui->m_lpGroupPause->setVisible(true);
		m_ui->m_lpPauseTime->setTime(PauseTime);
		m_ui->m_lpPauseDist->setValue(dwPauseDist);
		m_ui->m_lpPauseHR->setCurrentIndex(m_ui->m_lpHR->findText(szPauseHR));
		if(dwPausePace)
			m_ui->m_lpPauseSpeed->setValue(3600/dwPausePace);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpTime_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(time).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpDist->value();
	uint32_t	dwSpeed	= m_ui->m_lpSpeed->value();

	if(!dwTime)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwSpeed = 3600*dwDist/dwTime/1000;
		m_ui->m_lpSpeed->setValue(dwSpeed);
	}
	else if(dwSpeed)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpDist_valueChanged(int )
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpDist->value();
	uint32_t	dwSpeed	= m_ui->m_lpSpeed->value();

	if(!dwDist)
		return;

	if(dwTime)
	{
		m_bChanging = true;
		dwSpeed = 3600*dwDist/dwTime/1000;
		m_ui->m_lpSpeed->setValue(dwSpeed);
	}
	else if(dwSpeed)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpSpeed_valueChanged(int speed)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpDist->value();
	uint32_t	dwSpeed	= speed;

	if(!dwSpeed)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwTime = dwDist*3600/dwSpeed/1000;
		m_ui->m_lpTime->setTime(QTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)));
	}
	else if(dwTime)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpPauseTime_timeChanged(QTime time)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(time).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwSpeed	= m_ui->m_lpPauseSpeed->value();

	if(!dwTime)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwSpeed = 3600*dwDist/dwTime/1000;
		m_ui->m_lpPauseSpeed->setValue(dwSpeed);
	}
	else if(dwSpeed)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpPauseDist_valueChanged(int )
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpPauseTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwSpeed	= m_ui->m_lpPauseSpeed->value();

	if(!dwDist)
		return;

	if(dwTime)
	{
		m_bChanging = true;
		dwSpeed = 3600*dwDist/dwTime/1000;
		m_ui->m_lpPauseSpeed->setValue(dwSpeed);
	}
	else if(dwSpeed)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::on_m_lpPauseSpeed_valueChanged(int speed)
{
	if(m_bChanging)
	{
		m_bChanging = false;
		return;
	}
	uint32_t	dwTime	= CTime(m_ui->m_lpPauseTime->time()).toDWORD();
	uint32_t	dwDist	= m_ui->m_lpPauseDist->value();
	uint32_t	dwSpeed	= speed;

	if(!dwSpeed)
		return;

	if(dwDist)
	{
		m_bChanging = true;
		dwTime = dwDist*3600/dwSpeed/1000;
		m_ui->m_lpPauseTime->setTime(QTime(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)));
	}
	else if(dwTime)
	{
		m_bChanging = true;
		dwDist = dwSpeed*dwTime*1000/3600;
		m_ui->m_lpPauseDist->setValue(dwDist);
	}

	CalcTotal();
}

void CEditTaskS::CalcTotal()
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

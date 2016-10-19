#include "ceditsession.h"
#include "ui_ceditsession.h"
#include "common.h"

CEditSession::CEditSession(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CEditSession)
{
    m_ui->setupUi(this);
}

CEditSession::~CEditSession()
{
    delete m_ui;
}

void CEditSession::changeEvent(QEvent *e)
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

void CEditSession::on_buttonBox_accepted()
{
	if(!m_ui->m_lpDesc->text().length())
	{
		MESSAGE(tr("Description is empty!"));
		return;
	}
	if(!m_ui->m_lpDay->currentText().length())
	{
		MESSAGE(tr("No day selected!"));
		return;
	}
	if(!m_ui->m_lpSport->currentText().length())
	{
		MESSAGE(tr("No sport selected!"));
		return;
	}
	accept();
}

void CEditSession::on_buttonBox_rejected()
{
	reject();
}

QString CEditSession::GetDay()
{
	return(m_ui->m_lpDay->currentText());
}

QString CEditSession::GetDesc()
{
	return(m_ui->m_lpDesc->text());
}

QString CEditSession::GetSport()
{
	return(m_ui->m_lpSport->currentText());
}

void CEditSession::Set(const QStringList& DayList, const QStringList& SportList, const QString& szDay, const QString& szDesc, const QString& szSport)
{
	int	i;

	for(i = 0;i < DayList.size();i++)
		m_ui->m_lpDay->addItem(DayList.at(i));

	for(i = 0;i < SportList.size();i++)
		m_ui->m_lpSport->addItem(SportList.at(i));

	m_ui->m_lpDay->setCurrentIndex(m_ui->m_lpDay->findText(szDay));
	m_ui->m_lpDesc->setText(szDesc);
	m_ui->m_lpSport->setCurrentIndex(m_ui->m_lpSport->findText(szSport));
}

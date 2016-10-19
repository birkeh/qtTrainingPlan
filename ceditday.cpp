#include "ceditday.h"
#include "ui_ceditday.h"
#include "common.h"

CEditDay::CEditDay(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CEditDay)
{
    m_ui->setupUi(this);
}

CEditDay::~CEditDay()
{
    delete m_ui;
}

void CEditDay::changeEvent(QEvent *e)
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

void CEditDay::on_buttonBox_accepted()
{
	if(!m_ui->m_lpDesc->text().length())
	{
		MESSAGE(tr("Description is empty!"));
		return;
	}
	accept();
}

void CEditDay::on_buttonBox_rejected()
{
	reject();
}

int CEditDay::GetDay()
{
	return(m_ui->m_lpDay->value());
}

QString CEditDay::GetDesc()
{
	return(m_ui->m_lpDesc->text());
}

void CEditDay::Set(int iDay, const QString& szDesc)
{
	m_ui->m_lpDay->setValue(iDay);
	m_ui->m_lpDesc->setText(szDesc);
}

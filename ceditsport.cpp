#include "ceditsport.h"
#include "ui_ceditsport.h"
#include "common.h"

CEditSport::CEditSport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEditSport)
{
    ui->setupUi(this);
	ui->m_lpPace->setChecked(true);
}

CEditSport::~CEditSport()
{
    delete ui;
}

void CEditSport::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CEditSport::on_buttonBox_accepted()
{
	if(!ui->m_lpName->text().length())
	{
		MESSAGE(tr("Name is empty!"));
		return;
	}
	accept();
}

void CEditSport::on_buttonBox_rejected()
{
	reject();
}

QString CEditSport::GetName()
{
	return(ui->m_lpName->text());
}

int CEditSport::GetMaxHR()
{
	return(ui->m_lpMaxHR->value());
}

bool CEditSport::GetPace()
{
	return(ui->m_lpPace->isChecked());
}

void CEditSport::Set(const QString& szName, int iMaxHR, bool bPace)
{
	ui->m_lpName->setText(szName);
	ui->m_lpMaxHR->setValue(iMaxHR);
	ui->m_lpPace->setChecked(bPace);
	ui->m_lpSpeed->setChecked(!bPace);
}

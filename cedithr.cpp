#include "common.h"
#include "cedithr.h"
#include "ui_cedithr.h"

CEditHR::CEditHR(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEditHR)
{
    ui->setupUi(this);
}

CEditHR::~CEditHR()
{
    delete ui;
}

void CEditHR::changeEvent(QEvent *e)
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

QString CEditHR::GetName()
{
	return(ui->m_lpName->text());
}

QString CEditHR::GetDesc()
{
	return(ui->m_lpDesc->text());
}

uint32_t CEditHR::GetMin()
{
	return(ui->m_lpMinHR->value());
}

uint32_t CEditHR::GetMax()
{
	return(ui->m_lpMaxHR->value());
}

void CEditHR::Set(const QString& szName, const QString& szDesc, uint32_t dwMin, uint32_t dwMax)
{
	ui->m_lpName->setText(szName);
	ui->m_lpDesc->setText(szDesc);
	ui->m_lpMinHR->setValue(dwMin);
	ui->m_lpMaxHR->setValue(dwMax);
}

void CEditHR::on_buttonBox_accepted()
{
	if(!ui->m_lpName->text().length())
	{
		MESSAGE(tr("Name is empty!"));
		return;
	}
	accept();
}

void CEditHR::on_buttonBox_rejected()
{
	reject();
}

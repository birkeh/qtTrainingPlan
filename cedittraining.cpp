#include "cedittraining.h"
#include "ui_cedittraining.h"
#include "common.h"

CEditTraining::CEditTraining(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CEditTraining)
{
    m_ui->setupUi(this);
}

CEditTraining::~CEditTraining()
{
    delete m_ui;
}

void CEditTraining::changeEvent(QEvent *e)
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

void CEditTraining::on_buttonBox_accepted()
{
	if(!m_ui->m_lpTraining->text().length())
	{
		MESSAGE(tr("Name is empty!"));
		return;
	}
	accept();
}

void CEditTraining::on_buttonBox_rejected()
{
	reject();
}

QString CEditTraining::GetTraining()
{
	return(m_ui->m_lpTraining->text());
}

void CEditTraining::SetTraining(const QString& szTraining)
{
	m_ui->m_lpTraining->setText(szTraining);
}
